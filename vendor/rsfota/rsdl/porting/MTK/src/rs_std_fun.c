#include "rs_std_fun.h"
#include "rs_datatype.h"
#include "string.h"
#include "stdarg.h"
#include "stdlib.h"

const rs_s8* rs_strstr(const rs_s8* src, const rs_s8* sub)
{
	return strstr(src, sub);
}

rs_s32 rs_vsnprintf(rs_s8 * buf, rs_u32 bufSize, const rs_s8 * format, va_list arglist)
{
	return vsnprintf(buf, bufSize, format, arglist);
}

rs_s8* rs_strcpy(char* dest, const char* src)
{
	return strcpy(dest, src);
}

rs_s8* rs_strcat(char* dest, const char* src)
{
	return strcat(dest, src);
}

rs_u32 rs_strlen(const char* src)
{
	return strlen(src);
}

rs_s32 rs_strcmp(const char* str1, const char* str2)
{
	return strcmp(str1, str2);
}

const rs_s8* rs_strchr(const rs_s8* src, int chr)
{
	return strchr(src, chr);
}

void* rs_memcpy(void* dest, const void* src, rs_u32 size)
{
	return memcpy(dest, src, size);
}

void* rs_memset(void * dest, rs_s32 val, rs_u32 size)
{
	return memset(dest, val, size);
}

rs_s32 rs_memcmp(const void* src, const void* dest, rs_s32 size)
{
	return memcmp(src, dest, size);
}


rs_s32 rs_sprintf(rs_s8 * buf, const char * format, ...)
{
    rs_s32 ret;
    va_list args ;

    va_start (args, format) ;
    ret = vsprintf (buf, format, args) ;
    va_end (args) ;

    return ret;
}

rs_s32 rs_atoi(const char* str)
{
	return atoi(str);
}

