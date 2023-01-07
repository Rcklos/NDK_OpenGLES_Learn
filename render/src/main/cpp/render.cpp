#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <GLUtils.h>
#include <shader.h>
#include <GLBaseSample.h>
#include <GLBaseSampleInc.h>


GLBaseSample *sample = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_cn_lentme_gles_render_MyNativeRender_glesCreate(JNIEnv *env, jobject thiz, jobject asset_mgr) {
    GLUtils::setEnvAndAssetManager(env, asset_mgr);
    if(sample != nullptr) {
        sample->Create();
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_cn_lentme_gles_render_MyNativeRender_glesResize(JNIEnv *env, jobject thiz, jint width,
                                                     jint height) {
    if(sample != nullptr) {
        sample->Change(width, height);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_lentme_gles_render_MyNativeRender_glesDraw(JNIEnv *env, jobject thiz) {
    if(sample != nullptr) {
        sample->Draw();
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_cn_lentme_gles_render_MyNativeRender_glesSetType(JNIEnv *env, jobject thiz, jint type) {
    switch(type) {
        case GL_HELLO_TRIANGLE:
            sample = new HelloTriangle();
            break;
        case GL_HELLO_TRIANGLE2:
            sample = new HelloTriangle2();
            break;
        case GL_HELLO_TRIANGLE3:
            sample = new HelloTriangle3();
            break;
        case GL_HELLO_RECTANGLE:
            sample = new HelloRectangle();
            break;
        default:
            LOGE("sample type not found!!!!");
            break;
    }
}