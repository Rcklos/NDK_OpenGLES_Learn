package cn.lentme.gles.render

class MyNativeRender {

    /**
     * A native method that is implemented by the 'render' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'render' library on application startup.
        init {
            System.loadLibrary("glnative-render")
        }
    }
}