#include <jni.h>
#include <string>
#include <GLES3/gl3.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#define LOG_TAG "ndk-render"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define VERTEX_POS_INDX 0

AAssetManager* gAssetMgr = nullptr;
GLuint gProgram;
jint gWidth, gHeight;

// 加载asset文本文件
char* readTextFileFromAsset(const char* filename)
{
    if(gAssetMgr == nullptr) {
        LOGE("AssetManager is null!");
        return nullptr;
    }
    char* readBuf = nullptr;
    AAsset* asset = AAssetManager_open(gAssetMgr, filename, AASSET_MODE_UNKNOWN);
    if(!asset) {
        LOGE("asset is null!");
        return nullptr;
    }
    off_t size = AAsset_getLength(asset);
    // 这里size + 1是为了包含字符串结束符
    readBuf = (char *)malloc(size + 1);
    readBuf[size] = '\0';

    int num = AAsset_read(asset, readBuf, size);
    LOGI("%s: [%s]", filename, readBuf);
    AAsset_close(asset);
    return readBuf;
}

GLuint loadShader(GLenum type, const char *shader_src)
{
    GLuint shader;
    GLint compiled = 0;

    // 创建shader
    // glCreateShader用于创建指定类型的shader，并返回该shader的句柄
    // shader类型必须是GL_VERTEX_SHADER和GL_FRAGMENT_SHADER的其中一个
    // 如果创建失败会返回0
    shader = glCreateShader(type);

    if(shader == 0) return 0;

    // 设置shader源代码需要使用glShaderSource函数，该函数的参数如下：
    // 1. shader句柄
    // 2. 字符串数组数量(char* 是字符数组，char** 是字符串数组)
    // 3. 字符串数组的指针
    // 4. 描述字符串数组中每一个元素的长度的数组，传入NULL表示每个字符串都是以'\0'结尾，而无需特指长度
    glShaderSource(shader, 1, &shader_src, NULL);

    // 编译shader
    glCompileShader(shader);

    // 检查编译是否存在错误
    // glGetShaderiv 用于查询shader中的参数，该函数的参数如下:
    // 1. 需要被查询的shader句柄
    // 2. 需要查询的参数
    // 3. 用于接收查询结果
    // 该函数可查询的参数如下:
    // GL_SHADER_TYPE           : 查询shader的类型，返回的结果是GL_VERTEX_SHADER和GL_FRAGMENT_SHADER的其中一个
    // GL_DELETE_STATUS         : 查询shader是否已经被标记删除，返回结果是GL_TRUE和GL_FALSE的其中一个
    // GL_COMPILE_STATUS        : 查询shader是否编译成功，返回结果是GL_TRUE和GL_FALSE的其中一个
    // GL_INFO_LOG_LENGTH       : 返回shader操作的info log的字符串长度(包含结尾的空字符)
    // GL_SHADER_SOURCE_LENGTH  : 返回shader源代码的长度(包含结尾的空字符)
    // 注意: 如果出现了error, 用于接收查询结果的参数不会被修改
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        GLint info_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
        if(info_len > 1) {
            char *info_log = (char *)malloc(sizeof(char) * info_len);
            // glGetShaderInfoLog 用于获取shader操作的info log，该函数参数如下:
            // 1. shader句柄
            // 2. 接收info log的缓冲区大小
            // 3. 返回info log的字符串长度(不包含结尾的空字符)，如果不需要返回，可以置NULL
            // 4. 用于接收info log的缓冲区指针
            glGetShaderInfoLog(shader, info_len, NULL, info_log);
            LOGE("Error compiling shader: [%s]", info_log);
            free(info_log);
        }

        // 删除shader
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_lentme_gles_render_MyNativeRender_glesCreate(JNIEnv *env, jobject thiz, jobject asset_mgr) {
    char *vertex_shader_src, *frag_shader_src;
    GLuint vertex_shader, frag_shader;
    GLuint program_obj;
    GLint linked;

    gAssetMgr = AAssetManager_fromJava(env, asset_mgr);
    // 加载shader源码
    vertex_shader_src = readTextFileFromAsset("shader/vertex/hello_triangle.glsl");
    frag_shader_src = readTextFileFromAsset("shader/fragment/hello_triangle.glsl");

    // 加载shader
    vertex_shader   = loadShader(GL_VERTEX_SHADER, vertex_shader_src);
    frag_shader     = loadShader(GL_FRAGMENT_SHADER, frag_shader_src);

    // 创建program对象
    program_obj = glCreateProgram();

    if(!program_obj) {
        LOGE("Error creating program object!");
        return;
    }

    // 绑定shader
    glAttachShader(program_obj, vertex_shader);
    glAttachShader(program_obj, frag_shader);

    // 链接程序，这个过程和gcc的编译链接差不多
    glLinkProgram(program_obj);

    // 检查链接情况
    // glGetProgramiv 函数类似于glGetShaderiv，在上面已经详细说明，以下说明可查询的参数:
    // GL_ACTIVE_ATTRIBUTES: 查询有效的属性变量数量
    // GL_ACTIVE_ATTRIBUTE_MAX_LENGTH: 查询最长的有效属性名的长度(包含了结尾的空字符)，如果没有有效的属性，则返回0
    // GL_ACTIVE_UNIFORM_BLOCKS: 查询包含的有效统一变量所占用的统一变量块的数量
    // GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH: 返回最长的有效统一变量块名的长度(包含了结尾空字符)，如果没有有效的统一变量块，则返回0
    // GL_ACTIVE_UNIFORMS: 返回有效统一变量的数量
    // GL_ACTIVE_UNIFORM_MAX_LENGTH: 返回最长的有效统一变量名的长度(包含了结尾空字符)，如果没有有效的统一变量，则返回0
    // GL_ATTACHED_SHADERS: 返回关联的shader对象数量
    // GL_DELETE_STATUS: 返回program是否已被标记为删除，返回结果是GL_TRUE或GL_FALSE
    // GL_INFO_LOG_LENGTH: 返回info log的长度，包含了结尾空字符
    // GL_LINK_STATUS: 返回链接状态，返回结果是GL_TRUE或GL_FALSE
    // GL_PROGRAM_BINARY_RETRIEVABLE_HINT: 返回program是否启用了二进制检索提示
    // GL_TRANSFORM_FEEDBACK_BUFFER_MODE: 查询变换反馈缓冲的模式，GL_SEPARATE_ATTRIBS或GL_INTERLEAVED_ATTRIBS
    // GL_TRANSFORM_FEEDBACK_VARYINGS: 查询变换反馈模式下要捕获的变量数量
    // GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH: 查询用于变换反馈的最长变量名的长度，包含了结尾空字符
    // GL_VALIDATE_STATUS 查询最近验证操作的结果，返回结果是GL_TRUE或GL_FALSE
    glGetProgramiv(program_obj, GL_LINK_STATUS, &linked);
    if(linked) {
        gProgram = program_obj;
        glClearColor(1.f, 1.f, 1.f, 1.f);
    }
    else {
        GLint info_len = 0;
        glGetProgramiv(program_obj, GL_INFO_LOG_LENGTH, &info_len);
        if(info_len > 1) {
            char *info_log = (char *)malloc(sizeof(char) * info_len);
            // glGetProgramInfoLog 用于获取program操作的info log，该函数参数如下:
            // 1. program句柄
            // 2. 接收info log的缓冲区大小
            // 3. 返回info log的字符串长度(不包含结尾的空字符)，如果不需要返回，可以置NULL
            // 4. 用于接收info log的缓冲区指针
            glGetProgramInfoLog(program_obj, info_len, NULL, info_log);
            LOGE("Error linking program: [%s]", info_log);
            free(info_log);
        }

        // 删除program
        glDeleteProgram(program_obj);
    }

    // 释放shader源码的内存
    free(vertex_shader_src);
    free(frag_shader_src);
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_lentme_gles_render_MyNativeRender_glesResize(JNIEnv *env, jobject thiz, jint width,
                                                     jint height) {
    gWidth = width;
    gHeight = height;
}

extern "C"
JNIEXPORT void JNICALL
Java_cn_lentme_gles_render_MyNativeRender_glesDraw(JNIEnv *env, jobject thiz) {
    // 顶点数据
    GLfloat vertices[] = {
            0.0f,   0.5f,   0.0f,
            -0.5f,  -0.5f,  0.0f,
            0.5f,   -0.5f,  0.0f
    };

    // 设置视图
    glViewport(0, 0, gWidth, gHeight);
    glClear(GL_COLOR_BUFFER_BIT);

    // 应用program
    glUseProgram(gProgram);

    // 设置顶点属性指针，glVertexAttribPointer参数如下:
    // 1. 需要设置的顶点属性索引，注意这里的索引指的是我们需要配置的顶点属性的索引，而不是vertices(顶点数据)的索引
    // 2. 顶点属性类型，也就是顶点属性的维度，这里xyz三维则填3
    // 3. 是否需要标准化(归一化)，如果归一化，所有数据都会被设置到0~1之间(有符号型是-1~1)
    // 4. 步长，由于顶点数据在数组中相邻的都是直接挨着的，所以设0
    // 5. 顶点数据指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    // 启用顶点属性(默认是禁用)，其传入的参数值应是顶点属性对应的location
    glEnableVertexAttribArray(VERTEX_POS_INDX);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}