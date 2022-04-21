#include "Texture.h"
#include "Renderer.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path)
    : m_RenderId(0), m_FilePath(path), m_LocalBuffer(nullptr),
    m_Width(0), m_Height(0), m_BPP(0), m_slot(0)
{
    stbi_set_flip_vertically_on_load(true); //设置翻转
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
    ASSERT(m_LocalBuffer != nullptr);

    GLCall(glGenTextures(1, &m_RenderId));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderId));

    //为当前绑定的纹理对象设置环绕、过滤方式
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    float borderColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    //internalformat和format可以理解为纹理数据分别在显存和内存中存储纹理数据的格式 可选为GL_ALPHA，GL_RGB，GL_RGBA等
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderId));

    if (m_LocalBuffer) stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_RenderId));
}

void Texture::Bind(unsigned int slot)
{
    m_slot = slot;
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderId));
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