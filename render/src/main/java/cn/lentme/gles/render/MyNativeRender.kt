package cn.lentme.gles.render

import android.content.Context
import android.content.res.AssetManager
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MyNativeRender(context: Context) : GLSurfaceView.Renderer{
    // asset，主要用于加载shader、texture、font等资源
    private val mAssetMgr: AssetManager

    companion object {
        // Used to load the 'render' library on application startup.
        init {
            System.loadLibrary("glnative-render")
        }
    }

    /**
     * A native method that is implemented by the 'render' native library,
     * which is packaged with this application.
     */
    // 在Surface创建时调用
    private external fun glesCreate(assetMgr: AssetManager)
    // 在Surface变化时调用
    private external fun glesResize(width: Int, height: Int)
    // 在Surface渲染时调用
    private external fun glesDraw()

    init {
        mAssetMgr = context.assets
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        glesCreate(mAssetMgr)
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        glesResize(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        glesDraw()
    }
}