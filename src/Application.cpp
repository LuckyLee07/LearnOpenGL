#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Test.h"
#include "TestClearColor.h"
#include "TestTexture2D.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(void)
{
    std::cout << "Hello OpenGL!" << std::endl;
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    std::cout << "I'm aple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    unsigned int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
    unsigned int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
    std::cout << "oepngl shader version: " << major << "." << minor << std::endl;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //交换间隔表示交换缓冲区之前等待的帧数，通常称为V-Sync(垂直同步)
    glfwSwapInterval(1); //默认设置为0 关闭垂直同步

    //注册按键回调函数 设置ESCAPE关闭窗口
    glfwSetKeyCallback(window, key_callback);  


    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW Init Error!" << std::endl;
    }

    //Blend 混合是将源色和目标色以某种方式混合生成特效的技术 常用来绘制透明或半透明的物体
    glEnable(GL_BLEND); //启用混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        //位置属性             //颜色属性          //纹理坐标
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // 0左下
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // 1右下
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 2右上
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // 3左上
    };
    unsigned int indices[] = {
        0, 1, 2, // 1
        2, 3, 0, // 2
    };
    //VAO(vertex Array Object)：顶点数组对象
    //VBO(vertex Buffer Object)：顶点缓冲对象
    //IBO(index Buffer Object)：索引缓冲对象
    VertexArray vao; //顶点数组对象
    VertexBuffer vbo(positions, 8 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3); //位置
    layout.Push<float>(3); //颜色
    layout.Push<float>(2); //纹理
    vao.AddBuffer(vbo, layout);
    
    IndexBuffer ibo(indices, 6); //索引缓冲对象
    Shader shader("res/shaders/Basic.shader");
    shader.Bind(); //创建Program后绑定
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

    // 创建并使用纹理
    Texture texture1("res/textures/container.jpg");
    texture1.Bind(0);
    shader.SetUniform1i("u_Texture1", 0);
    texture1.Unbind();

    Texture texture2("res/textures/awesomeface.png");
    texture2.Bind(1);
    shader.SetUniform1i("u_Texture2", 1);
    texture2.Unbind();

    // Unbind Data
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //线框模式(Wireframe Mode)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Renderer renderer;

    float r = 0.0f;
    float increment = 0.05f;
    
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader.Bind(); //设置Uniform前需先绑定Shader
        //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

        vao.Bind(); //只需设置绑定VAO即可
        texture1.Active();
        texture2.Active();
        
        renderer.Draw(vao, ibo, shader);
        
        vao.Unbind();
        shader.Unbind();

        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.05f)
            increment = 0.05f;
        
        r += increment;
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE); // 关闭应用程序
}