#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <GL/glew.h>

//#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();(x);\
    assert(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* func, const char* file, int line);

#endif