#pragma warning(disable:4068)

#include "VertexArray.h"
#include "Common.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RenderId));
    GLCall(glBindVertexArray(m_RenderId));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RenderId));
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
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, stride, (const void*)(intptr_t)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
    vb.Unbind();
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RenderId));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
