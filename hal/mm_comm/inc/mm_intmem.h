/****************************************************************************
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
 *    mm_intmem.h
 *
 * Project:
 * --------
 *    MAUI
 *
 * Description:
 * ------------
 *
 * Author:
 * -------
 *    CT Fang (mtk02403)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by ClearCase. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Log$
 *
 * 11 29 2011 hung-wen.hsieh
 * [MAUI_03080162] MT6255 MDP Check-in
 * .
 *
 * 10 12 2011 chrono.wu
 * [MAUI_03038833] [MSBB] Remove include drv_features.h
 * .
 *
 * 09 16 2011 chelun.tsai
 * [MAUI_02992280] MT6256E2 MDP check in MAUI/11B
 * .
 *
 * 08 02 2011 chelun.tsai
 * [MAUI_02992280] MT6256E2 MDP check in MAUI/11B
 * .
 *
 * 07 27 2011 chelun.tsai
 * [MAUI_02992280] MT6256E2 MDP check in MAUI/11B
 * .
 *
 * 07 06 2011 peter.wang
 * [MAUI_02970410] Video_The video frame freeze after capturing.
 * .
 *
 * 05 27 2011 chelun.tsai
 * [MAUI_02884038] MDP
 * MT6526 E2 MDP driver.
 *
 * 05 16 2011 chrono.wu
 * [MAUI_02915837] MDP Code Shrinking Check In
 * .
 *
 * 04 20 2011 chrono.wu
 * [MAUI_02917426] Wrap mm_intmem<xxx> for mt6236: DRV_FEATURE__MM_INTMEM_IF
 * .
 *
 * 04 08 2011 chrono.wu
 * [MAUI_02905390] resolve layer rotate tearing
 * .
 *
 * 04 08 2011 chrono.wu
 * [MAUI_02742091] [Known issue] [video recorder]_Assert fail: mpeg4_encoder_component_v2.c 1570 0x0 0x0 0x0 - V2DEC
 * .
 *
 * 03 11 2011 hung-wen.hsieh
 * [MAUI_02738414] [FF,GS,GF] Zoom speed fail
 * add jpeg_dec segment to improve the velocity.
 *
 * 01 07 2011 hung-wen.hsieh
 * [MAUI_02854525] Fix 11A Build Error
 * Remove 6255 Label.
 *
 * 01 03 2011 hung-wen.hsieh
 * [MAUI_02854525] Fix 11A Build Error
 * .
 *
 * 12 03 2010 cm.huang
 * [MAUI_02842195] Fix MAUI pre-build error
 * .
 *
 * 12 02 2010 cm.huang
 * [MAUI_02840888] [HAL] MDP driver check-in MAUI
 * .
 *
 * 11 26 2010 hung-wen.hsieh
 * [MAUI_02840888] [HAL] MDP driver check-in MAUI
 * .
 *
 * 11 23 2010 cm.huang
 * [MAUI_02840888] [HAL] MDP driver check-in MAUI
 * .
 *
 * 11 17 2010 cm.huang
 * NULL
 * .
 *
 * 11 12 2010 cm.huang
 * NULL
 * .
 *
 * 11 09 2010 cm.huang
 * NULL
 * .
 *
 * 10 11 2010 hung-wen.hsieh
 * NULL
 * mt6268 internal memory header file.
 *
 * Jul 4 2010 mtk02685
 * [MAUI_02577019] [MT6256] MO call assert fail
 * MT6256 MO assertion
 *
 * Jun 15 2010 mtk02685
 * [MAUI_02562466] [MT6255] MT6256 naming check-in
 * MT6256 name check-in
 *
 * Jun 4 2010 mtk02685
 * [MAUI_02542177] [MT6255_DVT] The plan for merging to MAUI (For 55 MM HQA!)
 * MT6255 HQA check-in
 *
 * May 28 2010 mtk02403
 * [MAUI_02535206] [Vidio Telephony Check-In] MT6236 video renderer for Video Telephony
 * 
 *
 * May 12 2010 mtk02403
 * [MAUI_02406680] [CCT] MT6236 internal memory allocation fail
 * 
 *
 * Dec 23 2009 mtk02403
 * [MAUI_02020588] [V2 check in] check in video v2 with w09.49 temp load
 * 
 *
 * Oct 6 2009 mtk02403
 * [MAUI_01964115] [MT6236] MDP Driver Check In MAUI
 * 
 *
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by ClearCase. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef __MM_INTMEM_H__
#define __MM_INTMEM_H__

#include "kal_public_api.h"

/*****************************************************************************
 *  Dear mm_intmem owners, 
 *  
 *  Please be noted that the scenarion ID mm_scen_id_t should the same for all 
 *  chips, VIDEO_RECORD is VIDEO_RECORD but not VIDEO_ENCODE. 
 *  Currently identified scenario IDs include: 
 *    MM_SCEN__OVERLAY
 *    MM_SCEN__PURE_MMI (?)
 *    MM_SCEN__CAMERA_PREVIEW
 *    MM_SCEN__CAMERA_CAPTURE_JPEG
 *    MM_SCEN__VIDEO_PLAYBACK
 *    MM_SCEN__VIDEO_RECORD
 *    MM_SCEN__IMAGE_PLAYBACK
 *    MM_SCEN__VIDEO_TELEPHONY
 *  Newly added scenario IDs please DO NOT rename above.
 *  
 *****************************************************************************/

#if defined(MT6276)
typedef enum
{
    MM_SCEN__PURE_MMI
  , MM_SCEN__CAMERA_PREVIEW
  , MM_SCEN__CAMERA_CAPTURE_JPEG
  , MM_SCEN__VIDEO_PLAYBACK
  , MM_SCEN__VIDEO_RECORD
  , MM_SCEN__IMAGE_PLAYBACK
  , MM_SCEN__JPEG_DECODE
  , MM_SCEN__VIDEO_TELEPHONY
  , MM_SCEN__JPEG_ENCODE
  , MM_SCEN__ISDBT
  , MM_SCEN____COUNT
  , MM_SCEN____NONE
} mm_scen_id_t;

typedef enum
{
    MM_INTMEM_SEG__ROT0_0
  , MM_INTMEM_SEG__ROT0_1
  , MM_INTMEM_SEG__ROT0_2
  , MM_INTMEM_SEG__ROT0_3  
  , MM_INTMEM_SEG__ROT1_0
  , MM_INTMEM_SEG__ROT1_1
  , MM_INTMEM_SEG__ROT1_2
  , MM_INTMEM_SEG__ROT1_3  
  , MM_INTMEM_SEG__ROT2_0
  , MM_INTMEM_SEG__ROT2_1
  , MM_INTMEM_SEG__ROT2_2
  , MM_INTMEM_SEG__ROT2_3  
  , MM_INTMEM_SEG__ROT3_0
  , MM_INTMEM_SEG__ROT3_1
  , MM_INTMEM_SEG__ROT3_2
  , MM_INTMEM_SEG__ROT3_3   
  , MM_INTMEM_SEG__JPGDMA_0
  , MM_INTMEM_SEG__JPGDMA_1
  , MM_INTMEM_SEG__BRZ_0
  , MM_INTMEM_SEG__BRZ_1
  , MM_INTMEM_SEG__BRZ_2
  , MM_INTMEM_SEG__VRZ_0  
  , MM_INTMEM_SEG__DESCPT
  , MM_INTMEM_SEG__PNG
  , MM_INTMEM_SEG__GIF  
  , MM_INTMEM_SEG__JPEG_DEC
  , MM_INTMEM_SEG__SW_VIDEO
  , MM_INTMEM_SEG__ISP
  , MM_INTMEM_SEG__ISDBT
  , MM_INTMEM_SEG____COUNT
  , MM_INTMEM_SEG__JPEG_ENC
} mm_intmem_segment_id_t;

#elif defined(MT6256) 

typedef enum
{
    MM_SCEN__CAMERA_PREVIEW
  , MM_SCEN__CAMERA_CAPTURE_JPEG
  , MM_SCEN__VIDEO_PLAYBACK
  , MM_SCEN__VIDEO_RECORD_W_800  // video width is 800
  , MM_SCEN__VIDEO_RECORD_W_640  // video width is 640 & below
  , MM_SCEN__IMAGE_PLAYBACK
  , MM_SCEN__JPEG_DECODE
  , MM_SCEN__VIDEO_TELEPHONY
  , MM_SCEN__JPEG_ENCODE
  , MM_SCEN__MJPEG_DECODE
  , MM_SCEN__MJPEG_ENCODE
  , MM_SCEN__ISDBT
  , MM_SCEN____COUNT
  , MM_SCEN____NONE
} mm_scen_id_t;

typedef enum
{
    MM_INTMEM_SEG__ROT0_0 // VDOROT
  , MM_INTMEM_SEG__ROT0_1
  , MM_INTMEM_SEG__ROT0_2
  , MM_INTMEM_SEG__ROT0_3  
  , MM_INTMEM_SEG__ROT1_0 // RGBROT0
  , MM_INTMEM_SEG__ROT1_1
  , MM_INTMEM_SEG__ROT1_2
  , MM_INTMEM_SEG__ROT1_3  
  , MM_INTMEM_SEG__ROT2_0 // not use
  , MM_INTMEM_SEG__ROT2_1
  , MM_INTMEM_SEG__ROT2_2
  , MM_INTMEM_SEG__ROT2_3  
  , MM_INTMEM_SEG__ROT3_0 // not use
  , MM_INTMEM_SEG__ROT3_1
  , MM_INTMEM_SEG__ROT3_2
  , MM_INTMEM_SEG__ROT3_3   
  , MM_INTMEM_SEG__JPGDMA_0
  , MM_INTMEM_SEG__JPGDMA_1
  , MM_INTMEM_SEG__BRZ_0
  , MM_INTMEM_SEG__BRZ_1
  , MM_INTMEM_SEG__BRZ_2
  , MM_INTMEM_SEG__VRZ_0  
  , MM_INTMEM_SEG__DESCPT
  , MM_INTMEM_SEG__JPEG_DEC
  , MM_INTMEM_SEG__SW_VIDEO
  , MM_INTMEM_SEG__ISP
  , MM_INTMEM_SEG____COUNT
} mm_intmem_segment_id_t;

#elif defined(MT6255) 

typedef enum
{
    MM_SCEN__PURE_MMI = 0
  , MM_SCEN__CAMERA_PREVIEW
  , MM_SCEN__CAMERA_CAPTURE_JPEG
  , MM_SCEN__VIDEO_PLAYBACK
  , MM_SCEN__MJPEG_DECODE
  , MM_SCEN__VIDEO_RECORD
  , MM_SCEN__IMAGE_PLAYBACK
  , MM_SCEN__JPEG_DECODE
  , MM_SCEN__VIDEO_TELEPHONY
  , MM_SCEN__JPEG_ENCODE
  , MM_SCEN__ISDBT
  , MM_SCEN____COUNT
  , MM_SCEN____NONE
} mm_scen_id_t;

typedef enum
{
    MM_INTMEM_SEG__ROT0_0 // VDOROT
  , MM_INTMEM_SEG__ROT0_1
  , MM_INTMEM_SEG__ROT0_2
  , MM_INTMEM_SEG__ROT0_3  
  , MM_INTMEM_SEG__ROT1_0 // RGBROT0
  , MM_INTMEM_SEG__ROT1_1
  , MM_INTMEM_SEG__ROT1_2
  , MM_INTMEM_SEG__ROT1_3  
  , MM_INTMEM_SEG__DESCPT
  , MM_INTMEM_SEG__JPEG_DEC
  , MM_INTMEM_SEG__JPEG_ENC
  , MM_INTMEM_SEG__SW_VIDEO
  , MM_INTMEM_SEG__ISP
  , MM_INTMEM_SEG____COUNT
} mm_intmem_segment_id_t;

#elif defined(MT6268T) || defined(MT6268) || defined(MT6268A) || defined(MT6268B) || defined(MT6268H)


typedef enum
{
    MM_SCEN__PURE_MMI = 0
  , MM_SCEN__CAMERA_CAPTURE_BARCODE
  , MM_SCEN__CAMERA_CAPTURE_JPEG
  , MM_SCEN__CAMERA_CAPTURE_MEM
  , MM_SCEN__CAMERA_PREVIEW
  , MM_SCEN__IMAGE_EFFECT_PIXEL
  , MM_SCEN__IMAGE_RESIZE
  , MM_SCEN__JPEG_DECODE
  , MM_SCEN__JPEG_ENCODE
  , MM_SCEN__JPEG_RESIZE
  , MM_SCEN__RGB2YUV
  , MM_SCEN__VIDEO_PLAYBACK
  , MM_SCEN__VIDEO_EDITOR
  , MM_SCEN__VIDEO_RECORD
  , MM_SCEN__WEBCAM
  , MM_SCEN__VIDEO_TELEPHONY
  , MM_SCEN____COUNT
  , MM_SCEN____NONE
} mm_scen_id_t;


typedef enum
{
    MM_INTMEM_SEG__IRT0
  , MM_INTMEM_SEG__IRT1
  , MM_INTMEM_SEG__IRT2
  , MM_INTMEM_SEG__JPGDMA
  , MM_INTMEM_SEG__PRZ_FINE
  , MM_INTMEM_SEG__PRZ_COARSE
  , MM_INTMEM_SEG__MP4DEBLK
  , MM_INTMEM_SEG__VIDEO_ENCODE_WDMA
  , MM_INTMEM_SEG__TV_OUT
  , MM_INTMEM_SEG__ISP
  , MM_INTMEM_SEG__HW_VIDEO
  , MM_INTMEM_SEG__PNG
  , MM_INTMEM_SEG__GIF  
  , MM_INTMEM_SEG__IDP
  , MM_INTMEM_SEG__JPEG_DEC
  , MM_INTMEM_SEG____COUNT
  , MM_INTMEM_SEG__JPEG_ENC
} mm_intmem_segment_id_t;


#elif defined(MT6253) 

typedef enum
{
   //MM_SCEN__OVERLAY
  MM_SCEN__CAMERA_PREVIEW = 0
  , MM_SCEN__CAMERA_CAPTURE_JPEG
  , MM_SCEN__WEBCAM
  , MM_SCEN__VIDEO_RECORD
  , MM_SCEN__VIDEO_DECODE
  , MM_SCEN__JPEG_DECODE
  , MM_SCEN__JPEG_ENCODE

  , MM_SCEN__VIDEO_PLAYBACK
  , MM_SCEN____COUNT
  , MM_SCEN__PURE_MMI
  , MM_SCEN____NONE
} mm_scen_id_t;

/**
 *  
 */
typedef enum
{
    MM_INTMEM_SEG__LCD = 0
  , MM_INTMEM_SEG__CRZ
  , MM_INTMEM_SEG__JPEG_DEC
  , MM_INTMEM_SEG__JPEG_ENC

  , MM_INTMEM_SEG__SW_VIDEO
  , MM_INTMEM_SEG____COUNT
  , MM_INTMEM_SEG__PNG
  , MM_INTMEM_SEG__GIF  
} mm_intmem_segment_id_t;


#else  //MT6236/MT6235

typedef enum
{
    MM_SCEN__OVERLAY = 0 /**< OVERLAY is an always-on scenario for normal 
                              boot-up, it is used for normal UI image 
                              generation. This scenario may be used 
                              concurrently with other MM scenarios such as 
                              CAMERA_PREVIEW, CAMERA_CAPTURE_JPEG, ...etc, 
                              depending on MM internal memory design of each 
                              chip. 
                              @remark Must start from 0 or MM_SCEN____COUNT is 
                                      useless
                              */
  , MM_SCEN__CAMERA_PREVIEW
  , MM_SCEN__CAMERA_CAPTURE_JPEG
  , MM_SCEN__VIDEO_PLAYBACK
  , MM_SCEN__VIDEO_RECORD
  , MM_SCEN__IMAGE_PLAYBACK
  , MM_SCEN__JPEG_ENCODE
  , MM_SCEN__VIDEO_TELEPHONY

  , MM_SCEN__MEMORY_POOL /**< In MEMORY_POOL scenario, MM internal memory will 
                              be used as normal and continuous memory pool 
                              without separation on memory bank boundaries. 
                              Must use MM_INTMEM_SEG__POOL_0, 
                              MM_INTMEM_SEG__POOL_1, or similar segments.
                              */
  
  , MM_SCEN____COUNT
  , MM_SCEN__JPEG_DECODE
  , MM_SCEN__PURE_MMI
  , MM_SCEN____NONE
} mm_scen_id_t;

/**
 *  
 */
typedef enum
{
    MM_INTMEM_SEG__ROT0_0 = 0 
  , MM_INTMEM_SEG__ROT0_1
  , MM_INTMEM_SEG__ROT1_0
  , MM_INTMEM_SEG__ROT1_1
  , MM_INTMEM_SEG__JPGDMA_0
  , MM_INTMEM_SEG__JPGDMA_1
  , MM_INTMEM_SEG__ISP
  , MM_INTMEM_SEG__SW_VIDEO

  , MM_INTMEM_SEG__POOL_0 /**< MMSYS internal memory. */
  , MM_INTMEM_SEG__POOL_1 /**< Other internal memory pool based on different 
                               chip design. For example, MM region of DTCM on 
                               MT6236. */

  , MM_INTMEM_SEG____COUNT
  , MM_INTMEM_SEG__JPEG_ENC
  , MM_INTMEM_SEG__JPEG_DEC
  , MM_INTMEM_SEG__PNG
  , MM_INTMEM_SEG__GIF  
} mm_intmem_segment_id_t;

#endif//#if (defined(MT6276) || defined(MT6255)|| defined(MT6256))


/**
 * This function gets the internal memory for a perticular usage in a pre-defined 
 * multimedia scenario.
 * 
 * @return KAL_TRUE for success; KAL_FALSE o.w.
 *
 * @param scen_id [in] scenario ID.
 * @param segment_id [in] segment ID.
 * @param address [out] pointer to kal_uint32 to hold the start address of the intmem allocated
 * @param size [out] pointer to kal_uint32 to hold the size of the intmem allocated
 * 
 * @remark address alignment and size alignment will be considered by projects in implementation.
 */
extern kal_bool
mm_intmem_get(
  mm_scen_id_t scen_id, 
  mm_intmem_segment_id_t segment_id, 
  kal_uint32 *address, 
  kal_uint32 *size);

/**
 * This function frees the internal memory allocated for a perticular usage in a pre-defined 
 * multimedia scenario.
 * 
 * @return KAL_TRUE for success; KAL_FALSE o.w.
 *
 * @param scen_id [in] scenario ID.
 * @param segment_id [in] segment ID.
 *
 * @remark Only all allocated internal memory for a scenario is returned. Then mm_intmem_get() 
 *         could be invoked again.
 *
 */
extern kal_bool
mm_intmem_free(
  mm_scen_id_t scen_id, 
  mm_intmem_segment_id_t segment_id);

#if (defined(MT6276) || defined(MT6256) || defined(MT6268) || defined(MT6255))
extern kal_bool
mm_intmem_query_non_multimedia(void);

extern kal_bool
mm_intmem_query_scenario(mm_scen_id_t const scen_id);

#endif//(#if defined(MT6276) || defined(MT6256) || defined(MT6268) || defined(MT6255)) 

#endif // #ifndef __MM_INTMEM_H__

