#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void processInput(GLFWwindow* window, float delTime); //键盘输入处理
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera(glm::vec3(1.0f, 1.5f, 5.0f), worldUp, -98.0f, -17.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool stopRotate = false;
bool stopMouseMove = false;

glm::vec3 lightPos(1.0f, -0.5f, 2.0f);

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
        //位置属性Position    //法向量Normal
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    
    VertexBuffer VBO(vertices, 36 * 5 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3); //位置
    layout.Push<float>(3); //法向量

    VertexArray cubeVAO; //顶点数组对象    
    cubeVAO.AddBuffer(VBO, layout);
    
    VertexArray lampVAO; //顶点数组对象
    lampVAO.AddBuffer(VBO, layout);

    Shader shader("res/shaders/Basic.shader");
    shader.Bind(); //创建Program后绑定

    shader.SetUniform3f("objectColor", 1.0f, 0.5f, 0.3f);
    shader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);

    //设置模型矩阵/观察矩阵/投影矩阵
    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    shader.SetUniformMat4f("model", model);
    shader.SetUniformMat4f("view", view);
    shader.SetUniformMat4f("projection", projection);
    
    // 光源着色器的设置
    Shader lampShader("res/shaders/Light.shader");
    lampShader.Bind();

    lampShader.SetUniformMat4f("model", model);
    lampShader.SetUniformMat4f("view", view);
    lampShader.SetUniformMat4f("projection", projection);

    // 集成ImGui
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    const char* glsl_version = "#version 150";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    int specularFactor = 32;

    Renderer renderer;
    float aspect = (float)SCR_WIDTH/(float)SCR_HEIGHT;
    while (!glfwWindowShouldClose(window))
    {   
        float glwfCurTime = static_cast<float>(glfwGetTime());
        deltaTime = glwfCurTime - lastFrame;
        lastFrame = glwfCurTime;
        processInput(window, deltaTime); //键盘输入处理

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom()), aspect, 0.1f, 100.0f);

        lampShader.Bind();
        if (!stopRotate) //点击空白键可暂停旋转
        {
            float radius = 1.75f;
            lightPos.x = sin(glwfCurTime) * radius;
            lightPos.z = cos(glwfCurTime) * radius;;
        }
        
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); //a smaller cube
        lampShader.SetUniformMat4f("model", model);

        lampShader.SetUniformMat4f("view", view);
        lampShader.SetUniformMat4f("projection", projection);

        renderer.Draw(lampVAO, lampShader);
        lampVAO.Unbind();
        lampShader.Unbind();

        //设置Uniform前需先绑定Shader
        shader.Bind();
        glm::vec3 viewPos = camera.m_Position;
        //shader.SetUniform3f("viewPos", viewPos.x, viewPos.y, viewPos.z);
        shader.SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);

        model = glm::mat4(1.0f);
        float angle = -5.f;//55.0f * glwfCurTime;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.SetUniformMat4f("model", model);

        shader.SetUniformMat4f("view", view);
        shader.SetUniformMat4f("projection", projection);

        shader.SetUniform1f("ambientStrength", ambientStrength);
        shader.SetUniform1f("specularStrength", specularStrength);
        shader.SetUniform1i("specularFactor", specularFactor);

        renderer.Draw(cubeVAO, shader);
        cubeVAO.Unbind();
        shader.Unbind();

        {
            ImGui::Begin("HelloOpenGL");// Create a window
            ImGui::SliderFloat("ambient_strength", &ambientStrength, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderFloat("specular_strength", &specularStrength, 0.0f, 1.0f, "ratio = %.3f");
            ImGui::SliderInt("specular_factor", &specularFactor, 1, 256, "ratio = %d");

            float avgFPS = 1000.0f / ImGui::GetIO().Framerate;
            float totalFPS = ImGui::GetIO().Framerate;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", avgFPS, totalFPS);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

static float totalTime = 0.0f;
void processInput(GLFWwindow* window, float delTime)
{   
    totalTime += deltaTime;
    // 当用户按下ESC键时,设置window窗口的WindowShouldClose属性为true
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE); // 关闭应用程序
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (totalTime > 0.5f)
        {
            totalTime = 0.0f;
            stopRotate = !stopRotate;    
        }        
    }
    else if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
    {
        if (totalTime > 0.5f)
        {
            totalTime = 0.0f;
            stopMouseMove = !stopMouseMove;    
        }
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
    if (stopMouseMove) return;
    
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