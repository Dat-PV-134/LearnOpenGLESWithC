package com.rekoj134.learnopengleswithc

import android.opengl.GLSurfaceView.Renderer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class RendererWrapper : Renderer {
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        on_surface_created()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        on_surface_changed()
    }

    override fun onDrawFrame(gl: GL10) {
        on_draw_frame()
    }

    private external fun on_surface_created()
    private external fun on_surface_changed()
    private external fun on_draw_frame()
}