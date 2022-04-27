#ifndef __COMMON_H__
#define __COMMON_H__

#include <string.h>
#include <GL/glew.h>

#define ASSERT(x) if (!(x)) assert(false);
#define __FILENAME__ strrchr(__FILE__,'/')+1

extern const char* gl_error_as_cstr(GLenum error);
extern void gl_check_errors(const char* call, GLenum err, const char* file, int line);

#define GLCall(call) \
    call;       \
    gl_check_errors(#call, glGetError(), __FILENAME__, __LINE__);

// 优化日志打印
extern void LOG_INFO(const char *fmt, ...);

#endif