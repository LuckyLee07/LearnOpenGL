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
#include "FrameBuffer.h"

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
    glEnable(GL_BLEND); //启用混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST); //开启深度测试功能

    std::cout << glGetString(GL_VERSION) << std::endl;

    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
    };
    // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	float quadVertices[] = {
		// positions         // texCoords
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
	};

    VertexBufferLayout layout;
    layout.Push<float>(3); //位置
    layout.Push<float>(2); //纹理

    VertexArray cubeVAO; //方块顶点数组对象
    VertexBuffer cubeVBO(cubeVertices, 36*5 * sizeof(float));
    cubeVAO.AddBuffer(cubeVBO, layout);
    cubeVAO.Unbind();

    VertexArray planeVAO; //地板顶点数组对象
    VertexBuffer planeVBO(planeVertices, 6*5 * sizeof(float));
    planeVAO.AddBuffer(planeVBO, layout);
    planeVAO.Unbind();

    VertexArray quadVAO; //Quad顶点数组
	VertexBuffer quadVBO(quadVertices, 6 * 5 * sizeof(float));
	quadVAO.AddBuffer(quadVBO, layout);
	quadVAO.Unbind();

    Shader shader("res/shaders/Advance.shader");
    Shader screenShader("res/shaders/Buffer.shader");

    // 创建并使用纹理
    Texture cubeTexture("res/textures/container.jpg");
    Texture floorTexture("res/textures/metal.png");

    shader.Bind(); //创建Program后绑定
    shader.SetUniform1i("u_Texture", 0);

    screenShader.Bind(); //创建Program后绑定
    screenShader.SetUniform1i("screenTexture", 0);

    // Unbind Data
    glUseProgram(0);

    // 创建帧缓冲
    FrameBuffer frameBuff;

    //线框模式(Wireframe Mode)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Renderer renderer; //渲染器
    float aspect = (float)SCR_WIDTH/(float)SCR_HEIGHT;
    
    while (!glfwWindowShouldClose(window))
    {
        float currTime = glfwGetTime();
        deltaTime = currTime - lastFrame;
        lastFrame = currTime;

        processInput(window, deltaTime); //键盘输入处理

        frameBuff.Bind();
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.Bind();//设置Uniform前先绑定

        //设置模型矩阵/观察矩阵/投影矩阵
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom()), aspect, 0.1f, 100.0f);

        shader.SetUniformMat4f("view", view);
        shader.SetUniformMat4f("projection", projection);
        
        // 绘制箱子模型
        cubeVAO.Bind();
        cubeTexture.Active();

        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.SetUniformMat4f("model", model);
        renderer.Draw(cubeVAO, shader, 36);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.SetUniformMat4f("model", model);
        renderer.Draw(cubeVAO, shader, 36);
        cubeVAO.Unbind();
        
        // 绘制地板平面
        planeVAO.Bind();
        floorTexture.Active();
        shader.SetUniformMat4f("model", glm::mat4(1.0f));
        renderer.Draw(planeVAO, shader, 6);    
        planeVAO.Unbind();

        shader.Unbind();
        
        frameBuff.Unbind();
		glDisable(GL_DEPTH_TEST);

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		quadVAO.Bind();
        screenShader.Bind();
		glBindTexture(GL_TEXTURE_2D, frameBuff.GetTexID());
		renderer.Draw(quadVAO, screenShader, 6);
		quadVAO.Unbind();
		screenShader.Unbind();
        
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
