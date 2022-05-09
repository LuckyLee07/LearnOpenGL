#include "Common.h"
#include <stdio.h>
#include <string>
#include <iostream>

static int s_lLine_;
static const char* s_pFile_;
static const char* s_pFunc_;

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

bool Log_Message(const char* fmt, ...)
{
    va_list argsptr;
    // 初始化argsptr
    va_start(argsptr, fmt);

    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, argsptr);
    va_end(argsptr);

    char bufferl[2048];
    snprintf(bufferl, sizeof(bufferl), "[%s(%d)]%s(): %s", s_pFile_, s_lLine_, s_pFunc_, buffer);

    printf("%s\n", bufferl);

    return true;
}

void Log_SetParam(const char* pfile, const char* pfunc, int line)
{
    s_lLine_ = line;
    s_pFunc_ = pfunc;
    s_pFile_ = pfile;
    if (pfile == NULL) s_pFile_ = "";
    // 去掉路径截取文件信息
    if (pfile != NULL) s_pFile_ = strrchr(pfile, '/') + 1;
}

const char* g_FormatStr(const char *fmt, ...)
{
    va_list argsptr;
    // 初始化argsptr
    va_start(argsptr, fmt);

    memset(s_format, '\0', sizeof(s_format));
    vsnprintf(s_format, sizeof(s_format), fmt, argsptr);
    va_end(argsptr);

    return s_format;
}