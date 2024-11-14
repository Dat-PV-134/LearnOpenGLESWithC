//
// Created by DatPV on 11/13/2024.
//
#include <jni.h>
#include <android/log.h> // For logging errors
#include "hello_triangle.h"

const char *vertexShaderSourceZipper = "#version 300 es\n"
                                 "in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSourceZipper = "#version 300 es\n"
                                   "precision mediump float;\n"
                                   "uniform vec3 triangleColor;\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(triangleColor.x, triangleColor.y, triangleColor.z, 1.0f);\n"
                                   "}\0";

//float vertices[] = {
//        -0.5f, 0.5f, 0.0f,
//        0.5f, 0.5f, 0.0f,
//        0.0f, -0.5f, 0.0f
//};

float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
};
unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
};

unsigned int VBOZipper, VAOZipper, EBOZipper, shaderProgramZipper, vertexShaderZipper, fragmentShaderZipper;

clock_t t_start;

void on_surface_created() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile the vertex shader
    vertexShaderZipper = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderZipper, 1, &vertexShaderSourceZipper, NULL);
    glCompileShader(vertexShaderZipper);
    int success;
    glGetShaderiv(vertexShaderZipper, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShaderZipper, 512, NULL, infoLog);
        __android_log_print(ANDROID_LOG_ERROR, "OpenGL", "Vertex shader compilation failed: %s", infoLog);
    }

    // Compile the fragment shader
    fragmentShaderZipper = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderZipper, 1, &fragmentShaderSourceZipper, NULL);
    glCompileShader(fragmentShaderZipper);
    glGetShaderiv(fragmentShaderZipper, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShaderZipper, 512, NULL, infoLog);
        __android_log_print(ANDROID_LOG_ERROR, "OpenGL", "Fragment shader compilation failed: %s", infoLog);
    }

    // Link shaders to the program
    shaderProgramZipper = glCreateProgram();
    glAttachShader(shaderProgramZipper, vertexShaderZipper);
    glAttachShader(shaderProgramZipper, fragmentShaderZipper);
    glLinkProgram(shaderProgramZipper);
    glGetProgramiv(shaderProgramZipper, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgramZipper, 512, NULL, infoLog);
        __android_log_print(ANDROID_LOG_ERROR, "OpenGL", "Shader program linking failed: %s", infoLog);
    }
    glDeleteShader(vertexShaderZipper);
    glDeleteShader(fragmentShaderZipper);

    // Create and bind the vertex array object and buffer
    glGenVertexArrays(1, &VAOZipper);
    glGenBuffers(1, &VBOZipper);
    glGenBuffers(1, &EBOZipper);

    glBindVertexArray(VAOZipper);
    glBindBuffer(GL_ARRAY_BUFFER, VBOZipper);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOZipper);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define the vertex attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void on_surface_changed() {

}

void on_draw_frame() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen

    // Draw the triangle
    glUseProgram(shaderProgramZipper);
    glBindVertexArray(VAOZipper);

    GLint uniColor = glGetUniformLocation(shaderProgramZipper, "triangleColor");
    clock_t t_now = clock();  // Get the current time
    float time = ((float)(t_now - t_start)) / 100000;  // Convert clock ticks to seconds
    // Use the elapsed time to calculate the color (sinusoidal wave for red)
    glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_hello_1triangle_HelloTriangleRenderer_onSurfaceCreatedNativeTriangle(
        JNIEnv *env, jobject thiz) {
    on_surface_created();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_hello_1triangle_HelloTriangleRenderer_onSurfaceChangedNativeTriangle(JNIEnv *env, jobject thiz) {
    on_surface_changed();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_hello_1triangle_HelloTriangleRenderer_onDrawFrameNativeTriangle(JNIEnv *env, jobject thiz) {
    on_draw_frame();
}