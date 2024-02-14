#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/types-struct.h>

#define GLSL_LOADER_IMPL
#include "glsl_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct {
    int width, height;
    unsigned char *img;
} Image;

#define PI 3.141592f
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define array_size(a) (sizeof(a) / sizeof(*a))
Image load_image(char *path);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#define _SCREEN_W 800
#define _SCREEN_H 600

int screen_w = _SCREEN_W;
int screen_h = _SCREEN_H;

float vertices[] = {
//        positions         texcoords
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
};

// **Coordinate Systems**
// -----------------------------
//
// 1. Local Space (Object Space)
// 2. World Space
// 3. View Space (Eye Space)
// 4. Clip Space
// 5. Screen Space
//
// **Matrices used for conversion**
// -----------------------------
//
//    1. --(model matrix)--> 2. --(view matrix)--> 3. --(projection matrix)--> 4. --(automatically)--> 5.
// 
// **Brief Explanation**
// -----------------------------
//
// 1. Local coordinates are the coordinates of your object relative to its local origin; they're the coordinates your object begins in.
// 2. World-space coordinates are relative to some global origin of the world, together with many other objects also placed relative to this world's origin.
// 3. View-space coordinates are seen from the camera or viewer's point of view.
// 4. Clip coordinates determine the type of projection, are processed to the -1.0 and 1.0 range, and determine which vertices will end up on the screen.
// 5. Screen coordinates transforms the coordinates from -1.0 and 1.0 to the coordinate range defined by glViewport.
//

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(screen_w, screen_h, "6.1 Cube", NULL, NULL);
    if (window == NULL) {
        printf("Unable to open glfw window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Unable to initialize glad\n");
        return -1;
    }
    glViewport(0, 0, screen_w, screen_h);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    GLuint shader_program = glsl_load("part1/6_coordinate_systems/cube_shader.vert", "part1/6_coordinate_systems/cube_shader.frag");

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const int in_pos = glGetAttribLocation(shader_program, "in_pos");
    glVertexAttribPointer(in_pos, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(in_pos);

    const int in_tex_coord = glGetAttribLocation(shader_program, "in_tex_coord");
    glVertexAttribPointer(in_tex_coord, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(in_tex_coord);

    stbi_set_flip_vertically_on_load(true);

    GLuint texture0;
    {
        glGenTextures(1, &texture0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        Image container = load_image("part1/6_coordinate_systems/container.jpg");
        if (container.img) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, container.width, container.height, 0, GL_RGB, GL_UNSIGNED_BYTE, container.img);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(container.img);
        container.img = NULL;
    }

    GLuint texture1;
    {
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        Image face = load_image("part1/6_coordinate_systems/awesomeface.jpg");
        if (face.img) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, face.width, face.height, 0, GL_RGB, GL_UNSIGNED_BYTE, face.img);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(face.img);
        face.img = NULL;
    }

    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "texture0"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "texture1"), 1);

    while (!glfwWindowShouldClose(window)) {

        float time = glfwGetTime();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glfwGetWindowSize(window, &screen_w, &screen_h);
        float aspect_ratio = (float)screen_w / (float)screen_h;

        mat4 model = GLM_MAT4_IDENTITY_INIT;
        mat4 view = GLM_MAT4_IDENTITY_INIT;
        mat4 projection = GLM_MAT4_IDENTITY_INIT;

        for (int i = 0; i < 1; i++) {
            glm_rotate(model, time, (vec3s){0.5f, 1.0f, 0.0f}.raw);
            glm_translate(view, (vec3s){0.0f, 0.0f, -3.0f}.raw);

            const float near = 0.1f, far = 100.0f, fov = PI/4;
            /* glm_ortho(0.0f, screen_w, 0.0f, screen_h, near, far, projection); */
            glm_perspective(fov, aspect_ratio, near, far, projection);

            const bool transpose = GL_FALSE;
            glUniformMatrix4fv(glGetUniformLocation(shader_program, "model"), 1, transpose, (float*)model);
            glUniformMatrix4fv(glGetUniformLocation(shader_program, "view"), 1, transpose, (float*)view);
            glUniformMatrix4fv(glGetUniformLocation(shader_program, "projection"), 1, transpose, (float*)projection);
            glUseProgram(shader_program);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

Image load_image(char *path) {
    int width, height, channels_in_file;
    unsigned char *img = stbi_load(path, &width, &height, &channels_in_file, 0);
    if (img == NULL) {
        printf("Failed to load file %s\n", path);
        exit(-1);
    }
    return (Image){width, height, img};
}
