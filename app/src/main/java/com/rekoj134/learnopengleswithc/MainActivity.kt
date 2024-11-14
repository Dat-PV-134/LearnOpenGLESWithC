package com.rekoj134.learnopengleswithc

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.rekoj134.learnopengleswithc.databinding.ActivityHelloTriangleBinding
import com.rekoj134.learnopengleswithc.hello_triangle.HelloTriangleActivity
import com.rekoj134.learnopengleswithc.zipper_lock.ZipperLockActivity

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityHelloTriangleBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityHelloTriangleBinding.inflate(layoutInflater)
        setContentView(binding.root)
        handleEvent()
    }

    private fun handleEvent() {
        binding.btnHelloTriangle.setOnClickListener {
            startActivity(Intent(this@MainActivity, HelloTriangleActivity::class.java))
        }

        binding.btnZipperLock.setOnClickListener {
            startActivity(Intent(this@MainActivity, ZipperLockActivity::class.java))
        }
    }

    companion object {
        init {
            System.loadLibrary("learnopengleswithc")
        }
    }
}