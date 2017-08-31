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

/*******************************************************************************
 * Filename:
 * ---------
 *	lmmi_ind.h
 *
 * Project:
 * --------
 *   MT6208
 *
 * Description:
 * ------------
 *   This file is intends for ...
 *
 * Author:
 * -------
 *	Amanda Gau
 *
 *==============================================================================
 * 				HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Log$
 * 
 * 07 24 2012 chenhao.gong
 * [MAUI_03218713] Support snapshot rectangle AT command
 * .
 *
 * 04 01 2012 chenhao.gong
 * [MAUI_03160629] [L4C][AT]AT command support quick input
 * .
 *
 * 03 07 2012 chenhao.gong
 * [MAUI_03136357]  [ZTENJ21][6210]16.2.2 failure
 * [MAUI_03136357] [ZTENJ21][6210]16.2.2 failure
 * .
 *
 * 10 26 2011 hong.yu
 * [MAUI_03058461] [MT6276_ADAPT][LabTest] AT&T display issue
 * add domain_id for MMI_NW_ATTACH_IND
 *
 * 09 28 2011 hong.yu
 * [MAUI_03037878] [HAL] CTM restructure
 * remove CTM from L4
 *
 * 09 27 2011 hong.yu
 * [MAUI_03037763] [L4][AT] +ETEST revise
 * redirect +ETEST to MMI
 *
 * 06 02 2011 lexel.yu
 * [MAUI_02942774] [L4C][ATCI] Replace __EM_MODE__ with __MODEM_EM_MODE__
 * Integration change. 
 *
 * 05 31 2011 chenhao.gong
 * [MAUI_02947984] [L4C]Add back messages used by LG project
 * .
 *
 * 05 25 2011 danny.kuo
 * [MAUI_02917493] [L4C/AT] ETWS support
 * .
 *
 * 04 28 2011 hong.yu
 * [MAUI_02875562] [Smart Phone][AGPS] check in AGPS IPC
 * Integration change.
 *
 * 04 13 2011 lexel.yu
 * [MAUI_02908638] [ENS] RAT balancing and CSP plmn mode feature
 * Integration change. 
 *
 * 03 25 2011 chenhao.gong
 * [MAUI_02880774] AT ommand for ringtone
 * .
 *
 * 02 22 2011 hong.yu
 * [MAUI_02875562] [Smart Phone][AGPS] check in AGPS IPC
 * Check in AGPS IPC
 *
 * 02 15 2011 cylen.yao
 * [MAUI_02870267] [HAL] custom folder split
 * [HAL] MAUI custom module split
 *
 * 12 22 2010 kitty.sung
 * [MAUI_02606864] [AT] Support AT+ESMSS of __SMS_STORAGE_BY_MMI__
 * .
 *
 * 12 19 2010 hong.yu
 * [MAUI_02828429] [RHR][MAUIW1038OF_RHR] Integration to W10.43 check-in by PS5
 * Integration change.
 *
 * 12 15 2010 lexel.yu
 * [MAUI_02825294] [Gemini+][L4C] Support for N card
 * Integration change.
 *
 * 12 14 2010 hong.yu
 * [MAUI_02848691] [RHR] Check-in RHR revise to trunk
 * RHR check-in MAUI
 *
 * 12 07 2010 chenhao.gong
 * [MAUI_02643256] [L4][HAL] HAL Modification
 * Keypad HAL Modification
 *
 * 11 24 2010 lexel.yu
 * [MAUI_02641882] [CMCC TD] Preferred Language
 * Integration change.
 *
 * 11 17 2010 danny.kuo
 * [MAUI_02835013] CMCC AT CMD support for PHB
 * .
 *
 * 11 03 2010 danny.kuo
 * [MAUI_02827360] [Maui][MPM] check in MMI Protocol Modulation revise
 * .
 *
 * 10 29 2010 lexel.yu
 * [MAUI_02832452] [L4C] R7 EHPLMN support
 * R7 EHPLMN to MAUI
 *
 * 10 18 2010 hong.yu
 * [MAUI_02828429] [RHR][MAUIW1038OF_RHR] Integration to W10.43 check-in by PS5
 * Integration change.
 *
 * 09 04 2010 chenhao.gong
 * [MAUI_02634334] [L4]Move None used MMI-L4 interface to a separate block
 * .
 *
 * 09 03 2010 hong.yu
 * [MAUI_02526481] [WW FT][Brazil][Claro] The popup message when make MO Call with hide id is enable
 * .
 *
 * 08 16 2010 hong.yu
 * [MAUI_02601131] [3G AGPS CP] check in AGPS related patch
 * .
 *
 * 08 06 2010 hong.yu
 * [MAUI_01998659] [L4] Remove obsolete MMI messages
 * .
 *
 * 08 06 2010 kitty.sung
 * [MAUI_02603422] [L4] Remove obsolete MMI messages MSG_ID_MMI_CB_DOWNLOAD_IND
 * .
 *
 * 08 06 2010 mingtsung.sun
 * [MAUI_01998659] [L4] Remove obsolete MMI messages
 * .
 *
 * 08 06 2010 danny.kuo
 * [MAUI_01998659] [L4] Remove obsolete MMI messages
 * Remove obsolete MMI PHB messages
 *
 * 08 04 2010 mingtsung.sun
 * [MAUI_02602389] Signal Strength Report Revise
 * .
 *
 * Jul 8 2010 mtk80396
 * [MAUI_02221943] [AT][ATK]PhaseII support code merge
 * 
 *
 * Jul 7 2010 mtk02285
 * [MAUI_02361221] [MT6276] HSPA on/off menu and H icon
 * 
 *
 * May 12 2010 mtk80396
 * [MAUI_02192733] [AT][ATK]AT support Touch Screen and Screen shot on Orange project
 * Check in ATK phase-1 feature
 *
 * May 3 2010 mtk02508
 * [MAUI_02412594] [3G AGPS CP] AGPS code check-in for new CP implementation and UP modification
 * 
 *
 * Apr 16 2010 mtk02480
 * [MAUI_02398139] [Lipton] Support USIM_SUPPORT feature
 * 
 *
 * Apr 16 2010 mtk02088
 * [MAUI_02381894] [AT] Factory at command
 * 
 *
 * Apr 2 2010 mtk02514
 * [MAUI_02378253] [AT] Call log Storage Movement (__CLOG_STORAGE_BY_MMI__)
 * 
 *
 * Mar 26 2010 mtk01616
 * [MAUI_02379358] [V32/V33] Factory AT command AT%BTTM , AT%ECALL , AT%FMR
 * 
 *
 * Mar 22 2010 mtk02285
 * [MAUI_02361221] [MT6276] HSPA on/off menu and H icon
 * 
 *
 * Mar 21 2010 mtk02514
 * [MAUI_02378253] [AT] Call log Storage Movement (__CLOG_STORAGE_BY_MMI__)
 * 
 *
 * Mar 19 2010 mtk80396
 * [MAUI_02166146] [EQ] L4C EQ code revise
 * Remove DL messages
 *
 * Feb 26 2010 mtk02508
 * [MAUI_02362620] [AT] at+cmer revise
 * 
 *
 * Feb 26 2010 mtk02508
 * [MAUI_02360613] [AT] at+cbklt implementation
 * 
 *
 * Feb 11 2010 mtk02480
 * [MAUI_01650679] [AT&T][Aircom PTN]: Network selection question
 * 
 *
 * Feb 10 2010 mtk02088
 * [MAUI_02354493] [AT] AT+EVCLD support vcal_type
 * 
 *
 * Jan 25 2010 mtk02285
 * [MAUI_02333639] [LGE] Factory AT Support
 * 
 *
 * Jan 19 2010 mtk02480
 * [MAUI_02014578] [Lipton] Detect O2 PrePay and PostPay Cards
 * 
 *
 * Dec 30 2009 mtk02480
 * [MAUI_02326304] [SAT] BIP for WISE
 * 
 *
 * Dec 24 2009 mtk02088
 * [MAUI_02060274] Messages cannot be deleted in Phone Suite
 * index of MSG_ID_MMI_SMS_ATCMD_CMGD_IND should be kal_uint16 not kal_uint8.
 *
 * Nov 24 2009 mtk01616
 * [MAUI_01964704] [CC] Add sequence number in call present interface
 * 
 *
 * Nov 9 2009 mtk02508
 * [MAUI_01982044] [L4C] Fixed L4A interface failure
 * 
 *
 * Nov 5 2009 mtk02508
 * [MAUI_01980959] [L4][EQ] Turn off VM_SUPPORT
 * turn off VM_SUPPORT
 *
 * Oct 30 2009 mtk02088
 * [MAUI_01974462] [SMS] SMS Revise (SMS 2.0) Phase 1
 * 
 *
 * Oct 30 2009 mtk02285
 * [MAUI_01667870] [Wise] Check in code for wise development
 * 
 *
 * Oct 29 2009 mtk02088
 * [MAUI_01974462] [SMS] SMS Revise (SMS 2.0) Phase 1
 * 
 *
 * Oct 29 2009 mtk02514
 * [MAUI_01974405] [PHB] App/Srv Split
 * 
 *
 * Oct 29 2009 mtk02088
 * [MAUI_01668995] [WISE] sms
 * 
 *
 * Oct 7 2009 mtk02514
 * [MAUI_01936041] [AT] Move PHB Storage Manager
 * 
 *
 * Sep 25 2009 mtk01616
 * [MAUI_01960842] [L4C] UCM3.0 check-in
 * 
 *
 * Sep 4 2009 mtk02508
 * [MAUI_01950521] [AGPS 2G CP] Revise L4A compile option and add l4ccsm_ciss_aerp_end_cnf_hdlr body
 * wrap LCS messages with __AGPS_CONTROL_PLANE__
 *
 * Sep 3 2009 MTK02088
 * [MAUI_01684019] SMS R6 feature
 * 
 *
 * Aug 10 2009 mtk02508
 * [MAUI_01936096] [AGPS 2G CP] merge L4C codes
 * 
 *
 * Jul 29 2009 mtk02508
 * [MAUI_01929699] [AGPS 2G CP] add cell_id in MSG_ID_MMI_NW_ATTACH_IND
 * 
 *
 * Jul 8 2009 mtk02480
 * [MAUI_01716279] [SAT]Change the file_list in file_change_ind from kal_uint8 to kal_uint16
 * 
 *
 * May 6 2009 mtk01497
 * [MAUI_01680260] RAC  ���ѰϽX(����GPRS���~�n�D����)
 * Report rac (routing area code) to MMI in msg_id_nw_attach_ind.
 *
 * Apr 21 2009 mtk01616
 * [MAUI_01672387] [L4C] add cause in call_disc_ind interface
 * 
 *
 * Apr 17 2009 mtk02480
 * [MAUI_01664028] Wrong PLMN Name: MS displays "Qtel" instead of VF-QA  or vodafone!!!
 * 
 *
 * Apr 15 2009 mtk02480
 * [MAUI_01669424] [Wise] Check in Modem change for wise
 * 
 *
 * Apr 15 2009 MTK02088
 * [MAUI_01668995] [WISE] sms
 * 
 *
 * Apr 15 2009 mtk02285
 * [MAUI_01668339] [NW] General network registration cause report
 * 
 *
 * Mar 26 2009 mtk02514
 * [MAUI_01654422] [L4C] PHB optimization
 * remove unused messages and structures
 *
 * Mar 13 2009 mtk01616
 * [MAUI_01279614] [L4C][Revise] CC related interface change
 * add call_type in cpi_ind
 *
 * Jan 20 2009 mtk01616
 * [MAUI_01315252] [L4C][CSM] Call Progress indication and Call State Update indication
 * 
 *
 * Jan 16 2009 mtk02480
 * [MAUI_01352635] [WM][WM613 PPC][SAT]modem should not send response for Provide Local Infomation(Lang
 * STK interface change
 *
 * Jan 10 2009 mtk02480
 * [MAUI_01484869] [AT] AT+CLCK MMI no sync with AT
 * Add chv_status to l4c_sim_verify_pin_result_lind
 *
 * Dec 24 2008 mtk02514
 * [MAUI_01301722] [UEM] move USB_ENABLE compile option to function body instead of function prototype
 * remove __USB_ENABLE__ option to have a dummy function when this option is not enabled
 *
 * Dec 5 2008 mtk01497
 * [MAUI_01255000] [SMU][SMSAL] OTA feature of De-Personalisation SMSPP class1
 * Notify MMI if SIM-ME lock is unlocked by SMS.
 *
 * Nov 26 2008 mtk01616
 * [MAUI_01279614] [L4C][Revise] CC related interface change
 * 
 *
 * Oct 30 2008 mtk01616
 * [MAUI_01264772] [CC] Auto call rejection feature
 * 
 *
 * Oct 23 2008 mtk01616
 * [MAUI_01213844] [SMSAL][FixBug] send from storage by profile setting
 * 
 *
 * Oct 8 2008 mtk01616
 * [MAUI_01250760] [L4C] SMS multi source support and new interfaces
 * 
 *
 * Oct 3 2008 mtk01616
 * [MAUI_01241584] [NewFeature] Incoming call related information
 * 
 *
 * Sep 17 2008 mtk01616
 * [MAUI_00736162] [PhoneSuite]<MMS>Preview MMS popup "Fail to download MMS message !" after upload to
 * 
 *
 * Jul 31 2008 mtk01616
 * [MAUI_01094362] [AT] AT+CIND=? return number of parameters is wrong
 * 
 *
 * Jul 11 2008 mtk01616
 * [MAUI_00799529] [NewFeature] USSD Auto Reject and Reply Null
 * 
 *
 * Apr 25 2008 mtk01497
 * [MAUI_00755505] New API to replace is_sim_replaced()
 * 
 *
 * Apr 17 2008 mtk01497
 * [MAUI_00756618] [L4C] move lmmi_ind.c and lmmi_rsp.c to custom folder
 * 
 *
 * Apr 17 2008 mtk01497
 * [MAUI_00756618] [L4C] move lmmi_ind.c and lmmi_rsp.c to custom folder
 * Fix compile error for xxx_basic project.
 *
 * Apr 17 2008 mtk01616
 * [MAUI_00731176] new ECC service category for Rel5
 * 
 *
 * Apr 17 2008 mtk01616
 * [MAUI_00758137] Add irho flag in speech on/off indication
 * 
 *
 * Apr 15 2008 mtk01497
 * [MAUI_00756618] [L4C] move lmmi_ind.c and lmmi_rsp.c to custom folder
 * 
 *
 * Apr 15 2008 mtk01497
 * [MAUI_00756618] [L4C] move lmmi_ind.c and lmmi_rsp.c to custom folder
 * 
 *
 * Feb 22 2008 mtk01616
 * [MAUI_00622679] [L4C] [NewFeature] Rel4/Rel5 check-in
 * 
 *
 * Feb 19 2008 mtk01497
 * [MAUI_00624400] [Call]can not dial  out Emergency call
 * Report mmrr active/inactive status to MMI
 *
 * Jan 24 2008 mtk00924
 * [MAUI_00582286] [GEMINI][L4] modification for GEMINI
 * 
 *
 * Jan 24 2008 mtk00924
 * [MAUI_00582286] [GEMINI][L4] modification for GEMINI
 * 
 *
 * Oct 8 2007 mtk01616
 * [MAUI_00555631] [MAUI][ENS] Cingular Spec - Enhanced Network Selection
 * 
 *
 * Oct 6 2007 mtk00924
 * [MAUI_00556140] [AT][vCalendar] Support Todolist for PhoneSuite
 * 
 *
 * Sep 16 2007 mtk01616
 * [MAUI_00548804] [L4C][NewFeature] SATe feature check-in
 * 
 *
 * Sep 9 2007 mtk00924
 * [MAUI_00537970] [L4C][NewFeature] Check in video call support
 * 
 *
 * Aug 13 2007 mtk01616
 * [MAUI_00533530] [L4C][New feature]  AT for UCM
 * 
 *
 * Jul 15 2007 mtk00924
 * [MAUI_00402910] [L4C]Fix compile warning
 * 
 *
 * Jun 9 2007 mtk00924
 * [MAUI_00402910] [L4C]Fix compile warning
 * 
 *
 * Jun 2 2007 mtk01616
 * [MAUI_00398840] [MMI][EONS][new feature] LAC support for EF_OPL
 * Add lac in NW_ATTACH_IND
 *
 * May 21 2007 mtk01616
 * [MAUI_00376446] USSD: 7 bit GSM alphabet characters support, some char missing
 * Add length field in ussd SAP
 *
 * May 18 2007 mtk01616
 * [MAUI_00386216] �����u�����ӹq���X
 * Add redirect number in ring_ind
 *
 * May 7 2007 mtk01616
 * [MAUI_00389729] Add wap_get_service_module to get the right destination module
 * Get destination module for WAP
 *
 * Apr 23 2007 mtk01616
 * [MAUI_00385420] [L4C]CNAP support
 * 
 *
 * Apr 3 2007 mtk01616
 * [MAUI_00358228] The Immediate display in CB message.
 * Add l4csmsal_cb_gs_change_ind_hdlr
 *
 * Apr 2 2007 mtk01616
 * [MAUI_00375611] [MM][SM][RAC] sgsnr_flag modification and pass egprs status to RAC/L4C
 * Add egprs status in l4c_nw_attach_lind() and l4c_ps_gprs_status_update_lind()
 *
 * Apr 2 2007 mtk01616
 * [MAUI_00375611] [MM][SM][RAC] sgsnr_flag modification and pass egprs status to RAC/L4C
 * Add egprs status in l4c_nw_attach_lind() and l4c_ps_gprs_status_update_lind()
 *
 * Jan 4 2007 mtk00924
 * [MAUI_00350006] Notification of call forwarded during an outgoing call is missing
 * report ss_status and ss_notification to MMI
 *
 * Nov 20 2006 mtk00888
 * [MAUI_00344438] [MONZA] [L4C] sync MONZA and Trunk
 * 
 *
 * Nov 6 2006 mtk00924
 * [MAUI_00340910] [EM] New Request: Network Events Notification Mech.
 * 
 *
 * Oct 24 2006 mtk00924
 * [MAUI_00225269] restory factory setting display wrong information
 * add MSG_ID_MMI_SMU_VERIFY_PIN_RESULT_IND for sync SIM lock status between AT and MMI
 *
 * Oct 23 2006 MTK00758
 * [MAUI_00337656] [BT]VoIP over BT
 * 
 *
 * Sep 9 2006 mtk00924
 * [MAUI_00328498] [AT][NewFeature]update +ELCM for LCM calibration and testing tool
 * 
 *
 * Jul 23 2006 mtk00924
 * [MAUI_00015929] [SMSAL][Revise] Email indication shown on receiving Voice mail or fax  Indication.
 * modify sms_waiting_ind interface
 *
 * Jul 17 2006 mtk00888
 * [MAUI_00210759] [SMU][New Feature] NEW SIM-ME Lock Feature
 * 
 *
 * Jun 26 2006 mtk00888
 * [MAUI_00188113] [L4][MMI][PHB] Add USIM PHB feature
 * 
 *
 * Jun 19 2006 mtk00888
 * [MAUI_00203526] [L4C] for #203242 ([MMI][NITZ] Support PLMN)
 * 
 *
 * May 22 2006 mtk00924
 * [MAUI_00195602] [AT][MMI][NewFeature]MMI trace string
 * +EMMISTR cmd for MMI trace string
 *
 * Apr 24 2006 mtk00924
 * [MAUI_00189593] [New Feature]+EVCARD for optional PHB field
 * add +EVCARD for optional PHB field
 *
 * Apr 24 2006 mtk00888
 * [MAUI_00189410] [MMI][New Feature][Unified Message] Check in Unified Message
 * 
 *
 * Apr 17 2006 mtk00924
 * [MAUI_00186129] [L4C]remove compile warning
 * remove compile warning
 *
 * Apr 17 2006 mtk00924
 * [MAUI_00187907] [MMI][BCHS]to check in HFP VR to main trunk W06.16
 * Add BT VR AT cmd
 *
 * Apr 17 2006 mtk00888
 * [MAUI_00187884] [L4C] USIM PHB
 * 
 *
 * Apr 4 2006 mtk00924
 * [MAUI_00184688] [CSMCC][CC][AT] remove compile warning
 * remove compile warning
 *
 * Apr 2 2006 mtk00924
 * [MAUI_00183918] [MMI][BCHS]bluetooth loopback1 and loopback2 feature
 * Add BT loopback testing +EBTLB cmd
 *
 * Mar 19 2006 mtk00758
 * [MAUI_00176836] Call History-Enter "Max Cost" (or Price Per Unit) and input any number (Ex.99), "Unf
 * 
 *
 * Dec 12 2005 mtk00888
 * [MAUI_00161035] [L4C] new feature: CTM (__CTM_SUPPORT__)
 * 
 *
 * Dec 5 2005 mtk00888
 * [MAUI_00159921] [L4C] add primitive: MMI_EQ_LEAVE_PRECHARGE_IND/MMI_EQ_PMIC_CONFIG_REQ/MMI_EQ_PMIC_C
 * 
 *
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *==============================================================================
 *******************************************************************************/

/********************************************************************************
*  Copyright Statement:
*  --------------------
*
*  This product has been developed using a protocol stack
*  developed by Sasken Communication Technologies Limited.
*
********************************************************************************/


#ifndef _LMMI_IND_H
#define _LMMI_IND_H

#if ( !defined(__L1_STANDALONE__) && !defined(__MAUI_BASIC__))

//#include "kal_non_specific_general_types.h"
//#include "l4c_aux_struct.h"
//#include "csmcc_enums.h"
#include "csmcc_common_enums.h"
//#include "l4c_common_enum.h"
//#include "ps2sim_struct.h"
#include "l4c2smu_struct.h"
#include "keypad_sw.h"
//#include "l4c2uem_struct.h"
//#include "l4c2phb_enums.h"
//#include "l4c2phb_struct.h"
#include "l4c2smsal_struct.h"
#include "mcd_l4_common.h"
#include "ps2sat_struct.h"
#include "mcd_ps2sat_peer.h"
#include "app_ltlcom.h"
#include "kal_general_types.h"
#include "ps_public_struct.h"
#include "ps_public_enum.h"
#if defined(__AGPS_CONTROL_PLANE__) && defined(__ANDROID_RIL_SUPPORT__) //agps_ipc
#include "l4c_sendmsg.h"
#endif

#ifdef __EMPTY_CALLBACK__

#define l4c_cc_call_meter_lind(call_id,ccm,warning,total_ccm,puct_valid,currency,ppu) NULL

#define l4c_cc_call_ring_lind(call_id,num,sub_addr,redirect_num,name_present,cnap_info_exist,tag,name,dcs,length,auto_answer,call_type,addr_ind_present,addr_ind,signal_value_present,signal_value,alerting_pattern_present,alerting_pattern,no_cli_cause_present,no_cli_cause,call_priority_present,call_priority) NULL

#define l4c_cc_call_present_lind(call_id,num,sub_addr,redirect_num,name_present,cnap_info_exist,tag,name,dcs,length,auto_answer,call_type,addr_ind_present,addr_ind,
                                                      signal_value_present,signal_value,alerting_pattern_present,alerting_pattern,no_cli_cause_present,no_cli_cause,call_priority_present,call_priority,seq_num) NULL

#define l4c_cc_cpi_lind(call_id,msg_type,is_ibt,is_tch,dir,call_mode,numberP,number,disc_cause,src_id,call_type) NULL

#define l4c_cc_data_call_req_lind(source_id,opcode,call_type,number) NULL

#define l4c_cc_call_disc_lind(call_id,ccbs,cause,call_type,is_diag_present,diag) NULL

#define l4c_ss_ussr_lind(dcs,ussd_string,length,direction) NULL

#define l4c_ss_ussn_lind(dcs,ussd_string,length,direction) NULL

#define l4c_cphs_display_cfu_lind(action,line_num) NULL

#define l4c_nw_reg_state_lind(lac,cell_id,status,gprs_state,gprs_status) NULL

#define l4c_nw_reg_cause_lind(kal_uint8 domain, kal_uint8 cause) NULL //__NW_REG_CAUSE_REPORT__

#define l4c_nw_attach_lind(status,plmn,gsm_state,gprs_state,gprs_status, rat,cell_support_egprs,lac,rac,cell_id,cause,data_speed_support,is_on_hplmn,domain) NULL //hspa_mmi, mtk02285

#define l4c_nw_rx_level_lind(rat, rssi_in_qdbm, RSCP_in_qdbm, EcN0_in_qdbm, ber, current_band) NULL //rx_level, mtk02285

#define l4c_nw_time_zone_lind(full_nw_nameP,full_nw_name_dcs,full_nw_name_add_ci,full_nw_name_len,full_nw_name,short_nw_nameP,short_nw_name_dcs,short_nw_name_add_ci,short_nw_name_len,short_nw_name,nw_time_zoneP,nw_time_zone,nw_time_zone_timeP,nw_time_zone_time,lsa_idP,lsa_id_len,lsa_id,nw_day_light_saving_timeP,nw_day_light_saving_time,plmn) NULL

#define l4c_nw_update_ecc_lind(kal_uint8 no_ecc,l4c_rac_ecc_number_struct* ecc_list) NULL

#define  l4c_nw_ef_rat_mode_change_lind(is_valid_ef_rat_mode, ef_rat_mode) NULL

#define l4c_phb_update_lind(action,old_index,sim_used,nvram_used,no_list,list) NULL

#define l4c_phb_startup_read_lind(phb_entries,access_id) NULL

#define l4c_phb_startup_begin_lind(desc) NULL

#define l4c_mmi_read_phb_req_lind(src_id, type, storage, record_index, num_index) NULL      //mtk02514_plutommi_usim

#define l4c_mmi_write_phb_req_lind(src_id, entry) NULL

#define l4c_mmi_delete_phb_req_lind(src_id, type, storage, record_index, num_index) NULL    //mtk02514_plutommi_usim

#define l4c_mmi_read_clog_req_lind(src_id, type, index) NULL

#define l4c_mmi_write_clog_req_lind(src_id, type, index, entry) NULL

#define l4c_mmi_delete_clog_req_lind(src_id, type, index) NULL

#define l4c_mmi_read_additional_req_lind(src_id, type, storage, occurrence, record_index) NULL

#define l4c_mmi_write_additional_req_lind(src_id, type, storage, occurrence, record_index, entry) NULL

#define l4c_mmi_delete_additional_req_lind(src_id, type, storage, occurrence, record_index) NULL


#define l4c_sim_passwd_required_lind(type,is_sim_change,chv_info,cphs_retry_count) NULL

#define l4c_sim_startup_info_lind(num,ecc_entry,language,alpha_set,language_ext,alpha_set_ext,sim_type) NULL

#define l4c_sim_cipher_lind(cipher_ind,is_gsm_conn_exist,gsm_cipher_cond,gprs_cipher_cond) NULL

#define l4c_sim_verify_pin_result_lind(kal_uint8 next_type, sim_chv_info_struct chv_info, sim_chv_status_struct chv_status) NULL  // mtk02480 add chv_status

#define l4c_sim_o2_prepaid_sim_lind(is_o2_prepaid_sim) NULL

#define l4c_sim_support_plmn_list_lind(length) NULL


#define l4c_cphs_mmi_info_lind(is_spn_valid,is_spn_RPLMN_required,spn,is_opname_valid,opname,is_sopname_valid,sopname,no_msisdn,msisdn,is_valid_csp,csp,no_info_num,info_num, is_puct_valid, ef_puct, is_autolock_enable, autolock_result, autolock_remain, is_valid_ef_acting_hplmn,ef_acting_hplmn,imsi,digits_of_mnc,is_usim,is_valid_ef_ehplmn,num_of_ehplmn,ef_ehplmn) NULL 


#define l4c_eq_keypad_detect_lind(func_type) NULL

#define l4c_eq_keypad_event_act_lind(mode) NULL
#define l4c_eq_scrntch_event_act_lind(tscrn) NULL

#define l4c_eq_at_quick_input_lind(string) NULL

#define l4c_eq_at_cbklt_query_req_lind () NULL

#define l4c_eq_power_on_lind(poweron_mode,battery_level,build_label,rtc_time,flightmode_state,dual_sim_mode_setting, dual_sim_uart_setting) NULL

#define l4c_eq_battery_status_lind(battery_status,battery_voltage) NULL

#define l4c_eq_rtc_period_lind(rtc_type,rtc_time) NULL

#define l4c_eq_alarm_detect_lind(rtc_time) NULL

#define l4c_em_lcm_test_lind(color) NULL

#define l4c_em_keypad_event_act_lind(on_off) NULL

#define l4c_em_lcm_sleep_mode_req_lind(on_off) NULL

#define l4c_factory_test_lind(on_off) NULL

#define l4c_set_mmi_default_prof_lind(op,category,param1,param2,param3,param4) NULL

#define l4c_eq_audio_play_finish_lind(result,identifier) NULL

#define l4c_eq_fm_test_req_lind(opcode,frequency) NULL  

#define l4c_get_adc_all_channel_lind(vbat,bat_temp,vaux,charge_current,vcharge) NULL

#define l4c_eq_gpio_detect_lind(gpio_device,on_off) NULL
#ifdef __IRDA_SUPPORT__
#define l4c_eq_ircomm_disconnect_lind() NULL
#define l4c_eq_ircomm_connect_lind() NULL
#endif 

#define l4c_eq_set_volume_req_ind(volume_type,volume_level) NULL

#define l4c_eq_set_mute_mode_req_ind(audio_device,mute_onoff) NULL

#define l4c_eq_set_silent_mode_req_ind(mode) NULL

#define l4c_eq_set_gpio_level_req_ind(gpio_dev_type,gpio_dev_level,duration) NULL

#define l4c_eq_set_keypad_input_req_ind(keypad_status,keycode) NULL

#define l4c_eq_set_dspl_text_req_ind(text_string) NULL

#define l4c_eq_set_indicator_req_ind(icon_id,value) NULL

#define l4c_eq_set_rtc_time_req_ind(rtc_type,set_type,alarm) NULL

#if defined(__NONE_USED_L4MMI_MESSAGE__)
#define l4c_eq_exe_rtc_delete_req_ind(rtc_type,rtc_index) NULL
#endif

#define l4c_eq_set_date_time_format_req_ind(is_date_set,mode) NULL

#define l4c_eq_set_language_req_ind(lang_code) NULL

#define l4c_eq_set_greeting_text_req_ind(mode,dcs,text,length) NULL

#define l4c_eq_set_vibrator_mode_req_ind(mode) NULL

#define l4c_eq_at_alarm_set_req_lind(alarm) NULL
#define l4c_eq_at_alarm_delete_req_lind(alarm_index) NULL
#define l4c_eq_at_alarm_query_req_lind() NULL

#define l4c_eq_query_vibrator_mode_req_lind() NULL
#define l4c_eq_query_silent_mode_req_lind() NULL
#define l4c_eq_query_mute_mode_req_lind() NULL
#define l4c_eq_query_greeting_text_req_lind() NULL
#define l4c_eq_query_language_req_lind() NULL
#define l4c_eq_query_datetime_format_req_lind(is_date) NULL

#define l4c_eq_screen_touch_req_lind(action,x_coords,y_coords) NULL
#define l4c_eq_screen_shot_req_lind(op,x_coordinates,y_coordinates,width,height) NULL
#define l4c_eq_scrnorient_req_lind(mode, orient) NULL
#define l4c_eq_scrnsize_query_req_lind() NULL

#define l4c_eq_set_hand_free_mode_req_lind(is_hand_free) NULL   //mtk02088_factory
#define l4c_eq_query_hand_free_mode_req_lind() NULL
#define l4c_eq_set_cam_req_lind(mode) NULL   
#define l4c_eq_set_avr_req_lind(mode) NULL   

#ifdef VM_SUPPORT
#ifdef __FS_ON__
#define l4c_eq_vm_record_finish_lind(result,file_name) NULL
#else
#define l4c_eq_vm_record_finish_lind(result,file_id) NULL
#endif

#define l4c_eq_vm_play_finish_lind(result) NULL
#endif


#define l4c_cc_call_setup_req_ind(dest,call_type,clir_flag,cug_option) NULL
#define l4c_cc_call_accept_req_ind(call_id) NULL

#define l4c_cc_call_abort_req_ind() NULL

#define l4c_cc_call_sync_req_ind() NULL

#define l4c_cc_video_call_status_ind() NULL

#define l4c_nvm_ready_lind() NULL
#define l4c_sim_fail_lind(cause) NULL
#define l4c_smu_sml_status_notify_lind() NULL
#define l4c_phb_startup_finish_lind(sim_max_num,phb_max_num,phb_len,fdn_len,bdn_len,owner_len, desc) NULL
#define l4c_phb_ln_ready_lind() NULL

#define l4c_abm_ready_lind() NULL

#define l4c_cc_name_lind() NULL

#define l4c_cc_notify_ss_lind(call_id,type,status_present,status,notification_present,notification) NULL
//MAUI_01998659 #define l4c_cc_notify_ss_cug_lind(call_id,index) NULL
#define l4c_cc_notify_ss_ect_lind(call_id,present,addr,sub_addr) NULL
#define l4c_cc_notify_ss_cnap_lind(call_id,name_present,cnap_info_exist,tag,name,dcs,length) NULL
#define l4c_cc_notify_ss_ccbs_lind(call_id,number_present,addr,sub_addr_present,sub_addr,index_present,index,bs_code_present,bs_code,alert_present,alert_pattern) NULL

#define l4c_cphs_display_als_lind(line) NULL

#define l4c_cc_auto_dtmf_start_lind(call_id,digits) NULL
#define l4c_cc_auto_dtmf_compl_lind(call_id) NULL

#if defined(__NONE_USED_L4MMI_MESSAGE__)
#define l4c_cc_auto_dtmf_compl_wind(call_id) NULL
#endif

#define l4c_ss_cb_passwd_lind(pwd_no) NULL
#define l4c_init_lmmi() NULL

//hogan
#define l4c_sms_cb_gs_change_lind(geography_type) NULL


#define l4c_sms_mem_full_lind() NULL
#define l4c_sms_mem_exceed_lind() NULL
#define l4c_sms_mem_available_lind() NULL
#define l4c_sms_msg_waiting_lind(line_no,waiting_num,ind_type,is_show_num,is_clear,is_from_storage,msp_no,ext_indicator,msg_waiting) NULL
#define l4c_sms_enhanced_voice_mail_lind(evm_pdu_type,multi_sub_profile,is_store,vm_almost_full,vm_full,vm_status_ext_flg,vm_access_addr,number_of_vm_unread,number_of_vm_notify,number_of_vm_delete,l4_status,vm_status_ext_len,vm_status_ext_data,vm_msg){}
#define l4c_sms_new_msg_index_lind(dest_port,src_port,indx,storage,oa_num,alpha_id,mti,scts,concat_info) NULL
//#define l4c_sms_new_msg_text_lind(index,dest_port,src_port,mti,display_type,concat_info,message_len,msg_header,data) NULL
#define l4c_sms_new_msg_text_lind(index,dest_port,src_port,mti,display_type,storage_type,concat_info,message_len,msg_header,data) NULL
#define l4c_sms_new_msg_pdu_lind(mti,length,id,data) NULL

#define l4c_sms_sync_msg_lind(action,index,status,del_bitmap) NULL

#define l4c_sms_send_abort_start_lind() NULL
             	

#define l4c_sms_send_abort_finish_lind(result,cause,source_id,seq_num) NULL

#define l4c_sms_startup_begin_lind() NULL
#define l4c_sms_startup_finish_lind() NULL
#define l4c_sms_startup_read_msg_lind(kal_uint8 startup_seq_num,kal_uint8 mti,kal_uint8 status,kal_uint8 storage_type,kal_uint16 index,kal_uint16 dest_port,kal_uint16 src_port,smsal_concat_struct *concat_info,kal_uint16 message_len,void *msg_header,void *data) NULL
#define l4c_mmi_usm_at_req_lind(atcmd_type,domain_type,memory_type,stat_type,atcmd_index,cmgw_message,cmgd_delflag,cnmi_parameter) NULL
#define l4c_sms_mt_sms_final_ack_lind(result,error_cause) NULL
#define l4c_sms_new_msg_pdu_mode_lind(id,mti,pdu_length,pdu) NULL
#define l4c_sms_cb_msg_pud_lind(msg_length,msg_data) NULL
#define l4c_sms_startup_read_lind(sim_status) NULL
#define l4c_sms_get_mem_status_lind(source_id) NULL
#define l4c_sms_atcmd_cmgr_lind(source_id,mem,index,esmss_mode) NULL
#define l4c_sms_atcmd_cmgl_lind(source_id,mem,stat,esmss_mode) NULL
#define l4c_sms_atcmd_cmgd_lind(source_id,mem,index,delflag) NULL
#define l4c_sms_atcmd_cmgw_lind(source_id,mem,stat,pdu_length,pdu) NULL
#define l4c_sms_atcmd_eqsi_lind(source_id,mem) NULL
#define l4c_sms_atcmd_cgsms_lind(bearer) NULL
#define l4c_sms_atcmd_csas_lind(profile) NULL

#define l4c_eq_usbdetect_lind(action) NULL



#define l4c_cb_msg_lind(local_ptr) NULL

#define l4c_sms_app_data_lind(dest_mod_id,dest_port,src_port,dcs,mti,message_len,concat_info,oa,scts,msg_data) NULL

#define l4c_sms_ready_lind(result) NULL

#if 0
#define l4c_cb_dl_lind() NULL
#endif

#ifdef __MOD_TCM__
//#define l4c_ps_activate_lind() NULL
#define l4c_ps_gprs_status_update_lind(status,cell_support_egprs,data_speed_support,data_bearer_capability) NULL//mtk02285, hspa_mmi_h2
#define l4a_ps_gprs_status_update_lind(status,cell_support_egprs,data_speed_support,data_bearer_capability) NULL//mtk02285, hspa_mmi_h2
#endif

#ifdef __SAT__
#define l4c_sat_dspl_text_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_get_inkey_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_get_input_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_setup_menu_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_select_item_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_play_tone_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_setup_call_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_send_sms_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_send_ss_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_send_ussd_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_mmi_info_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_setup_idle_dspl_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_run_at_command_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_send_dtmf_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_file_change_lind(is_full_changed,refresh_type,num_of_file,file_list) NULL

#define l4c_sat_event_list_lind(event_list,proactive_cmd) NULL

#define l4c_sat_provide_lang_info_lind() NULL

#define l4c_sat_lang_notify_lind(local_ptr) NULL

#ifdef __SATCE__
#define l4c_sat_open_channel_lind(local_ptr,peer_ptr) NULL




#define l4c_sat_close_channel_lind(local_ptr,peer_ptr) NULL


                          

#define l4c_sat_send_data_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_recv_data_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_notify_mmi_lind(state)NULL

#ifdef EXTERNAL_MMI

#define l4c_rsat_open_channel_lind(local_ptr, peer_ptr) NULL

#define l4c_rsat_close_channel_lind(local_ptr, peer_ptr) NULL

#define l4c_rsat_send_data_lind(local_ptr, peer_ptr) NULL

#define l4c_rsat_recv_data_lind(local_ptr, peer_ptr) NULL

#define l4c_rsat_ch_status_lind(local_ptr);

#endif


#endif 
#define l4c_sat_launch_browser_lind(local_ptr,peer_ptr) NULL

#define l4c_sat_no_other_cmd_lind() NULL

#endif 

#if defined(__MODEM_EM_MODE__) || defined(__SLIM_MODEM_EM_MODE__)
#define l4c_em_status_ind(mod_id,em_info,info) NULL
#endif

#define l4c_eq_power_reset_lind(poweroff,poweron,rtc_timer) NULL
#define l4a_mms_read_folder_status_wind(folderId,retrievalMode) NULL
#define l4a_mms_upload_msg_wind() NULL
#define l4a_mms_delete_msg_wind(msgId,folderId) NULL
#define l4a_mms_read_msg_path_wind(msgId) NULL

#define l4c_mmi_java_local_install_req_lind(is_force,jad_file_name,jar_file_name) NULL
#define l4c_mmi_factory_restore_req_lind(pwd) NULL
#define l4c_eq_ringtone_play_req_lind(mode,style,volume,out_path,sound_id) NULL                                        
#define l4c_em_nw_event_notify_lind(group_id, event_id) NULL
#define l4c_em_quick_test_report_lind(void) NULL

#ifdef __AGPS_CONTROL_PLANE__
#define l4c_ss_mtlr_begin_lind(notification, ss_id) NULL
#define l4c_ss_aerq_begin_lind(aerq, ss_id) NULL
#define l4c_ss_aecl_begin_lind(aecl, ss_id) NULL
#define l4c_agps_new_key_needed_lind() NULL
#define l4c_agps_reset_positioning_lind() NULL
#define l4c_agps_cp_start_lind() NULL
#define l4c_agps_cp_end_lind() NULL
#endif

#define l4c_eq_ram_usage_req_lind(type) NULL
#define l4c_eq_launch_app_req_lind(type, mids_idx, midlet_idx, mode, appName) NULL
#define l4c_eq_app_info_output_req_lind(type, op) NULL


#else
extern void l4c_cc_call_meter_lind(kal_uint8 call_id,
                                   kal_uint32 ccm, 
                                   kal_uint8 warning,
                                   kal_uint32 total_ccm,
                                   kal_uint8 puct_valid,
                                   kal_uint8 *currency,
                                   kal_uint8 *ppu);

extern void l4c_cc_call_ring_lind(kal_uint8 call_id,
                                  l4c_number_struct *num,
                                  l4c_sub_addr_struct *sub_addr,
                                  l4c_number_struct *redirect_num,                                  
                                  kal_uint8 name_present,
                                  kal_uint8 cnap_info_exist,
                                  kal_uint8 tag,                            
                                  kal_uint8 *name,
                                  kal_uint8 dcs,
                                  kal_uint8 length,
                                  kal_uint8 auto_answer,
                                  kal_uint8 call_type,
                                  kal_uint8 addr_ind_present,
                                  kal_uint8 addr_ind,
                                  kal_uint8 signal_value_present,
                                  kal_uint8 signal_value,
                                  kal_uint8 alerting_pattern_present,
                                  kal_uint8 alerting_pattern,
                                  kal_uint8 no_cli_cause_present,
                                  kal_uint8 no_cli_cause,
                                  kal_uint8 call_priority_present,
                                  kal_uint8 call_priority);

extern void l4c_cc_call_present_lind(kal_uint8 call_id,
                                  l4c_number_struct *num,
                                  l4c_sub_addr_struct *sub_addr,
                                  l4c_number_struct *redirect_num,                                  
                                  kal_uint8 name_present,
                                  kal_uint8 cnap_info_exist,
                                  kal_uint8 tag,                            
                                  kal_uint8 *name,
                                  kal_uint8 dcs,
                                  kal_uint8 length,
                                  kal_uint8 auto_answer,
                                  kal_uint8 call_type,
                                  kal_uint8 addr_ind_present,
                                  kal_uint8 addr_ind,
                                  kal_uint8 signal_value_present,
                                  kal_uint8 signal_value,
                                  kal_uint8 alerting_pattern_present,
                                  kal_uint8 alerting_pattern,
                                  kal_uint8 no_cli_cause_present,
                                  kal_uint8 no_cli_cause,
                                  kal_uint8 call_priority_present,
                                  kal_uint8 call_priority,
                                  kal_uint8 seq_num);

extern void l4c_cc_cpi_lind(kal_uint8 call_id,
					csmcc_cpi_msg_type_enum msg_type,
					kal_uint8 is_ibt,
					kal_uint8 is_tch,
					clcc_dir_enum dir,
					clcc_mode_enum call_mode,
					kal_uint8 numberP,
					l4c_number_struct *number,
					kal_uint16 disc_cause,
					kal_uint8 src_id,
                    kal_uint8 call_type);

extern void l4c_cc_data_call_req_lind(kal_uint8 source_id,
                                      kal_uint8 opcode,
                                      kal_uint8 call_type,
                                      l4c_number_struct number);

extern void l4c_cc_call_disc_lind(kal_uint8 call_id, 
                                  kal_bool ccbs, 
                                  kal_uint16 cause, 
                                  kal_uint8 call_type, 
                                  kal_bool is_diag_present, 
                                  kal_uint8 diag);

extern void l4c_ss_ussr_lind(kal_uint8 dcs, kal_uint8 *ussd_string,kal_uint8 length,kal_uint8 direction);

extern void l4c_ss_ussn_lind(kal_uint8 dcs, kal_uint8 *ussd_string,kal_uint8 length,kal_uint8 direction);

extern void l4c_cphs_display_cfu_lind(kal_uint8 action, kal_uint8 line_num); //mtk00468 030602 add for CPHS featuer to MMI

extern void l4c_nw_reg_state_lind (kal_uint8 *lac,
                                   kal_uint16 cell_id,
                                   kal_uint8 status,
                                   kal_uint8 gprs_state,
                                   l4c_gprs_status_enum gprs_status);

extern void l4c_nw_reg_cause_lind(kal_uint8 domain, kal_uint8 cause); // __NW_REG_CAUSE_REPORT, mtk02285, 20090310

extern void l4c_nw_attach_lind (kal_uint8 status,
                                kal_uint8 *plmn,
                                kal_uint8 gsm_state,
                                kal_uint8 gprs_state,
                                l4c_gprs_status_enum gprs_status,
                                kal_uint8 rat,
                                kal_uint8	cell_support_egprs,
                                kal_uint8 *lac,
                                kal_uint8 rac,
                                kal_uint16 cell_id,
                                kal_uint8 cause,
                                kal_uint8 data_speed_support,
                                kal_bool is_on_hplmn,
                                domain_id_enum domain);

extern void l4c_nw_rx_level_lind(kal_uint8 rat,
	                      kal_int32 rssi_in_qdbm,
	                      kal_int32 RSCP_in_qdbm,
	                      kal_int32 EcN0_in_qdbm,
	                      kal_uint8 ber,
	                      kal_uint8 current_band/*l4c_nw_band_enum*/); //rx_level, mtk02285

extern void l4c_nw_time_zone_lind( kal_uint8 full_nw_nameP,
						    kal_uint8 full_nw_name_dcs,
						    kal_bool full_nw_name_add_ci,
						    kal_uint8 full_nw_name_len,
						    kal_uint8 full_nw_name[24],
						    kal_uint8 short_nw_nameP,
						    kal_uint8 short_nw_name_dcs,
						    kal_bool short_nw_name_add_ci,
						    kal_uint8 short_nw_name_len,
						    kal_uint8 short_nw_name[24],
						    kal_uint8 nw_time_zoneP,
						    kal_uint8 nw_time_zone,
						    kal_uint8 nw_time_zone_timeP,
						    l4c_nw_time_zone_time_struct nw_time_zone_time,
						    kal_uint8 lsa_idP,
						    kal_uint8 lsa_id_len,
						    kal_uint8 lsa_id[3],
						    kal_uint8 nw_day_light_saving_timeP,
						    kal_uint8 nw_day_light_saving_time,
						    kal_uint8 *plmn);  

extern void l4c_nw_sel_mode_lind(kal_uint8 network_mode);

extern void l4c_nw_mmrr_service_status_lind(kal_uint8 service_status);

extern void l4c_nw_cfun_state_req_lind(kal_uint8 cfun_state);

extern void l4c_nw_update_ecc_lind(kal_uint8 no_ecc,l4c_rac_ecc_number_struct* ecc_list);

extern void l4c_nw_ef_rat_mode_change_lind(kal_uint8 is_valid_ef_rat_mode, kal_uint8 ef_rat_mode);

extern void l4c_phb_update_lind(kal_uint8 action,/*l4c_phb_update_enum*/
                                  kal_uint16 old_index,
                                  kal_uint16 sim_used,
                                  kal_uint16 nvram_used,
                                    kal_uint16 no_list,
                                    l4c_phb_entry_struct *list);

extern void l4c_phb_startup_read_lind(l4c_phb_entries_struct *phb_entries, kal_uint8 access_id);

extern void l4c_phb_startup_begin_lind(l4c_phb_desc_struct desc[PHB_TYPE_TOTAL]);  

extern void l4c_mmi_read_phb_req_lind(kal_uint8 src_id,
                                    kal_uint8 type, 
                                    kal_uint8 storage,
                                    kal_uint16 record_index,
                                    kal_uint8 num_index);  

extern void l4c_mmi_write_phb_req_lind(kal_uint8 src_id, 
                                    l4c_phb_entry_struct entry);

extern void l4c_mmi_delete_phb_req_lind(kal_uint8 src_id,                                    
                                    kal_uint8 type,
                                    kal_uint8 storage,
                                    kal_uint16 record_index,
                                    kal_uint8 num_index);        

extern void l4c_mmi_read_clog_req_lind(kal_uint8 src_id,
                                    kal_uint8 type, 
                                    kal_uint16 index);

extern void l4c_mmi_write_clog_req_lind(kal_uint8 src_id, 
                                    kal_uint8 type, 
                                    kal_uint16 index,
                                    l4c_phb_ln_entry_struct *entry);

extern void l4c_mmi_delete_clog_req_lind(kal_uint8 src_id,                                    
                                    kal_uint8 type,
                                    kal_uint16 index);

extern void l4c_mmi_read_additional_req_lind(kal_uint8 src_id,
                                    phb_type_enum type,
                                    phb_storage_enum storage,
                                    kal_uint8 occurrence,
                                    kal_uint16 record_index);

extern void l4c_mmi_write_additional_req_lind(kal_uint8 src_id,
                                    phb_type_enum type,
                                    phb_storage_enum storage,
                                    kal_uint8 occurrence,
                                    kal_uint16 record_index,
                                    l4c_phb_usim_entry *entry);

extern void l4c_mmi_delete_additional_req_lind(kal_uint8 src_id,
                                    phb_type_enum type,
                                    phb_storage_enum storage,
                                    kal_uint8 occurrence,
                                    kal_uint16 record_index);

extern void l4c_sim_passwd_required_lind(kal_uint8 type, kal_uint8 is_sim_change, sim_chv_info_struct chv_info, kal_uint8 cphs_retry_count);

extern void l4c_sim_startup_info_lind(kal_uint8 num, sim_ecc_entry_struct *ecc_entry,
                                      kal_uint8 *language,
                                      kal_uint8 alpha_set,
                                      kal_uint8 *language_ext,
                                      kal_uint8 alpha_set_ext,
                                      kal_uint8 sim_type);

extern void l4c_sim_cipher_lind(kal_bool cipher_ind, 
							kal_uint8 is_gsm_conn_exist,
							kal_uint8 gsm_cipher_cond, 
							kal_uint8 gprs_cipher_cond);

extern void l4c_sim_verify_pin_result_lind(kal_uint8 next_type,
									sim_chv_info_struct chv_info,
									sim_chv_status_struct chv_status);  // mtk02480 add chv_status

extern void l4c_sim_o2_prepaid_sim_lind(kal_bool is_o2_prepaid_sim);

extern void l4c_sim_support_plmn_list_lind(kal_uint16 length);

/* mtk00468 030610 add for CPHS MMI INFO message indication to MMI */
extern void l4c_cphs_mmi_info_lind(kal_uint8 is_spn_valid,     // SPN
                               kal_uint8 is_spn_RPLMN_required,
							   kal_uint8 *spn,
							   kal_uint8 is_opname_valid,  // OPN
							   kal_uint8 *opname,
							   kal_uint8 is_sopname_valid, // sOPN
							   kal_uint8 *sopname,
							   kal_uint8 no_msisdn,        // MSISDN
							   l4csmu_addr_struct msisdn[2],
							   kal_uint8 no_info_num,      // Information Numbers
							   l4csmu_info_num_struct *info_num,
							   kal_uint8 is_valid_csp,
							   kal_uint8 *csp,
							   kal_uint8 is_puct_valid,
							   kal_uint8 *ef_puct,
							   kal_uint8 is_autolock_enable,
							   kal_uint8 autolock_result,
							   kal_uint8 autolock_remain,
							   kal_bool is_valid_ef_acting_hplmn,
							   kal_uint8 *ef_acting_hplmn,
							   kal_uint8 imsi[17],  
							   kal_uint8 digits_of_mnc,  
							   kal_bool is_usim,
							   kal_bool is_valid_ef_ehplmn,
							   kal_uint8 num_of_ehplmn,
							   kal_uint8 ef_ehplmn[12]);

extern void l4c_eq_keypad_detect_lind(kal_uint8 func_type);

extern void l4c_eq_keypad_event_act_lind(kal_uint8 mode);
extern void l4c_eq_scrntch_event_act_lind(kal_uint8 tscrn);

extern void l4c_eq_at_quick_input_lind(kal_uint16 *string);

extern void l4c_eq_at_cbklt_query_req_lind (void);

extern void l4c_eq_power_on_lind(kal_uint8 poweron_mode,
                                 kal_uint8 battery_level,
                                 kal_uint8 build_label,
                                 rtc_format_struct *rtc_time,
				     kal_uint8 flightmode_state,
				     kal_uint8 dual_sim_mode_setting,
				     kal_uint8 dual_sim_uart_setting);

extern void l4c_eq_battery_status_lind(kal_uint8 battery_status,
                                kal_uint8 battery_voltage);

extern void l4c_eq_rtc_period_lind(kal_uint8 rtc_type, rtc_format_struct *rtc_time);

extern void l4c_eq_alarm_detect_lind(rtc_format_struct *rtc_time);

extern void l4c_em_lcm_test_lind(kal_uint8 color);

extern void l4c_em_keypad_event_act_lind(kal_bool on_off);

extern void l4c_em_lcm_sleep_mode_req_lind(kal_bool on_off);

extern void l4c_factory_test_lind(kal_bool on_off);

extern void l4c_set_mmi_default_prof_lind(kal_uint8 op,
									kal_uint8 category,
									kal_uint8 param1,
									kal_uint8 param2,
									kal_uint8 param3,
									kal_uint8 *param4);

extern void l4c_eq_audio_play_finish_lind(kal_uint8 result, kal_uint16  identifier);


extern void l4c_get_adc_all_channel_lind(kal_int32 vbat,
                                      kal_int32	bat_temp,
                                      kal_int32	vaux,
                                      kal_int32	charge_current,
                                      kal_int32	vcharge);

extern void l4c_eq_gpio_detect_lind(kal_uint8 gpio_device,
                             kal_bool on_off);
#ifdef __IRDA_SUPPORT__
extern void l4c_eq_ircomm_disconnect_lind(void);
extern void l4c_eq_ircomm_connect_lind(void);
#endif /* __IRDA_SUPPORT__ */

extern void l4c_eq_set_volume_req_ind (kal_uint8 volume_type,
                                             kal_uint8 volume_level);

extern void l4c_eq_set_mute_mode_req_ind (kal_uint8 audio_device,
                                    kal_bool mute_onoff);

extern void l4c_eq_set_silent_mode_req_ind (kal_bool mode);

extern void l4c_eq_set_gpio_level_req_ind (kal_uint8 gpio_dev_type,
                                           kal_uint8 gpio_dev_level,
                                           kal_uint16 duration);

extern void l4c_eq_set_keypad_input_req_ind(kal_uint8 keypad_status,
                                        kal_uint8 keycode);

extern void l4c_eq_set_dspl_text_req_ind (kal_uint8 *text_string);

extern void l4c_eq_set_indicator_req_ind(kal_uint8 icon_id,
                                              kal_uint8 value);

extern void l4c_eq_set_rtc_time_req_ind (kal_uint8 rtc_type,
                                         kal_uint8 set_type,
                                         rtc_alarm_info_struct alarm);

#if defined(__NONE_USED_L4MMI_MESSAGE__)
extern void l4c_eq_exe_rtc_delete_req_ind (kal_uint8 rtc_type,
                                      kal_uint8 rtc_index);
#endif

extern void l4c_eq_set_date_time_format_req_ind (kal_bool is_date_set,
                                                 kal_uint8 mode);

extern void l4c_eq_set_language_req_ind (kal_uint8 lang_code);

extern void l4c_eq_set_greeting_text_req_ind (kal_bool mode,
                                   kal_uint8 dcs,
                                   kal_uint8 *text,
                                   kal_uint8 length);

extern void l4c_eq_set_vibrator_mode_req_ind(kal_bool mode);

extern void l4c_eq_at_alarm_set_req_lind (mmi_at_alarm_info_struct alarm);
extern void l4c_eq_at_alarm_delete_req_lind (kal_uint8 alarm_index);
extern void l4c_eq_at_alarm_query_req_lind (void);

extern void l4c_eq_query_vibrator_mode_req_lind(void);
extern void l4c_eq_query_silent_mode_req_lind(void);
extern void l4c_eq_query_mute_mode_req_lind(void);
extern void l4c_eq_query_greeting_text_req_lind(void);
extern void l4c_eq_query_language_req_lind(void);
extern void l4c_eq_query_datetime_format_req_lind(kal_bool  is_date);
extern void l4c_eq_screen_touch_req_lind(kal_uint8 action, kal_uint16 x_coords, kal_uint16 y_coords);
extern void l4c_eq_screen_shot_req_lind(kal_uint8 op,
                                                kal_uint16 x_coordinate,
                                                kal_uint16 y_coordinate,
                                                kal_uint16 width,
                                                kal_uint16 height);
extern void l4c_eq_scrnorient_req_lind(kal_uint8 mode, kal_uint8 orient);
extern void l4c_eq_scrnsize_query_req_lind(void);

extern void l4c_eq_set_hand_free_mode_req_lind(kal_bool  is_hand_free); //mtk02088_factory
extern void l4c_eq_query_hand_free_mode_req_lind(void);
extern void l4c_eq_set_cam_req_lind(kal_uint8  mode); 
extern void l4c_eq_set_avr_req_lind(kal_uint8  mode); 

#ifdef VM_SUPPORT
#ifdef __FS_ON__
extern void l4c_eq_vm_record_finish_lind (kal_uint8 result,
                                   kal_wchar *file_name);
#else
extern void l4c_eq_vm_record_finish_lind (kal_uint8 result,
                                          kal_uint8 file_id);
#endif

extern void l4c_eq_vm_play_finish_lind (kal_uint8 result);
#endif


extern void l4c_eq_leave_precharge_lind(void);

extern void l4c_eq_fm_test_req_lind(kal_uint8 opcode,kal_uint16 frequency);

extern void l4c_eq_factory_reset_lind(kal_uint8 request_type);


extern void l4c_eq_uart_transfer_lind(kal_uint8 port); //ws_pcsync

/* below not ready yet */
extern void l4c_cc_call_setup_req_ind (l4c_number_struct dest,
                                kal_uint8 call_type,
                                kal_uint8 clir_flag,
                                kal_bool cug_option);
extern void l4c_cc_call_accept_req_ind (kal_uint8 call_id);

extern void l4c_cc_call_abort_req_ind (void);

extern void l4c_cc_call_sync_req_ind (void);

extern void l4c_cc_video_call_status_ind(kal_uint8 call_id,
									kal_uint8 action,
									kal_bool result,
									kal_uint8 cause);

extern void l4c_nvm_ready_lind(void);
extern void l4c_sim_fail_lind (kal_uint16 cause);
extern void l4c_smu_sml_status_notify_lind(void);
extern void l4c_phb_startup_finish_lind(kal_uint16 sim_max_num,
                                   kal_uint16 phb_max_num,
                                   kal_uint8 phb_len,
                                   kal_uint8 fdn_len,
                                   kal_uint8 bdn_len,
                                   kal_uint8 owner_len, 
                                   l4c_phb_desc_struct desc[PHB_TYPE_TOTAL]);

extern void l4c_phb_ln_ready_lind(void);

extern void l4c_abm_ready_lind(void);

extern void l4c_cc_name_lind (void);

extern void l4c_cc_notify_ss_lind(kal_uint8 call_id, 
								kal_uint8 type,
								kal_bool status_present,
								kal_uint8 status,
								kal_bool notification_present,
								kal_uint8 notification);
//MAUI_01998659 extern void l4c_cc_notify_ss_cug_lind(kal_uint8 call_id, kal_uint16 index);
extern void l4c_cc_notify_ss_ect_lind(kal_uint8 call_id,
                                      kal_bool present,
                                      l4c_number_struct *addr,
                                      l4c_sub_addr_struct *sub_addr);
extern void l4c_cc_notify_ss_cnap_lind(kal_uint8 call_id,
                                       kal_uint8 name_present,
                                       kal_uint8 cnap_info_exist,
                                       kal_uint8 tag,
                                       kal_uint8 *name,
                                       kal_uint8 dcs,
                                       kal_uint8 length);
extern void l4c_cc_notify_ss_ccbs_lind(kal_uint8 call_id,
                                       kal_bool number_present,
                                       l4c_number_struct *addr,
                                       kal_bool sub_addr_present,
                                       l4c_sub_addr_struct *sub_addr,
                                       kal_bool index_present,
                                       kal_uint8 index,
                                       kal_bool bs_code_present,
                                       kal_uint8 bs_code,
                                       kal_bool alert_present,
                                       kal_uint8 alert_pattern);

extern void l4c_cphs_display_als_lind(kal_uint8 line);/*csmcc_als_line_id_enum*/

extern void l4c_cc_auto_dtmf_start_lind (kal_uint8 call_id,
                                         kal_uint8 *digits);
extern void l4c_cc_auto_dtmf_compl_lind (kal_uint8 call_id);

#if defined(__NONE_USED_L4MMI_MESSAGE__)
extern void l4c_cc_auto_dtmf_compl_wind (kal_uint8 call_id);
#endif

extern void l4c_ss_cb_passwd_lind(kal_uint8 pwd_no);
extern void l4c_init_lmmi(void);
//hogan
extern void l4c_sms_cb_gs_change_lind(kal_uint8 geography_type);

extern void l4c_sms_mem_full_lind(kal_uint8 type);
extern void l4c_sms_mem_exceed_lind(kal_uint8 type);
extern void l4c_sms_mem_available_lind(kal_uint8 type);

extern void l4c_sms_msg_waiting_lind(kal_uint8 line_no, 
								kal_uint8 waiting_num,
								kal_uint8 ind_type,
								kal_bool is_show_num,
								kal_bool is_clear,
								kal_bool is_from_storage,
								kal_uint8 msp_no, 
								kal_uint8 ext_indicator,
								l4csmsal_msg_waiting_ind_ext_struct *msg_waiting);
extern void l4c_sms_enhanced_voice_mail_lind(kal_uint8 evm_pdu_type, 
								kal_uint8 multi_sub_profile,
								kal_uint8 is_store,
								kal_uint8 vm_almost_full,
								kal_uint8 vm_full,
								kal_uint8 vm_status_ext_flg,
								l4_addr_bcd_struct vm_access_addr,
								kal_uint8 number_of_vm_unread,
								kal_uint8 number_of_vm_notify,
								kal_uint8 number_of_vm_delete,
								kal_uint8 l4_status,
								kal_uint8 vm_status_ext_len,
								kal_uint8 *vm_status_ext_data,
								smsal_evmi_msg_struct *vm_msg[32]);

extern void l4c_sms_new_msg_index_lind(kal_uint16 dest_port,
						kal_uint16 src_port,
						kal_uint16 indx,
                                       kal_uint8 storage,
                                       l4c_number_struct *oa_num,
                                       l4_name_struct *alpha_id,
                                       kal_uint8 mti,
                                       kal_uint8 *scts,
                                       void *concat_info);
//extern void l4c_sms_new_msg_text_lind(kal_uint8 mti,
//                                       kal_uint8 id,
//                                       l4_name_struct *alpha_id,
 //                                      void *data);
//extern void l4c_sms_new_msg_text_lind(kal_uint16 index,
//								kal_uint16 dest_port,
//								kal_uint16 src_port,
//      								kal_uint8 mti,
//      								kal_uint8 display_type,
//      								kal_uint32 *concat_info,
//      								kal_uint16 message_len,   
//      								void *msg_header,
//                                       void *data);

extern void l4c_sms_new_msg_text_lind(kal_uint16 index,
								kal_uint16 dest_port,
								kal_uint16 src_port,
      								kal_uint8 mti,
      								kal_uint8 display_type,
      								kal_uint8 storage_type,
      								smsal_concat_struct *concat_info,
      								kal_uint16 message_len,
      								void *msg_header,
			                                   void *data);
			                                   
extern void l4c_sms_new_msg_pdu_lind(kal_uint8 mti,
                                     kal_uint16 length,
                                     kal_uint8 id,
                                       void *data);

extern void l4c_sms_sync_msg_lind(kal_uint8 action,
								kal_uint16 index,
								kal_uint8 status,
								kal_uint8 *del_bitmap); 

extern void l4c_sms_send_abort_start_lind(void);//mtk00468 add for SMS abort tell MMI when MT coming
             	

extern void l4c_sms_send_abort_finish_lind(kal_bool result, kal_uint16 cause,kal_uint8 source_id,kal_uint8 seq_num);

extern void l4c_sms_startup_begin_lind(void);
extern void l4c_sms_startup_finish_lind(void);
extern void l4c_sms_startup_read_msg_lind(kal_uint8 startup_seq_num,
						kal_uint8 mti,
						kal_uint8 status,
						kal_uint8 storage_type,
						kal_uint16 index,
						kal_uint16 dest_port,
						kal_uint16 src_port,
						smsal_concat_struct *concat_info,
						kal_uint8 *submit_scts, /* Johnny: only for SMSAL_MTI_SUBMIT */
						kal_uint16 message_len,
						void *msg_header,
                                           	void *data);
extern void l4c_mmi_usm_at_req_lind(kal_uint8 atcmd_type,
								kal_uint8 domain_type,
								kal_uint8 memory_type,
								kal_uint8 stat_type,
								kal_uint32 atcmd_index,
								l4c_usm_cmgw_struct *cmgw_message,
								kal_uint32 cmgd_delflag,
								l4c_usm_cnmi_struct *cnmi_parameter);
extern void l4c_sms_mt_sms_final_ack_lind(kal_uint8  result,
								kal_uint32  error_cause);
extern void l4c_sms_new_msg_pdu_mode_lind(kal_uint8 id, 
								kal_uint8 mti, 
								kal_uint16 pdu_length, 
								kal_uint8 *pdu);
extern void l4c_sms_cb_msg_pud_lind(kal_uint16 msg_length, 
								kal_uint8 *msg_data);
			                                           				                                           	
extern void l4c_sms_startup_read_lind(kal_uint8 sim_status);
extern void l4c_sms_get_mem_status_lind(kal_uint8 source_id);
extern void l4c_sms_atcmd_cmgr_lind(kal_uint8 source_id,kal_uint8 mem,kal_uint32 index,kal_uint8 esmss_mode);
extern void l4c_sms_atcmd_cmgl_lind(kal_uint8 source_id,kal_uint8 mem,kal_uint8 stat,kal_uint8 esmss_mode);
extern void l4c_sms_atcmd_cmgd_lind(kal_uint8 source_id,kal_uint8 mem,kal_uint16 index,kal_uint8 delflag);
extern void l4c_sms_atcmd_cmgw_lind(kal_uint8 source_id,kal_uint8 mem,kal_uint8 stat,kal_uint16 pdu_length,kal_uint8 *pdu);
extern void l4c_sms_atcmd_eqsi_lind(kal_uint8 source_id,kal_uint8 mem);
extern void l4c_sms_atcmd_cgsms_lind(kal_uint8 bearer);
extern void l4c_sms_atcmd_csas_lind(kal_uint8 profile);

extern void l4c_eq_usbdetect_lind(kal_uint8  action);

extern void l4c_sms_etws_informaion_lind(kal_uint16 warning_type,
                                kal_uint16 message_id,
                                kal_uint16 serial_nubmer,
                                kal_bool is_security_information_valid,
                                kal_uint8 etws_security_information[L4C_MAX_ETWS_SECURITY_INFO_LEN],
                                kal_uint8 plmn_id[6]);


extern void l4c_cb_msg_lind(l4csmsal_cb_msg_text_ind_struct *local_ptr);

extern void l4c_sms_app_data_lind(kal_uint16 dest_mod_id,
						   kal_uint16 dest_port,
						   kal_uint16 src_port,
						   kal_uint8  dcs,
						   kal_uint8 mti,/* Message Type Identifier*/
						   kal_uint16 message_len, /* Length of Message*/
   						   void *concat_info, /* Concat. Info*/  
    					   l4c_number_struct *oa, /* Originating Address */
    					   kal_uint8 *scts,
						   kal_uint8 *msg_data);

extern void l4c_sms_ready_lind(kal_bool result);

#if 0
extern void l4c_cb_dl_lind (void);
#endif

#ifdef __MOD_TCM__
//extern void l4c_ps_activate_lind (void);
extern void l4a_ps_gprs_status_update_lind(l4c_gprs_status_enum status, kal_uint8 cell_support_egprs, kal_uint8 data_speed_support, kal_uint8 data_bearer_capability);//mtk02285, hspa_mmi_h2
extern void l4c_ps_gprs_status_update_lind(l4c_gprs_status_enum status, kal_uint8 cell_support_egprs, kal_uint8 data_speed_support, kal_uint8 data_bearer_capability);//mtk02285, hspa_mmi_h2
#endif

#ifdef __SAT__
extern void l4c_sat_dspl_text_lind (sat_dspl_text_ind_struct *local_ptr,
                                    sat_dspl_text_ind_peer_struct *peer_ptr);

extern void l4c_sat_get_inkey_lind (sat_get_inkey_ind_struct *local_ptr,
                                    sat_get_inkey_ind_peer_struct *peer_ptr);

extern void l4c_sat_get_input_lind (sat_get_input_ind_struct *local_ptr,
                                    sat_get_input_ind_peer_struct *peer_ptr);

extern void l4c_sat_setup_menu_lind (sat_setup_menu_ind_struct *local_ptr,
                                     sat_setup_menu_ind_peer_struct *peer_ptr);

extern void l4c_sat_select_item_lind (sat_select_item_ind_struct *local_ptr,
                                      sat_select_item_ind_peer_struct *peer_ptr);

extern void l4c_sat_play_tone_lind (sat_play_tone_ind_struct *local_ptr,
                                    sat_play_tone_ind_peer_struct *peer_ptr);

extern void l4c_sat_setup_call_lind (sat_setup_call_ind_struct *local_ptr,
                                     sat_setup_call_ind_peer_struct *peer_ptr);

extern void l4c_sat_send_sms_lind (sat_send_sms_ind_struct *local_ptr,
                                   sat_send_sms_ind_peer_struct *peer_ptr);

extern void l4c_sat_send_ss_lind (sat_send_ss_ind_struct *local_ptr,
                                  sat_send_ss_ind_peer_struct *peer_ptr);

extern void l4c_sat_send_ussd_lind (sat_send_ussd_ind_struct *local_ptr,
                                    sat_send_ussd_ind_peer_struct *peer_ptr);

extern void l4c_sat_mmi_info_lind (sat_mmi_info_ind_struct *local_ptr,
                                   sat_mmi_info_ind_peer_struct *peer_ptr);

extern void l4c_sat_setup_idle_dspl_lind (sat_setup_idle_dspl_ind_struct *local_ptr,
                                          sat_setup_idle_dspl_ind_peer_struct *peer_ptr); //030602 mtk00468 add for Paul SAT

extern void l4c_sat_run_at_command_lind (sat_run_at_command_ind_struct *local_ptr,
                                         sat_run_at_command_ind_peer_struct *peer_ptr); //030602 mtk00468 add for Paul SAT

extern void l4c_sat_send_dtmf_lind (sat_send_dtmf_ind_struct *local_ptr,
                                    sat_send_dtmf_ind_peer_struct *peer_ptr); //030602 mtk00468 add for Paul SAT

extern void l4c_sat_file_change_lind(kal_uint8 is_full_changed,
                           kal_uint8 refresh_type,
									kal_uint8 num_of_file,
									kal_uint16 *file_list);

extern void l4c_sat_event_list_lind(kal_uint8 *event_list, sat_procomm_struct proactive_cmd);  // mtk02480 add for Nick SAT

extern void l4c_sat_provide_lang_info_lind(void);

extern void l4c_sat_lang_notify_lind (sat_lang_notify_ind_struct *local_ptr); //030602 mtk00468 add for Paul SAT

#ifdef __SATCE__
extern void l4c_sat_open_channel_lind(l4csmu_sat_open_channel_ind_struct *local_ptr,
                            l4csmu_sat_open_channel_ind_peer_struct *peer_ptr);//030405 mtk00468 add for benson SAT


//extern void l4c_sat_set_gprs_data_account_lind(l4csat_set_gprs_data_account_ind_struct *local_ptr);//030405 mtk00468 add for benson SAT

//extern void l4c_sat_csm_open_csd_lind(l4csat_csm_open_csd_ind_struct *local_ptr);//040405 mtk00468 add for benson SAT

extern void l4c_sat_close_channel_lind(l4csmu_sat_close_channel_ind_struct *local_ptr,
                            l4csmu_sat_close_channel_ind_peer_struct *peer_ptr);//030405 mtk00468 add for benson SAT

//extern void l4c_sat_cc_call_disc_lind(l4csat_cc_call_disc_ind_struct *local_ptr);
                          
extern void l4c_sat_send_data_lind(l4csmu_sat_send_data_ind_struct *local_ptr,
                            l4csmu_sat_send_data_ind_peer_struct *peer_ptr);//030405 mtk00468 add for benson SAT

extern void l4c_sat_recv_data_lind(l4csmu_sat_recv_data_ind_struct *local_ptr,
                            l4csmu_sat_recv_data_ind_peer_struct *peer_ptr);//030405 mtk00468 add for benson SAT

extern void l4c_sat_notify_mmi_lind(kal_uint8 state);


#ifdef EXTERNAL_MMI

extern void l4c_rsat_open_channel_lind(sat_open_channel_ind_struct *local_ptr, sat_open_channel_ind_peer_struct *peer_ptr);

extern void l4c_rsat_close_channel_lind(sat_close_channel_ind_struct *local_ptr, sat_close_channel_ind_peer_struct *peer_ptr);

extern void l4c_rsat_send_data_lind(sat_send_data_ind_struct *local_ptr, sat_send_data_ind_peer_struct *peer_ptr);

extern void l4c_rsat_recv_data_lind(sat_recv_data_ind_struct *local_ptr, sat_recv_data_ind_peer_struct *peer_ptr);

extern void l4c_rsat_ch_status_lind(sat_ch_status_ind_struct *local_ptr);

#endif

#endif /* __SATCE__ */
extern void l4c_sat_launch_browser_lind (sat_launch_browser_ind_struct *local_ptr,
                                         sat_launch_browser_ind_peer_struct *peer_ptr); //030602 mtk00468 add for Paul SAT

extern void l4c_sat_no_other_cmd_lind (void);

#endif /* __SAT__ */

#if defined(__MODEM_EM_MODE__) || defined(__SLIM_MODEM_EM_MODE__)
extern void l4c_em_status_ind(kal_uint8 mod_id,
                              kal_uint32  em_info,
                              peer_buff_struct *info);
#endif

extern void l4c_eq_power_reset_lind(kal_uint8 poweroff, kal_uint8 poweron, kal_uint8 rtc_timer);
extern void l4a_mms_read_folder_status_wind(kal_uint8 folderId, kal_uint8 retrievalMode);
extern void l4a_mms_upload_msg_wind(kal_uint8 * filePath);
extern void l4a_mms_delete_msg_wind(kal_uint32 msgId, kal_uint8 folderId);
extern void l4a_mms_read_msg_path_wind(kal_uint32 msgId);
extern void l4c_mmi_java_local_install_req_lind(kal_bool is_force, kal_wchar* jad_file_name, kal_wchar* jar_file_name);
extern void l4c_mmi_factory_restore_req_lind(kal_uint8* pwd);
extern void l4c_eq_ringtone_play_req_lind(kal_uint8 mode,
                                          kal_uint8 style,
                                          kal_uint8 volume,
                                          kal_uint8 out_path,
                                          kal_uint16 sound_id);
extern void l4c_eq_vcard_req_lind(kal_uint8 src_id, kal_uint8 mode, kal_uint16 index, kal_uint8 *file_path);
extern void l4c_eq_vcalendar_req_lind(kal_uint8 mode, kal_uint16 vcal_type, kal_uint16 index, kal_uint8 *file_path);
extern void l4c_eq_str_enable_req_lind(kal_bool enable);
extern void l4c_eq_str_req_lind(kal_uint16 length, kal_uint8 *data_string);
extern void l4c_em_rgb_test_req_lind(kal_uint8 type, kal_uint16 red, kal_uint16 green, kal_uint16 blue);
extern void l4c_em_lcd_test_req_lind(kal_uint8 lcd_test_mode);
extern void l4c_em_nw_event_notify_lind(kal_uint8 group_id, kal_uint32 event_id);
extern void l4c_em_quick_test_report_lind(void);

#ifdef __HOMEZONE_SUPPORT__
extern void l4c_hz_tag_lind(kal_bool action, kal_bool is_hz, kal_uint8 tag[12]);
#endif /* __HOMEZONE_SUPPORT__ */

extern void l4c_eq_ram_usage_req_lind(kal_uint8 type);
extern void l4c_eq_launch_app_req_lind(kal_uint8 type, kal_uint8 mids_idx, kal_uint16 midlet_idx, kal_uint8 mode, kal_uint16 appName[260]);
extern void l4c_eq_app_info_output_req_lind(kal_uint8 type, kal_uint8 op);

#endif //__EMPTY_CALLBACK__


extern void l4c_cc_call_alert_lind(kal_uint8 alert);

extern void l4c_cc_call_wait_lind(kal_uint8 call_id,
                                  l4c_number_struct *num,
                                  kal_uint8 call_mode,
                                  kal_uint8  name_present,                                 
                                  kal_uint8 cnap_info_exist,
                                  kal_uint8 tag,
                                  kal_uint8 *name,
                                  kal_uint8 dcs,
                                  kal_uint8 length,
                                  kal_uint8 auto_answer,                                  
				  kal_uint8 addr_ind_present,
                                  kal_uint8 addr_ind,
                                  kal_uint8 signal_value_present,
                                  kal_uint8 signal_value,
                                  kal_uint8 alerting_pattern_present,
                                  kal_uint8 alerting_pattern,
                                  kal_uint8 no_cli_cause_present,
                                  kal_uint8 no_cli_cause,
                                  kal_uint8 call_priority_present,
                                  kal_uint8 call_priority);


extern void l4c_cc_call_chld_lind(kal_uint8 call_id, kal_uint8 opcode);

extern void l4c_cc_call_release_lind(kal_uint8 call_id, kal_uint16 cause, kal_bool is_diag_present, kal_uint8 diag);

extern void l4c_cc_call_connect_lind(l4c_number_struct *num,
                                     l4c_sub_addr_struct *sub_addr,
                                     kal_uint8 call_type,
                                     kal_uint8 call_id);

extern void l4c_cc_speech_lind (kal_bool on_off, kal_uint8 rat,kal_uint8 irho_speech_on_off);

#if 0 //__CTM_SUPPORT__
extern void l4c_ctm_connected_lind(void);
extern void l4c_ctm_recv_text_lind(kal_uint16 num_of_char, kal_uint16 *text);
extern void l4c_ctm_tty_plug_lind(kal_bool is_plug_in);
extern void l4c_ctm_default_changed_lind(kal_bool result, kal_bool is_turn_on, kal_uint8 ctm_interface);
#endif

//#ifdef __VOIP__
extern void l4c_mmi_voip_at_call_ctrl_approve_lind(kal_uint8 action, 
										void* string_ptr,
										void* node_ptr, 
										void* err_id);
//#endif
extern void l4c_bt_loopback_test_req_lind(kal_uint8* bd_addr, kal_uint8* pin_code );
extern void l4c_bt_set_vr_req_ind(kal_bool on_off);

extern void l4c_mmi_ucm_at_req_lind(kal_uint8 action,kal_uint8 source_id,kal_bool cmd_from_bt,
    									kal_uint8 length,kal_uint8 number[50],kal_uint8 opcode,kal_uint8 call_id);
extern void l4c_eq_pwnon_dual_sim_lind(void);
extern void l4c_sim_status_update_lind(kal_uint8 sim_inserted_status, kal_uint8 dual_sim_mode_setting);
extern void l4c_nw_rac_activated_lind(void);
extern void l4c_nw_search_normal_finish_lind(void);

#ifdef __AGPS_CONTROL_PLANE__
#if defined(__ANDROID_RIL_SUPPORT__) //agps_ipc
#define l4c_ss_mtlr_begin_lind(notification, ss_id) \
        l4c_ss_mtlr_begin_lind_ipc(notification, ss_id)
#define l4c_ss_aerq_begin_lind(aerq, ss_id) \
        l4c_ss_aerq_begin_lind_ipc(aerq, ss_id)
#define l4c_ss_aecl_begin_lind(aecl, ss_id) \
        l4c_ss_aecl_begin_lind_ipc(aecl, ss_id)
#define l4c_agps_new_key_needed_lind() \
        l4c_agps_new_key_needed_lind_ipc()
#define l4c_agps_reset_positioning_lind() \
        l4c_agps_reset_positioning_lind_ipc()
#define l4c_agps_cp_start_lind() \
        l4c_agps_cp_start_lind_ipc()
#define l4c_agps_cp_end_lind() \
        l4c_agps_cp_end_lind_ipc()
#else
extern void l4c_ss_mtlr_begin_lind(L4C_SS_LocationNotificationArg *notification, kal_uint8 ss_id);
extern void l4c_ss_aerq_begin_lind(L4C_SS_LCS_AreaEventRequestArg *aerq, kal_uint8 ss_id);
extern void l4c_ss_aecl_begin_lind(L4C_SS_LCS_AreaEventCancellationArg *aecl, kal_uint8 ss_id);
extern void l4c_agps_new_key_needed_lind(void);
extern void l4c_agps_reset_positioning_lind(void);
extern void l4c_agps_cp_start_lind(void);
extern void l4c_agps_cp_end_lind(void);
#endif /* defined(__ANDROID_RIL_SUPPORT__) */
#endif

#endif

#endif /* lmmi_ind.h */


