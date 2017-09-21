#ifndef RS_MEM_H_
#define RS_MEM_H_
#include "rs_datatype.h"

void* rs_malloc_porting(rs_u32 allocSize);

void  rs_free_porting(void* memBlock);
#endif
