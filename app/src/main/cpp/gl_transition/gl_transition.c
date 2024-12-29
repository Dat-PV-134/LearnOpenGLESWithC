//
// Created by Admin on 12/29/2024.
//
#include "gl_transition.h"
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
static unsigned int texture1, texture2, texture3, texture4, texture5, texture6, texture7, texture8, texture9;

static float timeElapsed = 0.0f;  // Track the elapsed time
static float animationSpeed = 0.5f; // Speed of animation (you can adjust this)
static int curTextureAnim = 1;

static void on_surface_created() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Use the shader_create function to create and compile the shader program
    shader = shader_create("shader/gl_transition_vertex_shader.glsl", "shader/gl_transition_fragment_shader.glsl");
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

    gen_texture(&texture1,"texture/transition_1.jpg");
    gen_texture(&texture2,"texture/transition_2.jpg");
    gen_texture(&texture3,"texture/transition_3.jpg");
    gen_texture(&texture4,"texture/transition_4.jpg");
    gen_texture(&texture5,"texture/transition_5.jpg");
    gen_texture(&texture6,"texture/transition_6.jpg");
    gen_texture(&texture7,"texture/transition_7.jpg");
    gen_texture(&texture8,"texture/transition_8.jpg");
    gen_texture(&texture9,"texture/transition_9.jpg");
}

static void gen_texture(unsigned int *texture, const char *path) {
    glGenTextures(1, texture);  // Generate texture ID
    glActiveTexture(GL_TEXTURE0);  // Choose texture unit 0
    glBindTexture(GL_TEXTURE_2D, *texture);  // Bind the texture to the active texture unit

    // Set the texture wrapping and filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrap horizontally
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrap vertically
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // Mipmap filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // Linear filtering for magnification

    // Load image, create texture, and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = loadAssetTexture(path, &width, &height, &nrChannels);
    if (data) {
        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);  // Generate mipmaps after loading texture
    } else {
        LOGE("Failed to load texture: %s", path);
    }
}

static void on_surface_changed() {

}

static void on_draw_frame() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen


    timeElapsed += animationSpeed * 0.016f;

    if (timeElapsed > 1.2f) {
        if (curTextureAnim < 9) {
            curTextureAnim++;
        } else {
            curTextureAnim = 1;
        }
        timeElapsed = 0.0f;
    }

    if (timeElapsed <= 1.0f) {
        shader_set_float(&shader, "progress", timeElapsed);
    } else {
        shader_set_float(&shader, "progress", 1.0f);
    }
    shader_set_int(&shader, "curType", curTextureAnim);

    // Use the shader program created by shader_create
    shader_use(&shader);

    switch (curTextureAnim) {
        case 1:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            shader_set_int(&shader, "texture1", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
            shader_set_int(&shader, "texture1", 1);
            break;
        case 2:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture2);
            shader_set_int(&shader, "texture1", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture3);
            shader_set_int(&shader, "texture1", 1);
            break;
        case 3:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture3);
            shader_set_int(&shader, "texture1", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture4);
            shader_set_int(&shader, "texture1", 1);
            break;
        case 4:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture4);
            shader_set_int(&shader, "texture1", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture5);
            shader_set_int(&shader, "texture1", 1);
            break;
        case 5:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture5);
            shader_set_int(&shader, "texture1", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture6);
            shader_set_int(&shader, "texture1", 1);
            break;
        case 6:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture6);
            shader_set_int(&shader, "texture1", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture7);
            shader_set_int(&shader, "texture1", 1);
            break;
        case 7:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture7);
            shader_set_int(&shader, "texture1", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture8);
            shader_set_int(&shader, "texture1", 1);
            break;
        case 8:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture8);
            shader_set_int(&shader, "texture1", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture9);
            shader_set_int(&shader, "texture1", 1);
            break;
        case 9:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture9);
            shader_set_int(&shader, "texture1", 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture1);
            shader_set_int(&shader, "texture1", 1);
            break;
        default:
            break;
    }


    // Bind the VAO and draw the triangle using the element indices
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_gl_1transition_GLTransitionRenderer_onSurfaceCreatedNative(
        JNIEnv *env, jobject thiz) {
    on_surface_created();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_gl_1transition_GLTransitionRenderer_onSurfaceChangedNative(
        JNIEnv *env, jobject thiz) {
    on_surface_changed();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_gl_1transition_GLTransitionRenderer_onDrawFrameNative(
        JNIEnv *env, jobject thiz) {
    on_draw_frame();
}