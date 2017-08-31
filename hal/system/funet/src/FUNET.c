#if defined(__FUNET_ENABLE__) && !defined(__UBL__)

/* include files */
#include "FUNET.h"
#include "reg_base.h"
#include "bl_mtk_bb_reg.h"

#ifndef ASSERT
#define ASSERT(x) { if (!(x))  while (1); }
#endif

void SetRetnFlag(kal_uint32 flag)
{
   *RETN_FLAG_SET = (0x4e000000 | flag);
}

void ClrRetnFlag(kal_uint32 flag)
{
   *RETN_FLAG_CLR = (0x4e000000 | flag);   
}

FUNET_ERROR_CODE FUNET_TriggerDL(WCHAR * filename, FUNET_UPDATE_TYPE update_type)
{
	FUNET_ERROR_CODE status = FUNET_ERROR_NONE;
	BL_FUNET_ERROR_CODE cd_error_code = BL_FUNET_ERROR_NONE;
	
	if(update_type != FUNET_UPDATE_FULL)
	{
		//BL_PRINT(LOG_DEBUG, "FUNET only supports full update now\n\r");
		ASSERT(0);
	}
	
	if(update_type == FUNET_UPDATE_FULL)
	{
		SetRetnFlag(RETN_FUNET_FULL_DL_EN);
	}
	else if(update_type == FUNET_UPDATE_PARTIAL)
	{
		SetRetnFlag(RETN_FUNET_PARTIAL_DL_EN);
	}
	
  // remap cd error code to FUNET_ERROR_CODE
	status = (BL_FUNET_ERROR_CODE) cd_error_code;
	
	return status;
}


#endif /* __FUNET_ENABLE__ && !__UBL__ */

