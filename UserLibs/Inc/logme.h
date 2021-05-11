#ifndef LOGME
#define LOGME

#include <macros.h>
#include <stdarg.h>

typedef void LOG_FUNCTION_TYPE(const char* text, ...);

extern const struct LogMe
{
    LOG_FUNCTION_TYPE* i;
    LOG_FUNCTION_TYPE* w;
    LOG_FUNCTION_TYPE* e;
    LOG_FUNCTION_TYPE* n;
    LOG_FUNCTION_TYPE* b;

    LOG_FUNCTION_TYPE* it;
    LOG_FUNCTION_TYPE* wt;
    LOG_FUNCTION_TYPE* et;
    LOG_FUNCTION_TYPE* nt;
    LOG_FUNCTION_TYPE* bt;
} LogMe;

// avoid calling this function from multiple threads
void logme_init();

#ifdef V_BARE_METAL
// 此函数输出格式化后的字符串。
void logme_vprintf(const char* restrict format, va_list vlist);
// 此函数返回当前时间值，用作日志中的时间标记。单位自定，含义自定。
long long logme_get_time();
#endif // V_BARE_METAL

#endif // LOGME