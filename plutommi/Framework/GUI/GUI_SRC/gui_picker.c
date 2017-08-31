/*******************************************************************************
*  Modification Notice:
*  --------------------------
*  This software is modified by MediaTek Inc. and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*******************************************************************************/

 /*******************************************************************************
 * Filename:
 * ---------
 *   gui_picker.c
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *  Picker CP
 *
 * Author:
 * -------
 * -------
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#include "mmi_features.h"
#include "gui_picker.h"

#ifdef __MMI_UI_TIME_PICKER__
#include "CustDataRes.h"
#include "CustDataProts.h"
#include "lcd_if.h"
#include "ScreenRotationGprot.h"
#include "wgui_softkeys.h"
#include "wgui_touch_screen.h"
#include "gui_typedef.h"
#include "mmi_frm_input_gprot.h"
#include "Gui_themes.h"
#include "gpiosrvgprot.h"

#include "gui_font_size.h"

#include "CommonScreens.h"

#include "CustThemesRes.h"

#ifdef __MMI_BT_BOX_TEST_DIALER_PAD_STYLE__
#include "PhoneSetupGprots.h"
#endif  //__MMI_BT_BOX_TEST_DIALER_PAD_STYLE__




static gui_picker_context_struct g_picker_cntx;




static U16 gui_picker_year_get_switch(U16 current_year, gui_picker_switch_type switch_type, U8 switch_offset);
static U8 gui_picker_month_get_switch(U8 current_month, gui_picker_switch_type switch_type, U8 switch_offset);
static U8 gui_picker_day_get_switch(U8 current_day, gui_picker_switch_type switch_type, U8 switch_offset);
static U8 gui_picker_hour_get_switch(U8 current_hour, gui_picker_switch_type switch_type, U8 switch_offset);
static U8 gui_picker_minute_get_switch(U8 current_minute, gui_picker_switch_type switch_type, U8 switch_offset);
static void gui_picker_ssp_recover(void);





/*****************************************************************************
 * FUNCTION
 *  gui_picker_get_max_num_of_days
 * DESCRIPTION
 *  
 * PARAMETERS
 *  y       [IN]        
 *  m       [IN]        
 * RETURNS
 *  
 *****************************************************************************/
U8 gui_picker_get_max_num_of_days(U16 y, U8 m)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
    {
        return 31;
    }
    else if (m == 4 || m == 6 || m == 9 || m == 11)
    {
        return 30;
    }
    else    /* Feb */
    {
        U8 isLeap = 0;

        if (y % 400 == 0)
        {
            isLeap = 1;
        }
        else if (y % 100 == 0)
        {
            isLeap = 0;
        }
        else if (y % 4 == 0)
        {
            isLeap = 1;
        }
        else
        {
            isLeap = 0;
        }

        return 28 + isLeap;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_sprintf_num
 * DESCRIPTION
 *  Sprintf num with bit
 * PARAMETERS
 *  str
 *  num
 *  bit
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_sprintf_num(U16* str, U16 num, U8 bit)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 i, j;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(str != NULL && num < GUI_PICKER_MAX_NUM && bit < GUI_PICKER_MAX_BIT);
    
    for (i = 0; i < bit; i++)
    {
        U16 divisor = 1, num_to_cat = 0;
        for (j = 0; j < bit-i-1; j++)
        {
            divisor *= 10;
        }
        num_to_cat = num / divisor;
        num -= num_to_cat * divisor;
        str[i] = 0x30 + num_to_cat;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_get_number_text_size
 * DESCRIPTION
 *  Draw Number
 * PARAMETERS
 *  number [IN] U16
 *  bit [IN] U8
 *  width
 *  height
 *  text_color
 *  font_attr
 * RETURNS
 *  gui_bound [OUT] return bound to resize picker bound
 *****************************************************************************/
static void gui_picker_get_number_text_size(U16 number, U8 bit, S32 *width, S32 *height, stFontAttribute *font_attr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U16 max = 1, i;
    U16 tmp_str[GUI_PICKER_TEXT_BUFFER_SIZE], str_size = GUI_PICKER_TEXT_BUFFER_SIZE * 2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(font_attr != NULL);
    gui_set_font(font_attr); 
   
    memset(tmp_str, 0, str_size);
    for (i = 0; i < bit - 1; i++)
    {
        max *= 10;
    }
    kal_wsprintf((WCHAR *)tmp_str, "%d", max);
    gui_measure_string((UI_string_type)tmp_str, width, height);
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_draw_number_text
 * DESCRIPTION
 *  Draw Number Text
 * PARAMETERS
 *  number [IN] U16
 *  bit [IN] U8
 *  bound [IN] gui_bound
 *  text_color
 *  font_attr
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_draw_number_text(U16 number, U8 bit, gui_bound bound, color text_color, stFontAttribute *font_attr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 num_width, num_height;
    gui_point current_pos;
    S32 x1, x2, y1, y2;
    U16 tmp_str[GUI_PICKER_TEXT_BUFFER_SIZE], str_size = GUI_PICKER_TEXT_BUFFER_SIZE * 2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(font_attr != NULL && number < GUI_PICKER_MAX_NUM && bit < GUI_PICKER_MAX_BIT);

    gdi_layer_get_text_clip(&x1, &y1, &x2, &y2);
    
    //gdi_layer_set_text_clip(bound.x, bound.y, bound.x + bound.width, bound.y + bound.height);
    
    gui_set_font(font_attr); 
   
    gui_picker_get_number_text_size(number, bit, &num_width, &num_height, font_attr);
    
    if (num_width <= 0 || num_height <= 0)
        MMI_ASSERT(0);
    
    gui_set_text_color(text_color);
    gui_set_text_border_color(*(current_MMI_theme->title_text_border_color));

    memset(tmp_str, 0, str_size);
    gui_picker_sprintf_num(tmp_str, number, bit);
    current_pos.x = bound.x + (bound.width - num_width) / 2;
    current_pos.y = bound.y + (bound.height - num_height) / 2;
    gui_move_text_cursor(current_pos.x, current_pos.y);
    gui_print_text((UI_string_type)tmp_str);
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_draw_number_image
 * DESCRIPTION
 *  Draw Number Image
 * PARAMETERS
 *  number [IN] U16
 *  bit [IN] U8
 *  bound [IN] gui_bound
 * num_color [IN] gui_picker_num_color
 * RETURNS
 *  gui_bound [OUT] return bound to resize picker bound
 *****************************************************************************/
static gui_bound gui_picker_draw_number_image(U16 number, U8 bit, gui_bound bound, gui_picker_num_color num_color)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i, j, num_width, num_height, num_total_width;
    gui_point current_pos;
    U16 num_0_img_id;
    gui_bound effective_bound;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(number < GUI_PICKER_MAX_NUM && bit < GUI_PICKER_MAX_BIT);

    if (num_color == GUI_PICKER_NUM_COLOR_WHITE)
    {
        num_0_img_id = IMG_ID_PICKER_0_DOWN;
    }
    else
    {
        num_0_img_id = IMG_ID_PICKER_0;
    }

    gdi_image_get_dimension_id(IMG_ID_PICKER_0, &num_width, &num_height);

    MMI_ASSERT(num_width > 0 && num_height > 0);

    num_total_width = num_width * bit;
    
    if (bound.width < num_total_width)
    {
        bound.width = num_total_width;
    }
    if (bound.height < num_height)
    {
        bound.height = num_height;
    }
    
    //Draw Number
    current_pos.x = bound.x + (bound.width - num_total_width) / 2;
    current_pos.y = bound.y + (bound.height - num_height) / 2;
    effective_bound.x = current_pos.x;
    effective_bound.y = current_pos.y;
    effective_bound.width = num_total_width;
    effective_bound.height = num_height;
    
    gdi_layer_push_and_set_clip(effective_bound.x, effective_bound.y, effective_bound.x + effective_bound.width, 
        effective_bound.y + effective_bound.height);
    
    for (i = 0; i < bit - 1; i++)
    {
        U16 divisor = 1, num_to_draw = 0;
        for (j = 0; j < bit-i-1; j++)
        {
            divisor *= 10;
        }
        num_to_draw = number / divisor;
        number -= num_to_draw * divisor;
        
        gui_draw_image_id_with_alpha_transparent(num_0_img_id + num_to_draw, current_pos.x, current_pos.y, 255);
        current_pos.x += num_width;
    }
    gui_draw_image_id_with_alpha_transparent(num_0_img_id + number, current_pos.x, current_pos.y, 255);

    gdi_layer_pop_clip();

    return effective_bound;
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_draw_number_hint_text
 * DESCRIPTION
 *  Draw Hint text of number
 * PARAMETERS
 *  get_switch_func
 *  number
 *  bit
 *  bound
 *  position_type
 *  hint_num_1 [IN] how many hints left/up
 *  hint_num_2 [IN] how many hints right/down
 *  highlight [IN] whether current column is highlight
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_draw_number_hint_text(gui_picker_get_switch_func_ptr get_switch_func, U16 number, U8 bit, gui_bound bound, 
    gui_picker_hint_position_type position_type, U8 hint_num_1, U8 hint_num_2, MMI_BOOL highlight)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 num_width, num_height;
    S32 switch_num;
    gui_bound text_bound;
    U8 i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(get_switch_func != NULL && number < GUI_PICKER_MAX_NUM && bit < GUI_PICKER_MAX_BIT);
    
    gui_picker_get_number_text_size(number, bit, &num_width, &num_height, &MMI_small_font);

    if (num_width <= 0 || num_height <= 0)
        MMI_ASSERT(0);
    
    text_bound.x = bound.x + (bound.width - num_width) / 2;
    text_bound.width = num_width;
    text_bound.height = num_height;
    
    //up
    if (position_type == GUI_PICKER_HINT_POSITION_AUTO_BY_BOUND)
    {
        text_bound.y = bound.y - bound.height + (bound.height - num_height) / 2;
    }
    else
    {
        text_bound.y = bound.y - num_height;
    }
    i = hint_num_1;
    switch_num = number;
    while (i > 0)
    {
        switch_num = get_switch_func(switch_num, GUI_PICKER_SWITCH_TYPE_UP, 1);
        if (switch_num != GUI_PICKER_INVALID_NUM)
        {
            if (position_type == GUI_PICKER_HINT_POSITION_AUTO_BY_BOUND)
            {
                text_bound.y -= (i == hint_num_1) ? 0: bound.height;
            }
            else
            {
                text_bound.y -= (i == hint_num_1) ? 0: num_height;
            }
            #ifdef __MMI_TOUCH_SCREEN__
            if (highlight && g_picker_cntx.pos_offset >= g_picker_cntx.picker_number.line_height / 3 && i == hint_num_1)
            {
                gui_picker_draw_number_text(switch_num, bit, text_bound, GUI_PICKER_TEXT_COLOR_THIN_BLACK, &MMI_small_font);
            }
            else
            #endif  //__MMI_TOUCH_SCREEN__
            {
                gui_picker_draw_number_text(switch_num, bit, text_bound, GUI_PICKER_TEXT_COLOR_GRAY, &MMI_small_font);
            }
        }

        i--;
    }

    //down
    if (position_type == GUI_PICKER_HINT_POSITION_AUTO_BY_BOUND)
    {
        text_bound.y = bound.y + bound.height + (bound.height - num_height) / 2;
    }
    else
    {
        text_bound.y = bound.y + bound.height;
    }
    i = hint_num_2;
    switch_num = number;
    while (i > 0)
    {
        switch_num = get_switch_func(switch_num, GUI_PICKER_SWITCH_TYPE_DOWN, 1);
        if (switch_num != GUI_PICKER_INVALID_NUM)
        {
            if (position_type == GUI_PICKER_HINT_POSITION_AUTO_BY_BOUND)
            {
                text_bound.y += (i == hint_num_2) ? 0: bound.height;
            }
            else
            {
                text_bound.y += (i == hint_num_2) ? 0: num_height;
            }
            #ifdef __MMI_TOUCH_SCREEN__
            if (highlight && g_picker_cntx.pos_offset <= -(g_picker_cntx.picker_number.line_height / 3) && i == hint_num_1)
            {
                gui_picker_draw_number_text(switch_num, bit, text_bound, GUI_PICKER_TEXT_COLOR_THIN_BLACK, &MMI_small_font);
            }
            else
            #endif  //__MMI_TOUCH_SCREEN__
            {
                gui_picker_draw_number_text(switch_num, bit, text_bound, GUI_PICKER_TEXT_COLOR_GRAY, &MMI_small_font);
            }
        }

        i--;
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_draw_number_picker
 * DESCRIPTION
 *  Draw Number Picker
 * PARAMETERS
 *  picker_number
 *  bound [IN] gui_bound
 *  display_row [IN] how many row to display
 *  highlight [IN] MMI_BOOL : whether it is highlight
 * RETURNS
 *  gui_bound [OUT] return bound to resize picker bound
 *****************************************************************************/
static gui_bound gui_picker_draw_number_picker(gui_picker_number_struct *picker_number, gui_bound bound, U8 display_row)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8 index;
    S32 num_width, num_height, bg_width, bg_height, num_total_width, shadow_width;
    gui_point current_pos;
    gui_bound effective_bound, number_bound;
    U8 hint_num_1, hint_num_2;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(picker_number != NULL && picker_number->column > 0);

    gdi_image_get_dimension_id(IMG_ID_PICKER_0, &num_width, &num_height);

    if (num_width <= 0 || num_height <= 0)
        MMI_ASSERT(0);

    //Calculate bound
    effective_bound.width = 0;
    effective_bound.height = num_height * display_row;
    bg_height = GUI_PICKER_9_SLICE_BG_TOP + GUI_PICKER_9_SLICE_BG_BOTTOM + effective_bound.height;
    picker_number->line_height = num_height;
    
    for (index = 0; index < picker_number->column; index++)
    {
        picker_number->column_bound[index].width = num_width * picker_number->bit[index] + GUI_PICKER_NUMBER_X_OFFSET * 2;
        picker_number->column_bound[index].height = effective_bound.height;
        effective_bound.width += num_width * picker_number->bit[index] + GUI_PICKER_NUMBER_X_OFFSET * 2;
    }
    effective_bound.width += (picker_number->column+1) * GUI_PICKER_COLUMNE_GAP;
    
    bg_width = GUI_PICKER_9_SLICE_BG_LEFT + GUI_PICKER_9_SLICE_BG_RIGHT + effective_bound.width;
    
    //bound checking
    if (bound.height < bg_height+GUI_PICKER_BORDER_GAP*2)
    {
        bound.height = bg_height + GUI_PICKER_BORDER_GAP * 2;
    }
    if (bound.width < bg_width+GUI_PICKER_BORDER_GAP*2)
    {
        bound.width = bg_width + GUI_PICKER_BORDER_GAP * 2;
    }

    //Draw Background
    current_pos.x = bound.x + (bound.width - bg_width) / 2;
    current_pos.y = bound.y + (bound.height - bg_height) / 2;
    
    gdi_image_draw_resized_id(current_pos.x, current_pos.y, bg_width, bg_height, IMG_ID_PICKER_BG);

    effective_bound.x = (current_pos.x += (bg_width - effective_bound.width) / 2);
    effective_bound.y = (current_pos.y += GUI_PICKER_9_SLICE_BG_TOP);

    //gdi_layer_set_text_clip(effective_bound.x, effective_bound.y, effective_bound.x + effective_bound.width, effective_bound.y + effective_bound.height);
    for (index = 0; index < picker_number->column; index++)
    {
        S32 title_width, title_height;
        gui_point title_pos;
        UI_string_type column_name = (UI_string_type)GetString(picker_number->column_name_id[index]);
        MMI_BOOL highlight = picker_number->column > 1 ? (g_picker_cntx.highlight_index == index ? MMI_TRUE : MMI_FALSE) : MMI_TRUE;

        MMI_ASSERT(column_name != NULL);
        
        current_pos.x += GUI_PICKER_COLUMNE_GAP;
        
        hint_num_1 = hint_num_2 = display_row / 2 + 2;
        
        //Show title
        gui_set_font(&MMI_medium_font);  
        //highlight
        if (highlight)
        {
            gui_set_text_color(GUI_PICKER_TEXT_COLOR_BLUE);
        }
        else
        {
            gui_set_text_color(GUI_PICKER_TEXT_COLOR_WHITE);
        }
        gui_measure_string(column_name, &title_width, &title_height);
        title_pos.x = current_pos.x + (picker_number->column_bound[index].width - title_width) / 2;
        //title_pos.y = current_pos.y + ((GUI_PICKER_9_SLICE_BG_TOP - title_height) / 2 - GUI_PICKER_9_SLICE_BG_TOP);
        title_pos.y = current_pos.y - (GUI_PICKER_9_SLICE_BG_TOP + title_height) / 2;
        gui_move_text_cursor(title_pos.x, title_pos.y);
        gui_print_text(column_name);
        
        num_total_width = num_width * picker_number->bit[index];
        picker_number->column_bound[index].x = current_pos.x;
        picker_number->column_bound[index].y = current_pos.y;
        //Draw Filed Background
        gdi_image_draw_resized_id(current_pos.x, current_pos.y, picker_number->column_bound[index].width, picker_number->column_bound[index].height, IMG_ID_PICKER_FIELD_BG);

        //Draw Number
        number_bound.x = current_pos.x = picker_number->column_bound[index].x + (picker_number->column_bound[index].width - num_total_width) / 2;
        number_bound.y = current_pos.y = picker_number->column_bound[index].y + (picker_number->column_bound[index].height - num_height) / 2;

        if (highlight)
        {
            #ifdef __MMI_TOUCH_SCREEN__
            S16 jump_index = g_picker_cntx.pos_offset / g_picker_cntx.picker_number.line_height;
            number_bound.y += g_picker_cntx.pos_offset;
            #else  //__MMI_TOUCH_SCREEN__
            S16 jump_index = 0;
            #endif  //__MMI_TOUCH_SCREEN__
            if (GUI_ABS(jump_index) > 0)
            {
                hint_num_1 += jump_index;
                hint_num_2 -= jump_index;
            }
        }

        //Draw Hint
        number_bound.width = num_total_width;
        number_bound.height = num_height;
        gdi_layer_push_and_set_clip(effective_bound.x, effective_bound.y, effective_bound.x + effective_bound.width, effective_bound.y + effective_bound.height);
        gui_picker_draw_number_hint_text(picker_number->get_switch_func[index], picker_number->number[index], picker_number->bit[index], number_bound, 
            GUI_PICKER_HINT_POSITION_AUTO_BY_BOUND, hint_num_1, hint_num_2, highlight);

        if (highlight && g_picker_cntx.ssp_state != GUI_PICKER_SSP_STATE_NONE)
        {
            #ifdef __MMI_TOUCH_SCREEN__
            if (GUI_ABS(g_picker_cntx.pos_offset) < g_picker_cntx.picker_number.line_height / 3)
            {
                gui_picker_draw_number_text(picker_number->number[index], picker_number->bit[index], number_bound, GUI_PICKER_TEXT_COLOR_THIN_BLACK, &MMI_small_font);
            }
            else
            #endif  //__MMI_TOUCH_SCREEN__
            {
                gui_picker_draw_number_text(picker_number->number[index], picker_number->bit[index], number_bound, GUI_PICKER_TEXT_COLOR_GRAY, &MMI_small_font);
            }
        }
        else
        {
            //gui_picker_draw_number_text(picker_number->number[index], picker_number->bit[index], number_bound, GUI_PICKER_TEXT_COLOR_BLACK, &MMI_large_font);
            gui_picker_draw_number_image(picker_number->number[index], picker_number->bit[index], number_bound, GUI_PICKER_NUM_COLOR_BLACK);
        }
        gdi_layer_pop_clip();

        //update position for next column
        current_pos.x = picker_number->column_bound[index].x + picker_number->column_bound[index].width + GUI_PICKER_COLUMNE_GAP;
        current_pos.y = picker_number->column_bound[index].y;
    }

    #if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
    #endif
    
    //Draw Arrow
    current_pos.x = effective_bound.x - GUI_PICKER_COLUMNE_GAP;
    current_pos.y = effective_bound.y + (effective_bound.height - GUI_PICKER_ARROW_HEIGHT) / 2;
    gdi_layer_push_and_set_clip(current_pos.x, current_pos.y, current_pos.x + GUI_PICKER_ARROW_WIDTH, 
        current_pos.y + GUI_PICKER_ARROW_HEIGHT);
    gui_draw_image_id_with_alpha_transparent(IMG_ID_PICKER_RIGHT_ARROW, current_pos.x, current_pos.y, 255);
    gdi_layer_pop_clip();
    current_pos.x += effective_bound.width - GUI_PICKER_ARROW_WIDTH + 2 * GUI_PICKER_COLUMNE_GAP;
    gdi_layer_push_and_set_clip(current_pos.x, current_pos.y, current_pos.x + GUI_PICKER_ARROW_WIDTH, 
        current_pos.y + GUI_PICKER_ARROW_HEIGHT);
    gui_draw_image_id_with_alpha_transparent(IMG_ID_PICKER_LEFT_ARROW, current_pos.x, current_pos.y, 255);
    gdi_layer_pop_clip();
    
    return effective_bound;
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_set_effective_bound
 * DESCRIPTION
 *  
 * PARAMETERS
 *  image_id
 * RETURNS
 *  S16
 *****************************************************************************/
static void gui_picker_set_effective_bound(gui_bound bound)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(bound.x  >= 0 && bound.x <= UI_device_width && bound.y >= 0 && bound.y <= UI_device_height && 
        bound.width > 0 && bound.width <= UI_device_width && bound.height > 0 && bound.height <= UI_device_height);
    memcpy((void*)&g_picker_cntx.effective_bound, (void*)&bound, sizeof(gui_bound));
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_show_date
 * DESCRIPTION
 *  Show Date Picker
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_show_date(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_bound bound = {0};
    gui_picker_date_struct *picker_date = (gui_picker_date_struct*)g_picker_cntx.user_data;
    gdi_handle base_layer;
	U8 max_day;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(picker_date->display_row % 2 != 0);
    
    bound.x = g_picker_cntx.original_bound.x;
    bound.y = g_picker_cntx.original_bound.y;
    bound.width = g_picker_cntx.original_bound.width;
    bound.height = g_picker_cntx.original_bound.height;
    
    gdi_layer_get_base_handle(&base_layer);
    gdi_layer_push_and_set_active(base_layer);
    gdi_layer_push_and_set_clip(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x + g_picker_cntx.original_bound.width, 
        g_picker_cntx.original_bound.y + g_picker_cntx.original_bound.height);
    gdi_draw_solid_rect(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x + g_picker_cntx.original_bound.width, 
        g_picker_cntx.original_bound.y + g_picker_cntx.original_bound.height, GDI_COLOR_TRANSPARENT);   

	//correct day
	max_day = gui_picker_get_max_num_of_days(picker_date->current_year, picker_date->month);
	if (picker_date->day > max_day)
		picker_date->day = max_day;

    //if (header->style == GUI_PICKER_STYLE_COSMOS)
    {
        memset(&g_picker_cntx.picker_number, 0, sizeof(gui_picker_number_struct));
        g_picker_cntx.picker_number.column = 1;
        if (g_picker_cntx.highlight_index == 1)
        {//month
            g_picker_cntx.picker_number.number[0] = picker_date->month;
            g_picker_cntx.picker_number.bit[0] = 2;
            g_picker_cntx.picker_number.get_switch_func[0] = &gui_picker_month_get_switch;
            g_picker_cntx.picker_number.column_name_id[0] = STR_ID_PICKER_MONTH;
        }
        else if (g_picker_cntx.highlight_index == 2)
        {//day
            g_picker_cntx.picker_number.number[0] = picker_date->day;
            g_picker_cntx.picker_number.bit[0] = 2;
            g_picker_cntx.picker_number.get_switch_func[0] = &gui_picker_day_get_switch;
            g_picker_cntx.picker_number.column_name_id[0] = STR_ID_PICKER_DAY;
        }
        else
        {//year
            g_picker_cntx.picker_number.number[0] = picker_date->current_year;
            g_picker_cntx.picker_number.bit[0] = 4;
            g_picker_cntx.picker_number.get_switch_func[0] = &gui_picker_year_get_switch;
            g_picker_cntx.picker_number.column_name_id[0] = STR_ID_PICKER_YEAR;
        }
        bound = gui_picker_draw_number_picker(&g_picker_cntx.picker_number, bound, picker_date->display_row);
    }

    //resize bound
    if (bound.width > g_picker_cntx.original_bound.width)
    {
        g_picker_cntx.original_bound.width = bound.width;
    }
    if (bound.height > g_picker_cntx.original_bound.height)
    {
        g_picker_cntx.original_bound.height = bound.height;
    }

    gui_picker_set_effective_bound(bound);

    gdi_layer_pop_clip();
    gdi_layer_pop_and_restore_active();
	gdi_layer_blt_previous(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x+g_picker_cntx.original_bound.width, g_picker_cntx.original_bound.y+g_picker_cntx.original_bound.height);
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_show_year
 * DESCRIPTION
 *  Show Year Picker
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_show_year(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_bound bound = {0};
    gui_picker_year_struct *picker_year = (gui_picker_year_struct*)g_picker_cntx.user_data;
    gdi_handle base_layer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(picker_year->display_row % 2 != 0);
    
    bound.x = g_picker_cntx.original_bound.x;
    bound.y = g_picker_cntx.original_bound.y;
    bound.width = g_picker_cntx.original_bound.width;
    bound.height = g_picker_cntx.original_bound.height;
    
    gdi_layer_get_base_handle(&base_layer);
    gdi_layer_push_and_set_active(base_layer);
    gdi_layer_push_and_set_clip(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x + g_picker_cntx.original_bound.width, 
        g_picker_cntx.original_bound.y + g_picker_cntx.original_bound.height);
    gdi_draw_solid_rect(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x + g_picker_cntx.original_bound.width, 
        g_picker_cntx.original_bound.y + g_picker_cntx.original_bound.height, GDI_COLOR_TRANSPARENT);   

    //if (header->style == GUI_PICKER_STYLE_COSMOS)
    {
        memset(&g_picker_cntx.picker_number, 0, sizeof(gui_picker_number_struct));
        g_picker_cntx.picker_number.column = 1;
        g_picker_cntx.picker_number.number[0] = picker_year->current_year;
        g_picker_cntx.picker_number.bit[0] = 4;
        g_picker_cntx.picker_number.get_switch_func[0] = &gui_picker_year_get_switch;
        g_picker_cntx.picker_number.column_name_id[0] = STR_ID_PICKER_YEAR;
        bound = gui_picker_draw_number_picker(&g_picker_cntx.picker_number, bound, picker_year->display_row);
    }

    //resize bound
    if (bound.width > g_picker_cntx.original_bound.width)
    {
        g_picker_cntx.original_bound.width = bound.width;
    }
    if (bound.height > g_picker_cntx.original_bound.height)
    {
        g_picker_cntx.original_bound.height = bound.height;
    }

    gui_picker_set_effective_bound(bound);

    gdi_layer_pop_clip();
    gdi_layer_pop_and_restore_active();
	gdi_layer_blt_previous(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x+g_picker_cntx.original_bound.width, g_picker_cntx.original_bound.y+g_picker_cntx.original_bound.height);
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_show_month_and_day
 * DESCRIPTION
 *  Show Month And Day Picker
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_show_month_and_day(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_bound bound = {0};
    gui_picker_month_and_day_struct *picker_month_and_day = (gui_picker_month_and_day_struct*)g_picker_cntx.user_data;
    gdi_handle base_layer;
	U8 max_day;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(picker_month_and_day->display_row % 2 != 0);
    
    bound.x = g_picker_cntx.original_bound.x;
    bound.y = g_picker_cntx.original_bound.y;
    bound.height = g_picker_cntx.original_bound.height;

    gdi_layer_get_base_handle(&base_layer);
    gdi_layer_push_and_set_active(base_layer);
    gdi_layer_push_and_set_clip(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x + g_picker_cntx.original_bound.width, 
        g_picker_cntx.original_bound.y + g_picker_cntx.original_bound.height);
    gdi_draw_solid_rect(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x + g_picker_cntx.original_bound.width, 
        g_picker_cntx.original_bound.y + g_picker_cntx.original_bound.height, GDI_COLOR_TRANSPARENT);  

	//correct day
	max_day = gui_picker_get_max_num_of_days(2000, picker_month_and_day->month);
	if (picker_month_and_day->day > max_day)
		picker_month_and_day->day = max_day;

    //if (header->style == GUI_PICKER_STYLE_COSMOS)
    {
        bound.width = g_picker_cntx.original_bound.width;
        memset(&g_picker_cntx.picker_number, 0, sizeof(gui_picker_number_struct));
        if (picker_month_and_day->two_page)
        {
            g_picker_cntx.picker_number.column = 1;
            if (g_picker_cntx.highlight_index)
            {
                g_picker_cntx.picker_number.number[0] = picker_month_and_day->day;
                g_picker_cntx.picker_number.bit[0] = 2;
                g_picker_cntx.picker_number.get_switch_func[0] = &gui_picker_day_get_switch;
                g_picker_cntx.picker_number.column_name_id[0] = STR_ID_PICKER_DAY;
            }
            else
            {
                g_picker_cntx.picker_number.number[0] = picker_month_and_day->month;
                g_picker_cntx.picker_number.bit[0] = 2;
                g_picker_cntx.picker_number.get_switch_func[0] = &gui_picker_month_get_switch;
                g_picker_cntx.picker_number.column_name_id[0] = STR_ID_PICKER_MONTH;
            }
        }
        else
        {
            g_picker_cntx.picker_number.column = 2;
            g_picker_cntx.picker_number.number[0] = picker_month_and_day->month;
            g_picker_cntx.picker_number.bit[0] = 2;
            g_picker_cntx.picker_number.get_switch_func[0] = &gui_picker_month_get_switch;
            g_picker_cntx.picker_number.column_name_id[0] = STR_ID_PICKER_MONTH;
            g_picker_cntx.picker_number.number[1] = picker_month_and_day->day;
            g_picker_cntx.picker_number.bit[1] = 2;
            g_picker_cntx.picker_number.get_switch_func[1] = &gui_picker_day_get_switch;
            g_picker_cntx.picker_number.column_name_id[1] = STR_ID_PICKER_DAY;
        }
        bound = gui_picker_draw_number_picker(&g_picker_cntx.picker_number, bound, picker_month_and_day->display_row);
        
        //resize bound
        if (bound.width > g_picker_cntx.original_bound.width)
        {
            g_picker_cntx.original_bound.width = bound.width;
        }
        if (bound.height > g_picker_cntx.original_bound.height)
        {
            g_picker_cntx.original_bound.height = bound.height;
        }
        
        gui_picker_set_effective_bound(bound);
    }
    
    gdi_layer_pop_clip();
    gdi_layer_pop_and_restore_active();
	gdi_layer_blt_previous(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x+g_picker_cntx.original_bound.width, g_picker_cntx.original_bound.y+g_picker_cntx.original_bound.height);
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_show_time
 * DESCRIPTION
 *  Show Time Picker
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_show_time(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_bound bound = {0};
    gui_picker_time_struct *picker_time = (gui_picker_time_struct*)g_picker_cntx.user_data;
    gdi_handle base_layer;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(picker_time->display_row % 2 != 0);
    
    bound.x = g_picker_cntx.original_bound.x;
    bound.y = g_picker_cntx.original_bound.y;
    bound.height = g_picker_cntx.original_bound.height;
    
    gdi_layer_get_base_handle(&base_layer);
    gdi_layer_push_and_set_active(base_layer);
    gdi_layer_push_and_set_clip(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x + g_picker_cntx.original_bound.width, 
        g_picker_cntx.original_bound.y + g_picker_cntx.original_bound.height);
    gdi_draw_solid_rect(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x + g_picker_cntx.original_bound.width, 
        g_picker_cntx.original_bound.y + g_picker_cntx.original_bound.height, GDI_COLOR_TRANSPARENT);    
    
    //if (header->style == GUI_PICKER_STYLE_COSMOS)
    {
        bound.width = g_picker_cntx.original_bound.width;
        memset(&g_picker_cntx.picker_number, 0, sizeof(gui_picker_number_struct));
        if (picker_time->two_page)
        {
            g_picker_cntx.picker_number.column = 1;
            if (g_picker_cntx.highlight_index)
            {
                g_picker_cntx.picker_number.number[0] = picker_time->minute;
                g_picker_cntx.picker_number.bit[0] = 2;
                g_picker_cntx.picker_number.get_switch_func[0] = &gui_picker_minute_get_switch;
                g_picker_cntx.picker_number.column_name_id[0] = STR_ID_PICKER_MINUTE;
            }
            else
            {
                g_picker_cntx.picker_number.number[0] = picker_time->hour;
                g_picker_cntx.picker_number.bit[0] = 2;
                g_picker_cntx.picker_number.get_switch_func[0] = &gui_picker_hour_get_switch;
                g_picker_cntx.picker_number.column_name_id[0] = STR_ID_PICKER_HOUR;
            }
        }
        else
            {
            g_picker_cntx.picker_number.column = 2;
            g_picker_cntx.picker_number.number[0] = picker_time->hour;
            g_picker_cntx.picker_number.bit[0] = 2;
            g_picker_cntx.picker_number.get_switch_func[0] = &gui_picker_hour_get_switch;
            g_picker_cntx.picker_number.column_name_id[0] = STR_ID_PICKER_HOUR;
            g_picker_cntx.picker_number.number[1] = picker_time->minute;
            g_picker_cntx.picker_number.bit[1] = 2;
            g_picker_cntx.picker_number.get_switch_func[1] = &gui_picker_minute_get_switch;
            g_picker_cntx.picker_number.column_name_id[1] = STR_ID_PICKER_MINUTE;
        }
        bound = gui_picker_draw_number_picker(&g_picker_cntx.picker_number, bound, picker_time->display_row);
        
        //resize bound
        if (bound.width > g_picker_cntx.original_bound.width)
        {
            g_picker_cntx.original_bound.width = bound.width;
        }
        if (bound.height > g_picker_cntx.original_bound.height)
        {
            g_picker_cntx.original_bound.height = bound.height;
        }
        
        gui_picker_set_effective_bound(bound);
    }

    gdi_layer_pop_clip();
    gdi_layer_pop_and_restore_active();
	gdi_layer_blt_previous(g_picker_cntx.original_bound.x, g_picker_cntx.original_bound.y, g_picker_cntx.original_bound.x+g_picker_cntx.original_bound.width, g_picker_cntx.original_bound.y+g_picker_cntx.original_bound.height);
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_year_get_switch
 * DESCRIPTION
 *  Get Year switch, it will be loop
 * PARAMETERS
 *  current_year
 *  switch_type
 *  switch_offset
 * RETURNS
 *  U16
 *****************************************************************************/
static U16 gui_picker_year_get_switch(U16 current_year, gui_picker_switch_type switch_type, U8 switch_offset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_picker_header *header = (gui_picker_header*)g_picker_cntx.user_data;
    U16 min_year, max_year;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(switch_offset > 0);

    if (header->type == GUI_PICKER_TYPE_DATE)
    {
        gui_picker_date_struct *picker_date = (gui_picker_date_struct*)g_picker_cntx.user_data;
        min_year = picker_date->min_year;
        max_year = picker_date->max_year;
    }
    else
    {
        gui_picker_year_struct *picker_year = (gui_picker_year_struct*)g_picker_cntx.user_data;
        min_year = picker_year->min_year;
        max_year = picker_year->max_year;
    }

    while (switch_offset > 0)
    {
        if (switch_type == GUI_PICKER_SWITCH_TYPE_LEFT || switch_type == GUI_PICKER_SWITCH_TYPE_UP)
        {
            if (current_year <= min_year)
            {
                current_year = max_year;
            }
            else
            {
                current_year--;
            }
        }
        else
        {
            if (current_year >= max_year)
            {
                current_year = min_year;
            }
            else
            {
                current_year++;
            }
        }

        switch_offset--;
    }

    return current_year;
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_month_get_switch
 * DESCRIPTION
 *  Get Month switch, it will be loop
 * PARAMETERS
 *  current_month
 *  switch_type
 *  switch_offset
 * RETURNS
 *  U8
 *****************************************************************************/
static U8 gui_picker_month_get_switch(U8 current_month, gui_picker_switch_type switch_type, U8 switch_offset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (switch_offset > 0)
    {
        if (switch_type == GUI_PICKER_SWITCH_TYPE_LEFT || switch_type == GUI_PICKER_SWITCH_TYPE_UP)
        {
            if (current_month <= 1)
            {
                current_month = 12;
            }
            else
            {
                current_month--;
            }
        }
        else
        {
            if (current_month >= 12)
            {
                current_month = 1;
            }
            else
            {
                current_month++;
            }
        }

        switch_offset--;
    }

    return current_month;
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_day_get_switch
 * DESCRIPTION
 *  Get Day switch, it will be loop
 * PARAMETERS
 *  current_day
 *  switch_type
 *  switch_offset
 * RETURNS
 *  U8
 *****************************************************************************/
static U8 gui_picker_day_get_switch(U8 current_day, gui_picker_switch_type switch_type, U8 switch_offset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	gui_picker_header *header = (gui_picker_header*)g_picker_cntx.user_data;
	U8 max_day;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	if (header->type == GUI_PICKER_TYPE_DATE)
	{
    	gui_picker_date_struct *picker_date = (gui_picker_date_struct*)g_picker_cntx.user_data;
		max_day = gui_picker_get_max_num_of_days(picker_date->current_year, picker_date->month);
	}
	else
	{
    	gui_picker_month_and_day_struct *picker_month_and_day = (gui_picker_month_and_day_struct*)g_picker_cntx.user_data;
		max_day = gui_picker_get_max_num_of_days(2000, picker_month_and_day->month);
	}
	
    while (switch_offset > 0)
    {
        if (switch_type == GUI_PICKER_SWITCH_TYPE_LEFT || switch_type == GUI_PICKER_SWITCH_TYPE_UP)
        {
            if (current_day <= 1)
            {
                current_day = max_day;
            }
            else
            {
                current_day--;
            }
        }
        else
        {
            if (current_day >= max_day)
            {
                current_day = 1;
            }
            else
            {
                current_day++;
            }
        }

        switch_offset--;
    }

    return current_day;
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_hour_get_switch
 * DESCRIPTION
 *  Get Hour switch, it will be loop
 * PARAMETERS
 *  current_hour
 *  switch_type
 *  switch_offset
 * RETURNS
 *  U8
 *****************************************************************************/
static U8 gui_picker_hour_get_switch(U8 current_hour, gui_picker_switch_type switch_type, U8 switch_offset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (switch_offset > 0)
    {
        if (switch_type == GUI_PICKER_SWITCH_TYPE_LEFT || switch_type == GUI_PICKER_SWITCH_TYPE_UP)
        {
            if (current_hour <= 0)
            {
                current_hour = 23;
            }
            else
            {
                current_hour--;
            }
        }
        else
        {
            if (current_hour >= 23)
            {
                current_hour = 0;
            }
            else
            {
                current_hour++;
            }
        }

        switch_offset--;
    }

    return current_hour;
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_minute_get_switch
 * DESCRIPTION
 *  Get Minute switch, it will be loop
 * PARAMETERS
 *  current_minute
 *  switch_type
 *  switch_offset
 * RETURNS
 *  U8
 *****************************************************************************/
static U8 gui_picker_minute_get_switch(U8 current_minute, gui_picker_switch_type switch_type, U8 switch_offset)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (switch_offset > 0)
    {
        if (switch_type == GUI_PICKER_SWITCH_TYPE_LEFT || switch_type == GUI_PICKER_SWITCH_TYPE_UP)
        {
            if (current_minute <= 0)
            {
                current_minute = 59;
            }
            else
            {
                current_minute--;
            }
        }
        else
        {
            if (current_minute >= 59)
            {
                current_minute = 0;
            }
            else
            {
                current_minute++;
            }
        }

        switch_offset--;
    }

    return current_minute;
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_switch_date
 * DESCRIPTION
 *  Switch year
 * PARAMETERS
 *  switch_type
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_switch_date(gui_picker_switch_type switch_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_picker_date_struct *picker_date = (gui_picker_date_struct*)g_picker_cntx.user_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_picker_cntx.highlight_index == 1)
    {
        picker_date->month = gui_picker_month_get_switch(picker_date->month, switch_type, 1);
    }
    if (g_picker_cntx.highlight_index == 2)
    {
        picker_date->day = gui_picker_day_get_switch(picker_date->day, switch_type, 1);
    }
    else
    {
        picker_date->current_year = gui_picker_year_get_switch(picker_date->current_year, switch_type, 1);
    }
    gui_picker_show_date();
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_switch_year
 * DESCRIPTION
 *  Switch year
 * PARAMETERS
 *  switch_type
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_switch_year(gui_picker_switch_type switch_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_picker_year_struct *picker_year = (gui_picker_year_struct*)g_picker_cntx.user_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    picker_year->current_year = gui_picker_year_get_switch(picker_year->current_year, switch_type, 1);
    gui_picker_show_year();
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_switch_month_and_day
 * DESCRIPTION
 *  Switch month and day
 * PARAMETERS
 *  switch_type
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_switch_month_and_day(gui_picker_switch_type switch_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_picker_month_and_day_struct *picker_month_and_day = (gui_picker_month_and_day_struct*)g_picker_cntx.user_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_picker_cntx.highlight_index == 1)
    {//switch day
        picker_month_and_day->day = gui_picker_day_get_switch(picker_month_and_day->day, switch_type, 1);
    }
    else
    {//switch month
        picker_month_and_day->month = gui_picker_month_get_switch(picker_month_and_day->month, switch_type, 1);
    }

    gui_picker_show_month_and_day();
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_switch_time
 * DESCRIPTION
 *  Switch time
 * PARAMETERS
 *  switch_type
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_switch_time(gui_picker_switch_type switch_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_picker_time_struct *picker_time = (gui_picker_time_struct*)g_picker_cntx.user_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_picker_cntx.highlight_index == 1)
    {//switch minute
        picker_time->minute = gui_picker_minute_get_switch(picker_time->minute, switch_type, 1);
    }
    else
    {//switch hour
        picker_time->hour = gui_picker_hour_get_switch(picker_time->hour, switch_type, 1);
    }
    
    gui_picker_show_time();
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_switch
 * DESCRIPTION
 *  Switch number
 * PARAMETERS
 *  switch_type
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_switch(gui_picker_switch_type switch_type)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_picker_header *header = (gui_picker_header*)g_picker_cntx.user_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    switch (header->type)
    {
        case GUI_PICKER_TYPE_DATE:
        {
            gui_picker_switch_date(switch_type);
            break;
        }
        case GUI_PICKER_TYPE_YEAR:
        {
            gui_picker_switch_year(switch_type);
            break;
        }
        case GUI_PICKER_TYPE_MONTH_AND_DAY:
        {
            gui_picker_switch_month_and_day(switch_type);
            break;
        }
        case GUI_PICKER_TYPE_TIME:
        {
            gui_picker_switch_time(switch_type);
            break;
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_switch_to_prev
 * DESCRIPTION
 *  Switch number to previous
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_switch_to_prev(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_picker_switch(GUI_PICKER_SWITCH_TYPE_LEFT);
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_switch_to_next
 * DESCRIPTION
 *  Switch number to next
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_switch_to_next(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_picker_switch(GUI_PICKER_SWITCH_TYPE_RIGHT);
}


#ifdef __MMI_TOUCH_SCREEN__

/*****************************************************************************
 * FUNCTION
 *  gui_picker_ssp_update_picker
 * DESCRIPTION
 *  
 * PARAMETERS
 *  voids
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_ssp_update_picker(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_picker_header *header = (gui_picker_header*)g_picker_cntx.user_data;
    S16 offset = g_picker_cntx.pos_offset / g_picker_cntx.picker_number.line_height;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (header->type == GUI_PICKER_TYPE_DATE)
    {
        gui_picker_date_struct *picker_date = (gui_picker_date_struct *)g_picker_cntx.user_data;

        if (offset > 0)
        {
            if (g_picker_cntx.highlight_index == 1)
            {
                picker_date->month = gui_picker_month_get_switch(picker_date->month, GUI_PICKER_SWITCH_TYPE_UP, offset);
            }
            else if (g_picker_cntx.highlight_index == 2)
            {
                picker_date->day = gui_picker_day_get_switch(picker_date->day, GUI_PICKER_SWITCH_TYPE_UP, offset);
            }
            else
            {
                picker_date->current_year = gui_picker_year_get_switch(picker_date->current_year, GUI_PICKER_SWITCH_TYPE_UP, offset);
            }
        }
        else
        {
            if (g_picker_cntx.highlight_index == 1)
            {
                picker_date->month = gui_picker_month_get_switch(picker_date->month, GUI_PICKER_SWITCH_TYPE_DOWN, -offset);
            }
            else if (g_picker_cntx.highlight_index == 2)
            {
                picker_date->day = gui_picker_day_get_switch(picker_date->day, GUI_PICKER_SWITCH_TYPE_DOWN, -offset);
            }
            else
            {
                picker_date->current_year = gui_picker_year_get_switch(picker_date->current_year, GUI_PICKER_SWITCH_TYPE_DOWN, -offset);
            }
        }
    }
    else if (header->type == GUI_PICKER_TYPE_YEAR)
    {
        gui_picker_year_struct *picker_year = (gui_picker_year_struct *)g_picker_cntx.user_data;

        if (offset > 0)
        {
            picker_year->current_year = gui_picker_year_get_switch(picker_year->current_year, GUI_PICKER_SWITCH_TYPE_UP, offset);
        }
        else
        {
            picker_year->current_year = gui_picker_year_get_switch(picker_year->current_year, GUI_PICKER_SWITCH_TYPE_DOWN, -offset);
        }
    }
    else if (header->type == GUI_PICKER_TYPE_MONTH_AND_DAY)
    {
        gui_picker_month_and_day_struct *picker_month_and_day = (gui_picker_month_and_day_struct *)g_picker_cntx.user_data;

        if (offset > 0)
        {
            if (g_picker_cntx.highlight_index == 1)
            {
                picker_month_and_day->day = gui_picker_day_get_switch(picker_month_and_day->day, GUI_PICKER_SWITCH_TYPE_UP, offset);
            }
            else
            {
                picker_month_and_day->month = gui_picker_month_get_switch(picker_month_and_day->month, GUI_PICKER_SWITCH_TYPE_UP, offset);
            }
        }
        else
        {
            if (g_picker_cntx.highlight_index == 1)
            {
                picker_month_and_day->day = gui_picker_day_get_switch(picker_month_and_day->day, GUI_PICKER_SWITCH_TYPE_DOWN, -offset);
            }
            else
            {
                picker_month_and_day->month = gui_picker_month_get_switch(picker_month_and_day->month, GUI_PICKER_SWITCH_TYPE_DOWN, -offset);
            }
        }
    }
    else if (header->type == GUI_PICKER_TYPE_TIME)
    {
        gui_picker_time_struct *picker_time = (gui_picker_time_struct *)g_picker_cntx.user_data;

        if (offset > 0)
        {
            if (g_picker_cntx.highlight_index == 1)
            {
                picker_time->minute = gui_picker_minute_get_switch(picker_time->minute, GUI_PICKER_SWITCH_TYPE_UP, offset);
            }
            else
            {
                picker_time->hour = gui_picker_hour_get_switch(picker_time->hour, GUI_PICKER_SWITCH_TYPE_UP, offset);
            }
        }
        else
        {
            if (g_picker_cntx.highlight_index == 1)
            {
                picker_time->minute = gui_picker_minute_get_switch(picker_time->minute, GUI_PICKER_SWITCH_TYPE_DOWN, -offset);
            }
            else
            {
                picker_time->hour = gui_picker_hour_get_switch(picker_time->hour, GUI_PICKER_SWITCH_TYPE_DOWN, -offset);
            }
        }
    }
    
    g_picker_cntx.pos_offset -= offset * g_picker_cntx.picker_number.line_height;
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_ssp_free_run
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_ssp_free_run(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_cancel_timer(gui_picker_ssp_free_run);
    
    //update picker offset
    if (GUI_ABS(g_picker_cntx.pos_offset) >= g_picker_cntx.picker_number.line_height)
    {
        gui_picker_ssp_update_picker();
    }

    if (g_picker_cntx.ssp_state == GUI_PICKER_SSP_STATE_FREE_RUN)
    {
#ifdef __GUI_PICKER_SSP_AUTO_FREE_RUN__
        float speed_reduce = g_picker_cntx.ssp_speed * (float)GUI_PICKER_SSP_REDUCING_FACTOR;
        g_picker_cntx.ssp_speed -= speed_reduce;
        if (GUI_ABS(g_picker_cntx.ssp_speed) < GUI_PICKER_SSP_STOP_THRESHOLD)
        {
            gui_picker_ssp_recover();
            return;
        }
        else
        {
            g_picker_cntx.pos_offset += g_picker_cntx.ssp_speed;
            gui_start_timer(GUI_PICKER_SSP_TIMER_DELAY, gui_picker_ssp_free_run);
        }
#endif  //__GUI_PICKER_SSP_AUTO_FREE_RUN__
    }
    else if (g_picker_cntx.ssp_state == GUI_PICKER_SSP_STATE_RECOVER)
    {
        if (GUI_ABS(g_picker_cntx.pos_offset) > 0)
        {
            if (g_picker_cntx.pos_offset > 0)
            {
                g_picker_cntx.pos_offset -= GUI_PICKER_SSP_RECOVER_STEP;
                g_picker_cntx.pos_offset = g_picker_cntx.pos_offset < 0 ? 0 : g_picker_cntx.pos_offset;
            }
            else
            {
                g_picker_cntx.pos_offset += GUI_PICKER_SSP_RECOVER_STEP;
                g_picker_cntx.pos_offset = g_picker_cntx.pos_offset > 0 ? 0 : g_picker_cntx.pos_offset;
            }
            gui_start_timer(GUI_PICKER_SSP_TIMER_DELAY, gui_picker_ssp_free_run);
        }
        else
        {
            g_picker_cntx.ssp_state = GUI_PICKER_SSP_STATE_NONE;
        }
    }
    
    gui_picker_show();
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_ssp_recover
 * DESCRIPTION
 *  
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_ssp_recover(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_picker_cntx.ssp_state = GUI_PICKER_SSP_STATE_RECOVER;
    
    //recover
    if (GUI_ABS(g_picker_cntx.pos_offset) >= g_picker_cntx.picker_number.line_height / 3)
    {
        if (g_picker_cntx.pos_offset > 0)
        {
            g_picker_cntx.pos_offset = g_picker_cntx.picker_number.line_height;
        }
        else
        {
            g_picker_cntx.pos_offset = -g_picker_cntx.picker_number.line_height;
        }
    }
    
    gui_picker_ssp_free_run();
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_pen_down_hdlr
 * DESCRIPTION
 *  Pen down handler
 * PARAMETERS
 *  pos
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_pen_down_hdlr(mmi_pen_point_struct pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_picker_header *header = (gui_picker_header*)g_picker_cntx.user_data;
    U32 ticks;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    gui_cancel_timer(gui_picker_ssp_free_run);
    
    kal_get_time(&ticks);
    g_picker_cntx.pen_down_time = kal_ticks_to_milli_secs(ticks);
    g_picker_cntx.last_pen_move_pos.x = g_picker_cntx.pen_down_pos.x = pos.x;
    g_picker_cntx.last_pen_move_pos.y = g_picker_cntx.pen_down_pos.y = pos.y;

    if (g_picker_cntx.ssp_state != GUI_PICKER_SSP_STATE_NONE)
    {
        #ifdef __GUI_PICKER_SSP_AUTO_FREE_RUN__
        g_picker_cntx.ssp_speed = 0;
        #endif
        g_picker_cntx.pos_offset = 0;
        gui_picker_ssp_recover();
    }

    if (pos.x >= g_picker_cntx.effective_bound.x && pos.x <= (g_picker_cntx.effective_bound.x + g_picker_cntx.effective_bound.width) &&
        pos.y >= g_picker_cntx.effective_bound.y && pos.y <= (g_picker_cntx.effective_bound.y + g_picker_cntx.effective_bound.height))
    {
        g_picker_cntx.pen_status = GUI_PICKER_PEN_STATUS_DOWN_IN_VISIBLE;
        g_picker_cntx.ssp_state = GUI_PICKER_SSP_STATE_HOLD;

        if (header->type == GUI_PICKER_TYPE_MONTH_AND_DAY)
        {
            gui_picker_month_and_day_struct *picker_month_and_day = (gui_picker_month_and_day_struct*)g_picker_cntx.user_data;
            if (!picker_month_and_day->two_page)
            {
                g_picker_cntx.highlight_index = 0;
                if (pos.x > g_picker_cntx.original_bound.width / 2)
                {
                    g_picker_cntx.highlight_index = 1;
                }
            }
        }
        else if (header->type == GUI_PICKER_TYPE_TIME)
        {
            gui_picker_time_struct *picker_time = (gui_picker_time_struct*)g_picker_cntx.user_data;
            if (!picker_time->two_page)
            {
                g_picker_cntx.highlight_index = 0;
                if (pos.x > g_picker_cntx.original_bound.width / 2)
                {
                    g_picker_cntx.highlight_index = 1;
                }
            }
        }
        gui_picker_show();
    }
    #if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
    #endif
    
    wgui_general_pen_down_hdlr(pos);
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_pen_move_hdlr
 * DESCRIPTION
 *  Pen move handler
 * PARAMETERS
 *  pos
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_pen_move_hdlr(mmi_pen_point_struct pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 y_diff = pos.y - g_picker_cntx.last_pen_move_pos.y;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (g_picker_cntx.pen_status == GUI_PICKER_PEN_STATUS_DOWN_IN_VISIBLE)
    {
        g_picker_cntx.ssp_state = GUI_PICKER_SSP_STATE_MOVE;
        g_picker_cntx.pos_offset += y_diff / 2;
        gui_picker_ssp_free_run();
    }
    
    g_picker_cntx.last_pen_move_pos.x = pos.x;
    g_picker_cntx.last_pen_move_pos.y = pos.y;

    wgui_general_pen_move_hdlr(pos);
}


#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif


/*****************************************************************************
 * FUNCTION
 *  gui_picker_pen_up_hdlr
 * DESCRIPTION
 *  Pen down handler
 * PARAMETERS
 *  pos
 * RETURNS
 *  void
 *****************************************************************************/
static void gui_picker_pen_up_hdlr(mmi_pen_point_struct pos)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S16 y_diff = pos.y - g_picker_cntx.pen_down_pos.y;
    U32 time_diff;
    U32 ticks, current_time;
        
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    kal_get_time(&ticks);
	
    current_time = kal_ticks_to_milli_secs(ticks);
    time_diff = current_time - g_picker_cntx.pen_down_time;
    
    if (g_picker_cntx.pen_status != GUI_PICKER_PEN_STATUS_NONE)
    { 
        if (g_picker_cntx.pen_status == GUI_PICKER_PEN_STATUS_DOWN_IN_VISIBLE)
        {
            #ifdef __GUI_PICKER_SSP_AUTO_FREE_RUN__
            if (time_diff < GUI_PICKER_SSP_TIME_THRESHOLD && GUI_ABS(y_diff) >= GUI_PICKER_SSP_FREE_RUN_THRESHOLD)
            {
                if (y_diff > 0)
                {
                    g_picker_cntx.ssp_speed = (float)GUI_PICKER_SSP_FREE_RUN_SPEED;
                }
                else
                {
                    g_picker_cntx.ssp_speed = -(float)GUI_PICKER_SSP_FREE_RUN_SPEED;
                }
                g_picker_cntx.ssp_state = GUI_PICKER_SSP_STATE_FREE_RUN;
                gui_picker_ssp_free_run();
            }
            else
            #endif  //__GUI_PICKER_SSP_AUTO_FREE_RUN__
            {
                gui_picker_ssp_recover();
            }
        }
    }

    g_picker_cntx.pen_status = GUI_PICKER_PEN_STATUS_NONE;
    g_picker_cntx.pen_down_time = 0;

    wgui_general_pen_up_hdlr(pos);
}

#endif  //__MMI_TOUCH_SCREEN__


/*****************************************************************************
 * FUNCTION
 *  gui_picker_set_highlight
 * DESCRIPTION
 *  Set picker highlight item (use for month&day or time)
 * PARAMETERS
 *  highlight_index
 * RETURNS
 *  void
 *****************************************************************************/
void gui_picker_set_highlight(U8 highlight_index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    g_picker_cntx.highlight_index = highlight_index;
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_get_user_data
 * DESCRIPTION
 *  Get Picker user data
 * PARAMETERS
 *  void
 * RETURNS
 *  U8*
 *****************************************************************************/
U8* gui_picker_get_user_data(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return (U8*)&g_picker_cntx.user_data;
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_create
 * DESCRIPTION
 *  Create Picker
 * PARAMETERS
 *  user_data [IN] it contains a gui picker struct
 *  bound [IN] the whole area picker in
 * RETURNS
 *  U8  -- if error, will return error code
 *****************************************************************************/
U8 gui_picker_create(U8* user_data, gui_bound bound)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_picker_header *header = (gui_picker_header *)user_data;
    #ifdef __MMI_BT_BOX_TEST_DIALER_PAD_STYLE__ 
	U8 picker_type = MMI_PHNSET_BT_BOX_PICKER_TYPE1;
	S16 error;
	#endif  //__MMI_BT_BOX_TEST_DIALER_PAD_STYLE__

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_TRACE(MMI_FW_TRC_G2_GUI, TRC_GUI_PICKER_CREATE, user_data, bound.x, bound.y, bound.width, bound.height);
    MMI_ASSERT(user_data != NULL);
    
    MMI_ASSERT(bound.x >= 0 && bound.x <= UI_device_width && bound.y >= 0 && bound.y <= UI_device_height &&
        bound.width >= 1 && bound.width <= UI_device_width && bound.height >= 1 && bound.height <= UI_device_height);
    
    if (header->type >= GUI_PICKER_TYPE_MAX)
        return (U8)GUI_PICKER_ERROR_WRONG_TYPE;

    #ifdef __MMI_BT_BOX_TEST_DIALER_PAD_STYLE__
	ReadValue(NVRAM_DISPCHAR_BT_BOX_PICKER_TYPE, &picker_type, DS_BYTE, &error);
    #endif  //__MMI_BT_BOX_TEST_DIALER_PAD_STYLE__
    
    if (header->type == GUI_PICKER_TYPE_DATE)
    {
        gui_picker_date_struct *picker_date = (gui_picker_date_struct*)user_data;
        //data correction
        if (picker_date->min_year > picker_date->current_year)
            picker_date->min_year = picker_date->current_year;
        if (picker_date->current_year > picker_date->max_year)
            picker_date->max_year = picker_date->current_year;
        if (picker_date->month > 12)
            picker_date->month = 12;
        if (picker_date->day > 31)
            picker_date->day = 31;
    }
    else if (header->type == GUI_PICKER_TYPE_YEAR)
    {
        gui_picker_year_struct *picker_year = (gui_picker_year_struct*)user_data;
        //data correction
        if (picker_year->min_year > picker_year->current_year)
            picker_year->min_year = picker_year->current_year;
        
        if (picker_year->current_year > picker_year->max_year)
            picker_year->max_year = picker_year->current_year;
    }
    else if (header->type == GUI_PICKER_TYPE_MONTH_AND_DAY)
    {
        gui_picker_month_and_day_struct *picker_month_and_day = (gui_picker_month_and_day_struct*)user_data;
        //data correction
        if (picker_month_and_day->month > 12)
            picker_month_and_day->month = 12;

        if (picker_month_and_day->day > 31)
            picker_month_and_day->day = 31;

        #ifdef __MMI_BT_BOX_TEST_DIALER_PAD_STYLE__
        if (picker_type)
        {
            picker_month_and_day->two_page = MMI_FALSE;
        }
        else
        {
            picker_month_and_day->two_page = MMI_TRUE;
        }
        #endif  //__MMI_BT_BOX_TEST_DIALER_PAD_STYLE__
    }
    else if (header->type == GUI_PICKER_TYPE_TIME)
    {
        gui_picker_time_struct *picker_time = (gui_picker_time_struct*)user_data;
        //data correction
        if (picker_time->hour > 23)
            picker_time->hour = 23;
        if (picker_time->minute > 59)
            picker_time->minute = 59;

        #ifdef __MMI_BT_BOX_TEST_DIALER_PAD_STYLE__
        if (picker_type)
        {
            picker_time->two_page = MMI_FALSE;
        }
        else
        {
            picker_time->two_page = MMI_TRUE;
        }
        #endif  //__MMI_BT_BOX_TEST_DIALER_PAD_STYLE__
    }
    
    if (!((U8)header->style >= (U8)GUI_PICKER_STYLE_START && (U8)header->style <= (U8)GUI_PICKER_STYLE_END))
        return (U8)GUI_PICKER_ERROR_WRONG_STYLE;

    if (user_data != g_picker_cntx.user_data)
    {
        memset(&g_picker_cntx, 0, sizeof(gui_picker_context_struct));
        memcpy((void*)g_picker_cntx.user_data, (void*)user_data, GUI_PICKER_STRUCT_MAX_SIZE);
    }
    else
    {
        memset(&g_picker_cntx, 0, sizeof(gui_picker_context_struct) - GUI_PICKER_STRUCT_MAX_SIZE);
    }
    memcpy((void*)&g_picker_cntx.original_bound, (void*)&bound, sizeof(gui_bound));

    return (U8)GUI_PICKER_NO_ERROR;
}


/*****************************************************************************
 * FUNCTION
 *  gui_picker_show
 * DESCRIPTION
 *  Show Picker
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void gui_picker_show(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    gui_picker_header *header = (gui_picker_header*)g_picker_cntx.user_data;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    MMI_ASSERT(header->type < GUI_PICKER_TYPE_MAX);

    switch (header->type)
    {
        case GUI_PICKER_TYPE_DATE:
        {
            gui_picker_show_date();
            break;
        }
        case GUI_PICKER_TYPE_YEAR:
        {
            gui_picker_show_year();
            break;
        }
        case GUI_PICKER_TYPE_MONTH_AND_DAY:
        {
            gui_picker_show_month_and_day();
            break;
        }
        case GUI_PICKER_TYPE_TIME:
        {
            gui_picker_show_time();
            break;
        }
    }
    
    SetKeyHandler(gui_picker_switch_to_prev, KEY_UP_ARROW, KEY_EVENT_UP);
    SetKeyHandler(gui_picker_switch_to_next, KEY_DOWN_ARROW, KEY_EVENT_UP);
    SetKeyHandler(gui_picker_switch_to_prev, KEY_UP_ARROW, KEY_EVENT_REPEAT);
    SetKeyHandler(gui_picker_switch_to_next, KEY_DOWN_ARROW, KEY_EVENT_REPEAT);
    
#ifdef __MMI_TOUCH_SCREEN__
    wgui_register_pen_down_handler(gui_picker_pen_down_hdlr);
    wgui_register_pen_move_handler(gui_picker_pen_move_hdlr);
    //wgui_register_pen_repeat_handler(gui_picker_pen_repeat_hdlr);
    //wgui_register_pen_long_tap_handler(gui_picker_pen_long_tap_hdlr);
    wgui_register_pen_up_handler(gui_picker_pen_up_hdlr);
#endif
}



/*****************************************************************************
 * FUNCTION
 *  gui_picker_inject_string
 * DESCRIPTION
 *  This function handle inject string. only available when __RANFOW_TUNING__ is defined
 * PARAMETERS
 *  index
 * RETURNS
 *  U8
 *****************************************************************************/
U8 gui_picker_inject_string(U8 index)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
#ifdef __RANFOW_TUNING__
    MMI_TRACE(MMI_FW_TRC_G2_GUI, TRC_GUI_PICKER_INJECT_STRING, g_picker_cntx.inject_index, index);
    g_picker_cntx.inject_index = index;
#endif  //__RANFOW_TUNING__
    return 0;
}


#endif  //__MMI_UI_TIME_PICKER__

