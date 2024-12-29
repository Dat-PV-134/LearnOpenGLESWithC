package com.rekoj134.learnopengleswithc.gl_transition

import android.opengl.GLSurfaceView.Renderer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class GLTransitionRenderer : Renderer {
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        onSurfaceCreatedNative()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        onSurfaceChangedNative()
    }

    override fun onDrawFrame(gl: GL10) {
        onDrawFrameNative()
    }

    private external fun onSurfaceCreatedNative()
    private external fun onSurfaceChangedNative()
    private external fun onDrawFrameNative()
}