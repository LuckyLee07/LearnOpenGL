#include "Texture.h"
#include <iostream>
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path, const char* cType)
    : m_RenderId(0), m_FilePath(path), m_LocalBuffer(nullptr),
    m_Width(0), m_Height(0), m_BPP(0), m_slot(0)
{
    if (cType != nullptr) m_type = cType;
    
    stbi_set_flip_vertically_on_load(true); //设置翻转
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0);
    
    if (m_LocalBuffer == nullptr)
    {
        LOG_INFO("Load texture failed ===>> %s", path.c_str());
        ASSERT(m_LocalBuffer != nullptr);    
    }

    GLenum format = GL_RGB;
    if (m_BPP == 1)
        format = GL_RED;
    else if (m_BPP == 3)
        format = GL_RGB;
    else if (m_BPP == 4)
        format = GL_RGBA;

    GLCall(glGenTextures(1, &m_RenderId));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderId));

    //为当前绑定的纹理对象设置环绕、过滤方式
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    //float borderColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
    
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    //internalformat和format可以理解为纹理数据分别在显存和内存中存储纹理数据的格式 可选为GL_ALPHA，GL_RGB，GL_RGBA等
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    
    if (m_LocalBuffer) stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
    //GLCall(glDeleteTextures(1, &m_RenderId));
}

void Texture::Bind(unsigned int slot)
{
    m_slot = slot;
    //GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderId));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Active() const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + m_slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderId));
}
