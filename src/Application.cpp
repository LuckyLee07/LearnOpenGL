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

static float mixRatio = 0.2f;
void processInput(GLFWwindow* window);

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
    //glfwSetKeyCallback(window, key_callback);  


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
    shader.SetUniform1f("u_ratio", mixRatio);
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

    //**变换操作：先缩放再旋转最后位移**
    // 将图片缩小0.5倍并逆时针旋转90度    
    glm::mat4 transform = glm::mat4(1.0f);
    //transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    //transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
    shader.SetUniformMat4f("transform", transform);

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

    float scale = 0.5f;
    float increment = 0.05f;
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window); //键盘输入处理

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        shader.Bind(); //设置Uniform前需先绑定Shader

        vao.Bind(); //只需设置绑定VAO即可
        texture1.Active();
        texture2.Active();

        //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
        shader.SetUniform1f("u_ratio", mixRatio);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 0.5f));
        shader.SetUniformMat4f("transform", transform);
        renderer.Draw(vao, ibo, shader);

        transform = glm::mat4(1.0f); //将矩阵重置为单位矩阵
        transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scalex = static_cast<float>(abs(sin(glfwGetTime())));
        transform = glm::scale(transform, glm::vec3(scalex, scalex, scalex));
        shader.SetUniformMat4f("transform", transform);
        renderer.Draw(vao, ibo, shader);
        
        vao.Unbind();
        shader.Unbind();

        if (scale > 0.5f)
            increment = -0.005f;
        else if (scale < 0.0f)
            increment = 0.005f;
        scale += increment;
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    // 当用户按下ESC键时,设置window窗口的WindowShouldClose属性为true
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE); // 关闭应用程序
    }
    else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixRatio += 0.005f;
        if (mixRatio > 1.0f) mixRatio = 1.0f;
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixRatio -= 0.005f;
        if (mixRatio < 0.0f) mixRatio = 0.0f;
    }
}