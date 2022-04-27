#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
public:
    void Clear();
    void Draw(const VertexArray& vao, const Shader& shader);
    void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader);
};

#endif