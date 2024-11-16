package com.rekoj134.learnopengleswithc.zipper_lock

import android.opengl.GLSurfaceView.Renderer
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class ZipperLockRenderer: Renderer {
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        onSurfaceCreatedNativeZipperLock()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        onSurfaceChangedNativeZipperLock(width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        onDrawFrameNativeZipperLock()
    }

    private external fun onSurfaceCreatedNativeZipperLock()
    private external fun onSurfaceChangedNativeZipperLock(width: Int, height: Int)
    private external fun onDrawFrameNativeZipperLock()
}