#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

class VertexBuffer
{
private:
    unsigned int m_RenderId;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    void Delete() const;//uniform类型不能直接删
};

#endif