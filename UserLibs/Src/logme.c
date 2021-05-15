#include "logme.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "macros.h"

#ifdef LOGME_WINDOWS // 用这个宏来判断是否是 windows 平台

#include <windows.h>

#define GREEN FOREGROUND_GREEN
#define YELLOW 6
#define RED FOREGROUND_RED
#define BLUE 3
#define WHITE 7
#define NORMAL_WHILE WHITE
#define NORMAL NORMAL_WHILE
#define LINE "\n"

#elif defined(V_BARE_METAL)

#define GREEN "I"
#define YELLOW "W"
#define RED "E"
#define BLUE "B"
#define NORMAL "N"
#define LINE "\n"
#define DELIMITER "/"

#else

#define GREEN "\x1B[1;32m"
#define YELLOW "\x1B[1;33m"
#define RED "\x1B[1;31m"
#define BLUE "\x1B[1;34m"
#define NORMAL "\x1B[0m"
#define LINE "\n"

#endif

static void* malloc_n(size_t n) {
    void* res = malloc(n);
    if (res != NULL)
    {
        memset(res, 0, n);
    }
    return res;
}

static char* line(const char* s) {
    char* res = malloc_n(strlen(s) + strlen(LINE) + 1);
    if (!res)
    {
        return NULL;
    }

    strcat(res, s);
    strcat(res, LINE);

    return res;
}

#ifdef LOGME_WINDOWS

static void set_console_text_color(WORD color) {
    HANDLE consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(consolehwnd, color);
    // 这里为什么不需要 CloseHandle ？因为 GetStdHandle 返回的 Handle 由系统持有。
    // CloseHandle(consolehwnd);
}

static void reset_console_text_color() {
    set_console_text_color(NORMAL);
}

static volatile HANDLE l_mutex = NULL;

static void l(const char* text, WORD color, va_list valist_list, ...) {
    if (l_mutex == NULL)
    {
        va_list empty_valist;
        va_start(empty_valist, valist_list);
        vprintf("Please call logme_init() from single thread to init LogMe first.\n", empty_valist);
        va_end(empty_valist);
        return;
    }
    while (WaitForSingleObject(l_mutex, INFINITE) == WAIT_FAILED);
    const char* text_line = line(text);
    text_line = text_line ? text_line : text;
    set_console_text_color(color);
    vprintf(text_line, valist_list);
    fflush(stdout);
    reset_console_text_color();
    text_line == text ? 0 : free(text_line);
    while (!ReleaseMutex(l_mutex));
}

#elif defined(V_BARE_METAL)

static const char* beautify(const char* s, const char* color) {
    char* res = malloc_n(strlen(s) + strlen(color) + strlen(DELIMITER) + 1);
    if (!res)
    {
        return NULL;
    }

    strcat(res, color);
    strcat(res, DELIMITER);
    strcat(res, s);

    return res;
}

static volatile int prepared = 0;

static void l(const char* text, const char* color, va_list valist_list, ...) {
    if (!prepared)
    {
        va_list empty_valist;
        va_start(empty_valist, valist_list);
        logme_vprintf("Please call logme_init() from single thread to init LogMe first.\n", empty_valist);
        va_end(empty_valist);
        return;
    }
    const char* bs = beautify(text, color);
    bs = bs ? bs : text;
    const char* bs_line = line(bs);
    bs_line = bs_line ? bs_line : bs;
    logme_vprintf(bs_line, valist_list);
    bs_line == bs ? 0 : free(bs_line);
    bs == text ? 0 : free(bs);
}

#else

static const char* beautify(const char* s, const char* color) {
    char* res = malloc_n(strlen(s) + strlen(color) + strlen(NORMAL) + 1);
    if (!res)
    {
        return NULL;
    }

    strcat(res, color);
    strcat(res, s);
    strcat(res, NORMAL);

    return res;
}

static void l(const char* text, const char* color, va_list valist_list) {
    const char* bs = beautify(text, color);
    bs = bs ? bs : text;
    const char* bs_line = line(bs);
    bs_line = bs_line ? bs_line : bs;
    vprintf(bs_line, valist_list);
    bs_line == bs ? 0 : free(bs_line);
    bs == text ? 0 : free(bs);
}

#endif

void logme_init() {
#ifdef LOGME_WINDOWS

    while (l_mutex == NULL)
    {
        l_mutex = CreateMutex(
            NULL,               // default security attributes
            0,                  // initially not owned
            NULL                // unnamed mutex
        );
    }

#elif defined(V_BARE_METAL)

    if (!prepared)
    {
        while (!logme_prepare());
        prepared = 1;
    }

#endif // LOGME_WINDOWS
}

static void log_me_i__(const char* text, ...) {
    va_list valist_list;
    va_start(valist_list, text);
    l(text, GREEN, valist_list);
    va_end(valist_list);
}
static void log_me_w__(const char* text, ...) {
    va_list valist_list;
    va_start(valist_list, text);
    l(text, YELLOW, valist_list);
    va_end(valist_list);
}
static void log_me_e__(const char* text, ...) {
    va_list valist_list;
    va_start(valist_list, text);
    l(text, RED, valist_list);
    va_end(valist_list);
}
static void log_me_n__(const char* text, ...) {
    va_list valist_list;
    va_start(valist_list, text);
    l(text, NORMAL, valist_list);
    va_end(valist_list);
}
static void log_me_b__(const char* text, ...) {
    va_list valist_list;
    va_start(valist_list, text);
    l(text, BLUE, valist_list);
    va_end(valist_list);
}

static void format_time(char* output, size_t len) {

#ifdef V_BARE_METAL
    long long
#else
    time_t
#endif // V_BARE_METAL
        rawtime;

    struct tm timeinfo;

#ifdef V_BARE_METAL
    rawtime = logme_get_time();
#else
    time(&rawtime);
#endif // V_BARE_METAL

#if defined(V_MSVC) && defined(V_WINDOWS)
    localtime_s(&timeinfo, &rawtime);

    snprintf(output, len, "[ %04d-%02d-%02d %02d:%02d:%02d ]",
        timeinfo.tm_year + 1900,
        timeinfo.tm_mon + 1,
        timeinfo.tm_mday,
        timeinfo.tm_hour,
        timeinfo.tm_min,
        timeinfo.tm_sec);
#else
    snprintf(output, len, "[ %lld ]", (long long)rawtime);
#endif // defined(V_MSVC) && defined(V_WINDOWS)
}

static char* with_time(const char *s) {
    char time[50];
    format_time(time, 50);

    const char* d = " ";
    const char* prefix = " ";
    const char* suffix = "";

    char* res = malloc_n(strlen(s) + strlen(time) + strlen(d) + strlen(prefix) + strlen(suffix) + 1);
    if (!res)
    {
        return NULL;
    }

    strcat(res, prefix);
    strcat(res, time);
    strcat(res, d);
    strcat(res, s);
    strcat(res, suffix);

    return res;
}

static void log_me_it__(const char* text, ...) {
    char* tt = with_time(text);
    tt = tt ? tt : text;
    va_list valist_list;
    va_start(valist_list, text);
    l(tt, GREEN, valist_list);
    va_end(valist_list);
    tt == text ? 0 : free(tt);
}
static void log_me_wt__(const char* text, ...) {
    char* tt = with_time(text);
    tt = tt ? tt : text;
    va_list valist_list;
    va_start(valist_list, text);
    l(tt, YELLOW, valist_list);
    va_end(valist_list);
    tt == text ? 0 : free(tt);
}
static void log_me_et__(const char* text, ...) {
    char* tt = with_time(text);
    tt = tt ? tt : text;
    va_list valist_list;
    va_start(valist_list, text);
    l(tt, RED, valist_list);
    va_end(valist_list);
    tt == text ? 0 : free(tt);
}
static void log_me_nt__(const char* text, ...) {
    char* tt = with_time(text);
    tt = tt ? tt : text;
    va_list valist_list;
    va_start(valist_list, text);
    l(tt, NORMAL, valist_list);
    va_end(valist_list);
    tt == text ? 0 : free(tt);
}
static void log_me_bt__(const char* text, ...) {
    char* tt = with_time(text);
    tt = tt ? tt : text;
    va_list valist_list;
    va_start(valist_list, text);
    l(tt, BLUE, valist_list);
    va_end(valist_list);
    tt == text ? 0 : free(tt);
}

const struct LogMe LogMe = { 
    log_me_i__, 
    log_me_w__, 
    log_me_e__, 
    log_me_n__,
    log_me_b__,
    log_me_it__,
    log_me_wt__,
    log_me_et__,
    log_me_nt__,
    log_me_bt__
};