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

#define SCREEN_W 800
#define SCREEN_H 600

float vertices[] = {
    // positions   // texcoords
    -0.5f, +0.5f,   0.0f, 1.0f,  // 0 top left 
    +0.5f, +0.5f,   1.0f, 1.0f,  // 1 top right 
    -0.5f, -0.5f,   0.0f, 0.0f,  // 2 bottom left
    +0.5f, -0.5f,   1.0f, 0.0f,  // 3 bottom right
};

GLuint indices[] = {
    1, 3, 0, // top right triangle
    3, 2, 0, // bottom left triangle
};


// **Common Transformation Matrices**
// 
//    At: transformation matrix
//     v: input vector
//  T(v): output vector
//
//          At            v    =    T(v)  
//
//  [ a00 a01 a02 a03 ] [ x ]     [ T(x) ]
//  [ a10 a11 a12 a13 ] [ y ]  =  [ T(y) ]
//  [ a20 a21 a22 a23 ] [ z ]  =  [ T(z) ]
//  [ a30 a31 a32 a33 ] [ 1 ]     [ T(1) ]
//          4x4          4x1         4x1
//
// * Translation
//  
//  (a, b, c): translation vector
//  
//  [ 1 0 0 a ] [ x ]     [ x + a ]
//  [ 0 1 0 b ] [ y ]  =  [ y + b ]
//  [ 0 0 1 c ] [ z ]  =  [ z + c ]
//  [ 0 0 0 1 ] [ 1 ]     [   1   ]
//  
// * Scaling
//  
//  s: scale factor
//  
//  [ s 0 0 0 ] [ x ]     [ s * x ]
//  [ 0 s 0 0 ] [ y ]  =  [ s * y ]
//  [ 0 0 s 0 ] [ z ]  =  [ s * z ]
//  [ 0 0 0 1 ] [ 1 ]     [   1   ]
//  
// * Rotating around Z axis
//  
//  t: angle //  c: cos(t) //  s: sin(t)
//  
//  [ c -s  0  0 ] [ x ]     [ c*x - s*y ]
//  [ s  c  0  0 ] [ y ]  =  [ s*x + c*y ]
//  [ 0  0  1  0 ] [ z ]  =  [     z     ]
//  [ 0  0  0  1 ] [ 1 ]     [     1     ]
//  
// * Rotating around X axis
//  
//  t: angle //  c: cos(t) //  s: sin(t)
//  
//  [ 1  0  0  0 ] [ x ]     [     x     ]
//  [ 0  c -s  0 ] [ y ]  =  [ c*y - s*z ]
//  [ 0  s  c  0 ] [ z ]  =  [ s*y + c*z ]
//  [ 0  0  0  1 ] [ 1 ]     [     1     ]
//  
// * Rotating around Y axis
//  
//  t: angle //  c: cos(t) //  s: sin(t)
//  
//  [  c  0  s  0 ] [ x ]     [ c*x + s*z ]
//  [  0  1  0  0 ] [ y ]  =  [     y     ]
//  [ -s  0  c  0 ] [ z ]  =  [ c*z - s*x ]
//  [  0  0  0  1 ] [ 1 ]     [     1     ]
//  

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_W, SCREEN_H, "5. Transformations", NULL, NULL);
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

    GLuint shader_program = glsl_load("part1/5_transformations/shader.vert", "part1/5_transformations/shader.frag");

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
    glVertexAttribPointer(in_pos, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(in_pos);

    const int in_tex_coord = glGetAttribLocation(shader_program, "in_tex_coord");
    glVertexAttribPointer(in_tex_coord, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
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

        Image container = load_image("part1/5_transformations/container.jpg");
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

        Image face = load_image("part1/5_transformations/awesomeface.jpg");
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

    while (!glfwWindowShouldClose(window)) {

        float time = glfwGetTime();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // First smiley
        {
            mat4 transform = GLM_MAT4_IDENTITY_INIT;
            glm_translate(transform, (vec3s){0.5f, -0.5f, 0.0f}.raw);
            glm_rotate(transform, time, (vec3s){0.0f, 0.0f, 1.0f}.raw);
            float s = (sinf(time*PI) * 0.25f) + 0.5f;
            glm_scale(transform, (vec3s){s, s, s}.raw);
            glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, (float*)transform);

            glUseProgram(shader_program);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, array_size(indices), GL_UNSIGNED_INT, 0);
            
        }

        // Second smiley
        {
            mat4 transform = GLM_MAT4_IDENTITY_INIT;
            glm_translate(transform, (vec3s){-0.5f, 0.5f, 0.0f}.raw);
            glm_rotate(transform, -time, (vec3s){0.0f, 0.0f, 1.0f}.raw);
            float s = (sinf(-time*PI) * 0.25f) + 0.5f;
            glm_scale(transform, (vec3s){s, s, s}.raw);
            glUniformMatrix4fv(glGetUniformLocation(shader_program, "transform"), 1, GL_FALSE, (float*)transform);

            glDrawElements(GL_TRIANGLES, array_size(indices), GL_UNSIGNED_INT, 0);
        }

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
