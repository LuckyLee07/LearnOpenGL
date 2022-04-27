#include "TestTexture2D.h"
#include "Common.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"

namespace test
{
    TestTexture2D::TestTexture2D()
        : m_TranslationA(0.0f, 0.0f, 0.0f), m_TranslationB(200.0f, 200.0f, 0.0f)
    {
        float positions[] = {
            //位置属性             //颜色属性          //纹理坐标
            0.0f,   0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // 0左下
            100.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // 1右下
            100.0f, 100.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f, // 2右上
            0.0f,   100.0f, 0.0f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f, // 3左上
        };
        unsigned int indices[] = {
            0, 1, 2, // 1
            2, 3, 0, // 2
        };
        m_VAO = std::make_unique<VertexArray>(); //顶点数组对象
        m_VBO = std::make_unique<VertexBuffer>(positions, 8 * 4 * sizeof(float));

        // VAO顶点数组对象设置属性
        VertexBufferLayout layout;
        layout.Push<float>(3); //位置
        layout.Push<float>(3); //颜色
        layout.Push<float>(2); //纹理
        m_VAO->AddBuffer(*m_VBO, layout);
        
        m_IBO = std::make_unique<IndexBuffer>(indices, 6); //索引缓冲对象
        // Shader着色器相关
        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind(); //创建Program后绑定
        m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        // 纹理Texture相关
        m_Texture = std::make_unique<Texture>("res/textures/BeatuyAvatar.png");
        m_Texture->Bind(0);
        m_Shader->SetUniform1i("u_Texture1", 0);
        
        m_VAO->Unbind();
        m_VBO->Unbind();
        m_IBO->Unbind();
        m_Texture->Unbind();
        m_Shader->Unbind();
        
        // 投影转换等数学相关
        m_proj = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
    }

    TestTexture2D::~TestTexture2D()
    {
        m_Shader->Unbind(); //这里不注释会导致退出时崩溃
    }

    void TestTexture2D::OnUpdate(float deltatime)
    {

    }

    void TestTexture2D::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        //设置Uniform前需先绑定Shader
        m_Shader->Bind();
        m_Texture->Bind(0);
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_proj * m_view * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_proj * m_view * model;
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IBO, *m_Shader);
        }
    }

    void TestTexture2D::OnImGuiRender()
    {
        ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0, 480);
        ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0, 480);

        float avgFPS = 1000.0f / ImGui::GetIO().Framerate;
        float totalFPS = ImGui::GetIO().Framerate;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", avgFPS, totalFPS);
    }
}