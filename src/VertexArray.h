#ifndef __VERTEX_ARRAY_H__
#define __VERTEX_ARRAY_H__

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
    unsigned int m_RenderId;
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;

    void Delete() const;//uniform类型不能直接删

    unsigned int GetID() const { return m_RenderId; }
};

#endif