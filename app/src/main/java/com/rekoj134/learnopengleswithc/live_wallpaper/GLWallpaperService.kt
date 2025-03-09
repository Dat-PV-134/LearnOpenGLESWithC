package com.rekoj134.learnopengleswithc.live_wallpaper

import android.annotation.SuppressLint
import android.app.ActivityManager
import android.content.Context
import android.opengl.GLSurfaceView
import android.service.wallpaper.WallpaperService
import android.util.DisplayMetrics
import android.util.Log
import android.view.MotionEvent
import android.view.SurfaceHolder
import android.widget.Toast
import com.rekoj134.learnopengleswithc.MainActivity.Companion.initShaderManager
import com.rekoj134.learnopengleswithc.test.TestActivityRenderer

class GLWallpaperService : WallpaperService() {
    override fun onCreateEngine(): Engine {
        return GLEngine()
    }

    inner class GLEngine : WallpaperService.Engine() {
        private var glSurfaceView: WallpaperGLSurfaceView? = null
        private var rendererSet = false

        @SuppressLint("ClickableViewAccessibility")
        override fun onCreate(surfaceHolder: SurfaceHolder?) {
            super.onCreate(surfaceHolder)

            val activityManager = getSystemService(ACTIVITY_SERVICE) as ActivityManager
            val configurationInfo = activityManager.deviceConfigurationInfo
            val supportsEs3 = configurationInfo.reqGlEsVersion >= 0x30000

            if (supportsEs3) {
                val myRenderer = TestActivityRenderer()
                glSurfaceView = WallpaperGLSurfaceView(this@GLWallpaperService)
                glSurfaceView?.setEGLContextClientVersion(3)
                glSurfaceView?.setRenderer(myRenderer)
                rendererSet = true
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
                    this@GLWallpaperService, "This device does not support OpenGL ES 3.0.",
                    Toast.LENGTH_LONG
                ).show()
                return
            }
        }

        override fun onVisibilityChanged(visible: Boolean) {
            super.onVisibilityChanged(visible)
            if (rendererSet) {
                if (visible) {
                    glSurfaceView?.onResume()
                } else {
                    glSurfaceView?.onPause()
                }
            }
        }

        override fun onDestroy() {
            super.onDestroy()
            glSurfaceView?.onWallpaperDestroy()
        }
    }

    inner class WallpaperGLSurfaceView(context: Context) : GLSurfaceView(context) {
        override fun getHolder(): SurfaceHolder {
            return super.getHolder()
        }

        fun onWallpaperDestroy() {
            super.onDetachedFromWindow()
        }
    }
}