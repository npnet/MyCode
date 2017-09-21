#ifndef _RS_STD_FUN_H_
#define _RS_STD_FUN_H_

#include "rs_datatype.h"
#include "stdio.h"
#include "stdarg.h"

const rs_s8* rs_strstr(const rs_s8* src, const rs_s8* sub);

rs_s32 rs_vsnprintf(rs_s8 * buf, rs_u32 bufSize, const rs_s8 * format, va_list arglist);

rs_s8* rs_strcpy(char* dest, const char* src);

rs_s8* rs_strcat(char* dest, const char* src);

rs_u32 rs_strlen(const char* src);

const rs_s8* rs_strchr(const rs_s8* src, int chr);

rs_s32 rs_strcmp(const char* str1, const char* str2);

void* rs_memcpy(void* dest, const void* src, rs_u32 size);

void* rs_memset(void * dest, rs_s32 val, rs_u32 size);

rs_s32 rs_memcmp(const void* src, const void* dest, rs_s32 size);

rs_s32 rs_sprintf(rs_s8 * buf, const char * format, ...);

rs_s32 rs_atoi(const char* str);


#endif