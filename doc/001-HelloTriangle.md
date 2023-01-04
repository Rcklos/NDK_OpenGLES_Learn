## 你好, 三角形

### 初始化项目

新建`Empty Project`的Android项目之后，新建一个`Native Library`作为编写OpenGL ES代码的模块。

![](http://cdn.lentme.cn/202301041624676.png)

修改默认的项目配置: 

1. 重命名`NativeLib.kt`为`MyNativeRender.kt`
2. 重命名CMake项目名：
    ```
   # CMakeList.txt
    project("glnative-render")
   ```
   记得也要修改对应的kt代码：
    ```
   // MyNativeRender.kt
    System.loadLibrary("glnative-render")
   ```

> 以上的配置可以根据自己的习惯进行调整，记录配置仅是为了后文的文件做备忘

配置OpenGL ES3

```
#CMakeList.txt

target_link_libraries( # Specifies the target library.
   glnative-render

   # 添加链接库
   android
   GLESv3

   # Links the target library to the log library
   # included in the NDK.
   ${log-lib})
```