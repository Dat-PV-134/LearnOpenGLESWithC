package com.rekoj134.learnopengleswithc

import android.app.ActivityManager
import android.opengl.GLSurfaceView
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Toast

class MainActivity : AppCompatActivity() {
    private var glSurfaceView: GLSurfaceView? = null
    private var rendererSet = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val activityManager = getSystemService(ACTIVITY_SERVICE) as ActivityManager
        val configurationInfo = activityManager.deviceConfigurationInfo

        val supportsEs2 =
            configurationInfo.reqGlEsVersion >= 0x20000

        if (supportsEs2) {
            glSurfaceView = GLSurfaceView(this)

            glSurfaceView?.setEGLConfigChooser(8, 8, 8, 8, 16, 0)

            glSurfaceView?.setEGLContextClientVersion(2)
            glSurfaceView?.setRenderer(RendererWrapper())
            rendererSet = true
            setContentView(glSurfaceView)
        } else {
            Toast.makeText(
                this, "This device does not support OpenGL ES 2.0.",
                Toast.LENGTH_LONG
            ).show()
            return
        }
    }

    override fun onPause() {
        if (rendererSet) {
            glSurfaceView?.onPause()
        }
        super.onPause()
    }

    override fun onResume() {
        if (rendererSet) {
            glSurfaceView?.onResume()
        }
        super.onResume()
    }

    companion object {
        init {
            System.loadLibrary("learnopengleswithc")
        }
    }
}