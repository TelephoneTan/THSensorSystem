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
// 此函数执行 LogMe 所依赖的外部模块初始化。
// 例如，如果在 logme_vprintf() 中使用串口输出，那么显然在使用 LogMe 前应初始化串口。
// 这种情况下，你可以将串口初始化代码放在 logme_prepare() 中，因为 logme_init() 会调用 logme_prepare()。
// logme_prepare() 的返回值指示外部模块是否初始化成功（0失败，非零值成功）。
// logme_init() 会循环调用 logme_prepare() 直至 logme_prepare() 返回非零值。
// 如果不需要初始化外部模块，请在 logme_prepare() 中直接返回非零值。
int logme_prepare();
#endif // V_BARE_METAL

#endif // LOGME