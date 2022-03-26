## LearnOpenGL
<u>OpenGL学习工程 2022/3/20开始</u>

-----2020.03.21-----
### <font size=4 color=#1E90FF>**<1> 在MacOS下使用vscode和cmake创建跨平台的OpenGL运行环境**</font>
 #### 0、下载并安装VSCode，安装C/C++编译所需的必要插件：C/C++、CMake、CMake Tools、C++代码提示
  * 参考链接：<u>https://blog.csdn.net/weixin_43063125/article/details/89470913</u>

 #### 1、创建工程目录并编写CMakeLists.txt文件，创建依赖库目录并下载GLFW库拷贝进去
  * cmake编写参考：<u>https://www.cnblogs.com/iwiniwin/p/13705456.html</u>
  * 总步骤参考：<u>https://blog.csdn.net/seewed/article/details/107148654</u>
  * 参考glfw文档编写main文件：<u>https://www.glfw.org/documentation.html</u>

 #### 2、编写main.cpp文件创建窗口，进入工程目录下执行./cmake命令进行编译，程序正常运行并显示窗口
  * 遇到的问题：`工程可运行但无法正常调试 ERROR: Unable to start debugging. Unexpected LLDB output from command "-exec-run"`
  * 解决方案：`Install CodeLLDB and set "type": "lldb" on launch.json`
  * 参考：<u>https://stackoverflow.com/questions/67270447/vscode-lldb-on-macos-error-when-starting-debugging-session</u>

 #### 3、VSCode配置文件创建及编写 [Shift+CMD+P]可打开命令模式，由于c_cpp配置有问题故task配置的是cmake
  * 1.选择`[C/Cpp: Edit Configurations(JSON)]` ：c_cpp_properties.json文件
  * 2.选择`[Tasks: Configure Task]`命令-》选择的模板为MSBuild ：tasks.json文件
  * 3.选择`[Debug: Open launch.json]`命令-》选择的模板为C/C++ ：launch.json文件
  * 参加链接1：<u>https://blog.csdn.net/brazy/article/details/92801958</u>
  * 参考链接2：<u>https://blog.csdn.net/u012372584/article/details/123010410</u>

 #### 4、在github重新启用时遇到的一些问题记录如下：访问令牌及代码推送问题
  * 问题及解决1: <u>https://blog.csdn.net/sinat_36728518/article/details/119733175</u>
  * 问题及解决2: <u>https://blog.csdn.net/tree_ifconfig/article/details/81557091</u>
  * 问题及解决3: <u>https://blog.csdn.net/ywl470812087/article/details/104459288</u>

 #### 5、第三方库GLEW的下载编译及使用、绘制三角形
  * 遇到的问题：GLEW库的源码编译无从下手、经查下载源码后解压进入根直接执行cmake及make命令即可
  * 相关的命令：`cmake ./build/cmake/ -DCMAKE_INSTALL_PREFIX=./install`
  * 参考链接为：<u>https://www.cnblogs.com/zjutzz/p/10982675.html</u>


-----2020.03.24-----
### <font size=4 color=#1E90FF>**<2> OpenGL相关技术：VAO/VBO/IBO/shader/texture的学习及掌握**</font>
 #### 0、由于Mac M1使用Metal技术，对OpenGL的支持比Intel芯片差，需要为此做相应的硬件适配。
  * 遇到的问题1：`version '330' is not supported`
  * 解决方案参考：将`VERSION_MAJOR/MINOR`改为3/3，将PROFILE改为`CORE`模式并开启向前兼容`GLFW_OPENGL_FORWARD_COMPAT`
  * 遇到的问题2：shader不再抱错但还是黑屏，原因为`CORE模式下不包含任何弃用功能`，因此不会再创建默认的VertexArray，需自己创建。
  * 参考的链接1：<u>https://www.cnblogs.com/liangliangh/archive/2016/08/23/4765645.html</u>
  * 参考的链接2：<u>https://stackoverflow.com/questions/30057286/how-to-use-vbos-without-vaos-with-opengl-core-profile</u>
  * 参考的链接3：<u>https://stackoverflow.com/questions/62990972/why-is-opengl-giving-me-the-error-error-01-version-330-is-not-support</u>
  * 参考的链接4：<u>https://www.glfw.org/docs/3.3/window_guide.html#GLFW_OPENGL_FORWARD_COMPAT_hint</u>
 
 #### 1、编写简单的shader绘制颜色、使用VAO/VBO/IBO绘制三角形及四边形
  * 注意点：索引缓冲区对象使用的类型是`GL_ELEMENT_ARRAY_BUFFER`而非`GL_INDEX_ARRAY`。

#### 2、使用stb_image库图片加载到内存并绘制在OpenGL场景中
  * 注意点：图片一般指png等格式化文件，由文件头和数据体组成。而纹理则是一块连续存放颜色值的区域。
  * cmake配置：`aux_source_directory(src/vendor/stb_image DIR_SRCS)`
  * 代码注意点：`glTexImage2D`函数中`internalformat`参数是指定OpenGL是如何管理纹理单元中数据格式的，一般使用`GL_RGBA`。format指的是载入纹理的格式，它告诉OpenGL外部数据是如何存储每个像素数据的。
  * 代码注意点：在绘制透明图片时需开启混合模式并设置混合方式：`glEnable(GL_BLEND)`。具体参考如下:
  * 参考的链接1：<u>https://blog.csdn.net/hudfang/article/details/46726465</u>
  * 参考的链接2：<u>https://blog.csdn.net/u011371324/article/details/77823860</u>
  * 参考的链接3：<u>https://blog.csdn.net/jeffasd/article/details/78135588</u>

