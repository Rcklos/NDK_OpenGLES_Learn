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
        case GL_HELLO_TRIANGLE_SAMPLE_TYPE:
            sample = new HelloTriangle();
            break;
        case GL_HELLO_TRIANGLE2_SAMPLE_TYPE:
            sample = new HelloTriangle2();
            break;
        default:
            LOGE("sample type not found!!!!");
            break;
    }
}