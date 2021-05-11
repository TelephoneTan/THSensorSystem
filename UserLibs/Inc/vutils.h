#ifndef VUTILS
#define VUTILS

#include <stddef.h>

#include <macros.h>

char* vitoa(int i, char* s, size_t len);

void* zero_malloc(size_t n);

int is_uri_reserved_character(char ch);

int is_uri_unreserved_character(char ch);

void url_encode(const char* utf8_byte_array, size_t encode_len, char* encoded_str_buf, size_t buf_len, int is_bin);

char* vstrstr(const char* haystack, const char* needle, int case_sensitive, int* success);

int vstrcmp(const char* s1, const char* s2, int case_sensitive, int* success);

#ifdef LOGME_WINDOWS
#include <uchar.h>
int test_wide_char_num_of_utf8_including_wide_null(const char *utf8str);
int utf8_to_utf16_must_have_sufficient_buffer_including_wide_null(const char* utf8str, char16_t* char16buf, int buf_len_wide_char_num);
#endif // LOGME_WINDOWS

#endif // !VUTILS
