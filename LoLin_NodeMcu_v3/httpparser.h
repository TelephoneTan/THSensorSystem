#ifndef HTTPPARSER
#define HTTPPARSER

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "macros.h"
#ifdef LOGME_WINDOWS
#define CASE_INSENSITIVE_STRSTR
#define CASE_INSENSITIVE_STRSTR_WINDOWS
#elif defined(LOGME_GCC)
#define CASE_INSENSITIVE_STRSTR
#define CASE_INSENSITIVE_STRSTR_GCC
#else 
#define CASE_INSENSITIVE_STRSTR
#define CASE_INSENSITIVE_STRSTR_VUTILS
#endif // LOGME_WINDOWS

#ifdef LOGME_MSVC
#define CASE_INSENSITIVE_STRCMP
#define CASE_INSENSITIVE_STRCMP_MSVC
#elif defined(LOGME_GCC)
#define CASE_INSENSITIVE_STRCMP
#define CASE_INSENSITIVE_STRCMP_GCC
#else 
#define CASE_INSENSITIVE_STRCMP
#define CASE_INSENSITIVE_STRCMP_VUTILS
#endif // LOGME_MSVC

#include "vlist.h"

#define MAX_HTTP_HEADERS_LENGTH 28672

typedef enum HttpMethod {
	GET = 0, POST, HEAD, PUT, DELETE_, CONNECT, OPTIONS, TRACE, PATCH, INVALID_METHOD, HTTP_RESPONSE_
} HttpMethod;
HttpMethod httpMethodFromStr(const char *method_name);
const char* getConstHttpMethodNameStr(HttpMethod http_method);

typedef char(GENERATOR_FUNCTION_TYPE)(void*, int*);

typedef void GENERATOR_PARAM_TYPE;

// 此函数从头查找第一个符合的子字符串。有两种工作模式：流模式和固定字符串模式。
// 如果工作在流模式下，函数返回时，如果 call_time 不是 NULL，字符生成器调用成功的次数存储在参数 call_time 指向的变量中。
// 如果工作在固定字符串模式下，如果 call_time 不是 NULL，那么函数运行时可能会修改参数 call_time 指向的变量的值，但写入 call_time 的值没有意义。
// 返回值：
// -1 ：没找到
// -2 : 动态内存分配失败
// -3 : 字符生成器调用失败
// >=0 : 找到了。没有错误发生。流模式下返回值是字符生成器被调用的次数；固定字符串模式下返回值是找到的子字符串的最后一个字符的后一个下标（不管是否超出数组界限）
int find_sub_str(size_t max_call_time, GENERATOR_FUNCTION_TYPE* generator, GENERATOR_PARAM_TYPE* generator_param_p, const char* str, const char* pattern, size_t* call_time, char* generated_buf, size_t generated_buf_len

#ifdef CASE_INSENSITIVE_STRSTR
	, int case_sensitive
#endif // CASE_INSENSITIVE_STRSTR

);

#ifdef CASE_INSENSITIVE_STRSTR
// 从流中取出下一个 HTTP 报文（不包括 body），不合法的数据也会被从流中取出，但不合法的数据会被丢弃。提取出的报文以字符串的形式存放于 message_pp 指向的指针指向的一块内存中。
// 如果提取成功了，必须在适当的时候释放 message_pp 指向的指针指向的内存，否则会造成内存泄漏。
// 返回值：
// >= 0 : HTTP 报文的长度（不包括结尾的空字符）
// -1 : 参数不合法，method_p 是空指针或 message_pp 是空指针
// -2 : find_sub_str() 动态内存分配失败
// -3 : 字符生成器 generator 调用失败
// -4 : 已经检测到合法报文，但尝试开辟一块内存来存放报文时发生了动态内存分配失败
// 其他负数 : find_sub_str() 返回此错误码
int next_http_message(HttpMethod* method_p, char** message_pp, GENERATOR_FUNCTION_TYPE* generator, GENERATOR_PARAM_TYPE* generator_param_p, int is_response);
#endif // CASE_INSENSITIVE_STRSTR

#ifdef CASE_INSENSITIVE_STRCMP
typedef struct HttpHeader {
	VLISTNODE
	char* field;
	char* value;
} HttpHeader, KeyValuePair;
void freeHttpHeader(HttpHeader* hh);
#define freeKeyValuePair(x) freeHttpHeader(x);
typedef struct HttpMessage {
	int malloc_success;
	int success;
	char* error_name;
	char* error_reason;
	HttpMethod method;
	int http_major;
	int http_minor;
	char* url;
	char* path;
	vlist query_string;
	vlist url_fragment;
	vlist http_headers;
	long content_length;
	long status_code;
	char* location;
} HttpMessage;
// 请调用此函数来获取一个已初始化的 HttpMessage 结构体
HttpMessage makeHttpMessage();
// 当 HttpMessage 结构体不再被使用，请调用此函数来释放 HttpMessage 结构体内的动态内存
void freeHttpMessage(HttpMessage* httpmsg);
// 传入HTTP报文字符串（报文不包括 body），此函数会解析HTTP报文得到一个 HttpMessage 结构体，然后将其返回。
// 从此函数返回后，应该先检查返回的 HttpMessage 结构体中的 malloc_success 字段，如果此字段为 0，说明此结构体已损坏，
// 请不要再使用此结构体并立即使用 freeHttpMessage() 释放此结构体。
// 如果 HttpMessage 结构体未损坏，那么 success 字段代表解析成功与否（0失败，non-zero成功），如果解析失败，
// 失败的信息和详细原因存储在 error_name 和 error_reason 指向的字符串中。
// 不要更改返回的结构体中的任何指针字段（可以修改指针指向的变量的值，但不能修改指针本身），否则会造成内存泄露。
// 当返回的 HttpMessage 结构体不再被使用，请调用 freeHttpMessage() 来释放它，否则会造成内存泄漏。
HttpMessage parse_http_message(const char* message, int is_response);
#endif // CASE_INSENSITIVE_STRCMP

#ifdef __cplusplus
}
#endif

#endif // !HTTPPARSER