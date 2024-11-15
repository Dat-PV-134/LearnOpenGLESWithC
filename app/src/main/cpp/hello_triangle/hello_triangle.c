//
// Created by DatPV on 11/13/2024.
//
#include "../shader.h"
#include "hello_triangle.h"

// Vertices and indices for the triangle (same as before)
float vertices[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,// bottom left
        -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f, // top left
};
unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
};

unsigned int VBO, VAO, EBO;
Shader shader;  // Shader object to hold the shader program ID

// Modify on_surface_created() to use shader_create()
static void on_surface_created() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Use the shader_create function to create and compile the shader program
    shader = shader_create("shader/vertex_shader.glsl", "shader/fragment_shader.glsl");
    if (shader.ID == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "OpenGL", "Shader creation failed");
        return;
    }

    // Create and bind the vertex array object and buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define the vertex attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static void on_surface_changed() {
    // This function can remain empty for now as it's not used in your current code.
}

static void on_draw_frame() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen

    // Use the shader program created by shader_create
    shader_use(&shader);

    // Bind the VAO and draw the triangle using the element indices
    glBindVertexArray(VAO);
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
