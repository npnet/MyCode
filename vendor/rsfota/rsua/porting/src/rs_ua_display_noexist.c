/*
 *
 * 版权(c) 2015 红石阳光（北京）科技有限公司
 * 版权(c) 2007-2015 红石阳光（北京）科技有限公司版权所有
 * 
 * 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
 * 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
 * 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
 *
 */

#include "drv_comm.h"
#include "drv_hisr.h"
#include "mm_power_ctrl.h"
#include "reg_base.h"
#include "intrCtrl.h"
#include "kal_release.h"
#include "lcd_if_hw.h"
#include "lcd_if.h"
#include "lcm_if.h"
#include "lcd_sw.h"
#include "lcd_sw_inc.h"
#include "drv_features_display.h"
#include "drv_sw_features_display.h"
#include "lcd_if_manager.h"
#include "dcl.h"
#include <bl_common.h>

#include "rs_ua_display.h"

#ifdef __cplusplus
extern "C" {
#endif

kal_uint32 lcd_if_mgr_handle;
kal_uint32 lcd_if_mgr_user_type;
kal_uint32 lcd_if_period_value;
LCD_IOCTRL_DEVICE_IF_T lcd_if_mgr_setting;

// lcd init
#if defined(__LCD_DRIVER_IN_BL__) 
static kal_bool lcd_inited = KAL_FALSE;
#endif /* __LCD_DRIVER_IN_BL__ */

/*************************************************************************
 * External reference definition
 *************************************************************************/
// Raymond : LCD related code, mark it now and you can remove it if it is really unnecessary
/*
#if defined(__LCD_DRIVER_IN_BL__)
extern void BL_LCDHWInit(void);
extern void BL_LCDSetBackLight(void);
extern void BL_ShowUpdateFirmwareInitBackground(void);
extern void BL_ShowUpdateFirmwareProgress(kal_uint16 percentage);
extern void BL_ShowUpdateFirmwareFail(kal_int32 r, kal_int32 g, kal_int32 b);
extern void BL_ShowUpdateFirmwareOK(void);
#endif */
extern void rs_ua_kick_watchdog(void);

extern void LCD_FunConfig(void);
extern void init_lcd_interface(void);
extern volatile LCD_Funcs  *MainLCD;
extern volatile LCD_Funcs  *SubLCD;
//extern volatile kal_uint16 lcd_cmd_period_count;
void lcd_power_ctrl(kal_bool enable){}

/*!
 *  Backup Port's Interface setting after power on LCD.
 */
static kal_bool lcd_BackupPortIFSetting(void)
{
	  LCD_IOCTRL_RESULT_E err;
   // get handle
   lcd_if_mgr_handle = lcd_get_ioctrl_device_if_handle(lcd_if_mgr_user_type);
   err = (LCD_IOCTRL_RESULT_E)lcd_ioctrl_device_if_settings(lcd_if_mgr_handle,
                                       LCD_IOCTRL_QUERY_DEVICE_IF,
                                       &lcd_if_mgr_setting,
                                       sizeof(LCD_IOCTRL_DEVICE_IF_T));
   if (LCD_IOCTRL_OK != err)
   {
       // do error handling
   }
}

static kal_bool lcd_RollbackPortIFSetting(void)
{
    LCD_IOCTRL_RESULT_E err;
    kal_uint32 roi_ctrl;
	 
    err = lcd_ioctrl_device_if_settings(lcd_if_mgr_handle, 
                                        LCD_IOCTRL_CONFIG_DEVICE_IF, 
                                        &lcd_if_mgr_setting, 
                                        sizeof(LCD_IOCTRL_DEVICE_IF_T)); 

    roi_ctrl = REG_LCD_ROI_CTRL;
    roi_ctrl &= (~0xF000DFFF); //clear: Layer_enable & cmd_enable & command_cnt & format
    roi_ctrl |= MAIN_LCD_OUTPUT_FORMAT;
    //roi_ctrl &= (~LCD_ROI_CTRL_CMDQ_SEL_BIT);
    //if (1 == cmdQueueIdx)
    //{
       roi_ctrl |= LCD_ROI_CTRL_CMDQ_SEL_BIT;
    //}
    REG_LCD_ROI_CTRL = roi_ctrl;
	 
    if(LCD_IOCTRL_OK != err)
    {
       // TODO: do error handling.... 
    }

    return KAL_TRUE;
}

void _lcd_config_pdn_turn_on_clock(void)
{
#if defined(MT6236) || defined(MT6236B) || defined(MT6256)

  *((volatile unsigned int *) MMSYS_CG_CLR ) = MMSYS_CG_CON_LCD;

#elif defined(MT6255)

  *((volatile unsigned int *) DRVPDN_MMSYS_CG_CLR ) = MMSYS_CG_CON_LCD;

#elif defined(MT6276)

  *((volatile unsigned int *) MMSYS_CG_CLR0 ) = MMSYS_CG_CON0_LCD;

#elif defined(MT6268)

   DRV_Reg(MMCG_CLR0) = MMCG_CON0_LCD;

#elif defined(MT6235) || defined(MT6235B) || defined(MT6252) || defined(MT6252H) || defined(MT6253)

   DRV_Reg(DRVPDN_CON1_CLR) = DRVPDN_CON1_LCD;

#elif defined(MT6251)

   PDN_CLR(PDN_LCD);
#elif (defined(MT6250) || defined(MT6260) || defined(MT6261) || defined(MT2501) || defined(MT2502))
   DRV_Reg(DRVPDN_MCUSYS_PDN_CON0_CLR) |=(DRVPDN_CON0_SW_LCD | DRVPDN_CON0_SW_SLCD);   
#endif

}

void _lcd_config_pdn_turn_off_clock(void)
{
#if defined(MT6236) || defined(MT6236B) || defined(MT6256)

  *((volatile unsigned int *) MMSYS_CG_SET) = MMSYS_CG_CON_LCD;

#elif defined(MT6255)

  *((volatile unsigned int *) DRVPDN_MMSYS_CG_SET) = MMSYS_CG_CON_LCD;

#elif defined(MT6276)

  *((volatile unsigned int *) MMSYS_CG_SET0) = MMSYS_CG_CON0_LCD;

#elif defined(MT6268)

   DRV_Reg(MMCG_SET0) = MMCG_CON0_LCD;

#elif defined(MT6235) || defined(MT6235B) || defined(MT6252) || defined(MT6252H) || defined(MT6253)

   DRV_Reg(DRVPDN_CON1_SET) = DRVPDN_CON1_LCD;

#elif defined(MT6251)

   PDN_SET(PDN_LCD);
#elif (defined(MT6250) || defined(MT6260) || defined(MT6261) || defined(MT2501) || defined(MT2502))
   DRV_Reg(DRVPDN_MCUSYS_PDN_CON0_CLR) |=(DRVPDN_CON0_SW_LCD | DRVPDN_CON0_SW_SLCD);     
#endif
}

static LCD_STATUS_CODE_ENUM _lcd_ConfigDither(void) //Tristan, change to static
{
  kal_uint32 dither_bit_r, dither_bit_g, dither_bit_b;
  LCD_ROI_OUTPUT_COLOR_FORMAT_ENUM lcd_to_lcm_fmt;

  dither_bit_r = 1;
  dither_bit_g = 1;
  dither_bit_b = 1;
  lcd_to_lcm_fmt = (LCD_ROI_OUTPUT_COLOR_FORMAT_ENUM)((GET_LCD_ROI_CTRL_OUTPUT_FORMAT & 0x38) >> 3);

  switch (lcd_to_lcm_fmt)
  {
    case LCD_ROI_OUTPUT_RGB666:
    {
       dither_bit_r = 1;
       dither_bit_g = 1;
       dither_bit_b = 1;
       break;
    }

    case LCD_ROI_OUTPUT_RGB565:
    {
       dither_bit_r = 2;
       dither_bit_g = 1;
       dither_bit_b = 2;
       break;
    }
  }

  // 1 MT6253/MT6252/MT6268 shall add macros definition... if need
  SET_LCD_DITHER_BIT(dither_bit_r, dither_bit_g, dither_bit_b);
  SET_LCD_DITHER_SEED(1, 1, 1);

  return LCD_OK;
}

kal_uint32 _lcd_GetFormatBpp(LCD_LAYER_SOURCE_COLOR_FORMAT_ENUM layer_format)
{
   switch(layer_format)
   {
      case LCD_LAYER_COLOR_8BPP_INDEX:
      return 1;

      case LCD_LAYER_COLOR_RGB565:
      return 2;

#if defined(LCD_LAYER_SOURCE_COLOR_UYVY422)
      case LCD_LAYER_COLOR_UYVY422:
      return 2;
#endif

      case LCD_LAYER_COLOR_RGB888:
      return 3;

#if defined(LCD_LAYER_SOURCE_COLOR_ARGB6666) ||  defined(LCD_LAYER_SOURCE_COLOR_PARGB6666)
      case LCD_LAYER_COLOR_ARGB6666:
      return 3;
      case LCD_LAYER_COLOR_PARGB6666:
      return 3;
#endif

#if defined(LCD_LAYER_SOURCE_COLOR_ARGB8888)
      case LCD_LAYER_COLOR_ARGB8888:
      return 4;
#endif

#if defined(LCD_LAYER_SOURCE_COLOR_PARGB8888)
      case LCD_LAYER_COLOR_PARGB8888:
      return 4;
#endif

      default:
       ASSERT(0);
       break;
   }

   return 2;
}

void _lcd_busy_waiting(void)
{
   while(LCD_IS_RUNNING);
   DRV_Reg(LCD_INT_STATUS_REG);
}

void lcd_init(LCD_TYPE_ENUM id, kal_uint16 background_color)
{
	kal_uint32 i, j;
  
	//workaround for MT6250 watchdog can't reset MM sys,uing sw reset MM sys here.
#if (defined(MT6250) || defined(MT6260) || defined(MT6261) || defined(MT2501) || defined(MT2502))
	(*(volatile kal_uint32 *)(0xA0180808)) &= 0xFFFFFFFE;
	(*(volatile kal_uint32 *)(0xA0180808)) |= 0x1;
#endif

	LCD_FunConfig();
	_lcd_config_pdn_turn_on_clock();

	{
	//  TODO: Chip Dependent here
		lcd_if_mgr_user_type = LCD_IF_USER_MAIN_LCD;
		lcd_if_mgr_setting.if_type = LCD_IF_TYPE_DBI_PARALLEL;
		lcd_if_mgr_setting.if_cs_no = LCD_IF_PARALLEL_0;
	}

	//INIT LCD HW registers
	// remove LCD int status
	(void)DRV_Reg(LCD_INT_STATUS_REG);
	REG_LCD_INT_ENABLE = 0;
	REG_LCD_ROI_OFFSET = 0;
	REG_LCD_ROI_SIZE = 0;
	DISABLE_LCD_ROI_CTRL_W2MEM;
	SET_LCD_ROI_WMEM_OFFSET(0, 0);
	SET_LCD_WMEM_PITCH(0);	  
	//reset HW layer registers

	for(i = 0; i < DRV_LAYER_TOTAL_COUNT; i++)
	{
		kal_uint32 lcd_layer_base_addr;

		lcd_layer_base_addr = LCD_LAYER0_BASE_ADDR + i * LCD_LAYER_ADDR_OFFSET;
		for(j = 0; j < LCD_LAYER_ADDR_OFFSET; j += 4){
			DRV_WriteReg32((lcd_layer_base_addr + j), 0);
		}
	}

	//#if (DRV_LCD_CMD_QUEUE_COUNT > 1)
	SET_LCD_CMD_QUEUE1;
	//#endif

	#if defined(MT6236)
		LCD_SET_GMCCON_BURST_SIZE(LCD_GMCCON_BURST_64_BYTE);
		_lcd_ConfigDither();
	#endif

	DISABLE_LCD_TE_DETECT();
	init_lcd_interface();
	lcd_BackupPortIFSetting();
	lcd_if_period_value = (REG_LCD_ROI_CTRL & LCD_ROI_CTRL_PERIOD_MASK) >> LCD_ROI_CTRL_PERIOD_OFFSET;
	DRV_WriteReg32(LCD_ROI_CMD_ADDR_REG, MAIN_LCD_CMD_ADDR);
	DRV_WriteReg32(LCD_ROI_DATA_ADDR_REG, MAIN_LCD_DATA_ADDR);
	SET_LCD_ROI_CTRL_OUTPUT_FORMAT(MAIN_LCD_OUTPUT_FORMAT);
	ENABLE_LCD_TRANSFER_COMPLETE_INT;
	SET_LCD_ROI_CTRL_PERIOD(lcd_if_period_value);

	if (MAIN_LCD == id) 
	{
		MainLCD->Init(background_color,0);
	}
	else  
	{
		SubLCD->Init(background_color,0);
	}
}

void lcd_config_layer(kal_uint32 layer_index, ddv2_lcd_layer_para_struct *layer_data)
{
	kal_uint32 bpp = 1;
	kal_uint32 lcd_layer_ctrl_reg = 0;
	kal_uint32 lcd_layer_base_addr = LCD_LAYER0_BASE_ADDR;

	#if defined(MT6252) || defined(MT6252H) || defined(MT6276) || defined(MT6256) || defined(MT6251) || defined(MT6255) || defined(MT6250) || defined(MT6260) || defined(MT6261) || defined(MT2501) || defined(MT2502)
		bpp = _lcd_GetFormatBpp((LCD_LAYER_SOURCE_COLOR_FORMAT_ENUM)layer_data->source_color_format);
	#endif

	#if !(defined(IC_MODULE_TEST) || defined(IC_BURNIN_TEST))
		layer_data->mem_pitch = layer_data->column_number * bpp;
		layer_data->lgamma_enable = KAL_FALSE;
		layer_data->dither_enable = KAL_FALSE;
		layer_data->mem_x_offset = 0;
		layer_data->mem_y_offset = 0;
		layer_data->dst_key_enable = KAL_FALSE;
	#endif

	/// Basic check
	ASSERT(layer_index < LCD_TOTAL_LAYER);

	if (layer_data->opacity_enable)
	{
		ASSERT(layer_data->opacity_value <= LCD_MAX_OPACITY);
	}

	ASSERT(layer_data->source_color_format <= LCD_LAYER_MONO);
	ASSERT(layer_data->rotate_value <= LCD_LAYER_MIRROR_ROTATE_270);

	#if defined(DRV_LCD_4_BYTES_ALIGN)
		ASSERT(0==(layer_data->frame_buffer_address & 0x3));
	#elif defined(DRV_LCD_8_BYTES_ALIGN)
		ASSERT(0==(layer_data->frame_buffer_address & 0x7));
	#endif

	lcd_layer_base_addr+= layer_index * LCD_LAYER_ADDR_OFFSET;
	lcd_layer_ctrl_reg = 0;

	lcd_layer_ctrl_reg &= ~LCD_LAYER_CTRL_SOURCE_KEY_ENABLE_BIT;
	lcd_layer_ctrl_reg &= ~LCD_LAYER_CTRL_DST_KEY_ENABLE_BIT;

	if (layer_data->source_key_enable)
	{
		lcd_layer_ctrl_reg |= LCD_LAYER_CTRL_SOURCE_KEY_ENABLE_BIT;
	}
	else if (layer_data->dst_key_enable)
	{
		lcd_layer_ctrl_reg |= LCD_LAYER_CTRL_DST_KEY_ENABLE_BIT;
	}

	lcd_layer_ctrl_reg &= ~LCD_LAYER_CTRL_OPACITY_ENABLE_BIT;

	if (layer_data->opacity_enable)
	{
		lcd_layer_ctrl_reg |= LCD_LAYER_CTRL_OPACITY_ENABLE_BIT;
		lcd_layer_ctrl_reg &= ~LCD_LAYER_CTRL_OPACITY_VALUE_MASK;
	}

	lcd_layer_ctrl_reg |= (layer_data->opacity_value << LCD_LAYER_ALPHA_VAL_OFFSET);

	lcd_layer_ctrl_reg &= ~LCD_LAYER_CTRL_ROTATE_MASK;
	lcd_layer_ctrl_reg |= ((layer_data->rotate_value) << LCD_LAYER_CTRL_ROTATE_OFFSET);

	lcd_layer_ctrl_reg &= ~LCD_LAYER_CTRL_COLOR_FORMAT_MASK;
	lcd_layer_ctrl_reg |= (layer_data->source_color_format << LCD_LAYER_CTRL_FORMAT_OFFSET);

	//4 For MT6236, set dither and linear gamma here
	lcd_layer_ctrl_reg &= ~LCD_LAYER_CTRL_DITHER_ENABLE_BIT;
	#ifdef LCD_LAYER_CTRL_LGAMMA_ENABLE_BIT
		lcd_layer_ctrl_reg &= ~LCD_LAYER_CTRL_LGAMMA_ENABLE_BIT;
	#endif

	if (layer_data->dither_enable)
	{
		lcd_layer_ctrl_reg |= LCD_LAYER_CTRL_DITHER_ENABLE_BIT;
	}

	_lcd_config_pdn_turn_on_clock();
	lcd_RollbackPortIFSetting();
	LCD_SET_LAYER_CTRL(layer_index, lcd_layer_ctrl_reg);
	LCD_SET_LAYER_COLOR_KEY(layer_index, layer_data->source_key);
	LCD_SET_LAYER_OFFSET(layer_index, (layer_data->x_offset), (layer_data->y_offset));
	LCD_SET_LAYER_BUFF_ADDR(layer_index, layer_data->frame_buffer_address);
	LCD_SET_LAYER_SIZE(layer_index, (layer_data->column_number), (layer_data->row_number));
	LCD_SET_LAYER_MEM_OFFSET(layer_index, (layer_data->mem_x_offset), (layer_data->mem_y_offset))
	SET_LCD_LAYER_MEM_PITCH(layer_index, layer_data->mem_pitch, bpp);
}

void lcd_config_roi(ddv2_lcd_roi_para_struct *roi_para)
{
	kal_uint32 roi_width, roi_height;
	kal_uint32 roi_ctrl_shadow;

	_lcd_config_pdn_turn_on_clock();
	lcd_RollbackPortIFSetting();
	DISABLE_LCD_TE_DETECT();
     
	roi_width = roi_para->tgt_end_x - roi_para->tgt_start_x + 1;
	roi_height = roi_para->tgt_end_y - roi_para->tgt_start_y + 1;	

	roi_ctrl_shadow = 0;
	roi_ctrl_shadow |= roi_para->update_layer;
   
	_lcd_busy_waiting(); //waiting if the previous request is running
	DISABLE_LCD_HW_TRIGGER_UPDATE;

	if ((LCD_UPDATE_FLT == roi_para->update) || (LCD_UPDATE_BLT_AND_FLT == roi_para->update))
	{
		kal_uint32 bpp = 2;

		switch(roi_para->memory_data_format)
		{
			case LCD_WMEM_COLOR_RGB888:
			{
 				LCD_WRITE_MEM_RGB888;
				bpp = 3;
				break;
			}   
			#if defined(LCD_LAYER_SOURCE_COLOR_PARGB6666)
				case LCD_WMEM_COLOR_PARGB6666:
				{
					LCD_WRITE_MEM_PARGB6666;
					bpp = 3;
					break;
				}   
			#endif
			case LCD_WMEM_COLOR_ARGB8888:
			{
				LCD_WRITE_MEM_ARGB8888;
				bpp = 4;
				break;
			}
			case LCD_WMEM_COLOR_RGB565:
			default:
			{
				LCD_WRITE_MEM_RGB565;
				bpp = 2;
				break;
			}   
		}

		LCD_SET_ROI_WMEM_CTRL_CLEAR;
		/* some chips can write to mem regardless pitch */
		LCD_SET_ROI_WMEM_CTRL_PITCH_EN;
		if (LCD_UPDATE_FLT == roi_para->update)
		{
			LCD_SET_ROI_WMEM_CTRL_MEM_ONLY;
		}
		else
		{
			LCD_SET_ROI_WMEM_CTRL_MEM_AND_LCM;
		}

		LCD_SET_ROI_WMEM_CTRL_FMT(roi_para->memory_data_format);
		LCD_SET_ROI_WMEM_CTRL_ALPHA(roi_para->memory_out_alpha);
		SET_LCD_WMEM_PITCH((roi_para->memory_block_width * bpp));
		SET_LCD_ROI_WMEM_OFFSET(roi_para->dest_offset_x, roi_para->dest_offset_y);
		LCD_SET_ROI_WMEM_ADDR(roi_para->memory_output_buffer_address);

		//disable gamma here
		{
		}
	}

	_lcd_ConfigDither();
	ENABLE_LCD_TRANSFER_COMPLETE_INT;
	LCD_SET_ROI_BG_COLOR(roi_para->roi_background_color);
	LCD_SET_ROI_CONTROL(roi_ctrl_shadow);
	SET_LCD_ROI_CTRL_OUTPUT_FORMAT(MAIN_LCD_OUTPUT_FORMAT);
	LCD_SET_ROI_OFFSET((roi_para->roi_offset_x), (roi_para->roi_offset_y));
	LCD_SET_ROI_SIZE(roi_width, roi_height);	
}

void lcd_start(
	LCD_UPDATE_ACT_ENUM mode,
	kal_bool block_req,
	kal_uint32 start_x, 
	kal_uint32 start_y, 
	kal_uint32 end_x, 
	kal_uint32 end_y)
{
	ENABLE_LCD_ROI_CTRL_CMD_FIRST;
	SET_LCD_CMD_QUEUE1;
	SET_LCD_ROI_CTRL_PERIOD(lcd_if_period_value);
	SET_LCD_ROI_CTRL_OUTPUT_FORMAT(MAIN_LCD_OUTPUT_FORMAT);
	DRV_WriteReg32(LCD_ROI_CMD_ADDR_REG, MAIN_LCD_CMD_ADDR);
	DRV_WriteReg32(LCD_ROI_DATA_ADDR_REG, MAIN_LCD_DATA_ADDR);

	if (LCD_UPDATE_FLT == mode)
	{
		DISABLE_LCD_ROI_CTRL_CMD_FIRST;
		START_LCD_TRANSFER;
		//read DBI ULTRA HIGH REG to delay 4T for MT6255 F2S bus early return potential risk.
		#if defined(MT6255) || defined(MT6256)
			DRV_Reg(LCD_DBI_ULTRA_TH_REG);
		#endif
	}
	else
	{
		MainLCD->BlockWrite(start_x, start_y, end_x, end_y);
	}

	if (block_req)
	{
		//read DBI ULTRA HIGH REG to delay 4T for MT6255 F2S bus early return potential risk.
		#if defined(MT6255) || defined(MT6256)
		DRV_Reg(LCD_DBI_ULTRA_TH_REG);
		#endif
		_lcd_busy_waiting(); //waiting if the previous request is running
	}

	return;
}

/**
 * rs_ua_initLCD
 *
 *
 */
static void rs_ua_initLCD(void)
{
#if defined(__LCD_DRIVER_IN_BL__)
   if(!lcd_inited)
   {
      BL_PRINT(LOG_INFO, "Init LCD\n\r");
      BL_LCDHWInit();
      BL_ShowUpdateFirmwareInitBackground();
      BL_LCDSetBackLight();

      {
         DCL_HANDLE rtc_handler;
         DCL_HANDLE pw_handle;

         //before call dclpw, rtc is needed to be initialized
         rtc_handler = DclRTC_Open(DCL_RTC,FLAGS_NONE);
         DclRTC_Control(rtc_handler, RTC_CMD_SETXOSC, (DCL_CTRL_DATA_T *)NULL);
         DclRTC_Control(rtc_handler, RTC_CMD_HW_INIT, (DCL_CTRL_DATA_T *)NULL);
         DclRTC_Close(rtc_handler);

         //Use dclpw to latch the power
         pw_handle=DclPW_Open(DCL_PW, FLAGS_NONE);
         DclPW_Control(pw_handle,PW_CMD_POWERON,NULL);
         DclPW_Close(pw_handle);
      }

      lcd_inited = KAL_TRUE;
   }
#endif /* __LCD_DRIVER_IN_BL__ */
}

/**
 * rs_ua_updateProgress
 *
 *
 */
void rs_ua_showProgress(kal_uint32 phase, kal_uint32 progress)
{
	// phase 0: initialztion, 0%
	// phase 1: verify update-package, 30%
	// phase 2: verify update process, 50%
	// phase 3: upgrading, [50% ~ 99%]
	// phase 4: finishing, 100%
	rs_ua_kick_watchdog();
	BL_PRINT(LOG_INFO, "%s phase[%d] progress[%d]\n\r", __func__, phase, progress);
	
	if (progress <= 100) {
		#if defined(__LCD_DRIVER_IN_BL__)
			BL_PRINT(LOG_INFO, "%s real phase[%d] progress[%d]\n\r", __func__, phase, progress);
			rs_ua_initLCD();
			BL_ShowUpdateFirmwareProgress(progress);
		#endif
	}
}

/**
 * rs_ua_updateProgress
 *
 *
 */
void rs_ua_showSuccess(void)
{
#if defined(__LCD_DRIVER_IN_BL__)
	BL_PRINT(LOG_INFO, "\n\r%s run\n\r", __func__);
	BL_ShowUpdateFirmwareOK();
#endif
}



/**
 * rs_ua_updateProgress
 *
 *
 */
void rs_ua_updateError(kal_uint32 status)
{
#if defined(__LCD_DRIVER_IN_BL__)
	kal_uint32 i;

	typedef struct {
		kal_uint32 error_start;
		kal_uint32 error_end;
		kal_uint32 rgb[3];
	} ERROR_MAP;

	const ERROR_MAP error_map[] =
	{
		{0x20, 0x5F, {255, 255,   0}}, //Yellow
		{0x60, 0x9F, {255,   0, 255}}, //Purple, 紫色
		{0xA0, 0xAF, {  0, 255, 255}}, //Light blue, 青色
		{0xB0, 0xBF, {255,   0,   0}}, //Red
		{0xC0, 0xCF, {  0,   0, 255}}, //Blue
		{0xD0, 0xFF, {  0, 255,   0}}, //Green
	};

	if(status == 0x00)//RS_OK
	{
		return;
	}

	BL_PRINT(LOG_INFO, "%s run, status[%x]\n\r", __func__, status);
	rs_ua_initLCD();

	for(i=0; i<sizeof(error_map)/sizeof(ERROR_MAP); i++)
	{
		BL_PRINT(LOG_INFO, "i[%d], cycle\n\r", i);
		if(status >= error_map[i].error_start && status <= error_map[i].error_end)
		{
			BL_PRINT(LOG_INFO, "i[%d], rgb[0]:%d, rgb[1]:%d, rgb[2]:%d\n\r", i, error_map[i].rgb[0], error_map[i].rgb[1], error_map[i].rgb[2]);
			BL_ShowUpdateFirmwareFail(error_map[i].rgb[0], error_map[i].rgb[1], error_map[i].rgb[2]);
			return;
		}
	}

	//Other erros
	BL_ShowUpdateFirmwareFail(0, 0, 0);
#endif /* __LCD_DRIVER_IN_BL__ */
}

void rs_ua_showError()
{
	 rs_ua_updateError(0xB0);
}

#ifdef __cplusplus
}
#endif

