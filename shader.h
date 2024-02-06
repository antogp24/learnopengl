#ifndef _SHADER_H
#define _SHADER_H

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <glad/glad.h>
#include <cglm/cglm.h>
#include <cglm/types-struct.h>

#define SHADER_LOG_SIZE 1024

typedef unsigned int ShaderID;

ShaderID shader_new(char *vs_path, char *fs_path);
void shader_check_compile_errors(GLuint shader, char *type);
void shader_check_linking_errors(GLuint shader);

#endif //_SHADER_H
