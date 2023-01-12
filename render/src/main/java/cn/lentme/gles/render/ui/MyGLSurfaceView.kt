package cn.lentme.gles.render.ui

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import cn.lentme.gles.render.MyNativeRender

class MyGLSurfaceView: GLSurfaceView {
    constructor(context: Context?): super(context) {}
    constructor(context: Context?, attrs: AttributeSet?): super(context, attrs) {}

    private var render: MyNativeRender? = null

    override fun setRenderer(renderer: Renderer?) {
        super.setRenderer(renderer)
        if(renderer is MyNativeRender) {
            render = renderer
        }
    }

    fun setDelta(x: Float, y: Float) {
        render?.run{
            setDelta(x, y)
        }
    }
}