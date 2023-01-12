//
// Created by 20152 on 2023/1/7.
//

#ifndef NDK_OPENGLES_LEARN_GLBASESAMPLE_H
#define NDK_OPENGLES_LEARN_GLBASESAMPLE_H

#include "GLBaseSampleType.h"
#include "GLUtils.h"
#include "model.h"
#include <GLES3/gl3.h>

#define DEFAULT_OGL_ASSETS_DIR "/data/data/cn.lentme.learn.gles/cache"

class GLBaseSample {
public:
    GLBaseSample(): GLBaseSample(DEFAULT_OGL_ASSETS_DIR) {}
    GLBaseSample(const char* cacheDir) {
        VERTEX_SHADER = GL_NONE;
        FRAGMENT_SHADER = GL_NONE;
        this->cacheDir = cacheDir;
        m_ProgramObj = 0;
        m_Width = 0;
        m_Height = 0;
    }

    virtual ~GLBaseSample() {}

    virtual void Create() = 0;

    virtual void Change(int width, int height) {
        LOGD("Change() width = %d , height = %d\n", width, height)
        m_Width = width;
        m_Height = height;
        // Set the viewport
        // 通知OpenGL ES 用于绘制的2D渲染表面的原点、宽度和高度。
        // 在OpenGL ES 中，视口(Viewport) 定义所有OpenGL ES 渲染操作最终显示的2D矩形
        // 视口(Viewport) 由原点坐标(x,y)和宽度(width) 、高度(height)定义。
        glViewport(0, 0, m_Width, m_Height);
    }

    virtual void Draw() = 0;

    virtual void Shutdown(){
        if (m_ProgramObj) {
            glDeleteProgram(m_ProgramObj);
            m_ProgramObj = GL_NONE;
        }
        if(VERTEX_SHADER != nullptr){
            delete[] VERTEX_SHADER;
            VERTEX_SHADER = nullptr;
        }
        if(FRAGMENT_SHADER!= nullptr){
            delete[] FRAGMENT_SHADER;
            FRAGMENT_SHADER = nullptr;
        }
    }

    // 默认啥都不做，等待有需要的子类去重写
//    virtual void SwitchBlendingMode() {}

    virtual void SetDelta(float x, float y) {}

//    virtual void SetMinFilter(int filter) {}

//    virtual void SetMagFilter(int filter) {}

//    virtual void LoadImage(NativeImage *pImage) {};

//    virtual void LoadMultiImageWithIndex(int index, NativeImage *pImage) {}

//    virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY) {}

//    virtual void LoadAudioData(short *buffer, int len) {}

//    virtual void SetTouchLocation(float x, float y) {}

//    virtual void SetGravityXY(float x, float y) {}

protected:
    /**
     * 程序对象
     */
    GLuint m_ProgramObj;
    const char *cacheDir;

    /**
     * 顶点着色器
     */
    const char *VERTEX_SHADER;
    /**
     * 片段着色器脚本
     */
    const char *FRAGMENT_SHADER;

    /**
     * 屏幕宽度
     */
    int m_Width;
    /**
     * 屏幕高度
     */
    int m_Height;
};


#endif //NDK_OPENGLES_LEARN_GLBASESAMPLE_H
