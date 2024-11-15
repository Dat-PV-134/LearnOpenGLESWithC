//
// Created by DatPV on 11/14/2024.
//
#ifndef LEARN_OPENGL_ES_WITH_C_SHADER_H
#define LEARN_OPENGL_ES_WITH_C_SHADER_H

#include <GLES3/gl3.h>  // Include GLEW to get all the required OpenGL headers
#include <stdbool.h>   // For using the 'bool' type
#include <android/log.h> // For logging errors
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

// Define a structure to hold the shader program ID
typedef struct Shader {
    unsigned int ID;
} Shader;

// Function prototypes
Shader shader_create(const char* vertexPath, const char* fragmentPath);
void shader_use(Shader* shader);
void shader_set_bool(Shader* shader, const char* name, bool value);
void shader_set_int(Shader* shader, const char* name, int value);
void shader_set_float(Shader* shader, const char* name, float value);

#endif //LEARN_OPENGL_ES_WITH_C_SHADER_H
