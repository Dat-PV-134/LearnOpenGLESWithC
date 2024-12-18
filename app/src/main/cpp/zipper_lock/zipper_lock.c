//
// Created by DatPV on 11/14/2024.
//
#include <jni.h>
#include <android/log.h> // For logging errors
#include "zipper_lock.h"
#include "../shader.h"

float verticesZipper[] = {
        1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,// top right
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// bottom right
        0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.5f, 0.0f,// bottom left
        0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.5f, 1.0f,// top left

        0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.5f, 1.0f,// top right
        0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.0f,// bottom right
        -1.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,// bottom left
        -1.0f,  -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,// top left

        // right line
        0.2f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,// top right
        0.2f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// bottom right
        0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left
        0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f,// top left

        // left line
        0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left
        0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f,// top left
        -0.2f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,// top right
        -0.2f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// bottom right
};

unsigned int indicesZipper[] = {  // note that we start from 0!
        0, 1, 3,
        1, 2, 3,

        4, 5, 6,
        5, 6, 7,

        8, 9, 10,
        9, 10, 11,

        12, 13, 14,
        13, 14, 15,
};

static float timeElapsed = 0.0f;  // Track the elapsed time
static float animationSpeed = 0.5f; // Speed of animation (you can adjust this)

static Shader shader;  // Shader object to hold the shader program ID
unsigned int VBOZipper, VAOZipper, EBOZipper;
static unsigned int texture, rightLineTexture, leftLineTexture;
static int textureWidth, textureHeight;

static void on_surface_created_zipper() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shader = shader_create("shader/vertex_shader_zipper_lock.glsl", "shader/fragment_shader_zipper_lock.glsl");
    if (shader.ID == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "OpenGL", "Shader creation failed");
        return;
    }

    // Create and bind the vertex array object and buffer
    glGenVertexArrays(1, &VAOZipper);
    glGenBuffers(1, &VBOZipper);
    glGenBuffers(1, &EBOZipper);

    glBindVertexArray(VAOZipper);
    glBindBuffer(GL_ARRAY_BUFFER, VBOZipper);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesZipper), verticesZipper, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOZipper);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesZipper), indicesZipper, GL_STATIC_DRAW);

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int nrChannels;
    unsigned char *data = loadAssetTexture("texture/zipper_background.png", &textureWidth, &textureHeight, &nrChannels);
    if (data) {
        // Upload the image data to OpenGL
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);  // Generate mipmaps for the texture
        freeData(data);
    } else {
        LOGE("Failed to load texture: %s");
    }

    glGenTextures(1, &leftLineTexture);
    glActiveTexture(GL_TEXTURE0);           // Choose texture unit
    glBindTexture(GL_TEXTURE_2D, leftLineTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = loadAssetTexture("texture/zipper_left.png", &textureWidth, &textureHeight, &nrChannels);
    if (data) {
        // Upload the image data to OpenGL
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);  // Generate mipmaps for the texture
        freeData(data);
    } else {
        LOGE("Failed to load texture: %s");
    }

    glGenTextures(1, &rightLineTexture);
    glActiveTexture(GL_TEXTURE0);           // Choose texture unit
    glBindTexture(GL_TEXTURE_2D, rightLineTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = loadAssetTexture("texture/zipper_right.png", &textureWidth, &textureHeight, &nrChannels);
    if (data) {
        // Upload the image data to OpenGL
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);  // Generate mipmaps for the texture
        freeData(data);
    } else {
        LOGE("Failed to load texture: %s");
    }
}

static void on_surface_changed_zipper(int screenWidth, int screenHeight) {

}

static void on_draw_frame_zipper() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen

    // Increment time (for animation effect)
    timeElapsed += animationSpeed * 0.016f; // Assume 60 FPS (time per frame is 1/60 ≈ 0.016f)

// Ensure timeElapsed stays between 0 and 1
    if (timeElapsed > 1.0f) {
        timeElapsed -= 1.0f;
    } else if (timeElapsed < 0.0f) { // Prevent negative timeElapsed values
        timeElapsed += 1.0f;
    }

    // Calculate the new y value using a sine wave for smooth up and down motion
    float newY = sin(timeElapsed * 2.0f * 3.14159265359f);  // Animate from 0.0 to 1.0

    // Update the vertex positions for animation
    verticesZipper[25] = newY;
    verticesZipper[33] = newY;
    verticesZipper[65] = newY;
    verticesZipper[81] = newY;
    verticesZipper[97] = newY;
    verticesZipper[113] = newY;

    // Update the VBO with the new vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBOZipper);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesZipper), verticesZipper);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Draw the triangle
    shader_use(&shader);
    glBindVertexArray(VAOZipper);

    // Bind default texture for the main shape
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);  // Draw first part of the shape

    // Now bind a different texture for the right line
    glBindTexture(GL_TEXTURE_2D, rightLineTexture);  // Bind the texture for the right line
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));  // Draw right line section

    // Bind a different texture for the left line
    glBindTexture(GL_TEXTURE_2D, leftLineTexture);  // Bind the texture for the left line
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(unsigned int)));  // Draw left line section

    glBindVertexArray(0);
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_zipper_1lock_ZipperLockRenderer_onSurfaceCreatedNativeZipperLock(
        JNIEnv *env, jobject thiz) {
    on_surface_created_zipper();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_zipper_1lock_ZipperLockRenderer_onSurfaceChangedNativeZipperLock(
        JNIEnv *env, jobject thiz, jint width, jint height) {
    on_surface_changed_zipper(width, height);
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_zipper_1lock_ZipperLockRenderer_onDrawFrameNativeZipperLock(
        JNIEnv *env, jobject thiz) {
    on_draw_frame_zipper();
}