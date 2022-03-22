# LearnOpenGL
<u>OpenGL学习工程 2022/3/20开始</u>

## <font size=4 color=#1E90FF>**<1> 在MacOS下使用vscode和cmake创建跨平台的OpenGL运行环境**</font>
 ### 0、下载并安装VSCode，安装C/C++编译所需的必要插件：C/C++、CMake、CMake Tools、C++代码提示
  * 参考链接：<u>https://blog.csdn.net/weixin_43063125/article/details/89470913</u>

 ### 1、创建工程目录并编写CMakeLists.txt文件，创建依赖库目录并下载GLFW库拷贝进去
  * cmake编写参考：<u>https://www.cnblogs.com/iwiniwin/p/13705456.html</u>
  * 总步骤参考：<u>https://blog.csdn.net/seewed/article/details/107148654</u>
  * 参考glfw文档编写main文件：<u>https://www.glfw.org/documentation.html</u>

 ### 2、编写main.cpp文件创建窗口，进入工程目录下执行./cmake命令进行编译，程序正常运行并显示窗口
  * 遇到的问题：`工程可运行但无法正常调试 ERROR: Unable to start debugging. Unexpected LLDB output from command "-exec-run"`
  * 解决方案：`Install CodeLLDB and set "type": "lldb" on launch.json`
  * 参考：<u>https://stackoverflow.com/questions/67270447/vscode-lldb-on-macos-error-when-starting-debugging-session</u>

 ### 3、VSCode配置文件创建及编写 [Shift+CMD+P]可打开命令模式，由于c_cpp配置有问题故task配置的是cmake
  * 1.选择`[C/Cpp: Edit Configurations(JSON)]` ：c_cpp_properties.json文件
  * 2.选择`[Tasks: Configure Task]`命令-》选择的模板为MSBuild ：tasks.json文件
  * 3.选择`[Debug: Open launch.json]`命令-》选择的模板为C/C++ ：launch.json文件
  * 参加链接1：<u>https://blog.csdn.net/brazy/article/details/92801958</u>
  * 参考链接2：<u>https://blog.csdn.net/u012372584/article/details/123010410</u>

 ### 4、在github重新启用时遇到的一些问题记录如下：访问令牌及代码推送问题
  * 问题及解决1: <u>https://blog.csdn.net/sinat_36728518/article/details/119733175</u>
  * 问题及解决2: <u>https://blog.csdn.net/tree_ifconfig/article/details/81557091</u>
  * 问题及解决3: <u>https://blog.csdn.net/ywl470812087/article/details/104459288</u>


## <font size=4 color=#1E90FF>**<2> 为工程添加GLEW库并在窗口上绘制三角形**</font>