#ifdef HD8020_GPS_SUPPORT
#include 	"stack_common.h"  
#include 	"stack_msgs.h"
#include 	"app_ltlcom.h"       /* Task message communiction */
#include 	"syscomp_config.h"
#include 	"task_config.h"
#include 	"drv_comm.h"
//#include    "adc.h"
#include    "drvsignals.h"
#include    "eint.h"
#include     "l1audio.h"
#include     "intrctrl.h"

#include "kal_general_types.h"
#include "stack_config.h"
#include "task_main_func.h"
#include "kal_public_api.h"
#include "stack_ltlcom.h"
#include "hisr_config.h"

#include    "dcl.h"

#include "mmi_features.h"
#include "TimerEvents.h"
#include "device.h"
#include "app_datetime.h"
#include "rj_dw02.h"

#ifdef __BIRD_GPS_APP__
#include "bird_gps_framework.h"   
#include "bird_gps_app.h"   
#endif

#ifdef RJ_GPS_APP
#include "bird_app.h"
#include "bird_task.h"
#endif
#include    "dcl_adc.h"

#include "nmeap.h"    // add by lqy
//#include "mdi_gps.h" 
#include "bird_gpscell.h" // add by lqy

#define GPIO_URXD3_NUM 0
#define GPIO_UTXD3_NUM 1
//#define uart_test


kal_uint8  gps_buf[25]={0};
kal_uint8 gps_cache_buf[50] ={0};


extern void RJ_GPS_Callback(mdi_gps_parser_info_enum type, void  *buffer, U32 length); //add by lqy





//add by lqy for BD
static nmeap_context_t nmea;	   /* parser context */
static nmeap_txt_t     txt;
static nmeap_gga_t     gga;		   /* this is where the data from GGA messages will show up */
static nmeap_rmc_t     rmc;		   /* this is where the data from RMC messages will show up */
static nmeap_gpgsv_t   gpgsv;     /* this is where the data from GPGSV messages will show up */
static nmeap_bdgsv_t   bdgsv;	  /* this is where the data from BDGSV messages will show up */
static nmeap_vtg_t     vtg;	  /* this is where the data from VTG messages will show up */
static int             user_data; /* user can pass in anything. typically it will be a pointer to some user data */
static bool bnmea = FALSE;  // for init once
static kal_uint8 g_gsv_num=0;  // for get gsv num for  gsv RJ_GPS_Callback commit
static kal_uint8 g_gsv_cyclenum=0;  // for bd and gsv cycle num total 8
static mdi_gps_nmea_gsv_struct gsv_data; // for gsv RJ_GPS_Callback commit
static kal_uint8 bd_sate_id[32]={0};              /*BDsatellite id*/
static kal_uint8 bd_snr[32]={0};                  /*BD SNR in dB*/

#ifdef BD_HDDZ
    static kal_uint8 g_gpgsv_row=0;  // for get gsv row num for  BD BJHK 
    static kal_uint8 g_bdgsv_row=0;  // for get gsv row num for  BD BJHK 
#endif 

//add by lqy for BD

/** simulate block IO */
/*
int readbuffer(char *buffer,int len)
{
    int i;
  
    char *pvec = in_buffer;	
    if (*pvec == 0) {
        // end of file
        return -1;
    }
    
    for(i=0;i<len;i++) {
        // quit when no more data 
        if (*pvec == 0) {
            break;
        }
        buffer[i] = *pvec++;
    }
    return i;
}*/

//add by lqy for BD
/*   functiong quik sort   by lqy modify 2014-12-12
  *             in   source arr[32],sort[32],
                 out target arr[32],sort[32] 
*/   
void QuickSort(unsigned char* arr,unsigned char* sort,unsigned char startPos, unsigned char endPos) 
{ 
	unsigned char i,j; 
	unsigned char key,st; 
	key=arr[startPos]; 
	st = sort[startPos];
	i=startPos; 
	j=endPos; 
	while(i<j) 
	{ 
		while(arr[j]<=key && i<j)--j; 
		arr[i]=arr[j]; 
		sort[i] = sort[j];
		while(arr[i]>=key && i<j)++i; 
		arr[j]=arr[i]; 
		sort[j]=sort[i];
	} 
	arr[i]=key; 
	sort[i]=st;
	if(i-1>startPos) QuickSort(arr,sort,startPos,i-1); 
	if(endPos>i+1) QuickSort(arr,sort,i+1,endPos); 
}

/** do something with the GGA data */
static void print_gga(nmeap_gga_t *gga)
{
    
    kal_prompt_trace(MOD_GPS,"found GNGGA message %f,%f,%f,%d,%d,%d,%f,%f\n",
            gga->latitude  ,
            gga->longitude, 
            gga->altitude , 
            gga->time     , 
            gga->satellites,
            gga->quality   ,
            gga->hdop      ,
            gga->geoid     
            );
}

/** called when a gntxt message is received and parsed */
static void gntxt_callout(nmeap_context_t *context,void *data,void *user_data)
{
    nmeap_txt_t *ptxt = (nmeap_txt_t *)data;	
    mdi_gps_nmea_txt_struct  txt_data;

    memset(txt_data.hed_string, 0x00, sizeof(txt_data.hed_string));    
    txt_data.hed_0= ptxt->hed_0;	
    txt_data.hed_1= ptxt->hed_1;
    txt_data.hed_2= ptxt->hed_2;
    strcpy(txt_data.hed_string, ptxt->hed_string);
    //kal_prompt_trace(MOD_GPS," gngga_callout pgga->satellites =%d",pgga->satellites);
    //kal_prompt_trace(MOD_GPS," gngga_callout gga_data->sat_in_view =%d",gga_data.sat_in_view);	
    //kal_prompt_trace(MOD_GPS," gngga_callout pgga->quality  =%d",pgga->quality);	
    //kal_prompt_trace(MOD_GPS," gngga_callout gga_data->quality  =%d",gga_data.quality);		
    kal_prompt_trace(MOD_GPS,"txt_data.hed_string = %s, %d, %d, %d", txt_data.hed_string, txt_data.hed_0, txt_data.hed_1, txt_data.hed_2);	
	
    RJ_GPS_Callback(MDI_GPS_PARSER_NMEA_TXT, &txt_data, 0); 

    //kal_prompt_trace(MOD_GPS," ------------gngga_callout\n");	
    //print_gga(pgga);
}

/** called when a gngga message is received and parsed */
static void gngga_callout(nmeap_context_t *context,void *data,void *user_data)
{
    nmeap_gga_t *pgga = (nmeap_gga_t *)data;	
    mdi_gps_nmea_gga_struct  gga_data;
    
    gga_data.sat_in_view  = pgga->satellites;	
    gga_data.quality = pgga->quality;
    gga_data.latitude = 	pgga->latitude;
    gga_data.longitude = pgga->longitude;
    gga_data.altitude = pgga->altitude;
    gga_data.east_west= pgga->east_west;
    gga_data.north_south= pgga->north_south;
    gga_data.h_precision = pgga->hdop;
    //kal_prompt_trace(MOD_GPS," gngga_callout pgga->satellites =%d",pgga->satellites);
    //kal_prompt_trace(MOD_GPS," gngga_callout gga_data->sat_in_view =%d",gga_data.sat_in_view);	
    //kal_prompt_trace(MOD_GPS," gngga_callout pgga->quality  =%d",pgga->quality);	
    //kal_prompt_trace(MOD_GPS," gngga_callout gga_data->quality  =%d",gga_data.quality);		
    //kal_prompt_trace(MOD_GPS," ------------gngga_callout enter");	
	
    RJ_GPS_Callback(MDI_GPS_PARSER_NMEA_GGA, &gga_data, 0); 
			
    //kal_prompt_trace(MOD_GPS," ------------gngga_callout\n");	
    //print_gga(pgga);
}


/** do something with the RMC data */
static void print_rmc(nmeap_rmc_t *rmc)
{
     kal_prompt_trace(MOD_GPS,"found GNRMC Message %d %c %f %f %f %f %d %f\n",
            rmc->time,
            rmc->warn,
            rmc->latitude,
            rmc->longitude,
            rmc->speed,
            rmc->course,
            rmc->date,
            rmc->magvar
            );
}

/** called when a gnrmc message is received and parsed */
static void gnrmc_callout(nmeap_context_t *context,void *data,void *user_data)
{
    nmeap_rmc_t *prmc = (nmeap_rmc_t *)data;
    kal_prompt_trace(MOD_GPS,"-------------gnrmc_callout\n");
    //print_rmc(prmc);
}

/** do something with the VTG data */
static void print_vtg(nmeap_vtg_t *vtg)
{
    printf("found GNVTG Message %.f %c %f %c %f %c %f %c\n",
            vtg->dir,
            vtg->dir_t,
            vtg->dec,
            vtg->dec_m,
            vtg->spn,
            vtg->spn_n,
            vtg->spk,
            vtg->spk_k
            );
}

/** called when a gprmc message is received and parsed */
static void gnvtg_callout(nmeap_context_t *context,void *data,void *user_data)
{
    nmeap_vtg_t *vtg = (nmeap_vtg_t *)data;

    mdi_gps_nmea_vtg_struct  vtg_data;
    
    vtg_data.hspeed_km  = vtg->spk;	
    vtg_data.true_heading = vtg->dir;
	
    RJ_GPS_Callback(MDI_GPS_PARSER_NMEA_VTG, &vtg_data, 0); 	
	
     //kal_prompt_trace(MOD_GPS,"-------------gnvtg_callout\n");
    //print_vtg(vtg);
}

/** do something with the GPGSV data */
static void print_gpgsv(nmeap_gpgsv_t *gpgsv)
{
    kal_prompt_trace(MOD_GPS,"found GPGSV Message %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
            gpgsv->pack_count,
            gpgsv->pack_index,
            gpgsv->sat_count,
            gpgsv->sat_data[0].id,
            gpgsv->sat_data[0].elv,
            gpgsv->sat_data[0].azimuth,
            gpgsv->sat_data[0].sig,
            gpgsv->sat_data[1].id,
            gpgsv->sat_data[1].elv,
            gpgsv->sat_data[1].azimuth,
            gpgsv->sat_data[1].sig,
	     gpgsv->sat_data[2].id,
            gpgsv->sat_data[2].elv,
            gpgsv->sat_data[2].azimuth,
            gpgsv->sat_data[2].sig,
	     gpgsv->sat_data[3].id,
            gpgsv->sat_data[3].elv,
            gpgsv->sat_data[3].azimuth,
            gpgsv->sat_data[3].sig
            );
}
kal_uint16 gp_gsv_num=0;
kal_uint16 gp_gsv_sat=0;
/** called when a gpgsv message is received and parsed */
static void gpgsv_callout(nmeap_context_t *context,void *data,void *user_data)
{
      kal_uint8 i;
	  
    nmeap_gpgsv_t *pgpgsv = (nmeap_gpgsv_t *)data;



        #ifdef BD_HDDZ
	    if(pgpgsv->sat_count % 4 !=0)
	    {
	    	  g_gpgsv_row = pgpgsv->sat_count /4 +1;
	    }		  
	    else 
	    {
		 g_gpgsv_row = pgpgsv->sat_count /4;	
	    }		
	    if (g_gsv_cyclenum >	g_gpgsv_row) g_gsv_cyclenum = 0;
	    if(g_gsv_cyclenum == 0)  
	    {
	          g_gsv_num = 0;
		  for(i=0;i<32;i++)
		   {
		          bd_sate_id[i]= 0;
		          bd_snr[i]= 0;	
		   }	  
	    }

	    g_gsv_cyclenum = g_gsv_cyclenum +1;	
	     kal_prompt_trace(MOD_GPS,"for  gpgsv_callout g_gsv_cyclenum=%d,g_gpgsv_row = %d",g_gsv_cyclenum,g_gpgsv_row); 				
	     for(i=0;i<4;i++) {   			
		  bd_sate_id[g_gsv_num] = pgpgsv->sat_data[i].id;	
	         bd_snr[g_gsv_num]       = pgpgsv->sat_data[i].sig;	
	         g_gsv_num = g_gsv_num + 1;	 	
		if(gp_gsv_sat<bd_snr[i])
			gp_gsv_sat=bd_snr[i];
			//kal_prompt_trace(MOD_GPS,"gp_gsv_sat=%d",gp_gsv_sat);	
	         kal_prompt_trace(MOD_GPS,"for  g_gsv_num=%d, sat_data id = %d, sat_data sig=%d",g_gsv_num,pgpgsv->sat_data[i].id,pgpgsv->sat_data[i].sig); 			
	    	}	
    #endif 	// BD_HDDZ
		gp_gsv_num=pgpgsv->sat_count;
	//kal_prompt_trace(MOD_GPS,"gp_gsv_num=%d",gp_gsv_num);	
    //kal_prompt_trace(MOD_GPS,"-------------gpgsv_callout\n");
    print_gpgsv(pgpgsv);
}

/** do something with the BDGSV data */
static void print_bdgsv(nmeap_bdgsv_t *bdgsv)
{
     kal_prompt_trace(MOD_GPS,"found BDGSV Message %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
            bdgsv->pack_count,
            bdgsv->pack_index,
            bdgsv->sat_count,
            bdgsv->sat_data[0].id,
            bdgsv->sat_data[0].elv,
            bdgsv->sat_data[0].azimuth,
            bdgsv->sat_data[0].sig,
            bdgsv->sat_data[1].id,
            bdgsv->sat_data[1].elv,
            bdgsv->sat_data[1].azimuth,
            bdgsv->sat_data[1].sig,
	     bdgsv->sat_data[2].id,
            bdgsv->sat_data[2].elv,
            bdgsv->sat_data[2].azimuth,
            bdgsv->sat_data[2].sig,
	     bdgsv->sat_data[3].id,
            bdgsv->sat_data[3].elv,
            bdgsv->sat_data[3].azimuth,
            bdgsv->sat_data[3].sig
            );
}

/** called when a bdgsv message is received and parsed */
static void bdgsv_callout(nmeap_context_t *context,void *data,void *user_data)
{
      kal_uint8 i;
    
    nmeap_bdgsv_t *pbdgsv = (nmeap_bdgsv_t *)data;

    #ifdef BD_HDDZ
	    if(pbdgsv->sat_count % 4 !=0)
	    {
	    	  g_bdgsv_row = pbdgsv->sat_count /4 +1;
	    }		  
	    else 
	    {
		 g_bdgsv_row = pbdgsv->sat_count /4;	
	    }			 	
	    	
	    g_gsv_cyclenum = g_gsv_cyclenum +1;	
           kal_prompt_trace(MOD_GPS,"for  bdgsv_callout g_gsv_cyclenum=%d,g_bdgsv_row = %d",g_gsv_cyclenum,g_bdgsv_row); 
	    
	    for(i=0;i<4;i++) {
	         bd_sate_id[g_gsv_num] = pbdgsv->sat_data[i].id;	
	         bd_snr[g_gsv_num]       = pbdgsv->sat_data[i].sig;	
	         g_gsv_num = g_gsv_num + 1;	 	
	         kal_prompt_trace(MOD_GPS,"for g_gsv_num =%d, sat_data id = %d, sat_data sig=%d",g_gsv_num,pbdgsv->sat_data[i].id,pbdgsv->sat_data[i].sig); 		    		
	    	}		
	 
	    
	    if (g_gsv_cyclenum>=(g_gpgsv_row+g_bdgsv_row))	{
		  kal_prompt_trace(MOD_GPS,"-------------quiksort start");	
		  QuickSort(bd_snr,bd_sate_id,0,31);	
		  kal_prompt_trace(MOD_GPS,"-------------quiksort end");	
		  for(i=0;i<12;i++)
		  {
		      gsv_data.rsv[i].sate_id = bd_sate_id[i];
		      gsv_data.rsv[i].snr = bd_snr[i];
		      kal_prompt_trace(MOD_GPS,"bd_sate_id=%d",bd_sate_id[i]);	  
		      kal_prompt_trace(MOD_GPS,"bd_snr=%d",bd_snr[i]);	  	  
		  }  
		  g_gsv_cyclenum = 0;
		  g_gsv_num = 0;
		  g_gpgsv_row = 0;
		  g_bdgsv_row = 0;
	         RJ_GPS_Callback(MDI_GPS_PARSER_NMEA_GSV, &gsv_data, 0);		 
	    }		
    #endif  //BD_HDDZ	
	
    //kal_prompt_trace(MOD_GPS,"-------------bdgsv_callout\n");
    print_bdgsv(pbdgsv);
}
//add by lqy for BD
void BD_GPS_Manufacture_INIT(void)
{	
	kal_prompt_trace(MOD_GPS," BD_GPS_Manufacture_INIT ");	
	Vcamd_Supply();	
	//Vcama_Supply();	
	Vvibr_Supply();		

	GPIO_ModeSetup(0, 0);		/*UART*/	
	GPIO_InitIO(0, 0);  
	GPIO_ModeSetup(1, 0);		/*UART*/	
	GPIO_InitIO(0, 1);	
/*Commit for HD8020 Firmware update PRTRG pin by Huangwenjian PRTRG begin 20160623*/
	GPIO_InitIO(1, 16);
	GPIO_WriteIO(0, 16);	
	kal_sleep_task(500); 	
	GPIO_InitIO(0, 16);	
/*Commit for HD8020 Firmware update PRTRG pin by Huangwenjian PRTRG end 20160623*/
}
extern void bird_agps_ack_handle(kal_uint8 *Buffaddr, kal_uint16 Length);
void BD_NMEAP_INIT(kal_uint8 *bufferaddr)
{	
       int  status;
       int  rem;
	int  offset;
	int  i,len;
	char buffer[32];
	 char *pvec = bufferaddr;

       if (bnmea != TRUE) 
	{ 	
		kal_prompt_trace(MOD_GPS," BD_NMEAP_INIT enter");	
		/* ---------------------------------------*/
		/*STEP 2 : initialize the nmea context    */                                                
		/* ---------------------------------------*/
	    status = nmeap_init(&nmea,(void *)&user_data);
	    if (status != 0) {
	        kal_prompt_trace(MOD_GPS," nmeap_init status = %d ",status);
	    }
	    
		/* ---------------------------------------*/
		/*STEP 3 : add standard GPGGA parser      */                                                
		/* -------------------------------------- */
	    status = nmeap_addParser(&nmea,"GNGGA",nmeap_gngga,gngga_callout,&gga);
	    if (status != 0) {
	        kal_prompt_trace(MOD_GPS," nmeap_add GNGGA status = %d ",status);
	    }
#ifdef HD8020_ACK_CHECK
		/* ---------------------------------------*/
		/*STEP 3 : add standard GPTXT parser      */                                                
		/* -------------------------------------- */
	    status = nmeap_addParser(&nmea,"GNTXT",nmeap_gntxt,gntxt_callout,&txt);
	    if (status != 0) {
	        kal_prompt_trace(MOD_GPS," nmeap_add GNTXT status = %d ",status);
	    }		
#endif
		/* ---------------------------------------*/
		/*STEP 4 : add standard GPRMC parser      */                                                
		/* -------------------------------------- */
	   /* status = nmeap_addParser(&nmea,"GNRMC",nmeap_gnrmc,gnrmc_callout,&rmc);
	    if (status != 0) {
	        kal_prompt_trace(MOD_GPS," nmeap_add GNRMC status = %d ",status);
	    }*/
		/* ---------------------------------------*/
		/*STEP 4 : add standard GNVTG parser      */                                                
		/* -------------------------------------- */
	    status = nmeap_addParser(&nmea,"GNVTG",nmeap_gnvtg,gnvtg_callout,&vtg);
	    if (status != 0) {
	        kal_prompt_trace(MOD_GPS," nmeap_add GNVTG status = %d ",status);
	    }
		
	    /* ---------------------------------------*/
		/*STEP 4 : add standard GPGSV parser      */                                                
		/* -------------------------------------- */
		
	    status = nmeap_addParser(&nmea,"GPGSV",nmeap_gpgsv,gpgsv_callout,&gpgsv);
	    if (status != 0) {
	        kal_prompt_trace(MOD_GPS," nmeap_add GPGSV status = %d ",status);
	    }

		/* ---------------------------------------*/
		/*STEP 4 : add standard BDGSV parser      */                                                
		/* -------------------------------------- */

	    status = nmeap_addParser(&nmea,"BDGSV",nmeap_bdgsv,bdgsv_callout,&bdgsv);
	    if (status != 0) {
	        kal_prompt_trace(MOD_GPS," nmeap_add BDGSV status = %d ",status);
	    }
	     bnmea = TRUE;
       	}
	   
	/* ---------------------------------------*/
	/*STEP 5 : process input until done       */                                                
	/* -------------------------------------- */
    for(;;) {
           /* ---------------------------------------*/
		/*STEP 6 : get a buffer of input          */                                                
		/* -------------------------------------- */
        //len = rem = readbuffer(buffer,sizeof(buffer));
        len = sizeof(buffer);
		    
		if (*pvec == 0) 
		{
			// end of file
			len = rem = -1;
		}
		else 
		{    
		  for(i=0;i<len;i++) 
		  {
			  /* quit when no more data */
			  if (*pvec == 0) {
				 break;
		  	  }
			  buffer[i] = *pvec++;
		   }
		   len = rem = i;
		}
		/* ---------------------------------------*/
		/*STEP 6 : get a buffer of input          */                                                
		/* -------------------------------------- */
        if (len <= 0) {
            break;
        }
        
        kal_prompt_trace(MOD_GPS,"BD_NMEAP_INIT readbuffer=%s  len = %d ",buffer,len);
#ifdef HD8020_ACK_CHECK		
        bird_agps_ack_handle(buffer, len);
#endif
		/* ----------------------------------------------*/
		/*STEP 7 : process input until buffer is used up */                                                
		/* --------------------------------------------- */
		offset = 0;
        while(rem > 0) {
			/* --------------------------------------- */
			/*STEP 8 : pass it to the parser           */
			/* status indicates whether a complete msg */
			/* arrived for this byte                   */
			/* NOTE : in addition to the return status */
			/* the message callout will be fired when  */
			/* a complete message is processed         */
			/* --------------------------------------- */
            status = nmeap_parseBuffer(&nmea,&buffer[offset],&rem);
			offset += (len - rem); 
            
			/* ---------------------------------------*/
			/*STEP 9 : process the return code        */                                                
			/* -------------------------------------- */
            switch(status) {
            case NMEAP_GNGGA:
                kal_prompt_trace(MOD_GPS,"NMEAP_GNGGA-------------switch\n");
                //print_gga(&gga);
                break;
/*            case NMEAP_GNRMC:
                kal_prompt_trace(MOD_GPS,"NMEAP_GNRMC-------------switch\n");
                print_rmc(&rmc);
                break;*/
            case NMEAP_GNVTG:
                kal_prompt_trace(MOD_GPS,"NMEAP_GNVTG-------------switch\n");
                //print_vtg(&vtg);
                break;    
	     case NMEAP_GPGSV:
                kal_prompt_trace(MOD_GPS,"NMEAP_GPGSV-------------switch\n");
                //print_gpgsv(&gpgsv);
                break;
            case NMEAP_BDGSV:
                kal_prompt_trace(MOD_GPS,"NMEAP_BDGSV-------------switch\n");
                //print_bdgsv(&bdgsv);
                break;			
            default:
                break;
            }
        }
    }
    	
}


#endif

