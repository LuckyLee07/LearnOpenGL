#pragma warning(disable:4068)

#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RenderId);
    glBindVertexArray(m_RenderId);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RenderId);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    vb.Bind();

    unsigned int offset = 0;
    unsigned int stride = layout.GetStride();
    const auto& elements = layout.GetElements();
    for (size_t i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (const void*)(intptr_t)offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
    
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RenderId);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}
