#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define APP_MAIN
#ifdef APP_MAIN //启用新的main函数

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void processInput(GLFWwindow* window, float deltaTime); //键盘输入
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello OpenGL", NULL, NULL);
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

    //设置鼠标光标不显示
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //注册按键回调函数 非实时调用导致卡顿因此换种实现
    //glfwSetKeyCallback(window, keys_callback);
    //注册鼠标回调函数 鼠标滚动/移动
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW Init Error!" << std::endl;
    }

    //Blend 混合是将源色和目标色以某种方式混合生成特效的技术 常用来绘制透明或半透明的物体
    glEnable(GL_BLEND); //启用混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST); //开启深度测试功能

    std::cout << glGetString(GL_VERSION) << std::endl;

    float vertices[] = { //顶点相关数据
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

    // 指定坐标轴三角形箭头属性数据 顶点 颜色
	GLfloat axisTriangleData[] = {
		0.945f,    0.03125f,  0.0f,   1.0f, 0.0f, 0.0f,// +x轴三角形
		1.0f,      0.0f,      0.0f,   1.0f, 0.0f, 0.0f,
		0.945f,    -0.03125f, 0.0f,   1.0f, 0.0f, 0.0f,
		-0.03125f, 0.945f,    0.0f,   0.0f, 1.0f, 0.0f,// +y轴三角形
		0.0f,      1.0f,      0.0f,   0.0f, 1.0f, 0.0f,
		0.03125f,  0.945f,    0.0f,   0.0f, 1.0f, 0.0f,
		-0.03125f, 0.0f,      0.945f, 0.0f, 0.0f, 1.0f,// +z轴三角形
		0.0f,      0.0f,      1.0f,   0.0f, 0.0f, 1.0f,
		0.03125f,  0.0f,      0.945f, 0.0f, 0.0f, 1.0f,
	};
	// 指定坐标轴直线属性数据 顶点 颜色
	GLfloat axisLineData[] = {
		-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f
	};

    //VAO(vertex Array Object)：顶点数组对象
    //VBO(vertex Buffer Object)：顶点缓冲对象
    //IBO(index Buffer Object)：索引缓冲对象
    VertexArray VAO; //顶点数组对象
    VertexBuffer VBO(vertices, 4 * 8 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3); //位置
    layout.Push<float>(3); //颜色
    layout.Push<float>(2); //纹理
    VAO.AddBuffer(VBO, layout);
    IndexBuffer IBO(indices, 6); //索引缓冲对象

    Shader shader("res/shaders/Texture.shader");
    shader.Bind(); //创建Program后绑定
    
    //设置模型矩阵/观察矩阵/投影矩阵
    glm::mat4 model(1.0f);
    shader.SetUniformMat4f("model", model);

    glm::mat4 view(1.0f);
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    shader.SetUniformMat4f("view", view);

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);
    shader.SetUniformMat4f("projection", projection);

    // 创建并使用纹理
    Texture texture1("res/textures/container.jpg", 0);
    shader.SetUniform1i("u_Texture1", texture1.GetSlot());

    Texture texture2("res/textures/cute_cat.png", 1);
    shader.SetUniform1i("u_Texture2", texture2.GetSlot());

    // Unbind Data
    glUseProgram(0);
    glBindVertexArray(0);


    // 坐标系绘制
    VertexBufferLayout axisLayout;
    axisLayout.Push<float>(3); //位置
    axisLayout.Push<float>(3); //颜色

    VertexArray axisLineVAO;
    VertexBuffer axisLineVBO(axisLineData, 6 * 6 * sizeof(GLfloat));
    axisLineVAO.AddBuffer(axisLineVBO, axisLayout);
    axisLineVAO.Unbind();

    VertexArray axisTrsVAO;
    VertexBuffer axisTrsVBO(axisTriangleData, 12 * 6 * sizeof(GLfloat));
    axisTrsVAO.AddBuffer(axisTrsVBO, axisLayout);
    axisTrsVAO.Unbind();

    Shader axisShader("res/shaders/Axis.shader");
    axisShader.Bind(); //创建Program后绑定

    //线框模式(Wireframe Mode)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Renderer renderer; //渲染器
    while (!glfwWindowShouldClose(window))
    {
        float currTime = glfwGetTime();
        deltaTime = currTime - lastFrame;
        lastFrame = currTime;

        processInput(window, deltaTime); //键盘输入处理

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        texture1.Active();
        texture2.Active();

        //设置Uniform前需先绑定Shader
        shader.Bind();

        VAO.Bind();
        
        model = glm::mat4(1.0f); //第一象限 右上
        model = glm::translate(model, glm::vec3( 0.5f, 0.5f, 0.0f));
        shader.SetUniformMat4f("model", model);
        renderer.Draw(VAO, IBO, shader);

        model = glm::mat4(1.0f); //第二象限 右下
        model = glm::translate(model, glm::vec3( 0.5f, -0.5f, 0.0f));
        model = glm::scale(model, glm::vec3( 1.0f, -1.0f, 1.0f));
        shader.SetUniformMat4f("model", model);
        renderer.Draw(VAO, IBO, shader);

        model = glm::mat4(1.0f); //第三象限 左下
        model = glm::translate(model, glm::vec3(-0.5f, -0.5f, 0.0f));
        model = glm::scale(model, glm::vec3( -1.0f, -1.0f, 1.0f));
        shader.SetUniformMat4f("model", model);
        renderer.Draw(VAO, IBO, shader);

        model = glm::mat4(1.0f); //第四象限 左上
        model = glm::translate(model, glm::vec3(-0.5f, 0.5f, 0.0f));
        model = glm::scale(model, glm::vec3( -1.0f, 1.0f, 1.0f));
        shader.SetUniformMat4f("model", model);
        renderer.Draw(VAO, IBO, shader);

        VAO.Unbind();
        shader.Unbind();
        
        // 绘制坐标轴
		axisShader.Bind();

        axisTrsVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 9);
        axisTrsVAO.Unbind();
        
        axisLineVAO.Bind();
		glDrawArrays(GL_LINES, 0, 6);
        axisLineVAO.Unbind();
        
        axisShader.Unbind();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, float deltaTime)
{
    // 当用户按下ESC键时,设置window窗口的WindowShouldClose属性为true
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE); // 关闭应用程序
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyInput(MOVE_FORWARD, deltaTime);

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyInput(MOVE_BACKWARD, deltaTime);

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyInput(MOVE_LEFT, deltaTime);

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyInput(MOVE_RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos, lastY = ypos;

    camera.ProcessMouseMove(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffsetIn, double yoffsetIn)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffsetIn));
}

#endif