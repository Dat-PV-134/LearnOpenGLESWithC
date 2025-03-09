package com.rekoj134.learnopengleswithc.test

import android.graphics.Canvas
import android.opengl.GLSurfaceView.Renderer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class TestActivityRenderer : Renderer {
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        onSurfaceCreatedNative()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        onSurfaceChangedNative()
    }

    override fun onDrawFrame(gl: GL10) {
        onDrawFrameNative()
    }

    fun handleTouchPress(normalizeX: Float, normalizeY: Float) {
        updateMousePosition(normalizeX, normalizeY)
    }

    fun handleTouchDrag(normalizeX: Float, normalizeY: Float) {
        updateMousePosition(normalizeX, normalizeY)
    }

    private external fun onSurfaceCreatedNative()
    private external fun onSurfaceChangedNative()
    private external fun onDrawFrameNative()
    private external fun updateMousePosition(mouseX: Float, mouseY: Float)
}