package cn.lentme.learn.gles.activity

import android.Manifest
import android.app.ActivityManager
import android.content.pm.PackageManager
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import cn.lentme.gles.render.MyNativeRender
import cn.lentme.gles.render.ui.RockerView
import cn.lentme.learn.gles.databinding.ActivityRenderBinding

class GLRenderActivity: AppCompatActivity() {
    companion object {
        private const val CONTEXT_CLIENT_VERSION = 3

        private val REQUEST_PERMISSIONS = arrayOf(
            Manifest.permission.WRITE_EXTERNAL_STORAGE,
            Manifest.permission.RECORD_AUDIO
        )
        private const val PERMISSION_REQUEST_CODE = 1
    }

    private lateinit var mRender: MyNativeRender
    private lateinit var surface: GLSurfaceView
    private lateinit var rockerView: RockerView
    private lateinit var mBinding: ActivityRenderBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mBinding = ActivityRenderBinding.inflate(layoutInflater)
        setContentView(mBinding.root)

        // 创建并配置GLSurfaceView
        initSurface()

        // 创建并配置RockerView
        initRockerView()
    }

    private fun initSurface() {
        mRender = MyNativeRender(this)
        surface = mBinding.renderView
        if(detectOpenGLES30()) {
            // 设置OpenGL ES版本
            surface.setEGLContextClientVersion(CONTEXT_CLIENT_VERSION)
            // 绑定render
            surface.setRenderer(mRender)
            // 设置渲染模式
            surface.renderMode = GLSurfaceView.RENDERMODE_CONTINUOUSLY
        } else {
            Log.e("MyNativeRender", "OpenGL ES 3.0 not supported on device.  Exiting...")
            finish()
        }

        intent.extras?.let {
            val type = it.getInt("type")
            mRender.glesSetType(type)
        }
    }

    private fun initRockerView() {
        rockerView = mBinding.rockerView
        rockerView.setOnWheelChangedListener {
            Log.i(localClassName, "angle == $it")
        }
    }

    override fun onResume() {
        super.onResume()

        surface.onResume()
        if(!hasPermissionsGranted(REQUEST_PERMISSIONS))
            ActivityCompat.requestPermissions(this,
                REQUEST_PERMISSIONS, PERMISSION_REQUEST_CODE)
        else {
            mRender.copyAssetsToCache(this)
        }
    }

    private fun detectOpenGLES30(): Boolean {
        val am = getSystemService(ACTIVITY_SERVICE) as ActivityManager
        val info = am.deviceConfigurationInfo
        return info.reqGlEsVersion >= 0x30000
    }

    private fun hasPermissionsGranted(permissions: Array<String>): Boolean {
        for (permission in permissions) {
            if (ActivityCompat.checkSelfPermission(this, permission)
                != PackageManager.PERMISSION_GRANTED
            ) {
                return false
            }
        }
        return true
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String?>,
        grantResults: IntArray
    ) {
        if (requestCode == PERMISSION_REQUEST_CODE) {
            if (!hasPermissionsGranted(REQUEST_PERMISSIONS)) {
                Toast.makeText(
                    this,
                    "We need the permission: RECORD_AUDIO && WRITE_EXTERNAL_STORAGE",
                    Toast.LENGTH_SHORT
                ).show()
            }
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }
    }
}