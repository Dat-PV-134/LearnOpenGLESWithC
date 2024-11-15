//
// Created by Admin on 11/15/2024.
//
#include "hello_texture.h"
#include "../shader.h"

static float vertices[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,// top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left
        -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f// top left
};

static unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
};

static unsigned int VBO, VAO, EBO;
static Shader shader;  // Shader object to hold the shader program ID
static unsigned int texture;

static void on_surface_created() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Use the shader_create function to create and compile the shader program
    shader = shader_create("shader/vertex_shader_with_texture.glsl", "shader/fragment_shader_with_texture.glsl");
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);           // Choose texture unit
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = loadAssetTexture("texture/hello_texture.png", &width, &height, &nrChannels);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        LOGE("Failed to load texture");
    }
}

static void on_surface_changed() {

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
Java_com_rekoj134_learnopengleswithc_hello_1texture_HelloTextureRenderer_onSurfaceCreatedNativeTexture(
        JNIEnv *env, jobject thiz) {
   on_surface_created();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_hello_1texture_HelloTextureRenderer_onSurfaceChangedNativeTexture(
        JNIEnv *env, jobject thiz) {
    on_surface_changed();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_hello_1texture_HelloTextureRenderer_onDrawFrameNativeTexture(
        JNIEnv *env, jobject thiz) {
    on_draw_frame();
}