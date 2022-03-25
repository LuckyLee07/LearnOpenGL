#ifndef __INDEX_BUFFER_H__
#define __INDEX_BUFFER_H__

class IndexBuffer
{
private:
    unsigned int m_RenderId;
    unsigned int m_Count;
public:
    IndexBuffer(const void* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    unsigned int GetCount() const { return m_Count; }
};

#endif