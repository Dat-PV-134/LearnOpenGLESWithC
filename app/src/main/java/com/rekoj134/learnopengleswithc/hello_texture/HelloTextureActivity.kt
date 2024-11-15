package com.rekoj134.learnopengleswithc.hello_texture

import android.app.ActivityManager
import android.content.Context
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.rekoj134.learnopengleswithc.MainActivity.Companion.initShaderManager
import com.rekoj134.learnopengleswithc.hello_triangle.HelloTriangleRenderer

class HelloTextureActivity : AppCompatActivity() {
    private var glSurfaceView: GLSurfaceView? = null
    private var rendererSet = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val activityManager = getSystemService(ACTIVITY_SERVICE) as ActivityManager
        val configurationInfo = activityManager.deviceConfigurationInfo
        val supportsEs2 = configurationInfo.reqGlEsVersion >= 0x30000

        if (supportsEs2) {
            initShaderManager(this@HelloTextureActivity)
            glSurfaceView = GLSurfaceView(this)
            glSurfaceView?.setEGLContextClientVersion(2)
            glSurfaceView?.setRenderer(HelloTextureRenderer())
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