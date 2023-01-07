//
// Created by 20152 on 2023/1/7.
//

#ifndef NDK_OPENGLES_LEARN_HELLOTRIANGLE2_H
#define NDK_OPENGLES_LEARN_HELLOTRIANGLE2_H
#include "GLBaseSample.h"

// 第二个例子，颜色由用户控制，不再写死到shader
class HelloTriangle2: public GLBaseSample {
public:
    const unsigned int VERTEX_POS_INDX = 0;
    const unsigned int COLOR_POS_INDX = 1;
public:
    HelloTriangle2() = default;
    virtual ~HelloTriangle2() = default;
    void Create() override;
    void Draw() override;
};


#endif //NDK_OPENGLES_LEARN_HELLOTRIANGLE2_H
