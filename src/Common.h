#ifndef __COMMON_H__
#define __COMMON_H__

#include <string.h>
#include <GL/glew.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

#define ASSERT(x) if (!(x)) assert(false);
#define __FILENAME__ strrchr(__FILE__,'/')+1
// GL报错信息打印
extern const char* gl_error_as_cstr(GLenum error);
extern void gl_check_errors(const char* call, GLenum err, const char* file, int line);

#define GLCall(call) \
    call;       \
    gl_check_errors(#call, glGetError(), __FILENAME__, __LINE__);

// 优化日志打印系统
extern bool Log_Message(const char *fmt, ...);
extern void Log_SetParam(const char* pfile, const char* pfunc, int line);

#define LOG_INFO (Log_SetParam(__FILE__, __FUNCTION__, __LINE__), Log_Message)

// 格式化输出字符串
static char s_format[1024] = { 0 };
const char* g_FormatStr(const char *fmt, ...);

#endif