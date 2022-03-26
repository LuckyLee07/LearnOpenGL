#include "Texture.h"
#include "Renderer.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
    : m_RenderId(0), m_FilePath(path), m_LocalBuffer(nullptr),
    m_Width(0), m_Height(0), m_BPP(0)
{
    stbi_set_flip_vertically_on_load(1); //设置翻转
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    ASSERT(m_LocalBuffer != nullptr);

    GLCall(glGenTextures(1, &m_RenderId));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderId));

    //为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //internalformat和format可以理解为纹理数据分别在显存和内存中存储纹理数据的格式 可选为GL_ALPHA，GL_RGB，GL_RGBA等
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
    glBindTexture(GL_TEXTURE_2D, m_RenderId);

    if (m_LocalBuffer) stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_RenderId));
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RenderId);
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}