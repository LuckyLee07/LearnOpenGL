#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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

    //std::cout << "Flag:" << glfwIsCompositionEnabled() << std::endl;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW Init Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // 0
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f, // 3
    };

    unsigned int indices[] = {
        0, 1, 2, // 1
        2, 3, 0, // 2
    };

    //VAO(vertex Array Object)：顶点数组对象
    //VBO(vertex Buffer Object)：顶点缓冲对象
    //IBO(index Buffer Object)：索引缓冲对象

    VertexArray vao; //顶点数组对象

    VertexBuffer vbo(positions, 8 * 6 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(3); //位置
    layout.Push<float>(3); //颜色
    vao.AddBuffer(vbo, layout);
    
    IndexBuffer ibo(indices, 6); //索引缓冲对象

    Shader shader("res/shaders/Basic.shader");
    shader.Bind(); //创建Program后绑定
    
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

    // Unbind Data
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Bind(); //设置Uniform前需先绑定Shader
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

        vao.Bind();
        ibo.Bind();

        //glDrawArrays(GL_TRIANGLES, 0, 6);
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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