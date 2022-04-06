#ifndef __TEST_TEXTURE2D_H__
#define __TEST_TEXTURE2D_H__

#include "Test.h"
#include "glm/glm.hpp"
#include <memory>
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace test
{
    class TestTexture2D : public Test
    {
    public:
        TestTexture2D();
        virtual ~TestTexture2D();

        void OnUpdate(float deltatime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        glm::mat4 m_proj;
        glm::mat4 m_view;

        glm::vec3 m_TranslationA;
        glm::vec3 m_TranslationB;

        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VBO;
        std::unique_ptr<IndexBuffer> m_IBO;
        std::unique_ptr<Texture> m_Texture;
    };
}

#endif