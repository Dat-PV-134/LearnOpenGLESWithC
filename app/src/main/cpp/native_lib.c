#include <jni.h>
#include <GLES3/gl3.h>
#include <android/log.h> // For logging errors

const char *vertexShaderSource = "#version 300 es\n"
                                 "in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 300 es\n"
                                   "precision mediump float;\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";

float vertices[] = {
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, -0.5f, 0.0f
};

unsigned int VBO, VAO, shaderProgram, vertexShader, fragmentShader;

void on_surface_created() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile the vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        __android_log_print(ANDROID_LOG_ERROR, "OpenGL", "Vertex shader compilation failed: %s", infoLog);
    }

    // Compile the fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        __android_log_print(ANDROID_LOG_ERROR, "OpenGL", "Fragment shader compilation failed: %s", infoLog);
    }

    // Link shaders to the program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        __android_log_print(ANDROID_LOG_ERROR, "OpenGL", "Shader program linking failed: %s", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Create and bind the vertex array object and buffer
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_RendererWrapper_onSurfaceCreatedNative(JNIEnv *env, jobject thiz) {
    on_surface_created();
}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_RendererWrapper_onSurfaceChangedNative(JNIEnv *env, jobject thiz) {

}

JNIEXPORT void JNICALL
Java_com_rekoj134_learnopengleswithc_RendererWrapper_onDrawFrameNative(JNIEnv *env, jobject thiz) {
    on_draw_frame();
}
