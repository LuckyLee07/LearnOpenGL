#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <string>
#include "Common.h"

class Texture
{
private:
    unsigned int m_RenderId;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
    unsigned int m_slot;
    std::string m_type;

public:
    Texture() {}
    Texture(const std::string& path, const char* cType);
    ~Texture();

    void Bind(unsigned int slot = 0);
    void Unbind() const;
    void Active() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

    const char* GetType() { return m_type.c_str(); }
};

#endif