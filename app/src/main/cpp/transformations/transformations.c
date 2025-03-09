//
// Created by Admin on 1/15/2025.
//
#include "../shader.h"
#include "transformations.h"

static float vertices[] = {
        0.0f, 0.5f, 1.0f, 0.5f, 0.5f, 1.0f,
        -0.5f, -0.5f, 0.5f, 1.0f, 0.5f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.5f, 1.0f, 1.0f
};

static unsigned int VBO, VAO;
static Shader shader;

static void on_surface_created() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shader = shader_create("shader/transformations_vertex_shader.glsl", "shader/transformations_fragment_shader.glsl");
    if (shader.ID == 0) {
        __android_log_print(ANDROID_LOG_ERROR, "OpenGL", "Shader creation failed");
        return;
    }
    shader_use(&shader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static void on_surface_changed() {

}

static void on_draw_frame() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_tranformations_TransformationsRenderer_onSurfaceCreatedNative(
        JNIEnv *env, jobject thiz) {
    on_surface_created();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_tranformations_TransformationsRenderer_onSurfaceChangedNative(
        JNIEnv *env, jobject thiz) {
    on_surface_changed();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_tranformations_TransformationsRenderer_onDrawFrameNative(
        JNIEnv *env, jobject thiz) {
    on_draw_frame();
}