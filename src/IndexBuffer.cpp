#include "IndexBuffer.h"
#include "Common.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
    : m_Count(count)
{
    glGenBuffers(1, &m_RenderId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    //glDeleteBuffers(1, &m_RenderId);
}

void IndexBuffer::Delete() const
{
    glDeleteBuffers(1, &m_RenderId);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderId);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
