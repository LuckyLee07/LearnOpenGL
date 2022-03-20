# LearnOpenGL

OpenGL学习工程 2022/3/20开始

<1> 在MacOS下使用vscode和cmake创建跨平台的OpenGL运行环境
 0、下载并安装VSCode，安装C/C++编译所需的必要插件：C/C++、CMake、CMake Tools、C++代码提示
  # 参考链接：https://blog.csdn.net/weixin_43063125/article/details/89470913

 1、创建工程目录并编写CMakeLists.txt文件，创建依赖库目录并下载GLFW库拷贝进去
  # cmake编写参考：https://www.cnblogs.com/iwiniwin/p/13705456.html
  # 总步骤参考：https://blog.csdn.net/seewed/article/details/107148654
  # 参考glfw文档编写main文件：https://www.glfw.org/documentation.html

 2、编写main.cpp文件创建窗口，进入工程目录下执行./cmake命令进行编译，程序正常运行并显示窗口
  # 遇到的问题：工程可运行但无法正常调试 ERROR: Unable to start debugging. Unexpected LLDB output from command "-exec-run"
  # 解决方案：Install CodeLLDB and set "type": "lldb" on launch.json
  # 参考：https://stackoverflow.com/questions/67270447/vscode-lldb-on-macos-error-when-starting-debugging-session

 3、VSCode配置文件创建及编写 [Shift+CMD+P]可打开命令模式，由于c_cpp配置有问题故task配置的是cmake
  # 1.选择[C/Cpp: Edit Configurations(JSON)] ：c_cpp_properties.json文件
  # 2.选择[Tasks: Configure Task]命令-》选择的模板为MSBuild ：tasks.json文件
  # 3.选择[Debug: Open launch.json]命令-》选择的模板为C/C++ ：launch.json文件
  # 参加链接1：https://blog.csdn.net/brazy/article/details/92801958
  # 参考链接2：https://blog.csdn.net/u012372584/article/details/123010410

 4、在github重新启用时遇到的一些问题记录如下：访问令牌及代码推送问题
  # 问题及解决1: https://blog.csdn.net/sinat_36728518/article/details/119733175
  # 问题及解决2: https://blog.csdn.net/tree_ifconfig/article/details/81557091
  # 问题及解决3: https://blog.csdn.net/ywl470812087/article/details/104459288


<2> 为工程添加GLEW库并在窗口上绘制三角形