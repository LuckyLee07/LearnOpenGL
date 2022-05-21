#ifndef __CUBE_MAP_H__
#define __CUBE_MAP_H__

#include <vector>
#include <GL/glew.h>

class CubeMap
{
private:
    unsigned int m_RenderId;
    unsigned int m_texSlot;
public:
    CubeMap(const std::vector<const char*>& tex_faces);
    ~CubeMap();

    static GLenum GetFormat(int channels);

    void Bind() const;
    void Unbind() const;
    void Active() const;

    void SetSlot(unsigned int slot) { m_texSlot = slot; }
    inline unsigned int GetSlot() const { return m_texSlot; }
};

#endif