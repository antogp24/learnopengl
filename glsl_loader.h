#ifndef _GLSL_LOADER_H
#define _GLSL_LOADER_H

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <glad/glad.h>
#include <cglm/cglm.h>
#include <cglm/types-struct.h>

#define GLSL_LOADER_LOG_SIZE 1024

GLuint glsl_load(char *vs_path, char *fs_path);
void glsl_loader_check_compile_errors(GLuint shader, char *type);
void glsl_loader_check_linking_errors(GLuint shader);


#ifdef GLSL_LOADER_IMPL


GLuint glsl_load(char *vs_path, char *fs_path) {
    GLuint shader;
    
    FILE *vs_file = fopen(vs_path, "r");
    assert(vs_file != NULL);
    fseek(vs_file, 0, SEEK_END);
    long vs_file_size = ftell(vs_file);
    fseek(vs_file, 0, SEEK_SET);

    FILE *fs_file = fopen(fs_path, "r");
    assert(fs_file != NULL);
    fseek(fs_file, 0, SEEK_END);
    long fs_file_size = ftell(fs_file);
    fseek(fs_file, 0, SEEK_SET);

    char *vs_contents = malloc((vs_file_size + 1) * sizeof(char));
    char *fs_contents = malloc((fs_file_size + 1) * sizeof(char));

    for (int c = 0, i = 0; ; i++) {
        c = fgetc(vs_file);
        vs_contents[i] = c;
        if (c < 0 || c >= 128) {
            vs_contents[i] = '\0';
            vs_file_size = i;
            break;
        }
    }

    for (int c = 0, i = 0; ; i++) {
        c = fgetc(fs_file);
        fs_contents[i] = c;
        if (c < 0 || c >= 128) {
            fs_contents[i] = '\0';
            fs_file_size = i;
            break;
        }
    }

    fclose(vs_file);
    fclose(fs_file);

    GLuint vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const char **)&vs_contents, NULL);
    glCompileShader(vertex);
    glsl_loader_check_compile_errors(vertex, "Vertex");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const char **)&fs_contents, NULL);
    glCompileShader(fragment);
    glsl_loader_check_compile_errors(fragment, "Fragment");

    shader = glCreateProgram();
    glAttachShader(shader, vertex);
    glAttachShader(shader, fragment);
    glLinkProgram(shader);
    glsl_loader_check_linking_errors(shader);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shader;
}


void glsl_loader_check_compile_errors(GLuint shader, char *type) {
    GLint success;
    GLchar info_log[GLSL_LOADER_LOG_SIZE];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, GLSL_LOADER_LOG_SIZE, NULL, info_log);
        printf("%s shader failed to compile:\n%s\n", type, info_log);
    }
}


void glsl_loader_check_linking_errors(GLuint shader) {
    GLint success;
    GLchar info_log[GLSL_LOADER_LOG_SIZE];

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, GLSL_LOADER_LOG_SIZE, NULL, info_log);
        printf("Shader program failed to link:\n%s\n", info_log);
    }
}


#endif // ifdef GLSL_LODADER_IMPL


#endif // _GLSL_LOADER_H
