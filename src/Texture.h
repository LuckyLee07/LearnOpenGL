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
    
    GLint m_WrapType;
    std::string m_texType;
    unsigned int m_texSlot;

public:
    Texture() {}
    Texture(const std::string& path);
    Texture(const std::string& path, int wrapType);
    Texture(const std::string& path, const char* cType);

    ~Texture();

    void Delete();//Texture是uniform不能直接删
    bool Initialize(const std::string& path);

    void Bind() const;
    void Unbind() const;
    void Active() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

    const char* GetType() { return m_texType.c_str(); }

    void SetSlot(unsigned int slot) { m_texSlot = slot; }
    inline unsigned int GetSlot() const { return m_texSlot; }
};

#endif