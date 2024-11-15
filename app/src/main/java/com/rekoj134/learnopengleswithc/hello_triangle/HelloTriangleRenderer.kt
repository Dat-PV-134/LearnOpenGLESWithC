package com.rekoj134.learnopengleswithc.hello_triangle

import android.opengl.GLSurfaceView.Renderer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10


class HelloTriangleRenderer : Renderer {
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        onSurfaceCreatedNativeTriangle()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        onSurfaceChangedNativeTriangle()
    }

    override fun onDrawFrame(gl: GL10) {
        onDrawFrameNativeTriangle()
    }

    private external fun onSurfaceCreatedNativeTriangle()
    private external fun onSurfaceChangedNativeTriangle()
    private external fun onDrawFrameNativeTriangle()
}