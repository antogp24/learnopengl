#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <cglm/types-struct.h>

vec2s normalized_to_screen(float nx, float yn);
vec2s screen_to_normalized(float x, float y);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#define SCREEN_W 800
#define SCREEN_H 600

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_W, SCREEN_H, "1. Window", NULL, NULL);
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

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


// x = (W + W * nx) / 2   ;  -1 <= nx <= 1
// y = (H + H * ny) / 2   ;  -1 <= ny <= 1
vec2s normalized_to_screen(float nx, float ny) {
    return (vec2s){(SCREEN_W + SCREEN_W * nx) * 0.5f,(SCREEN_H + SCREEN_H * ny) * 0.5f};
}


// nx = (2x - W) / W   ;  0 <= x <= W
// ny = (2y - H) / H   ;  0 <= y <= H
vec2s screen_to_normalized(float x, float y) {
    return (vec2s){(2.0f * x - SCREEN_W) / SCREEN_W, (2.0f * y - SCREEN_H) / SCREEN_H};
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
