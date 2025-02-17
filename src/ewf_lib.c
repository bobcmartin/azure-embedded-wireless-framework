/************************************************************************//**
 * @file
 * @version Preview
 * @copyright Copyright (c) Microsoft Corporation. All rights reserved.
 * SPDX-License-Identifier: MIT
 * @brief The Embedded Wireless Framework library API
 ****************************************************************************/

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "ewf_lib.h"

#ifdef EWF_DEBUG
char const * _escape_str_to_str_buffer(const char * str, unsigned len)
{
    static char buffer[EWF_ESCAPE_STR_BUFFER_SIZE];
    char * guard = buffer + sizeof(buffer) - 5; /* one character may be escaped to four by using \xhh */
    char * out = buffer;
    const char * in = str;
    for ( ; *in && (out < guard) && ((len == 0) ? (true) : ((unsigned)(in - str) < len)); in++)
    {
    switch(*in)
    {
    case '\\':
        *out++ = '\\';
        *out++ = '\\';
        break;
    case '\n':
        *out++ = '\\';
        *out++ = 'n';
        break;
    case '\r':
        *out++ = '\\';
        *out++ = 'r';
        break;
    case '\t':
        *out++ = '\\';
        *out++ = 't';
        break;
    case '\v':
        *out++ = '\\';
        *out++ = 'v';
        break;
    case '\f':
        *out++ = '\\';
        *out++ = 'f';
        break;
    case '\a':
        *out++ = '\\';
        *out++ = 'a';
        break;
    case '\b':
        *out++ = '\\';
        *out++ = 'b';
        break;
    default:
        if (*in < 32) {
        *out++ = '\\';
        *out++ = 'x';
        *out++ = '0' + ((*in & 0xF0) >> 4);
        *out++ = '0' + (*in & 0xF);
        } else  {
        *out++ = *in;
        }
        break;
    }
    }
    *out = 0;
    return buffer;
}
#endif


char const * _unsigned_to_str_buffer(unsigned u)
{
    static char str[(sizeof(unsigned) <= 4) ? 11 : 20];
    return _unsigned_to_str(u, str, sizeof(str));
}


char * _unsigned_to_str(unsigned u, char * str, unsigned len)
{
    if (!str) return NULL;
    if (len < 1) return NULL;
    char * rev = str + len - 1;
    *rev = 0;
    do {
        div_t r = div(u, 10);
        u = r.quot;
        *--rev = (char)('0' + r.rem);
        if (rev < str) return NULL;
    } while (u);
    return rev;
}


unsigned _str_to_unsigned(const char * str)
{
    if (!str) return 0;
    unsigned u = 0;
    for (const char * p = str; *p && isdigit((int)*p); p++)
        u = (u * 10) + (*p - '0');
    return u;
}


unsigned _str_length(const char * str)
{
    unsigned len = 0;
    if (!str) return 0;
    while (*str++) len++;
    return len;
}


bool _str_starts_with(const char * str, const char * prefix)
{
    if (!str || !prefix) return false;
    while (*prefix) if (*prefix++ != *str++) return false;
    return true;
}


bool _buffer_ends_with(const char* buffer, unsigned buffer_length, const char* suffix, unsigned suffix_length)
{
    if (!buffer) return false;
    if (!buffer_length) return false;
    if (!suffix) return false;
    if (!suffix_length) return false;
    if (suffix_length > buffer_length) return false;
    const char* buffer_tail = &(buffer[buffer_length-1]);
    const char* suffix_tail = &(suffix[suffix_length-1]);
    for (; (buffer_tail != buffer) && (suffix_tail != suffix); buffer_tail--, suffix_tail--)
    {
        if (*buffer_tail != *suffix_tail) return false;
    }
    return true;
}


bool _buffer_ends_with_wildcard_string(const char* buffer, unsigned buffer_length, const char* suffix, unsigned suffix_length)
{
    if (!buffer) return false;
    if (!buffer_length) return false;
    if (!suffix) return false;
    if (!suffix_length) return false;
    if (suffix_length > buffer_length) return false;
    const char* buffer_tail = &(buffer[buffer_length - 1]);
    const char* suffix_tail = &(suffix[suffix_length - 1]);
    for (; (buffer_tail != buffer) && (suffix_tail != suffix); buffer_tail--, suffix_tail--)
    {
        if (*suffix_tail == '?')
        {
            // A question mark matches any character
            continue;
        }

        if (*buffer_tail != *suffix_tail) return false;
    }
    return true;
}


bool _str_equals_str(const char * str1, const char * str2)
{
    if (_str_length(str1)!= _str_length(str2)) return false;
    if (!str1 || !str2) return false;
    for (; *str1 == *str2; str1++, str2++) if (!*str1 && !*str2) return true;
    return false;
}


bool _str_contains_str(const char * str, const char * substr)
{
    while(*str)
    {
        const char *start = str;
        /* Check if starting characters match, if true check for complete match of substring */
        while((*str == *substr) && (*str && *substr) )
        {
            str++;
            substr++;
        }
        /* If substring is present, return true */
        if(!*substr)
        {
            return true;
        }
        /* Increatemt the string */
        str = start+1;
    }
    return false;
}

char * _str_n_cpy(char* dest, const char* src_str, uint32_t n)
{
    *((char*)memcpy(dest, src_str, n) + n)  = '\0';
    return dest;
}

char * _str_remove_white_spaces(char* str)
{
    while ((*str != '\0') && (isspace((int)*str)) != 0)
    {
        str++;
    }

    char* str_end = str + _str_length(str) - 1;
    while (isspace((int)*str_end))
    {
        --str_end;
    };
    *(str_end + 1) = '\0';

    return str;
}

char * _str_remove_suffix_str(char* str, const char* suffix_str)
{
    char* str_end = str + _str_length(str);
    const char* suffix_str_end = suffix_str + _str_length(suffix_str);

    while (*str_end == *suffix_str_end)
    {
        str_end--;
        suffix_str_end--;
    };
    *(str_end + 1) = '\0';

    return str;
}

char * adapter_response_str_extract(char * str)
{
    char* response_str = _str_remove_suffix_str(str, "OK\r\n");

    response_str = _str_remove_white_spaces(response_str);

    return response_str;
}
