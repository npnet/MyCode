/*
 *
 * 版权(c) 2015 红石阳光（北京）科技有限公司
 * 版权(c) 2011-2015 红石阳光（北京）科技有限公司版权所有
 * 
 * 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
 * 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
 * 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
 *
 */

#ifndef _RS_DATATYPE_H_
#define _RS_DATATYPE_H_

#ifndef __RSPLATFORM_H__
#define __RSPLATFORM_H__

typedef char				rs_s8;
typedef short				rs_s16;
typedef long				rs_s32;
typedef unsigned char		rs_u8;
typedef unsigned short		rs_u16;
typedef unsigned long		rs_u32;
typedef int					rs_bool;
typedef char				rs_char;
typedef void				rs_void;
typedef void*				rs_ptr;
typedef const void*			rs_cptr;

#endif



#ifndef RS_NULL
#define RS_NULL            0
#endif


#ifndef RS_TRUE
#define RS_TRUE            1
#endif

#ifndef RS_FALSE
#define RS_FALSE           0
#endif

#endif