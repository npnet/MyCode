/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
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
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *    guobiengine.c
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
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
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/

#include "MMI_features.h"
#include "GlobalConstants.h"
#include "ImeGprot.h"
#include "PixtelDataTypes.h"
#include "Fat_fs.h"
#include "DebugInitDef.h"
#include "Imc_symbol.h"
#include "Imc_config.h"
#include "gui_themes.h"



#if defined(MMI_GUOBI)

#include "gbapi.h"
#include "gbtype.h"
#include "gbim.h"
#include "gbdef.h"
#include "gb_en.h"
#include "gb_candidate.h"
#include "guobiengine.h"
#include "Word_engine.h"
#include "wgui_inputs.h"
#include "ImmProt.h"
#ifdef __MMI_GB_SM_CHINESE__
#include "gb_sc.h"
#endif
#ifdef __MMI_GB_HINDI__
#include "gb_hi.h"
#endif
#ifdef __MMI_GB_INDONESIAN__
#include "gb_id.h"
#endif
#ifdef __MMI_GB_ARABIC__
#include "gb_ar.h"
#endif
#ifdef __MMI_GB_PERSIAN__
#include "gb_fa.h"
#endif
#ifdef __MMI_GB_FRENCH__
#include "gb_fr.h"
#endif
#include "IMERes.h"     /* include cfor sIMEModeDetails */
#include "Imc.h"

typedef struct
{
    S32								input_mode;
    const GBU8 						*key_map_p;
    U32	                          	nLdbNum;
    U32                             required_keys;
    U32                             symbol_keys;
    mmi_ime_get_spellings_funcptr   get_spell_funcptr;
    mmi_ime_get_associates_funcptr  get_assoc_funcptr;
    mmi_ime_get_candidates_funcptr  get_cand_funcptr;
}  mmi_ime_mode_info_struct, * mmi_ime_mode_info_struct_p;


typedef struct
{
	pGBInputStruct						ime_init_mode_p;					
	GBAuxInfo							*aux_p;					
	const mmi_ime_mode_info_struct 		*ime_mode_info_p;
	const GBLDB							*ldbs_num_map_p;	
	GBU16								g_nDataCount;
	const GBU8  						*key_map_p;
	U8										case_type;
	
} GBEngine, *PGBEngine;

/*****************************************************************************
 * Local Functions
 *****************************************************************************/
	

static void mmi_ime_goubi_word_get_candidates_alphabetic( mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr );
static void mmi_ime_guobi_get_spellings_stroke( mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr );
static void mmi_ime_guobi_get_associates( mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr );
static void mmi_ime_guobi_get_candidates_stroke( mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr );
static void mmi_ime_guobi_get_spellings_pinyin( mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr );
static void mmi_ime_guobi_get_candidates_pinyin( mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr );
static U16 mmi_ime_word_get_corresponding_stroke(UI_character_type code);
static MMI_BOOL mmi_ime_guobi_have_next_cand_page(PGBEngine g_ime_engine);
static void mmi_ime_guobi_GetNextPageAssociates(PGBEngine g_ime_engine);
static void mmi_ime_guobi_GetPrevPageAssociates(PGBEngine g_ime_engine);
static S32 mmi_ime_guobi_have_prev_cand_page(PGBEngine g_ime_engine);
static S32 mmi_ime_guobi_send_key_event(PGBEngine g_ime_engine, GBINT evt, GBU32 param);
static UI_character_type mmi_ime_guobi_word_symbol_to_GBcode(U16 symbol);
static GBRETURN mmi_ime_guobi_setpageoption(PGBEngine g_ime_engine, GBBOOL isftepage);
static S32 mmi_ime_guobi_GetStrWidth(void * pParam, GBLPCWCHAR str);
static void mmi_ime_guobi_get_candidates_pinyin_smart(mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr);
static MMI_BOOL mmi_ime_guobi_is_ChnUAWStatus(PGBEngine g_ime_engine);
static MMI_BOOL mmi_ime_guobi_HaveNextSyllPage(PGBEngine g_ime_engine);
static UI_character_type mmi_ime_guobi_word_symbol_to_code_stroke(U16 symbol);


/*****************************************************************************
 * Local variables
 *****************************************************************************/
#if defined(__MMI_GB_SM_CHINESE__) || defined(__MMI_GB_TR_CHINESE__)
static const mmi_imc_symb_stroke_enum g_guobi_keys_stroke[MMI_IMC_SYMB_KEY_MAX - MMI_IMC_SYMB_KEY_BASE] =
{
    0,                                /* MMI_IMC_SYMB_KEY_0 */
    MMI_IMC_SYMB_STROKE_OVER,         /* MMI_IMC_SYMB_KEY_1 */
    MMI_IMC_SYMB_STROKE_DOWN,         /* MMI_IMC_SYMB_KEY_2 */
    MMI_IMC_SYMB_STROKE_LEFT,         /* MMI_IMC_SYMB_KEY_3 */
    MMI_IMC_SYMB_STROKE_DOT,          /* MMI_IMC_SYMB_KEY_4 */
    MMI_IMC_SYMB_STROKE_OVER_DOWN,    /* MMI_IMC_SYMB_KEY_5 */
    MMI_IMC_SYMB_STROKE_WILDCARD,     /* MMI_IMC_SYMB_KEY_6 */
    0,                                /* MMI_IMC_SYMB_KEY_7 */
    0,                                /* MMI_IMC_SYMB_KEY_8 */
    0,                                /* MMI_IMC_SYMB_KEY_9 */
    0,                                /* MMI_IMC_SYMB_KEY_STAR */
    0                                 /* MMI_IMC_SYMB_KEY_POUND */
};
#endif

static const GBLDB g_ldbs_map[] = {

					#ifdef __MMI_GB_ARABIC__
					{
						GBL_Arabic,
						GBL_Sub_Not_Spec,
						prv_arDataArray
					},
					#endif
					#ifdef __MMI_GB_ENGLISH__
					{
						GBL_English,
						GBL_Sub_Not_Spec,
						prv_enDataArray
					},
					#endif
					#ifdef __MMI_GB_PERSIAN__
					{
						GBL_Farsi,
						GBL_Sub_Not_Spec,
						prv_faDataArray
					},
					#endif
					#ifdef __MMI_GB_FRENCH__
					{
						GBL_French,
						GBL_Sub_Not_Spec,
						prv_frDataArray
					},
					#endif
					#ifdef __MMI_GB_HINDI__
					{
						GBL_Hindi,
						GBL_Sub_Not_Spec,
						prv_hiDataArray
					},
					#endif
					#ifdef __MMI_GB_INDONESIAN__
					{
						GBL_Indonesian,
						GBL_Sub_Not_Spec,
						prv_idDataArray
					},
					#endif
					#ifdef __MMI_GB_SM_CHINESE__
					{
						GBL_Chinese,
						GBL_Sub_Not_Spec,
						prv_scdataArray
					},
					#endif
					#ifdef __MMI_GB_TR_CHINESE__
					{
						GBL_Traditional_Chinese,
						GBL_Sub_Not_Spec,
						prv_tcdataArray
					},
					#endif
					{
						GBL_NULL,
						GBL_Sub_Not_Spec,
						NULL
					}
				};
static const GBU8 MapKey[] =
{
    9, 0, 1, 2, 3, 4, 5, 6, 7, 8
};

UI_character_type g_key_map_alphabetic[MMI_IMC_SYMB_KEY_MAX - MMI_IMC_SYMB_KEY_BASE] = 
{
		GBKEY_0,		
		GBKEY_1,
		GBKEY_2,
		GBKEY_3,
		GBKEY_4,
		GBKEY_5,
		GBKEY_6,
		GBKEY_7,
		GBKEY_8,
		GBKEY_9
};
	
	


static const mmi_ime_mode_info_struct g_mode_infos[] = 
{
	#ifdef __MMI_GB_ENGLISH__
	{	
			
			INPUT_MODE_SMART_UPPERCASE_ABC,
			MapKey,
			GBL_English,
			MMI_IME_REQUIRED_ALL_KEYS,
			MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
			NULL,
			NULL,
			mmi_ime_goubi_word_get_candidates_alphabetic
	},
	{	
			INPUT_MODE_SMART_LOWERCASE_ABC,
			MapKey,
			GBL_English,
			MMI_IME_REQUIRED_ALL_KEYS,
			MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
			NULL,
			NULL,
			mmi_ime_goubi_word_get_candidates_alphabetic
	},
	#endif /* #if defined(__MMI_GB_ENGLISH__) */
	
	#ifdef __MMI_GB_ARABIC__
    {   
        INPUT_MODE_SMART_ARABIC,
        MapKey,
        GBL_Arabic,
        MMI_IME_REQUIRED_ALL_KEYS,
        MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
        NULL,
        NULL,
        mmi_ime_goubi_word_get_candidates_alphabetic
    },
	#endif /* #if defined(__MMI_GB_ARABIC__) */
	
	#ifdef __MMI_GB_PERSIAN__
    {   
        INPUT_MODE_SMART_PERSIAN,
        MapKey,
        GBL_Farsi,
        MMI_IME_REQUIRED_ALL_KEYS,
        MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
        NULL,
        NULL,
        mmi_ime_goubi_word_get_candidates_alphabetic
    },
	#endif /* #if defined(__MMI_GB_ARABIC__) */
		
		
	#ifdef __MMI_GB_FRENCH__
	{	 
		 INPUT_MODE_SMART_UPPERCASE_FRENCH,
		 MapKey,
		 GBL_French,
		 MMI_IME_REQUIRED_ALL_KEYS,
		 MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
		 NULL,
		 NULL,
		 mmi_ime_goubi_word_get_candidates_alphabetic
	 },
	 {	 
		 INPUT_MODE_SMART_LOWERCASE_FRENCH,
		 MapKey,
		 GBL_French,
		 MMI_IME_REQUIRED_ALL_KEYS,
		 MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
		 NULL,
		 NULL,
		 mmi_ime_goubi_word_get_candidates_alphabetic
	 },
	#endif /* #if defined(__MMI_GB_FRENCH__) */
	
	#if defined(__MMI_GB_HINDI__)
	{	
		INPUT_MODE_SMART_HINDI,
		MapKey,
		GBL_Hindi,
		MMI_IME_REQUIRED_ALL_KEYS,
		MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
		NULL,
		NULL,
		mmi_ime_goubi_word_get_candidates_alphabetic
	},
	#endif /* #if defined(__MMI_GB_HINDI__) */
		
		
	#if defined(__MMI_GB_INDONESIAN__)
	{	
		INPUT_MODE_SMART_UPPERCASE_INDONESIAN,
		MapKey,
		GBL_Indonesian,
		MMI_IME_REQUIRED_ALL_KEYS,
		MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
		NULL,
		NULL,
		mmi_ime_goubi_word_get_candidates_alphabetic
	},
	{	
		INPUT_MODE_SMART_LOWERCASE_INDONESIAN,
		MapKey,
		GBL_Indonesian,
		MMI_IME_REQUIRED_ALL_KEYS,
		MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
		NULL,
		NULL,
		mmi_ime_goubi_word_get_candidates_alphabetic
	},
	#endif /* #if defined(__MMI_GB_Indonesian__) */
    #if defined(__MMI_GB_SM_CHINESE__)
	 {
		INPUT_MODE_MULTITAP_PINYIN,
        MapKey,
		GBL_Chinese,
		MMI_IME_REQUIRED_ALL_KEYS,
		MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
		NULL,
        mmi_ime_guobi_get_associates,
        mmi_ime_guobi_get_candidates_pinyin
	},
    {
		INPUT_MODE_SM_STROKE,
        MapKey,
		GBL_Chinese,
		MMI_IME_REQUIRED_ALL_KEYS,
		MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
		mmi_ime_guobi_get_spellings_stroke,
        mmi_ime_guobi_get_associates,
        mmi_ime_guobi_get_candidates_stroke
	},
	{
		INPUT_MODE_SM_PINYIN,
        MapKey,
		GBL_Chinese,
		MMI_IME_REQUIRED_ALL_KEYS,
		MMI_IME_REQUIRED_ALL_NUM_KEYS & (~MMI_IME_REQUIRED_KEY_0),
		mmi_ime_guobi_get_spellings_pinyin,
        mmi_ime_guobi_get_associates,
        mmi_ime_guobi_get_candidates_pinyin_smart
	},
    #endif /*#if defined(__MMI_GB_SM_CHINESE__) */
	{
       	INPUT_MODE_MAX_NUM,
       	NULL,
       	GBL_NULL,
       	0,
       	0,
       	NULL,
       	NULL,
       	NULL
    }

};




mmi_ime_mode_info_struct_p	g_ime_info_mode_p;
GBAuxInfo					g_aux;	 
GBEngine 					g_ime_engine;
GBInputStruct				g_ime_init_mode;




static UI_character_type mmi_ime_guobi_word_symbol_to_code(U16 symbol);




static UI_character_type mmi_ime_guobi_word_symbol_to_code(U16 symbol)
{
#if defined(__MMI_GB_SM_CHINESE__) || defined(__MMI_GB_TR_CHINESE__)
	if (MMI_IMC_IS_SYMB_PY(symbol))
	{
            return g_key_map_alphabetic[(symbol - MMI_IMC_SYMB_PY_BASE) ];
	}
	else if(MMI_IMC_IS_SYMB_STROKE(symbol))
	{
		return g_key_map_alphabetic[(symbol - MMI_IMC_SYMB_STROKE_BASE)];
	}
	else
	#endif
	{
	   return g_key_map_alphabetic[(symbol - MMI_IMC_SYMB_KEY_BASE) ];
	}
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_get_spellings
 * DESCRIPTION
 * PARAMETERS
 *  param_ptr       [IN]        
 *  result_ptr      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_word_get_spellings(mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	result_ptr->result_cnt = 0;

    if (g_ime_engine.ime_mode_info_p)
    {
        if ( NULL != g_ime_engine.ime_mode_info_p->get_spell_funcptr )
        {
            g_ime_engine.ime_mode_info_p->get_spell_funcptr( param_ptr, result_ptr );
        }
    }
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_goubi_word_get_candidates_alphabetic
 * DESCRIPTION
 * PARAMETERS
 *  param_ptr       [IN]        
 *  result_ptr      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_goubi_word_get_candidates_alphabetic(mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr)
{

	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

	S32 cnt = 0, i = 0, j = 0,k=0 ;
	S32  max_no_of_cand = 32;
	S32 max_no_of_assc = 3;
	U16 TempString[32] = {0};
	UI_string_type str;
	GBINT result_cnt = 0;
	GBRETURN ret;
    U16 first_index;
	mmi_ime_mode_details_struct * mode_details_ptr;
	/*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    first_index = param_ptr->first_index;
	memset((result_ptr->result_buffer), 0, sizeof(result_ptr->result_buffer));
	result_ptr->result_cnt = 0;
	mode_details_ptr = (mmi_ime_mode_details_struct *)mmi_imm_get_input_mode_array_item();
	MMI_ASSERT(mode_details_ptr != NULL);
	
		for(i = 0; i< param_ptr->elembuf_cnt; i++)
		{
					TempString[i] = mmi_ime_guobi_word_symbol_to_code(param_ptr->elembuf_ptr[i]);
		}
		ret = GBMMIReset(g_ime_engine.ime_init_mode_p);
		ret = GBMMISetDefaultCandListType(g_ime_engine.ime_init_mode_p, GBCL_ROW_KEYBD);
		ret = GBMMISetCandRowCount(g_ime_engine.ime_init_mode_p, GBCS_INPUT, max_no_of_cand);
 		ret = GBMMISetCandRowCount(g_ime_engine.ime_init_mode_p, GBCS_ASSOCIATE, max_no_of_assc);
	    GBMMISetSwitchOption(g_ime_engine.ime_init_mode_p, GBKEY_AMB_SPECIAL_B, GBL_English, GBL_Sub_Not_Spec, GBIM_Predictive);
		if ((mode_details_ptr->flags & MMI_IME_LOWERCASE))
	   {
			ret  = GBSetEngineOption(g_ime_engine.ime_init_mode_p, GB_ENGINE_FUNCTION, GBFO_Alp_Get_Only_Cap, 0);
			ret = GBSetEngineOption(g_ime_engine.ime_init_mode_p, GB_ENGINE_FUNCTION, GBFO_Alp_Get_Only_Lower, 1); 
    		ret = GBSetShiftCap(g_ime_engine.ime_init_mode_p, 0, 0);
		
	   }
    
       else if (mode_details_ptr->flags & MMI_IME_UPPERCASE)
	   {
    		ret = GBSetShiftCap(g_ime_engine.ime_init_mode_p, 1, 1);
			ret = GBSetEngineOption(g_ime_engine.ime_init_mode_p, GB_ENGINE_FUNCTION, GBFO_Alp_Get_Only_Cap, 1); 
			ret = GBSetEngineOption(g_ime_engine.ime_init_mode_p, GB_ENGINE_FUNCTION, GBFO_Alp_Get_Only_Lower, 0); 
	   }
		//GBSetEngineOption(g_ime_engine.ime_init_mode_p,GB_ENGINE_FUNCTION, GBFO_Alp_UAW, 0);//close alphabic new word database
        //GBSetEngineOption(g_ime_engine.ime_init_mode_p,GB_ENGINE_FUNCTION, GBFO_Alp_No_Response_When_Key_No_Cand, 1); 
		 for(i = 0; i< param_ptr->elembuf_cnt; i++)
         {
	 		ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,TempString[i], 0);
         }
	
	/*
    else if(first_index > g_ime_engine.ime_init_mode_p->outputInfo.nFirstCandIndex)
	{
		//next page candidate
	     ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,GBKEY_NEXT_PAGE_CANDIATE, 0);
	     if(ret == GBOK)
	{
		   int t;
		   t =0;
	}
	}
	else if(first_index <= g_ime_engine.ime_init_mode_p->outputInfo.nFirstCandIndex)
	{
		//previous page candidate
          ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,GBKEY_PREV_PAGE_CANDIATE, 0);
	     if(ret == GBOK)
	     {
		   int t;
		   t =0;
	     }
	}
    */
    if(!(UCS2Strcmp((S8*) g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[0], (S8*) L"Spell?")))
	{
		//no more candidates
		return;
	}
	result_cnt = g_ime_engine.ime_init_mode_p->outputInfo.nCandNum;
	
	if (param_ptr->first_index >= result_cnt)
    {
        return;
    }
	str = result_ptr->result_buffer;
	j = first_index;
	k=0;
	while(j < (param_ptr->desired_cnt+first_index) && j < result_cnt)
	{
		S32 str_len;
		str_len = UCS2Strlen((const S8 *)g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[j]);
		memcpy(str, g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[j],str_len* sizeof(UI_character_type));
		str += str_len;
		*str= 0;
		str++;
		j++;
		k++;
	}
	result_ptr->result_cnt = k;
	
	 if (param_ptr->desired_cnt < result_cnt - first_index)
    {
        result_ptr->next_page = MMI_TRUE;
    }
    else
    {
        result_ptr->next_page = MMI_FALSE;
    }
	
}
#if defined(__MMI_GB_SM_CHINESE__) || defined(__MMI_GB_TR_CHINESE__)
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_word_symbol_to_code_stroke
 * DESCRIPTION
 * PARAMETERS
 *  param_ptr       [IN]        
 *  result_ptr      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
UI_character_type mmi_ime_guobi_word_symbol_to_code_stroke(U16 symbol)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	    switch(symbol)
		{
		
			case MMI_IMC_SYMB_STROKE_OVER:
		             return '1' ;
					
		        case MMI_IMC_SYMB_STROKE_DOWN:
		             return '2';
					
		        case MMI_IMC_SYMB_STROKE_LEFT:
		             return '3';
					
		        case MMI_IMC_SYMB_STROKE_DOT:
		             return '4';
					
		        case MMI_IMC_SYMB_STROKE_OVER_DOWN:     
			
				     return '5';

			    case MMI_IMC_SYMB_STROKE_WILDCARD:
				     return '6';
				
			default:
				return 0;
		}
		
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_get_spellings_stroke
 * DESCRIPTION
 * PARAMETERS
 *  param_ptr       [IN]        
 *  result_ptr      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_guobi_get_spellings_stroke(mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr)
{

	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    U8  i;
    U8  symb;
    UI_string_type temp_str;
	 /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	switch(*param_ptr->elembuf_ptr)
		{
		case MMI_IMC_SYMB_KEY_1:
	            *result_ptr->result_buffer  =  0x4E00  ;
		          result_ptr->result_cnt  =  1;
	            break;
	    case MMI_IMC_SYMB_KEY_2:
	            *result_ptr->result_buffer  =  0x4E28 ;	
			    result_ptr->result_cnt  =  1;
	            break;
	    case MMI_IMC_SYMB_KEY_3:
	            *result_ptr->result_buffer  =  0x4E3F ;
		        result_ptr->result_cnt  =  1;
				break;
	    case MMI_IMC_SYMB_KEY_4:
	            *result_ptr->result_buffer  =  0x4E36;
		        result_ptr->result_cnt  =  1;
	            break;
	     case MMI_IMC_SYMB_KEY_5:     
			     *result_ptr->result_buffer  =  0xF005;
		         result_ptr->result_cnt  =  1;
			    break;

		case MMI_IMC_SYMB_KEY_6:
		       *result_ptr->result_buffer  =  0x003F;
		       result_ptr->result_cnt  =  1;
	            break;

		default:
			*result_ptr->result_buffer  =  0;
			result_ptr->result_cnt  =  0;
			break;
	}
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_get_associates
 * DESCRIPTION
 * PARAMETERS
 *  param_ptr       [IN]        
 *  result_ptr      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_guobi_get_associates(mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr)
{

	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i = 0, j = 0, CanNum = 0;
	U16 TempString[32] = {0};
	UI_string_type* pCandBuf = NULL;
	UI_string_type str;
	U16 first_index;
	GBRETURN ret;
	 /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
   first_index = param_ptr->first_index;

   if(first_index == 0)
   {
		  GBMMIReset(g_ime_engine.ime_init_mode_p);
		  ret = GBGetAssocWord(g_ime_engine.ime_init_mode_p, param_ptr->pre_string, GBTrue );
		  if(ret == GBOK)
		 {
			int t;
			t =0;
		  }
		  if(g_ime_engine.ime_init_mode_p->outputInfo.nCandNum > 0)
		  {
				CanNum = g_ime_engine.ime_init_mode_p->outputInfo.nCandNum;
				result_ptr->result_cnt = CanNum;
				result_ptr->next_page = mmi_ime_guobi_have_next_cand_page(&g_ime_engine);
				str = result_ptr->result_buffer;
				while(j < param_ptr->desired_cnt && j < result_ptr->result_cnt)
				{
							memcpy(str, g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[j], sizeof(UI_character_type));
							str += 1;
							*str= 0;
							str++;
							j++;
				}
		  }
   }
   else if(first_index > g_ime_engine.ime_init_mode_p->outputInfo.nFirstCandIndex)
   {
          mmi_ime_guobi_GetNextPageAssociates(&g_ime_engine);
		   if(g_ime_engine.ime_init_mode_p->outputInfo.nCandNum > 0)
		  {
				CanNum = g_ime_engine.ime_init_mode_p->outputInfo.nCandNum;
				result_ptr->result_cnt = CanNum;
				result_ptr->next_page = mmi_ime_guobi_have_next_cand_page(&g_ime_engine);
				str = result_ptr->result_buffer;
				while(j < param_ptr->desired_cnt && j < result_ptr->result_cnt)
				{
							memcpy(str, g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[j], sizeof(UI_character_type));
							str += 1;
							*str= 0;
							str++;
							j++;
				}
		  }

   }
   else
   {
         mmi_ime_guobi_GetPrevPageAssociates(&g_ime_engine);
          if(g_ime_engine.ime_init_mode_p->outputInfo.nCandNum > 0)
		  {
				CanNum = g_ime_engine.ime_init_mode_p->outputInfo.nCandNum;
				result_ptr->result_cnt = CanNum;
				result_ptr->next_page = mmi_ime_guobi_have_next_cand_page(&g_ime_engine);
				str = result_ptr->result_buffer;
				while(j < param_ptr->desired_cnt && j < result_ptr->result_cnt)
				{
							memcpy(str, g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[j], sizeof(UI_character_type));
							str += 1;
							*str= 0;
							str++;
							j++;
				}
		  }
   }
   

}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_get_candidates_stroke
 * DESCRIPTION
 * PARAMETERS
 *  param_ptr       [IN]        
 *  result_ptr      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_guobi_get_candidates_stroke(mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr)
{

	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

	 /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	S32 cnt = 0, i = 0, j = 0 ;
	S32  max_no_of_cand = 1;
	S32 max_no_of_assc = 1;
	U16 first_index;
	U16 TempString[32] = {0};
	UI_string_type str;
	GBINT result_cnt = 0;
	GBRETURN ret;
	U16 stroke =0 ;

	/*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    first_index = param_ptr->first_index;
	result_ptr->result_cnt = 0;
	if(param_ptr->elembuf_cnt >32)
	{
		return;
	}
	
	
	if(first_index == 0)// first cand page
	{
		 
		     for(i = 0; i< param_ptr->elembuf_cnt; i++)
			{
				TempString[i] = mmi_ime_guobi_word_symbol_to_code_stroke(param_ptr->elembuf_ptr[i]);
				if(TempString[i] == 0)
				{
					return;
				}
		    }
		    ret = GBMMIReset(g_ime_engine.ime_init_mode_p);
		ret = GBMMISetDefaultCandListType(g_ime_engine.ime_init_mode_p, GBCL_ROW_KEYBD);
	 if(ret == GBOK)
	{
		int t;
		t =0;
	}
	ret = GBMMISetCandRowCount(g_ime_engine.ime_init_mode_p, GBCS_INPUT, max_no_of_cand);
	 if(ret == GBOK)
	{
		int t;
		t =0;
	}
 	ret = GBMMISetCandRowCount(g_ime_engine.ime_init_mode_p, GBCS_ASSOCIATE, max_no_of_assc);
	 if(ret == GBOK)
	{
		int t;
		t =0;
	}
	ret = GBMMISetSwitchOption(g_ime_engine.ime_init_mode_p, GBKEY_AMB_SPECIAL_B, GBL_Chinese, GBL_Sub_Not_Spec, GBIM_Stroke);
	//ret = GBMMISetSwitchOption(g_ime_engine.ime_init_mode_p, GBKEY_AMB_SPECIAL_B, GBL_English, GBL_Sub_Not_Spec, GBIM_Predictive);
	if(ret == GBOK)
	{
		int t;
		t =0;
	}

			//memset(&(g_ime_engine.ime_init_mode_p->inputString), 0, GB_INPUT_BUFFER_LEN*sizeof(GBWCHAR));
			g_ime_engine.ime_init_mode_p->inputStringLen = 0;

			
		 for(i = 0; i< param_ptr->elembuf_cnt; i++)
		{
	 			ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,TempString[i], 0);
			 if(ret == GBOK)
			{
			   int t;
			   t =0;
			}
		 }  
	}
	else if(first_index > g_ime_engine.ime_init_mode_p->outputInfo.nFirstCandIndex)
	{
		//next page candidate
	     ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,GBKEY_NEXT_PAGE_CANDIATE, 0);
	     if(ret == GBOK)
	     {
		   int t;
		   t =0;
	     }
	}
	else if(first_index <= g_ime_engine.ime_init_mode_p->outputInfo.nFirstCandIndex)
	{
		//previous page candidate
          ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,GBKEY_PREV_PAGE_CANDIATE, 0);
	     if(ret == GBOK)
	     {
		   int t;
		   t =0;
	     }
	}
	 
     result_ptr->next_page = mmi_ime_guobi_have_next_cand_page(&g_ime_engine);
	 //result_ptr->next_page = MMI_TRUE;
	 result_cnt = g_ime_engine.ime_init_mode_p->outputInfo.nCandNum;
	 result_ptr->result_cnt = result_cnt;
	 str = result_ptr->result_buffer;
		
	while(j < param_ptr->desired_cnt && j < result_ptr->result_cnt)
	{
		S32 str_len;
		str_len = UCS2Strlen((const S8 *)g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[j]);
		memcpy(str, g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[j],str_len * sizeof(UI_character_type));
		str += str_len;
		*str= 0;
		str++;
		j++;
	}
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_get_spellings_pinyin
 * DESCRIPTION
 * PARAMETERS
 *  param_ptr       [IN]        
 *  result_ptr      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_guobi_get_spellings_pinyin(mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr)
{

	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 str_len = 0;
	U16 TempString[32] = {0};
	S32 i, j=0, sylNum = 0;
	GBRETURN ret;
	UI_string_type* pSylBuf = NULL;
	UI_string_type str;
	U16 first_index;
	 /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	result_ptr->result_cnt = 0;
	first_index = param_ptr->first_index;
    if(first_index == 0)
    {
		/*if(mmi_imc_get_state() == MMI_IMC_STATE_SPELL_INPUTING && !mmi_ime_guobi_is_ChnUAWStatus(&g_ime_engine))
	{
				str_len = UCS2Strlen((const S8 *)g_ime_engine.ime_init_mode_p->inputString);
			if(str_len == param_ptr->elembuf_cnt-1)
			{
                   ret = mmi_ime_guobi_send_key_event(&g_ime_engine, mmi_ime_word_symbol_to_code(param_ptr->elembuf_ptr[param_ptr->elembuf_cnt-1]), 0);
			}
			else if(str_len == param_ptr->elembuf_cnt+1)
			{
				   ret = mmi_ime_guobi_send_key_event(&g_ime_engine, GBKEY_BACK, 0);
			}
	}
	else*/
	{
		for(i = 0; i< param_ptr->elembuf_cnt; i++)
	    {
				TempString[i] = mmi_ime_guobi_word_symbol_to_GBcode(param_ptr->elembuf_ptr[i]);
		}
		ret = GBMMIReset(g_ime_engine.ime_init_mode_p);
		if(param_ptr->elembuf_cnt <= 1)
		{
				ret = GBSetInputMode(g_ime_engine.ime_init_mode_p, GBIM_Pinyin);
			mmi_ime_guobi_setpageoption(&g_ime_engine, 0);
			ret = GBMMISetCandRowCount(g_ime_engine.ime_init_mode_p, GBCS_INPUT, 1);
			ret = GBMMISetCandRowCount(g_ime_engine.ime_init_mode_p, GBCS_ASSOCIATE, 1);
			//GBSetKeyboardTypeFlag(MMI_FALSE);
		}
		i = 0;
		while(TempString[i])
		{
				mmi_ime_guobi_send_key_event(&g_ime_engine, TempString[i++], 0);
		}
	     }
	}
	else if(first_index > g_ime_engine.ime_init_mode_p->pAuxInfo->nSyllableIndex)
	{
           mmi_ime_guobi_send_key_event(&g_ime_engine, GBKEY_NEXT_PAGE_SYLLABLE, 0);
	}
	else
	{
           mmi_ime_guobi_send_key_event(&g_ime_engine, GBKEY_PREV_PAGE_SYLLABLE, 0);
	}
	if(mmi_ime_guobi_have_syllable(&g_ime_engine))
	{
       sylNum = g_ime_engine.ime_init_mode_p->pAuxInfo->nSyllableNum;
	   str = result_ptr->result_buffer;
	   for(i=0,j=0; i<sylNum; i++)
	  {
		memcpy(str, g_ime_engine.ime_init_mode_p->pAuxInfo->pSyllables[j],param_ptr->elembuf_cnt * sizeof(UI_character_type));
		str += param_ptr->elembuf_cnt;
		*str= 0;
		str++;
		j++;
		}
		result_ptr->result_cnt = sylNum;
		 result_ptr->next_page = mmi_ime_guobi_HaveNextSyllPage(&g_ime_engine);
	}

	
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_get_candidates_pinyin
 * DESCRIPTION
 * PARAMETERS
 *  param_ptr       [IN]        
 *  result_ptr      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_guobi_get_candidates_pinyin(mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr)
{

	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

	 /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	S32 cnt = 0, i = 0, j = 0 ;
	S32  max_no_of_cand = 1;
	S32 max_no_of_assc = 1;
	U16 TempString[32] = {0};
	UI_string_type str;
	GBINT result_cnt = 0;
	GBRETURN ret;
	U16 first_index;

	/*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	result_ptr->result_cnt = 0;
	first_index = param_ptr->first_index;
	if(param_ptr->elembuf_cnt >32)
	{
		return;
	}
	if(first_index == 0)
	{
		    for(i = 0; i< param_ptr->elembuf_cnt; i++)
			{
				TempString[i] = mmi_ime_word_symbol_to_code(param_ptr->elembuf_ptr[i]);
		    }
		    ret = GBMMIReset(g_ime_engine.ime_init_mode_p);
			ret = GBMMISetDefaultCandListType(g_ime_engine.ime_init_mode_p, GBCL_ROW_KEYBD);
			 if(ret == GBOK)
			{
				int t;
				t =0;
			}
			ret = GBMMISetCandRowCount(g_ime_engine.ime_init_mode_p, GBCS_INPUT, max_no_of_cand);
			 if(ret == GBOK)
			{
				int t;
				t =0;
			}
 			ret = GBMMISetCandRowCount(g_ime_engine.ime_init_mode_p, GBCS_ASSOCIATE, max_no_of_assc);
			 if(ret == GBOK)
			{
				int t;
				t =0;
			}
			ret = GBMMISetSwitchOption(g_ime_engine.ime_init_mode_p, GBKEY_AMB_SPECIAL_B, GBL_Chinese, GBL_Sub_Not_Spec, GBIM_Pinyin);
			if(ret == GBOK)
			{
				int t;
				t =0;
			}
			

			 /*if (param_ptr->first_index >= result_cnt)
			{
				return;
			}*/
			 for(i = 0; i< param_ptr->elembuf_cnt; i++)
			{
	 			ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,TempString[i], 0);
				
			}
			 
	}
	else if(first_index > g_ime_engine.ime_init_mode_p->outputInfo.nFirstCandIndex)
	{
		ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,GBKEY_NEXT_PAGE_CANDIATE, 0);
	     if(ret == GBOK)
	     {
		   int t;
		   t =0;
	     }
	}
	else
	{
		ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,GBKEY_PREV_PAGE_CANDIATE, 0);
	     if(ret == GBOK)
	     {
		   int t;
		   t =0;
	     }
	}
	
	result_ptr->result_cnt = g_ime_engine.ime_init_mode_p->outputInfo.nCandNum;
	str = result_ptr->result_buffer;
	
	while(j < param_ptr->desired_cnt && j < result_ptr->result_cnt)
	{
		S32 str_len;
		str_len = UCS2Strlen((const S8 *)g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[j]);
		memcpy(str, g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[j],str_len * sizeof(UI_character_type));
		str += str_len;
		*str= 0;
		str++;
		j++;
	}
	 
     result_ptr->next_page = mmi_ime_guobi_have_next_cand_page(&g_ime_engine);;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_get_candidates_pinyin_smart
 * DESCRIPTION
 * PARAMETERS
 *  param_ptr       [IN]        
 *  result_ptr      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_guobi_get_candidates_pinyin_smart(mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr)
{
     /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

	 /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	S32 cnt = 0, i = 0, j = 0 ;
	S32  max_no_of_cand = 1;
	S32 max_no_of_assc = 1;
	U16 TempString[32] = {0};
	UI_string_type str;
	GBINT result_cnt = 0;
	GBRETURN ret;
	U16 first_index;

	/*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(param_ptr->elembuf_cnt >32)
	{
		return;
	}
	result_ptr->result_cnt = 0;
	first_index = param_ptr->first_index;
	if(first_index == 0)
	{
		/*if(mmi_imc_get_state() == MMI_IMC_STATE_SPELL_INPUTING)
		{
			
			
				GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p, GBEVENT_SELECT_SYLLABLE_PAGED,1);
			
		}
		else*/
		{
		     for(i = 0; i< param_ptr->elembuf_cnt; i++)
			{
				TempString[i] = mmi_ime_word_symbol_to_code(param_ptr->elembuf_ptr[i]);
			}
		     if(TempString[0] >= 'a' && TempString[0] <= 'z')
			{
				
					//GBEngine_SwitchInputMode(GBEngine_GetEnineInstance(),GBIM_Qwerty_Pinyin, GBL_Chinese, GBL_Sub_Not_Spec);
				ret = GBSetInputMode(g_ime_engine.ime_init_mode_p, GBIM_Qwerty_Pinyin );
			}
			 else
			 {
                  ret = GBSetInputMode(g_ime_engine.ime_init_mode_p, GBIM_Pinyin );
			 }
		    ret = GBMMIReset(g_ime_engine.ime_init_mode_p);
			ret = GBMMISetDefaultCandListType(g_ime_engine.ime_init_mode_p, GBCL_ROW_KEYBD);
			 if(ret == GBOK)
			{
				int t;
				t =0;
			}
			ret = GBMMISetCandRowCount(g_ime_engine.ime_init_mode_p, GBCS_INPUT, max_no_of_cand);
			 if(ret == GBOK)
			{
				int t;
				t =0;
			}
 			ret = GBMMISetCandRowCount(g_ime_engine.ime_init_mode_p, GBCS_ASSOCIATE, max_no_of_assc);
			 if(ret == GBOK)
			{
				int t;
				t =0;
			}
			ret = GBMMISetSwitchOption(g_ime_engine.ime_init_mode_p, GBKEY_AMB_SPECIAL_B, GBL_Chinese, GBL_Sub_Not_Spec, GBIM_Pinyin);
			//ret = GBMMISetSwitchOption(g_ime_engine.ime_init_mode_p, GBKEY_AMB_SPECIAL_B, GBL_English, GBL_Sub_Not_Spec, GBIM_Predictive);
			if(ret == GBOK)
			{
				int t;
				t =0;
			}
			/*ret = GBGetCandidatesCount(g_ime_engine.ime_init_mode_p,&result_cnt);
			 if(ret == GBOK)
			{
				int t;
				t =0;
			}*/
			memset(&(g_ime_engine.ime_init_mode_p->inputString), 0, GB_INPUT_BUFFER_LEN*sizeof(GBWCHAR));
			g_ime_engine.ime_init_mode_p->inputStringLen = 0;

			 /*if (param_ptr->first_index >= result_cnt)
			{
				return;
			}*/
			 for(i = 0; i< param_ptr->elembuf_cnt; i++)
			{
	 			ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,TempString[i], 0);
				 if(ret == GBOK)
				{
				   int t;
				   t =0;
				}
			}
		}
			 
	}
	else if(first_index > g_ime_engine.ime_init_mode_p->outputInfo.nFirstCandIndex)
	{
		ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,GBKEY_NEXT_PAGE_CANDIATE, 0);
	     if(ret == GBOK)
	     {
		   int t;
		   t =0;
	     }
	}
	else
	{
		ret = GBMMIHandleKeyEvent(g_ime_engine.ime_init_mode_p  ,GBKEY_PREV_PAGE_CANDIATE, 0);
	     if(ret == GBOK)
	     {
		   int t;
		   t =0;
	     }
	}
	
	result_ptr->result_cnt = g_ime_engine.ime_init_mode_p->outputInfo.nCandNum;
	str = result_ptr->result_buffer;
	
	while(j < param_ptr->desired_cnt && j < result_ptr->result_cnt)
	{
		S32 str_len;
		str_len = UCS2Strlen((const S8 *)g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[j]);
		memcpy(str, g_ime_engine.ime_init_mode_p->outputInfo.pCandidates[j],str_len* sizeof(UI_character_type));
		str += str_len;
		*str= 0;
		str++;
		j++;
	}
	 
     result_ptr->next_page = mmi_ime_guobi_have_next_cand_page(&g_ime_engine);;
}
#endif
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_get_associates
 * DESCRIPTION
 * PARAMETERS
 *  param_ptr       [IN]        
 *  result_ptr      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_word_get_associates(mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	result_ptr->result_cnt = 0;

    if (g_ime_engine.ime_mode_info_p)
    {
        if ( NULL != g_ime_engine.ime_mode_info_p->get_assoc_funcptr )
        {
            g_ime_engine.ime_mode_info_p->get_assoc_funcptr( param_ptr, result_ptr );
        }
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_get_associates
 * DESCRIPTION
 * PARAMETERS
 *  param_ptr       [IN]        
 *  result_ptr      [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_word_get_candidates(mmi_ime_query_param_struct_p param_ptr, mmi_ime_query_result_struct_p result_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    kal_uint32 time1, time2;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	kal_get_time(&time1);
	result_ptr->result_cnt = 0;

    if (g_ime_engine.ime_mode_info_p)
    {
        if ( NULL != g_ime_engine.ime_mode_info_p->get_cand_funcptr )
        {
            g_ime_engine.ime_mode_info_p->get_cand_funcptr( param_ptr, result_ptr );
        }
    }
	kal_get_time(&time2);
	MMI_PRINT(MOD_MMI_FW,TRACE_GROUP_8,"[Guobi]candidate recognition time %d %d %d ms",time2,time1, kal_ticks_to_milli_secs(time2 - time1));
	//MMI_TRACE(MMI_FW_TRC_G5_IME, TRC_MMI_FW_HW_CANDIDATES_RECONGNITION_TIME, kal_ticks_to_milli_secs(time2 - time1));
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_is_valid_key
 * DESCRIPTION
 * PARAMETERS
 *  input_mode      [IN]        
 *  key_code        [IN]        
 * RETURNS
 *****************************************************************************/
MMI_BOOL mmi_ime_word_is_valid_key(S32 input_mode, U16 key_code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_FALSE;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_word_symbol_to_GBcode
 * DESCRIPTION
 * PARAMETERS
 *  symbol      [IN]        
 * RETURNS
 *****************************************************************************/
UI_character_type mmi_ime_guobi_word_symbol_to_GBcode(U16 symb_key)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (symb_key >= MMI_IMC_SYMB_KEY_0 && symb_key <= MMI_IMC_SYMB_KEY_POUND)
    {
       return g_key_map_alphabetic[(symb_key - MMI_IMC_SYMB_KEY_0) ];
    }
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_symbol_to_code
 * DESCRIPTION
 * PARAMETERS
 *  symbol      [IN]        
 * RETURNS
 *****************************************************************************/
UI_character_type mmi_ime_word_symbol_to_code(U16 symbol)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if (MMI_IMC_IS_SYMB_TONE(symbol))
    {
        switch (symbol)
        {
            case MMI_IMC_SYMB_TONE_1:
                return 0x02C9;
                
            case MMI_IMC_SYMB_TONE_2:
                return 0x02CA;
            case MMI_IMC_SYMB_TONE_3:
                return 0x02C7;
            case MMI_IMC_SYMB_TONE_4:
                return 0x02CB;
            case MMI_IMC_SYMB_TONE_5:
                return 0x02D9;
            default:
                ASSERT(0);
        }
    }
	#if defined(__MMI_GB_SM_CHINESE__)
	else if (MMI_IMC_IS_SYMB_STROKE(symbol))
	{
		switch(symbol)
		{
			case MMI_IMC_SYMB_STROKE_OVER:
				//symbol key 1
			    return 0x4E00;  
			
			case MMI_IMC_SYMB_STROKE_DOWN:
		         return 0x4E28 ;
					
		    case MMI_IMC_SYMB_STROKE_LEFT:
		          return 0x4E3F;
					
		    case MMI_IMC_SYMB_STROKE_DOT:
		            return 0x4E36;
					
		     case MMI_IMC_SYMB_STROKE_OVER_DOWN:
		            return 0xF005;
			case MMI_IMC_SYMB_STROKE_WILDCARD:
				return 0x003F;
				
			default:
				return 0x0000;
		}
	}
	else if (MMI_IMC_IS_SYMB_PY(symbol))
	{
		return symbol - MMI_IMC_SYMB_PY_BASE + 'a';
	}
    #endif
	return symbol;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_code_to_symbol
 * DESCRIPTION
 * PARAMETERS
 *  code        [IN]        
 * RETURNS
 *****************************************************************************/
U16 mmi_ime_word_code_to_symbol(UI_character_type code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    #ifdef __MMI_GB_SM_CHINESE__
	 if (code >= 'a' && code <= 'z')
     {
            return code - 'a' + MMI_IMC_SYMB_PY_A; 
     }
	 else if(code >= 'A' && code <= 'Z')
	 {
	        return code - 'A' + MMI_IMC_SYMB_PY_A; 
	 }
  
	 switch(code)
	 {
              case 0x4E00:
				  return MMI_IMC_SYMB_STROKE_OVER;
			  case 0x4E28:
				  return MMI_IMC_SYMB_STROKE_DOWN;
			  case 0x4E3F:	
				  return MMI_IMC_SYMB_STROKE_LEFT;
			  case 0x4E36:		
				  return MMI_IMC_SYMB_STROKE_DOT;
			  case 0xF005:     
				  return MMI_IMC_SYMB_STROKE_OVER_DOWN;
			  case 0x003F:
					return MMI_IMC_SYMB_STROKE_WILDCARD;
					
		
	 }
    #endif
	 return code;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_get_corresponding_stroke
 * DESCRIPTION
 * PARAMETERS
 *  code        [IN]        
 * RETURNS
 *****************************************************************************/
U16 mmi_ime_word_get_corresponding_stroke(UI_character_type code)
{
  #if defined(__MMI_GB_SM_CHINESE__)
    
		switch(code)
		{
			case 0x4E00:
				//symbol key 1
			      return  MMI_IMC_SYMB_STROKE_OVER;
			
			case 0x4E28:
		         return MMI_IMC_SYMB_STROKE_DOWN;
					
		    case 0x4E3F:
		           return MMI_IMC_SYMB_STROKE_LEFT;
					
		    case 0x4E36:
		           return MMI_IMC_SYMB_STROKE_DOT;
					
		     case 0xF005:
		           return MMI_IMC_SYMB_STROKE_OVER_DOWN;
			case 0x003F:
				   return MMI_IMC_SYMB_STROKE_WILDCARD;
				
			default:
				return 0;
		}
  #endif
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_is_symbol_key
 * DESCRIPTION
 * PARAMETERS
 *  input_mode      [IN]        
 *  key_code        [IN]        
 * RETURNS
 *****************************************************************************/
MMI_BOOL mmi_ime_word_is_symbol_key(S32 input_mode, mmi_imc_key_custom_value key_code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    
    switch (key_code)
    {
        case MMI_IMC_KEY_UP:
        case MMI_IMC_KEY_DOWN:
        case MMI_IMC_KEY_LEFT:
        case MMI_IMC_KEY_RIGHT:
        case MMI_IMC_KEY_CONFIRM:
        case MMI_IMC_KEY_CANCEL:
        case MMI_IMC_KEY_CLEAR:
            return MMI_FALSE;
        case MMI_IMC_KEY_SYM_0:
        case MMI_IMC_KEY_SYM_1:
        case MMI_IMC_KEY_SYM_2:
        case MMI_IMC_KEY_SYM_3:
        case MMI_IMC_KEY_SYM_4:
        case MMI_IMC_KEY_SYM_5:
        case MMI_IMC_KEY_SYM_6:
        case MMI_IMC_KEY_SYM_7:
        case MMI_IMC_KEY_SYM_8:
        case MMI_IMC_KEY_SYM_9:
            if (g_ime_engine.ime_mode_info_p->symbol_keys & (MMI_IME_REQUIRED_KEY_0 << (key_code - MMI_IMC_KEY_SYM_0)))
            {
                return MMI_TRUE;
            }
            else
            {
                return MMI_FALSE;
            }
        case MMI_IMC_KEY_SYM_STAR:
            if (g_ime_engine.ime_mode_info_p->symbol_keys & MMI_IME_REQUIRED_KEY_STAR)
            {
                return MMI_TRUE;
            }
            else
            {
                return MMI_FALSE;
            }
        case MMI_IMC_KEY_SYM_POUND:
            if (g_ime_engine.ime_mode_info_p->symbol_keys & MMI_IME_REQUIRED_KEY_POUND)
            {
                return MMI_TRUE;
            }
            else
            {
                return MMI_FALSE;
            }
        default:
            return MMI_FALSE;
    }
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_is_redicals
 * DESCRIPTION
 * PARAMETERS
 *  key_code        [IN]        
 * RETURNS
 *****************************************************************************/
MMI_BOOL mmi_ime_word_is_redicals(UI_character_type key_code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_is_wildcard
 * DESCRIPTION
 * PARAMETERS
 *  code        [IN]        
 * RETURNS
 *****************************************************************************/
MMI_BOOL mmi_ime_word_is_wildcard(UI_character_type code)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_add_separator_to_candidate_buffer
 * DESCRIPTION
 * PARAMETERS
 *  destination_buffer      [OUT]       
 *  source_buffer           [IN]        
 *  num                     [IN]        
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_word_add_separator_to_candidate_buffer(UI_character_type *destination_buffer, UI_character_type *source_buffer, S32 num)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    S32 i = 0, j = 0;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    while (i < num && source_buffer[i] != 0)
    {
        destination_buffer[j] = source_buffer[i];
        destination_buffer[j + 1] = 0; /* Default separator is '\0' */
        i++;
        j += 2;
    }

    destination_buffer[j] = 0;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_engine_get_char_info
 * DESCRIPTION
 * PARAMETERS
 *  code            [IN]        
 *  info_buffer     [IN]        
 *  buffer_size     [IN]        
 *  mode            [IN]        
 * RETURNS
 *****************************************************************************/
MMI_BOOL mmi_ime_word_engine_get_char_info(UI_character_type code, UI_string_type info_buffer, S32 buffer_size, S32 mode)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    return MMI_FALSE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_init
 * DESCRIPTION
 *  Initialize word engine object
 * PARAMETERS
 *  void
 * RETURNS
 *****************************************************************************/

//char temp[5704];
//GBLDB x[1] ;



MMI_BOOL mmi_ime_word_init(void)
{
  	
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	S32 ldb_count= 0;
	
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	
	g_ime_engine.ime_init_mode_p = &g_ime_init_mode;
	g_ime_engine.aux_p =  &g_aux;
	g_ime_engine.ldbs_num_map_p = g_ldbs_map ;
	g_ime_init_mode.pAuxInfo = &g_aux;
	g_ime_init_mode.engine_private_buffer_size = sizeof(g_ime_init_mode.engine_private_buffer);
	g_ime_engine.case_type = MMI_IME_WORD_CASE_LOWER;

	for( ldb_count = 0; g_ldbs_map[ldb_count].nLang != GBL_NULL; ldb_count++);
	
	g_ime_engine.g_nDataCount = ldb_count;


	if(GBInitialize((g_ime_engine.ime_init_mode_p), g_ime_engine.ldbs_num_map_p, g_ime_engine.g_nDataCount) == 0)
		return MMI_TRUE;
	else
		ASSERT(0);
	
}
 

/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_deinit
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_word_deinit(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

	/*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	g_ime_engine.ime_init_mode_p = NULL;
	g_ime_engine.aux_p = NULL;
	g_ime_engine.ldbs_num_map_p = NULL;
	g_ime_engine.key_map_p = NULL;
	g_ime_engine.ime_mode_info_p = NULL;
	g_ime_info_mode_p = NULL;
	g_ime_init_mode.pAuxInfo = NULL;
	memset(&g_ime_init_mode, 0, sizeof(GBInputStruct));
    memset(&g_ime_engine, 0, sizeof(GBEngine));
	memset(&g_aux, 0, sizeof(GBAuxInfo)); 
	
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_change_mode
 * DESCRIPTION
 * PARAMETERS
 *  mode_details_ptr        [IN]        
 * RETURNS
 *****************************************************************************/
MMI_BOOL mmi_ime_word_change_mode(const mmi_ime_mode_details_struct *mode_details_ptr)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

	S32  mode_count = 0, i = 0;
	GBSUBLANG sub_lang;
    GBRETURN ret;
	/*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

	//need to put break in for loop
	for(mode_count = 0; g_mode_infos[mode_count].input_mode != INPUT_MODE_MAX_NUM; mode_count++)
	{
		if(g_mode_infos[mode_count].input_mode == mode_details_ptr->mode)
		{
			g_ime_engine.ime_mode_info_p = &(g_mode_infos[mode_count]);
			break;
		}
	}
	for(i = 0; g_ldbs_map[i].nLang != GBL_NULL; i ++)
	{
		if(g_ime_engine.ime_mode_info_p->nLdbNum == g_ldbs_map[i].nLang)
		{
			sub_lang = g_ldbs_map[i].nSubLang;
			break;
		}
	}
   // GBMMIReset(&(g_ime_engine.ime_init_mode_p));
	ret = GBSetCurLanguage(&g_ime_init_mode, g_ime_engine.ime_mode_info_p->nLdbNum, sub_lang);
    if(ret == GBOK)
	{
		int t;
		t =0;
	}
	switch(g_ime_engine.ime_mode_info_p->input_mode)
	{
	    case INPUT_MODE_SMART_UPPERCASE_ABC:
		case INPUT_MODE_SMART_LOWERCASE_ABC:
			ret = GBSetInputMode(g_ime_engine.ime_init_mode_p, GBIM_Predictive);
			break;
		case INPUT_MODE_MULTITAP_PINYIN:
			 ret = GBSetInputMode(g_ime_engine.ime_init_mode_p, GBIM_Qwerty_Pinyin);
			 break;
		case INPUT_MODE_SM_PINYIN:
            ret = GBSetInputMode(g_ime_engine.ime_init_mode_p, GBIM_Pinyin);
			break;
		case INPUT_MODE_SM_STROKE:
            ret = GBSetInputMode(g_ime_engine.ime_init_mode_p, GBIM_Stroke);
			break;
		default:
			ret = GBSetInputMode(g_ime_engine.ime_init_mode_p, GBIM_Predictive);
			break;
             //nothing
	}
	if(ret == GBOK)
	{
		int t;
		t =0;
	}
	
   
	return MMI_TRUE;
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_word_leave_current_mode
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_word_leave_current_mode(void)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_have_next_cand_page
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL mmi_ime_guobi_have_next_cand_page(PGBEngine g_ime_engine)
{
     /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	if( GBHaveNextPage( g_ime_engine->ime_init_mode_p ) == GBOK )
		return MMI_TRUE;
	else
		return MMI_FALSE;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_GetNextPageAssociates
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_guobi_GetNextPageAssociates(PGBEngine g_ime_engine)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	S32 nStartIndex = 0;
	GBRETURN ret;

	if( mmi_ime_guobi_have_next_cand_page(g_ime_engine) != MMI_FALSE )
	{		
		nStartIndex = g_ime_engine->ime_init_mode_p->outputInfo.nCandNum + g_ime_engine->ime_init_mode_p->outputInfo.nFirstCandIndex;
		ret = GBNextPageCandidate(g_ime_engine->ime_init_mode_p, nStartIndex);
		if(ret == GBOK)
	   {
		   int t;
		   t =0;
	   }
		
	}
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_GetPrevPageAssociates
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
void mmi_ime_guobi_GetPrevPageAssociates(PGBEngine g_ime_engine)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	S32 ret = mmi_ime_guobi_have_prev_cand_page(g_ime_engine);
	S32 nStartIndex = 0;

	if( ret != 0 )
	{
		nStartIndex = g_ime_engine->ime_init_mode_p->outputInfo.nFirstCandIndex - 1;
		GBPrevPageCandidate(g_ime_engine->ime_init_mode_p, nStartIndex);
	}
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_have_prev_cand_page
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
S32 mmi_ime_guobi_have_prev_cand_page(PGBEngine g_ime_engine)
{
     /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
	if( g_ime_engine->ime_init_mode_p->pAuxInfo->nStatus == GBIMS_ALP_UAW_NO_MORE_CAND )
		return 1;
	else if( GBHavePrevPage( g_ime_engine->ime_init_mode_p ) == GBOK )
		return 2;
	else
		return 0;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_send_key_event
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
S32 mmi_ime_guobi_send_key_event(PGBEngine g_ime_engine, GBINT evt, GBU32 param)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
   GBRETURN ret;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	ret = GBMMIHandleKeyEvent(g_ime_engine->ime_init_mode_p, evt, param);
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_setpageoption
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
GBRETURN mmi_ime_guobi_setpageoption(PGBEngine g_ime_engine, GBBOOL isftepage)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
	GBGlobalPageOption pageOption;
	GBRETURN ret;
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    ret = GBGetPageOption(g_ime_engine->ime_init_mode_p, &pageOption);
	pageOption.fpGetWStringWidth = mmi_ime_guobi_GetStrWidth;

	pageOption.mChnCandPageOption.nMaxCandCountPerPage = GB_CN_CANDIDATE_COUNT;
/*	if(isftepage)
	{
		pageOption.mChnCandPageOption.nTotalWidthInPixel = (CAND_WIDTH -GB_SEL_BOX_FTE_ARROW_WIDTH * 2 - GB_RESIZABLE_COMPOSITION_BOX_SPACING*2 - 2);
		pageOption.mChnCandPageOption.nSplitWidthInPixel = GB_SEL_BOX_CAND_SPACING * 2 + 2;  
	}
	else*/
	{
		pageOption.mChnCandPageOption.nTotalWidthInPixel = CAND_AREA_WIDTH;
		pageOption.mChnCandPageOption.nSplitWidthInPixel = CAND_LINE_SPLIT_WIDTH;
	}
	pageOption.mSyllablePageOption.nMaxCandCountPerPage = GB_CN_CANDIDATE_COUNT;
	/*if(isftepage)
	{
		pageOption.mSyllablePageOption.nTotalWidthInPixel = (CAND_WIDTH - GB_SEL_BOX_FTE_ARROW_WIDTH * 2 - GB_RESIZABLE_COMPOSITION_BOX_SPACING*2 - 2);
		pageOption.mSyllablePageOption.nSplitWidthInPixel = GB_SEL_BOX_CAND_SPACING * 2 + 2;  
	}
	else*/
	{
		pageOption.mSyllablePageOption.nTotalWidthInPixel = CAND_AREA_WIDTH;
		pageOption.mSyllablePageOption.nSplitWidthInPixel = CAND_LINE_SPLIT_WIDTH;
	}

	pageOption.mPageOption.nMaxCandCountPerPage = GB_LATIN_CANDIDATE_COUNT;
	/*if(isftepage)
	{
		pageOption.mPageOption.nTotalWidthInPixel  = (CAND_WIDTH - GB_SEL_BOX_FTE_ARROW_WIDTH * 2 - GB_RESIZABLE_COMPOSITION_BOX_SPACING*2 - 2);
		pageOption.mPageOption.nSplitWidthInPixel = GB_SEL_BOX_CAND_SPACING * 2 + 2;  
	}
	else*/
	{
		pageOption.mPageOption.nTotalWidthInPixel = CAND_AREA_WIDTH;
		pageOption.mPageOption.nSplitWidthInPixel = CAND_LINE_SPLIT_WIDTH;
	}
	
	pageOption.mPageOption.nCharWidthInPixel = 0;

	ret = GBSetPageOption(g_ime_engine->ime_init_mode_p, &pageOption);
	return ret;
}
/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_GetStrWidth
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
S32 mmi_ime_guobi_GetStrWidth(void * pParam, GBLPCWCHAR str)
{
	gui_set_font(&MMI_medium_font);
	return gui_get_string_width((UI_string_type)str);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_is_IsChnUAWStatus
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL mmi_ime_guobi_is_ChnUAWStatus(PGBEngine g_ime_engine)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	
//	ASSERT(GBIsInited((g_ime_engine->ime_init_mode_p)) == GBOK && g_ime_engine->engine_opened);
	return (MMI_BOOL)(UCS2Strlen((const S8 *)g_ime_engine->ime_init_mode_p->pAuxInfo->pSyllables[0]) > 0 
		&&  (g_ime_engine->ime_init_mode_p->pAuxInfo->pSyllables[0][0] > '9' || g_ime_engine->ime_init_mode_p->pAuxInfo->pSyllables[0][0] < '0'));
		
    
}


/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_have_syllable
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
S32 mmi_ime_guobi_have_syllable(PGBEngine g_ime_engine)
{
	/*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
      
	   return GBIS_HaveSyllableSelection(g_ime_engine->ime_init_mode_p);
}

/*****************************************************************************
 * FUNCTION
 *  mmi_ime_guobi_have_syllable
 * DESCRIPTION
 * PARAMETERS
 *  void
 * RETURNS
 *  void
 *****************************************************************************/
MMI_BOOL mmi_ime_guobi_HaveNextSyllPage(PGBEngine g_ime_engine)
{
     /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*--------------------------------------------s--------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
    if(!mmi_ime_guobi_have_syllable(g_ime_engine))
		return 0;
	return (GBU8)(GBIS_HaveMoreSyllable(g_ime_engine->ime_init_mode_p));
}


#endif /* !defined(__MMI_ZI__) && !defined(__MMI_T9__) && !defined(__MMI_CSTAR__) */ /* #if !defined(__MMI_ZI__) && !defined(__MMI_T9__) && !defined(__MMI_CSTAR__) */


