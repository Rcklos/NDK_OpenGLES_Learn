//
// Created by 20152 on 2023/1/9.
//
#pragma once
#include "GLBaseSample.h"

typedef struct delta_t{
    float x = 0.0f;
    float y = 0.0f;
    delta_t(float x, float y): x(x), y(y) {}
} delta_t;

class SimpleCamera3: public GLBaseSample {
public:

    void Create() override;
    void Draw() override;
    void Shutdown() override;
    void SetDelta(float x, float y) override;

private:
    glm::mat4 m_MVPMatrix;
    glm::mat4 m_ModelMatrix;

    Shader *mShader;
    Model *mModel;

    glm::mat4 mvp;

    float computeDeltaTime();
    void update(float deltaTime);
    long mLastTime = 0;
    float degree = 0;

    delta_t mDelta = { 0.0f, 0.0f };
};
