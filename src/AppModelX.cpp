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
#include "Model.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void processInput(GLFWwindow* window, float deltaTime); //键盘输入
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.5f, 3.0f));
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
    //glEnable(GL_BLEND); //启用混合
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST); //开启深度测试功能
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST); // 开启模版测试

    std::cout << glGetString(GL_VERSION) << std::endl;

    float cubeVertices[] = {
        // positions                      // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions                      // texture Coords
         5.0f, -0.5f,  5.0f,  0.0f, 0.0f, 0.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 0.0f, 0.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  0.0f, 0.0f, 0.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 0.0f, 0.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  0.0f, 0.0f, 0.0f,  2.0f, 2.0f								
    };
    
    float stencilVertices[] = {
        -1.5f,  0.5f, 0.5f, // C
        -1.5f, -0.5f, 0.5f, // D
        -0.5f, -0.5f, 0.5f, // A

        -0.5f, -0.5f, 0.5f, // A
        -0.5f,  0.5f, 0.5f,	// B
        -1.5f,  0.5f, 0.5f, // C
	};
    
    VertexBufferLayout layout;
    layout.Push<float>(3); //位置
    layout.Push<float>(3); //法线
    layout.Push<float>(2); //纹理

    VertexArray cubeVAO; //方块顶点数组对象
    VertexBuffer cubeVBO(cubeVertices, 36*8 * sizeof(float));
    cubeVAO.AddBuffer(cubeVBO, layout);
    cubeVAO.Unbind();

    VertexArray planeVAO; //地板顶点数组对象
    VertexBuffer planeVBO(planeVertices, 6*8 * sizeof(float));
    planeVAO.AddBuffer(planeVBO, layout);
    planeVAO.Unbind();

    VertexBufferLayout slayout;
	slayout.Push<float>(3); //位置
	VertexArray stencilVAO; //Stencil顶点数组
	VertexBuffer stencilVBO(stencilVertices, 6 * 5 * sizeof(float));
	stencilVAO.AddBuffer(stencilVBO, slayout);
	stencilVAO.Unbind();

	Shader stl_Shader("res/shaders/Stencil.shader");
    stl_Shader.Unbind();

    Shader shader("res/shaders/ModelX.shader");
    shader.Bind(); //创建Program后绑定

    //Model ourModel("res/miniw/cloudportal.obj");
    Model ourModel("res/nanosuit/nanosuit.obj");
    
    // 创建并使用纹理
    Texture cubeTexture("res/textures/marble.jpeg");
    Texture floorTexture("res/textures/metal.png");

    shader.SetUniform1i("texture_diffuse1", 0);
    
    // Unbind Data
    shader.Unbind();
    cubeVAO.Unbind();
    planeVAO.Unbind();

    //线框模式(Wireframe Mode)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_STENCIL_TEST);

    Renderer renderer; //渲染器
    float aspect = (float)SCR_WIDTH/(float)SCR_HEIGHT;
    glm::vec3 modelPos[] = {
        glm::vec3( 0.5f, -0.5f,  0.5f),
        glm::vec3(-1.0f,  0.0f, -1.0f),
        glm::vec3( 2.0f,  0.0f,  0.0f)
    };
    while (!glfwWindowShouldClose(window))
    {
        float currTime = glfwGetTime();
        deltaTime = currTime - lastFrame;
        lastFrame = currTime;

        processInput(window, deltaTime); //键盘输入处理

        glEnable(GL_DEPTH_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // 默认清除时写入0
    
        //模型矩阵/观察矩阵/投影矩阵
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom()), aspect, 0.1f, 100.0f);
        
        //绘制地板时禁止写入
        glStencilMask(0x00);
        // 绘制平面地板
        shader.Bind();//设置Uniform前先绑定Shader
        shader.SetUniformMat4f("view", view);
        shader.SetUniformMat4f("projection", projection);
        
        planeVAO.Bind();
        floorTexture.Active();
        shader.SetUniformMat4f("model", glm::mat4(1.0f));
        renderer.Draw(planeVAO, shader, 6);    
        planeVAO.Unbind();

        // 绘制物体模型及其轮廓
        for (int idx = 2; idx >= 0; idx--)
        {
            // 开启模版缓冲写入
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            glStencilMask(0xFF);

            if (idx > 0) //箱子模型
            {
                cubeVAO.Bind();
                cubeTexture.Active();
            }

            // 先绘制物体模型
            shader.Bind();
            model = glm::mat4(1.0f);
            model = glm::translate(model, modelPos[idx]);
            if (idx == 0) model = glm::scale(model, glm::vec3(0.1f));
            shader.SetUniformMat4f("model", model);

            if (idx == 0) 
                ourModel.Draw(shader);
            else
                renderer.Draw(cubeVAO, shader, 36);

            // 设置禁用深度及模版缓冲写入
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00); //禁止写入stencil
            glDisable(GL_DEPTH_TEST);

            // 放大模型绘制轮廓
            stl_Shader.Bind();
            if (idx == 0)
                stl_Shader.SetUniform1f("scaleFactor", 0.01f);
            else
                stl_Shader.SetUniform1f("scaleFactor", 0.00f);
            stl_Shader.SetUniformMat4f("view", view);
            stl_Shader.SetUniformMat4f("projection", projection);

            model = glm::mat4(1.0f);
            model = glm::translate(model, modelPos[idx]);
            if (idx == 0)
                model = glm::scale(model, glm::vec3(0.1f)); //保持不变 法线方向外扩
            else
                model = glm::scale(model, glm::vec3(1.02f)); //放大一点点
            stl_Shader.SetUniformMat4f("model", model);
            if (idx == 0) 
                ourModel.Draw(stl_Shader);
            else
                renderer.Draw(cubeVAO, stl_Shader, 36);
            if (idx > 0) cubeVAO.Unbind();

            glStencilMask(0xFF); //允许写入stencil
            //清空STENCIL_BUFFER 绘制重叠物体间的轮廓
            glClear(GL_STENCIL_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
        }
                
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
