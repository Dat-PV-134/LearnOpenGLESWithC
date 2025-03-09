package com.rekoj134.learnopengleswithc.test

import android.annotation.SuppressLint
import android.app.ActivityManager
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.DisplayMetrics
import android.util.Log
import android.view.MotionEvent
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.rekoj134.learnopengleswithc.MainActivity.Companion.initShaderManager


class TestActivity : AppCompatActivity() {
    private var glSurfaceView: GLSurfaceView? = null
    private var rendererSet = false

    @SuppressLint("ClickableViewAccessibility")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val activityManager = getSystemService(ACTIVITY_SERVICE) as ActivityManager
        val configurationInfo = activityManager.deviceConfigurationInfo
        val supportsEs3 = configurationInfo.reqGlEsVersion >= 0x30000


        // Create a DisplayMetrics object
        val displayMetrics = DisplayMetrics()
        // Get the screen dimensions
        windowManager.defaultDisplay.getMetrics(displayMetrics)
        // Get screen width and height in pixels
        val screenWidth = displayMetrics.widthPixels
        val screenHeight = displayMetrics.heightPixels
        // Display or use the screen width and height
        Log.e("ANCUTKO","Screen Width: " + screenWidth + "px")
        Log.e("ANCUTKO","Screen Height: " + screenHeight + "px")

        if (supportsEs3) {
            val myRenderer = TestActivityRenderer()
            initShaderManager(this@TestActivity)
            glSurfaceView = GLSurfaceView(this)
            glSurfaceView?.setEGLContextClientVersion(3)
            glSurfaceView?.setRenderer(myRenderer)
            rendererSet = true
            setContentView(glSurfaceView)

            glSurfaceView?.setOnTouchListener { v, event ->
                event?.let {
                    val normalizeX = (event.x / v.width) * 2 - 1
                    val normalizeY = -((event.y / v.height) * 2 - 1)
                    Log.e("ANCUTKO", normalizeX.toString() + " - " + normalizeY)
                    if (it.action == MotionEvent.ACTION_DOWN) {
                        glSurfaceView?.queueEvent { myRenderer.handleTouchPress(normalizeX, normalizeY) }
                    } else if (it.action == MotionEvent.ACTION_MOVE) {
                        glSurfaceView?.queueEvent { myRenderer.handleTouchDrag(normalizeX, normalizeY) }
                    }
                    true
                }
                true
            }
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