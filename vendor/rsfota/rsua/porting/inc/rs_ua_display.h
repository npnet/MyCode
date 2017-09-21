/*
 *
 * 版权(c) 红石阳光（北京）科技有限公司
 * 版权(c) 红石阳光（北京）科技有限公司版权所有
 * 
 * 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
 * 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
 * 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
 *
 */

#ifndef __RS_FOTA_DISPLAY_H__
#define __RS_FOTA_DISPLAY_H__

#if defined(__MTK_TARGET__)
#include "drv_comm.h"
#include "kal_release.h"
//#include "lcd_if_hw.h"
#include "lcd_if.h"
//#include "lcd_sw.h"		///Bin: removed for unnecessary inclusion
//#include "lcd_if_manager\inc\lcd_if_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
 LCD_UPDATE_BLT = 0,
 LCD_UPDATE_FLT,
 LCD_UPDATE_BLT_AND_FLT,
 LCD_UPDATE_NONE = 0xF 
}LCD_UPDATE_ACT_ENUM;


typedef struct
{
   kal_bool    layer_update_queue;     /* lcd layer parameter queue is full or not */
   kal_bool    source_key_enable;      /* enable/disable source key for specified layer */
   kal_bool    color_palette_enable;   /* enable/disable color palette for specified layer */
   kal_bool    opacity_enable;         /* enable/disable opacity for specified layer */
  
   kal_uint8   source_color_format;    /* color format of the specified layer*/ //LCD_LAYER_SOURCE_COLOR_FORMAT_ENUM
   kal_uint8   color_palette_select;   /* selection of color palette table */
   kal_uint8   opacity_value;          /* opacity value for specified layer */   
   kal_uint8   rotate_value;           /* rotate select for specified layer */ //LCD_LAYER_ROTATE_ENUM
   kal_uint16  x_offset;               /* x axis offset from main window for specified layer */
   kal_uint16  y_offset;               /* y axis offset from main widnow for specified layer */
   kal_uint16  row_number;             /* layer buffer height of specified layer */
   kal_uint16  column_number;          /* layer buffer width of specified layer */
   kal_uint32  source_key;             /* source key color in RGB565 format for specified layer */
   kal_uint32  frame_buffer_address;   /* frame buffer start address of specified layer */

   // For MT6236
   kal_bool    dst_key_enable;         /* enable/disable source key for specified layer */
   kal_bool    swap_enable;          /* enable/disable source key for specified layer */
   kal_bool    lgamma_enable;          /* enable/disable source key for specified layer */
   kal_bool    dither_enable;          /* enable/disable source key for specified layer */
   kal_uint16  mem_x_offset;           /* x axis offset from mem space window for specified layer */
   kal_uint16  mem_y_offset;           /* y axis offset from mem space window for specified layer */
   kal_uint16  mem_pitch;              /* pitch of mem space window for specified layer */
} ddv2_lcd_layer_para_struct;


typedef struct
{ 
   kal_uint8   lcd_id; /// which lcd will be updated (MAIN_LCD or SUB_LCD)
   LCD_UPDATE_ACT_ENUM update; 
   kal_bool    block_mode_flag; /// block caller or not   
   void        (*lcd_block_mode_cb)(void); /// callback when lcd update is done
   
   kal_uint16  tgt_start_x; /// the starting x coordinate of LCM to be updated    
   kal_uint16  tgt_start_y; /// the starting y coordinate of LCM to be updated   
   kal_uint16  tgt_end_x;   /// the ending x coordinate of LCM to be updated   
   kal_uint16  tgt_end_y;   /// the ending y coordinate of LCM to be updated               
   kal_uint16  roi_offset_x; /// the ROI window offset x from main window   
   kal_uint16  roi_offset_y; /// the ROI window offset y from main window   

   kal_uint32  update_layer; /// the layers to be updated   
   kal_uint32  roi_background_color; /// background color of ROI

   //flatten parameters
   kal_uint8   block_mode;
   kal_uint8   memory_data_format;
   kal_uint8   memory_out_alpha;
   kal_uint16  memory_block_width;
   kal_uint16  dest_offset_x;
   kal_uint16  dest_offset_y;
   kal_uint32  memory_output_buffer_address;
   kal_uint32  memory_output_buffer_size;
   kal_uint32  dest_temp_address;

   //kal_uint32  landsp_ui_blt;
}  ddv2_lcd_roi_para_struct;

/******************************************************************
* Export APIs
*******************************************************************/
void lcd_init(LCD_TYPE_ENUM lcd_id, kal_uint16 background_color);
void lcd_config_layer(kal_uint32 layer_index, ddv2_lcd_layer_para_struct *layer_data);
void lcd_config_roi(ddv2_lcd_roi_para_struct *roi_para);
void lcd_start(
	LCD_UPDATE_ACT_ENUM mode,
	kal_bool block_req,
	kal_uint32 start_x, 
	kal_uint32 start_y, 
	kal_uint32 end_x, 
	kal_uint32 end_y);


void rs_ua_updateProgress(kal_uint32 phase, kal_uint32 progress);
void rs_ua_updateSuccess(void);
void rs_ua_updateError(kal_uint32 status);

#ifdef __cplusplus
}
#endif

#endif //defined(__MTK_TARGET__)
#endif