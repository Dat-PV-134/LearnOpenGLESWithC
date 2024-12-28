//
// Created by Admin on 11/15/2024.
//
#include "test.h"
#include "../shader.h"

static float vertices[] = {
        1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,// top right
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// bottom right
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left
        -1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f// top left
};

static unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
};

static unsigned int VBO, VAO, EBO;
static Shader shader;  // Shader object to hold the shader program ID
static unsigned int texture1, texture2;

static float timeElapsed = 0.0f;  // Track the elapsed time
static float animationSpeed = 0.5f; // Speed of animation (you can adjust this)

static void on_surface_created() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Use the shader_create function to create and compile the shader program
    shader = shader_create("shader/test_vertex_shader.glsl", "shader/test_fragment_shader.glsl");
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

    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);           // Choose texture unit
    glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
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

    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE0);           // Choose texture unit
    glBindTexture(GL_TEXTURE_2D, texture2); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width2, height2, nrChannels2;
    unsigned char *data2 = loadAssetTexture("texture/zipper_background.png", &width2, &height2, &nrChannels2);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        LOGE("Failed to load texture");
    }
}

static void on_surface_changed() {

}

static int isReverse = false;

static void on_draw_frame() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen

    if (isReverse) {
        timeElapsed -= animationSpeed * 0.016f;
    } else {
        timeElapsed += animationSpeed * 0.016f;
    }

    if (timeElapsed > 1.5f) {
        isReverse = true;
    } else if (timeElapsed < -0.5f) {
        isReverse = false;
    }

    shader_set_float(&shader, "progress", timeElapsed);

    // Use the shader program created by shader_create
    shader_use(&shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    shader_set_int(&shader, "texture1", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    shader_set_int(&shader, "texture1", 1);

    // Bind the VAO and draw the triangle using the element indices
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_test_TestActivityRenderer_onSurfaceCreatedNative(JNIEnv *env,
                                                                                      jobject thiz) {
    on_surface_created();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_test_TestActivityRenderer_onSurfaceChangedNative(JNIEnv *env,
                                                                                      jobject thiz) {
    on_surface_changed();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_test_TestActivityRenderer_onDrawFrameNative(JNIEnv *env,
                                                                                 jobject thiz) {
    on_draw_frame();
}