#include "Common.h"
#include <stdio.h>
#include <string>
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

void LOG_INFO(const char *fmt, ...)
{
    // 定义两个va_list 类型的变量，这种变量可以用来处理变长参数：...
    va_list args, args1;            

    // 初始化args
    va_start(args, fmt);

    // args1 是 args 的一个拷贝
    va_copy(args1, args);

    // 使用nullptr和0作为前两个参数来获取格式化这个变长参数列表所需要的字符串长度
    // 使用 string(size_t n, char c) 构造函数，构造一个长度为n的字符串，内容为n个c的拷贝
    std::string res(1 + vsnprintf(nullptr, 0, fmt, args1), 0);
    // args1 任务完成，将其关闭，清理。
    va_end(args1);

    // 使用args来格式化要返回的字符串res， 指定长度size
    vsnprintf(&res[0], res.size(), fmt, args);
    // args 任务完成，关闭，清理
    va_end(args);

    std::cout << res << std::endl;
}