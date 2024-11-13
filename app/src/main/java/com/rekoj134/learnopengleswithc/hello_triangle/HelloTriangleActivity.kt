package com.rekoj134.learnopengleswithc.hello_triangle

import android.app.ActivityManager
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.rekoj134.learnopengleswithc.R
import com.rekoj134.learnopengleswithc.RendererWrapper
import com.rekoj134.learnopengleswithc.databinding.ActivityHelloTriangleBinding

class HelloTriangleActivity : AppCompatActivity() {
    private var glSurfaceView: GLSurfaceView? = null
    private var rendererSet = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val activityManager = getSystemService(ACTIVITY_SERVICE) as ActivityManager
        val configurationInfo = activityManager.deviceConfigurationInfo
        val supportsEs2 = configurationInfo.reqGlEsVersion >= 0x30000

        if (supportsEs2) {
            glSurfaceView = GLSurfaceView(this)
            glSurfaceView?.setEGLContextClientVersion(2)
            glSurfaceView?.setRenderer(HelloTriangleRenderer())
            rendererSet = true
            setContentView(glSurfaceView)
        } else {
            Toast.makeText(
                this, "This device does not support OpenGL ES 3.0.",
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
}