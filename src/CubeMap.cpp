#include "CubeMap.h"
#include "Common.h"
#include "stb_image/stb_image.h"

CubeMap::CubeMap(const std::vector<const char*>& tex_faces) : m_texSlot(0)
{   
    glGenTextures(1, &m_RenderId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RenderId);
    
    unsigned char* localBuffer = nullptr;
    int width = 0, height = 0, channels = 0;
    for (size_t idx = 0; idx < tex_faces.size(); idx++)
    {
        //stbi_set_flip_vertically_on_load(true); //设置翻转
        localBuffer = stbi_load(tex_faces[idx], &width, &height, &channels, 0);
        ASSERT(localBuffer != nullptr);

        GLenum format = GetFormat(channels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + idx, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, localBuffer);

        if (localBuffer) stbi_image_free(localBuffer);
    }

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeMap::~CubeMap()
{

}

GLenum CubeMap::GetFormat(int channels)
{
    GLenum format = GL_RGB;
    if (channels == 1)
        format = GL_RED;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;

    return format;
}

void CubeMap::Bind() const
{
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RenderId));
}

void CubeMap::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

void CubeMap::Active() const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + m_texSlot));
    GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RenderId));
}