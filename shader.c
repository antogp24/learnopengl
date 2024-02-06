#include "shader.h"

ShaderID shader_new(char *vs_path, char *fs_path) {
    ShaderID shader = 0;
    
    FILE *vs_file = fopen(vs_path, "r");
    assert(vs_file != NULL);
    fseek(vs_file, 0, SEEK_END);
    int vs_file_size = ftell(vs_file);
    fseek(vs_file, 0, SEEK_SET);

    FILE *fs_file = fopen(fs_path, "r");
    assert(fs_file != NULL);
    fseek(fs_file, 0, SEEK_END);
    int fs_file_size = ftell(fs_file);
    fseek(fs_file, 0, SEEK_SET);

    char *vs_contents = malloc(vs_file_size * sizeof(char));
    char *fs_contents = malloc(fs_file_size * sizeof(char));

    fread(vs_contents, sizeof(char), vs_file_size, vs_file);
    fread(fs_contents, sizeof(char), fs_file_size, fs_file);

    for (int i = vs_file_size - 15; i < vs_file_size; i++) {
        char c = vs_contents[i];
        if (c < 0 || c >= 128) {
            vs_contents[i] = '\0';
            break;
        }
        
    }
    for (int i = fs_file_size - 15; i < fs_file_size; i++) {
        char c = fs_contents[i];
        if (c < 0 || c >= 128) {
            fs_contents[i] = '\0';
            break;
        }
    }

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vs_contents, NULL);
    glCompileShader(vertex);
    shader_check_compile_errors(vertex, "Vertex");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fs_contents, NULL);
    glCompileShader(fragment);
    shader_check_compile_errors(fragment, "Fragment");

    shader = glCreateProgram();
    glAttachShader(shader, vertex);
    glAttachShader(shader, fragment);
    glLinkProgram(shader);
    shader_check_linking_errors(shader);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    fclose(vs_file);
    fclose(fs_file);

    return shader;
}

void shader_check_compile_errors(GLuint shader, char *type) {
    GLint success;
    GLchar info_log[SHADER_LOG_SIZE];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, SHADER_LOG_SIZE, NULL, info_log);
        printf("%s shader failed to compile:\n%s\n", type, info_log);
    }
}

void shader_check_linking_errors(GLuint shader) {
    GLint success;
    GLchar info_log[SHADER_LOG_SIZE];

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, SHADER_LOG_SIZE, NULL, info_log);
        printf("Shader program failed to link:\n%s\n", info_log);
    }
}
