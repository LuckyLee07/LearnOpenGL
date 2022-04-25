#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define __FILENAME__ strrchr(__FILE__,'/')+1
#define ASSERT(x) if (!(x)) assert(false);

void gl_check_errors(const char* call, GLenum err, const char* file, int line);

#define GLCall(call) \
    call;       \
    gl_check_errors(#call, glGetError(), __FILENAME__, __LINE__);

class Renderer
{
public:
    void Clear();
    void Draw(const VertexArray& vao, const Shader& shader);
    void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader);
};

#endif