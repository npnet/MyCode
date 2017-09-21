#include <stdlib.h>

#include "rs_mem.h"
#include "rs_debug.h"


// MTK header
#include "mmi_frm_mem_gprot.h"
#include "MMI_features.h"
#include "MMIDataType.h"
#include "syscomp_config.h"
#include "task_config.h"
#include "kal_public_api.h"
#include "TimerEvents.h"
#include "Mmiapi_struct.h" //for message
#include "mmi_msg_struct.h"
#include "Mmi_frm_queue_gprot.h"
#include "Mmi_frm_events_gprot.h"
#include "Mmi_cb_mgr_gprot.h"


void* rs_malloc_porting(rs_u32 allocSize)
{
	void *returnPointer;


	returnPointer = mmi_malloc(allocSize);

	return(returnPointer);
}

void  rs_free_porting(void* memBlock)
{


	mmi_mfree(memBlock);
}

