#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#ifdef APP_MAIN //启用新的main函数

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void processInput(GLFWwindow *window, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

bool stopMouseMove = false;
bool stopSelfRotate = false;

glm::vec3 lightPos(2.5f, 1.5f, 0.0f);

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
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HelloOpenGL", NULL, NULL);
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Init Error!" << std::endl;
	}

	//Blend 混合 常用来绘制透明或半透明的物体
	//glEnable(GL_BLEND); //启用混合
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST); //开启深度测试功能

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);

	std::cout << glGetString(GL_VERSION) << std::endl;
	
	Shader shader("res/shaders/Model.shader");
    shader.Bind(); //创建Program后绑定

    shader.SetUniform3f("light.position", lightPos);
    shader.SetUniform3f("light.ambient", glm::vec3(0.3f));
    shader.SetUniform3f("light.diffuse", glm::vec3(0.5f));
    shader.SetUniform3f("light.specular", glm::vec3(0.8f));
    
	Model ourModel("res/nanosuit/nanosuit.obj");
	//Model ourModel("res/miniw/cloudportal.obj");

    shader.Unbind();

	//线框模式(Wireframe Mode)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	while (!glfwWindowShouldClose(window))
	{
		float glfwCurTime = static_cast<float>(glfwGetTime());
		deltaTime = glfwCurTime - lastFrame;
		lastFrame = glfwCurTime;

		processInput(window, deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!stopSelfRotate) //点击空白键可暂停旋转
        {
            float radius = 2.15f;
            lightPos.x = sin(glfwCurTime) * radius;
            lightPos.z = cos(glfwCurTime) * radius;;
        }

		shader.Bind(); //设置Uniform前需先绑定Shader
		
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom()), aspect, 0.1f, 100.0f);
		shader.SetUniformMat4f("projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		shader.SetUniformMat4f("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(35.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		shader.SetUniformMat4f("model", model);

        shader.SetUniform3f("viewPos", camera.m_Position);
        shader.SetUniform3f("light.position", lightPos);

		ourModel.Draw(shader); //绘制模型

        shader.Unbind();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow *window, float deltaTime)
{
    static float totalTime = 0.0f;

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
            stopSelfRotate = !stopSelfRotate;    
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

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyInput(MOVE_FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyInput(MOVE_BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyInput(MOVE_LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyInput(MOVE_RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
     if (stopMouseMove) {
        camera.ProcessMouseMove(0.0f, 0.0f);
        return;
    }

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

	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMove(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

#endif