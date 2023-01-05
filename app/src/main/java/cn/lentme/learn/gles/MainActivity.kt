package cn.lentme.learn.gles

import android.app.ActivityManager
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import cn.lentme.gles.render.MyNativeRender

class MainActivity : AppCompatActivity() {
    companion object {
        private const val CONTEXT_CLIENT_VERSION = 3
    }

    private lateinit var mRender: MyNativeRender

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // 创建并配置GLSurfaceView
        val surface = GLSurfaceView(this)
        mRender = MyNativeRender(this)
        if(detectOpenGLES30()) {
            // 设置OpenGL ES版本
            surface.setEGLContextClientVersion(CONTEXT_CLIENT_VERSION)
            // 绑定render
            surface.setRenderer(mRender)
            // 设置渲染模式
            surface.renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY
        } else {
            Log.e("MyNativeRender", "OpenGL ES 3.0 not supported on device.  Exiting...")
            finish()
        }

        setContentView(surface)
    }


    private fun detectOpenGLES30(): Boolean {
        val am = getSystemService(ACTIVITY_SERVICE) as ActivityManager
        val info = am.deviceConfigurationInfo
        return info.reqGlEsVersion >= 0x30000
    }
}