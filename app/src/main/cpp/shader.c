// Created by DatPV on 11/14/2024.
//
#include "shader.h"
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>

#define LOG_TAG "OpenGL"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static AAssetManager* asset_manager = NULL;

// Function to load shader source code from the assets folder
static char* load_shader_source(const char* filePath) {
    AAsset* asset = AAssetManager_open(asset_manager, filePath, AASSET_MODE_STREAMING);
    if (!asset) {
        LOGE("Failed to open shader file: %s", filePath);
        return NULL;
    }

    off_t fileLength = AAsset_getLength(asset);
    char* buffer = (char*)malloc(fileLength + 1);  // +1 for null terminator
    if (!buffer) {
        LOGE("Failed to allocate memory for shader source");
        AAsset_close(asset);
        return NULL;
    }

    AAsset_read(asset, buffer, fileLength);
    buffer[fileLength] = '\0';  // Ensure null-terminated string

    AAsset_close(asset);
    return buffer;
}

// Function to compile a shader and check for errors
static GLuint compile_shader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }

    return shader;
}

// Function to create shader program
Shader shader_create(const char* vertexPath, const char* fragmentPath) {
    Shader shader;

    // 1. Load vertex and fragment shader source code from files
    char* vertexSource = load_shader_source(vertexPath);
    char* fragmentSource = load_shader_source(fragmentPath);

    if (!vertexSource || !fragmentSource) {
        shader.ID = 0;
        return shader;  // Return an invalid shader if loading fails
    }

    // 2. Compile vertex and fragment shaders
    GLuint vertexShader = compile_shader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compile_shader(fragmentSource, GL_FRAGMENT_SHADER);

    // 3. Create shader program and link shaders
    shader.ID = glCreateProgram();
    glAttachShader(shader.ID, vertexShader);
    glAttachShader(shader.ID, fragmentShader);
    glLinkProgram(shader.ID);

    // Check for linking errors
    GLint success;
    char infoLog[512];
    glGetProgramiv(shader.ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader.ID, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    // 4. Clean up: shaders are no longer needed after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Free allocated memory for shader source code
    free(vertexSource);
    free(fragmentSource);

    return shader;
}

// Function to use/activate the shader program
void shader_use(Shader* shader) {
    glUseProgram(shader->ID);
}

// Function to set a boolean uniform in the shader
void shader_set_bool(Shader* shader, const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

// Function to set an integer uniform in the shader
void shader_set_int(Shader* shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
}

// Function to set a float uniform in the shader
void shader_set_float(Shader* shader, const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader->ID, name), value);
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_MainActivity_00024Companion_initShaderManager(JNIEnv *env, jobject thiz, jobject context) {
    jclass contextClass = (*env)->GetObjectClass(env, context);  // Use C syntax for JNI
    jmethodID getAssetsMethod = (*env)->GetMethodID(env, contextClass, "getAssets", "()Landroid/content/res/AssetManager;");
    jobject assetManagerObject = (*env)->CallObjectMethod(env, context, getAssetsMethod);

    // Convert the AssetManager object to a pointer that we can use in C++
    AAssetManager* assetManager = AAssetManager_fromJava(env, assetManagerObject);

    asset_manager = assetManager;
}