#include "Renderer.h"
#include <iostream>

const char* gl_error_as_cstr(GLenum error)
{
    switch (error)
    {
        case GL_NO_ERROR: return "GL_NO_ERROR";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
        default: return "UNKNOWN";
    }
}

void gl_check_errors(const char* call, GLenum err, const char* file, int line)
{
    if (err != GL_NO_ERROR)
    {
        fprintf(stderr, "%s:%d: OpenGL error %s: %s\n", file, line, gl_error_as_cstr(err), call);
        abort();
    }
}

void Renderer::Clear()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader)
{
    vao.Bind();
    ibo.Bind();
    //shader.Bind();
    
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
