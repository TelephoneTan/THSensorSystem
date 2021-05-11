#include "vutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <macros.h>

char* vitoa(int i, char* s, size_t len) {
    snprintf(s, len, "%d", i);
    return s;
}

void* zero_malloc(size_t n) {
    void* res = malloc(n);
    if (res != NULL)
    {
        memset(res, 0, n);
    }
    return res;
}

int is_uri_reserved_character(char ch) {
    return
            ch == '!' ||
            ch == '#' ||
            ch == '$' ||
            ch == '&' ||
            ch == '\'' ||
            ch == '(' ||
            ch == ')' ||
            ch == '*' ||
            ch == '+' ||
            ch == ',' ||
            ch == '/' ||
            ch == ':' ||
            ch == ';' ||
            ch == '=' ||
            ch == '\?' ||
            ch == '@' ||
            ch == '[' ||
            ch == ']'
            ;
}

int is_uri_unreserved_character(char ch) {
    return
            ch == 'A' ||
            ch == 'B' ||
            ch == 'C' ||
            ch == 'D' ||
            ch == 'E' ||
            ch == 'F' ||
            ch == 'G' ||
            ch == 'H' ||
            ch == 'I' ||
            ch == 'J' ||
            ch == 'K' ||
            ch == 'L' ||
            ch == 'M' ||
            ch == 'N' ||
            ch == 'O' ||
            ch == 'P' ||
            ch == 'Q' ||
            ch == 'R' ||
            ch == 'S' ||
            ch == 'T' ||
            ch == 'U' ||
            ch == 'V' ||
            ch == 'W' ||
            ch == 'X' ||
            ch == 'Y' ||
            ch == 'Z' ||
            ch == 'a' ||
            ch == 'b' ||
            ch == 'c' ||
            ch == 'd' ||
            ch == 'e' ||
            ch == 'f' ||
            ch == 'g' ||
            ch == 'h' ||
            ch == 'i' ||
            ch == 'j' ||
            ch == 'k' ||
            ch == 'l' ||
            ch == 'm' ||
            ch == 'n' ||
            ch == 'o' ||
            ch == 'p' ||
            ch == 'q' ||
            ch == 'r' ||
            ch == 's' ||
            ch == 't' ||
            ch == 'u' ||
            ch == 'v' ||
            ch == 'w' ||
            ch == 'x' ||
            ch == 'y' ||
            ch == 'z' ||
            ch == '0' ||
            ch == '1' ||
            ch == '2' ||
            ch == '3' ||
            ch == '4' ||
            ch == '5' ||
            ch == '6' ||
            ch == '7' ||
            ch == '8' ||
            ch == '9' ||
            ch == '-' ||
            ch == '_' ||
            ch == '.' ||
            ch == '~'
            ;
}

void url_encode(const char* utf8_byte_array, size_t encode_len, char* encoded_str_buf, size_t buf_len, int is_bin) {
    int is_urc;
    while (
            encode_len-- > 0 &&
            (is_urc = (!is_bin && is_uri_unreserved_character(*utf8_byte_array)), buf_len >= (is_urc ? 2 : 4))
            )
    {
        snprintf(encoded_str_buf, (is_urc ? 2 : 4), (is_urc ? "%c" : "%%%02X"), *(utf8_byte_array++) & 0xff);
        encoded_str_buf += (is_urc ? 1 : 3); buf_len -= (is_urc ? 1 : 3);
    }
    if (buf_len > 0)
    {
        *encoded_str_buf = '\0';
    }
}

char* vstrstr(const char* haystack, const char* needle, int case_sensitive, int *success) {
    char *haystack_m, *needle_m;
    if (!case_sensitive)
    {
        haystack_m = zero_malloc(strlen(haystack) + 1);
        needle_m = zero_malloc(strlen(needle) + 1);
        if (!haystack_m || !needle_m)
        {
            *success = 0;
            return NULL;
        }
        memcpy(haystack_m, haystack, strlen(haystack));
        memcpy(needle_m, needle, strlen(needle));
        for (int i = 0; i < strlen(haystack_m); i++)
        {
            haystack_m[i] = toupper(haystack_m[i]);
        }
        for (int i = 0; i < strlen(needle_m); i++)
        {
            needle_m[i] = toupper(needle_m[i]);
        }
    }
    else
    {
        haystack_m = haystack;
        needle_m = needle;
    }
    *success = 1;
    char* temp = strstr(haystack_m, needle_m);
    char* res = temp ? haystack + (temp - haystack_m) : temp;
    if (!case_sensitive)
    {
        free(haystack_m); haystack_m = NULL;
        free(needle_m); needle_m = NULL;
    }
    return res;
}

int vstrcmp(const char* s1, const char* s2, int case_sensitive, int* success) {
    char* s1m, * s2m;
    if (!case_sensitive)
    {
        s1m = zero_malloc(strlen(s1) + 1);
        s2m = zero_malloc(strlen(s2) + 1);
        if (!s1m || !s2m)
        {
            *success = 0;
            return -1;
        }
        memcpy(s1m, s1, strlen(s1));
        memcpy(s2m, s2, strlen(s2));
        for (int i = 0; i < strlen(s1m); i++)
        {
            s1m[i] = toupper(s1m[i]);
        }
        for (int i = 0; i < strlen(s2m); i++)
        {
            s2m[i] = toupper(s2m[i]);
        }
    }
    else
    {
        s1m = s1;
        s2m = s2;
    }
    *success = 1;
    int res = strcmp(s1m, s2m);
    if (!case_sensitive)
    {
        free(s1m); s1m = NULL;
        free(s2m); s2m = NULL;
    }
    return res;
}

#ifdef LOGME_WINDOWS
#include<windows.h>
int test_wide_char_num_of_utf8_including_wide_null(const char* utf8str) {
	return MultiByteToWideChar(65001, 0, utf8str, -1, NULL, 0);
}
int utf8_to_utf16_must_have_sufficient_buffer_including_wide_null(const char* utf8str, char16_t* char16buf, int buf_len_wide_char_num) {
	return MultiByteToWideChar(65001, 0, utf8str, -1, char16buf, buf_len_wide_char_num);
}
#endif // LOGME_WINDOWS