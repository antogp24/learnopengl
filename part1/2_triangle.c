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

const char *vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragment_shader_source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

float vertices[] = {
    +0.0f, +0.5f, 0.0f, // top middle
    -0.5f, -0.5f, 0.0f, // bottom left
    +0.5f, -0.5f, 0.0f, // bottm right
};

int main(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Triangle", NULL, NULL);
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

    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    int vertex_success;
    char vertex_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_success);
    if (!vertex_success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, vertex_log);
        printf("vertex shader compilation error:\n%s\n", vertex_log);
        return -1;
    }

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    int fragment_success;
    char fragment_log[512];
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_success);
    if (!fragment_success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, fragment_log);
        printf("fragment shader compilation error:\n%s\n", fragment_log);
        return -1;
    }

    int shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int program_success;
    char program_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &program_success);
    if (!program_success) {
        glGetProgramInfoLog(shader_program, 512, NULL, program_log);
        printf("shader program linking error:\n%s\n", program_log);
        return -1;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const int aPos = glGetAttribLocation(shader_program, "aPos");
    glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(aPos);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
