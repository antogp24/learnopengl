#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/types-struct.h>

#define GLSL_LOADER_IMPL
#include "glsl_loader.h"

// #version version_number
// in type name;
// out type name;
// uniform type name;
//   
// void main() {
//   // process input(s) and do some weird graphics stuff
//   ...
//   // output processed stuff to output variable
//   out_name = weird_stuff_we_processed;
// }

#define array_size(a) (sizeof(a) / sizeof(*a))
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#define SCREEN_W 800
#define SCREEN_H 600
#define PI 3.141592f

float vertices[] = {
    // positions      // colors
    -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   // 0 bottom left
    +0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   // 1 bottom right
    +0.5f, +0.5f,   0.0f, 0.0f, 1.0f,   // 2 top right
    -0.5f, +0.5f,   1.0f, 0.0f, 1.0f,   // 3 top left
};

GLuint indices[] = {
    2, 1, 3, // top right triangle
    1, 0, 3, // bottom left triangle
};

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_W, SCREEN_H, "3. Shaders", NULL, NULL);
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

    int max_n_attributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_n_attributes);
    printf("Max number of vertex attributes in OpenGL: %i\n", max_n_attributes);

    GLuint shader_program = glsl_load("part1/3_shaders/shader.vert", "part1/3_shaders/shader.frag");

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const int aPos = glGetAttribLocation(shader_program, "aPos");
    glVertexAttribPointer(aPos, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPos);
    const int aColor = glGetAttribLocation(shader_program, "aColor");
    glVertexAttribPointer(aColor, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(aColor);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        float time = glfwGetTime();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float t = (sinf(PI*time) * 0.25f) + 0.25f;

        glUseProgram(shader_program);
        glUniform1f(glGetUniformLocation(shader_program, "t"), t);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, array_size(indices), GL_UNSIGNED_INT, (void*)0);

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
