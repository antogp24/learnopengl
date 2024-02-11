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

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define array_size(a) (sizeof(a) / sizeof(*a))
Image load_image(char *path);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#define SCREEN_W 800
#define SCREEN_H 600

float vertices[] = {
    // positions         // colors          // texcoords
    -0.5f, +0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 2.0f,  // 0 top left 
    +0.5f, +0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  2.0f, 2.0f,  // 1 top right 
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 2 bottom left
    +0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  2.0f, 0.0f,  // 3 bottom right
};

GLuint indices[] = {
    1, 3, 0, // top right triangle
    3, 2, 0, // bottom left triangle
};

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_W, SCREEN_H, "4. Textures", NULL, NULL);
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
    glViewport(0, 0, SCREEN_W, SCREEN_H);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLuint shader_program = glsl_load("part1/4_textures/shader.vert", "part1/4_textures/shader.frag");

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const int in_pos = glGetAttribLocation(shader_program, "in_pos");
    glVertexAttribPointer(in_pos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(in_pos);

    const int in_color = glGetAttribLocation(shader_program, "in_color");
    glVertexAttribPointer(in_color, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(in_color);
    
    const int in_tex_coord = glGetAttribLocation(shader_program, "in_tex_coord");
    glVertexAttribPointer(in_tex_coord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
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

        Image wall = load_image("part1/4_textures/wall.jpg");
        if (wall.img) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wall.width, wall.height, 0, GL_RGB, GL_UNSIGNED_BYTE, wall.img);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(wall.img);
        wall.img = NULL;
    }

    GLuint texture1;
    {
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        Image face = load_image("part1/4_textures/awesomeface.jpg");
        if (face.img) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, face.width, face.height, 0, GL_RGB, GL_UNSIGNED_BYTE, face.img);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(face.img);
        face.img = NULL;
    }

    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "tex0"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "tex1"), 1);

    float mix_t = 0.5f;

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            mix_t = MIN(mix_t + 0.0005f, 1.0f);
        } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            mix_t = MAX(mix_t - 0.0005f, 0.0f);
        }
        glUniform1f(glGetUniformLocation(shader_program, "mix_t"), mix_t);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, array_size(indices), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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
