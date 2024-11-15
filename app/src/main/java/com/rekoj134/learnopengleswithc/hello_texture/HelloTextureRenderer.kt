package com.rekoj134.learnopengleswithc.hello_texture

import android.opengl.GLSurfaceView.Renderer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class HelloTextureRenderer : Renderer {
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        onSurfaceCreatedNativeTexture()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        onSurfaceChangedNativeTexture()
    }

    override fun onDrawFrame(gl: GL10) {
        onDrawFrameNativeTexture()
    }

    private external fun onSurfaceCreatedNativeTexture()
    private external fun onSurfaceChangedNativeTexture()
    private external fun onDrawFrameNativeTexture()
}