package com.rekoj134.learnopengleswithc.tranformations

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class TransformationsRenderer : GLSurfaceView.Renderer {
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        onSurfaceCreatedNative()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        onSurfaceChangedNative()
    }

    override fun onDrawFrame(gl: GL10?) {
        onDrawFrameNative()
    }

    private external fun onSurfaceCreatedNative()
    private external fun onSurfaceChangedNative()
    private external fun onDrawFrameNative()
}