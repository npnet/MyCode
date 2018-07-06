/*
Copyright (c) 2005, David M Howard (daveh at dmh2000.com)
All rights reserved.

This product is licensed for use and distribution under the BSD Open Source License.
see the file COPYING for more details.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 

*/

/**
 * nmeap.c
 * nmeap gps data parser
 * 
 * 
 * modify by lqy for mtk  and BD platform 2014-11-20
 *
 *
 * see the file COPYING for terms of the licnese
*/ 
 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <kal_trace.h> // add by lqy for mtk trace
#include <app_datetime.h> //for datetime
#include "Bird_app.h"  
#include "nmeap.h"

/* this only works if you are sure you have an upper case hex digit */
#define HEXTOBIN(ch) ((ch <= '9') ? ch - '0' : ch - ('A' - 10))

/* forward references */
int nmeap_init(nmeap_context_t *context,void *user_data);
int nmeap_addParser(nmeap_context_t         *context,
					 const char             *sentence_name,
					 nmeap_sentence_parser_t sentence_parser,
					 nmeap_callout_t         sentence_callout,
                     void                  *sentence_data
					 );
int nmeap_tokenize(nmeap_context_t *context);
int nmeap_process(nmeap_context_t *context);
int nmeap_parse(nmeap_context_t *context,char ch);
int nmeap_parseBuffer(nmeap_context_t *context,const char *buffer,int *length);

static char errinput[NMEAP_MAX_SENTENCE_LENGTH + 1];

/**
 * get a latitude out of a pair of nmea tokens 
 */
double nmeap_latitude(const char *plat,const char *phem)
{
    double lat;
    int    deg;
    double min;
    int    ns;
    
    assert(plat != 0);
    assert(phem != 0);
    
    if (*plat == 0) {
        return 0.0;
    }
    if (*phem == 0) {
        return 0.0;
    }
    
    /* north lat is +, south lat is - */
    if (*phem == 'N') {
        ns = 1;
    }
    else {
        ns = -1;
    }
    
    /* latitude is degrees, minutes, fractional minutes */
    /* no validation is performed on the token. it better be good.*/
    /* if it comes back 0.0 then probably the token was bad */
    lat = atof(plat);
    
    /* extract the degree part */
    deg = (int)(lat / 100.0);
    
    /* mask out the degrees */
    min = lat - (deg * 100.0);
    
    /* compute the actual latitude in degrees.decimal-degrees */
    lat = (deg + (min / 60.0)) * ns;
    
    return lat;
}

/**
 * get a longitude out of a pair of nmea tokens 
 */
double nmeap_longitude(const char *plon,const char *phem)
{
    double lon;
    int    deg;
    double min;
    int    ew;
    
    assert(plon != 0);
    assert(phem != 0);
    
    if (*plon == 0) {
        return 0.0;
    }
    if (*phem == 0) {
        return 0.0;
    }
    
    /* west long is negative, east long is positive */
    if (*phem == 'E') {
        ew = 1;
    }
    else {
        ew = -1;
    }
    
    /* longitude is degrees, minutes, fractional minutes */
    /* no validation is performed on the token. it better be good.*/
    /* if it comes back 0.0 then probably the token was bad */
    lon = atof(plon);
    
    /* extract the degree part */
    deg = (int)(lon / 100.0);
    
    /* mask out the degrees */
    min = lon - (deg * 100.0);
    
    /* compute the actual lonitude in degrees.decimal-degrees */
    lon = (deg + (min / 60.0)) * ew;

    
    return lon;
}

/**
 * get an altitude longitude out of a pair of nmea tokens
 * ALTITUDE is returned in METERS 
 */
double nmeap_altitude(const char *palt,const char *punits)
{
    double alt;
    
    if (*palt == 0) {
        return 0.0;
    }
    
    /* convert with no error checking */
    alt = atof(palt);
    
    if (*punits == 'M') {
        /* already in meters */ 
    }
    else if (*punits == 'F') {
        /* convert to feet */
        alt = alt * 3.2808399;
    }
    
    return alt;
}

/**
 * initialize an NMEA parser
 */
int nmeap_init(nmeap_context_t *context,void *user_data)
{
    assert(context != 0);

    memset(context,0,sizeof(*context));

    context->user_data = user_data;

    return 0;    
}

 
/**
 * register an NMEA sentence parser
 */
int nmeap_addParser(nmeap_context_t         *context,
					 const char             *sentence_name,
					 nmeap_sentence_parser_t sentence_parser,
					 nmeap_callout_t         sentence_callout,
                     void                  *sentence_data
					 )
{
    nmeap_sentence_t *s = 0;
    
    /* runtime error */
    assert(context != 0);
    
    /* sentence capacity overflow */
    if (context->sentence_count >= NMEAP_MAX_SENTENCES) {
        return -1;
    }
    
    /* point at next empty sentence buffer */
    s = &context->sentence[context->sentence_count];
    
    /* advance sentence data count */
    context->sentence_count++;
    
    /* clear the sentence data */
    memset(s,0,sizeof(*s));
    
    /* name */
    strncpy(s->name,sentence_name,NMEAP_MAX_SENTENCE_NAME_LENGTH);
    
    /* parser */
    s->parser = sentence_parser;
    
    /* callout */
    s->callout = sentence_callout;
    
    /* data */
    s->data    = sentence_data;

    return 0;
}

/** 
 * tokenize a buffer
 */
int nmeap_tokenize(nmeap_context_t *context)
{
    char *s;
    int   tokens;
    int   state;
    
    /* first token is header. assume it is there */
    tokens = 0;
    s = context->input;
    context->token[tokens] = s;
    
    /* get rest of tokens */
    tokens = 1;
    state = 0;
    while((*s != 0)&&(tokens < NMEAP_MAX_TOKENS)) {
        switch(state) {
        case 0:
            /* looking for end of a token */
            if (*s == ',') {
                /* delimit at the comma */
                *s    = 0;
                /* new token */
                state = 1;
            }
            break;
        case 1:
            /* start of next token, might be another comma */
            context->token[tokens++] = s;
            if (*s == ',') {
                /* delimit at the comma */
                *s    = 0;
            }
            else {   
                /* not a comma */
                state = 0;
            }
            break;
        default:
            state = 0;
            break;
        }
        
        // next character
        s++;
    }
    return tokens;
}

/**
 * process a sentence 
 */
int nmeap_process(nmeap_context_t *context)
{
    int id;
    int i;
    nmeap_sentence_t *s;
	
    /* copy the input to a debug buffer */
    /* remove debug_input when everything is working. */
    strncpy(context->debug_input,context->input,sizeof(context->debug_input));
    
    /* tokenize the input */
    context->tokens = nmeap_tokenize(context);
    
    /* try to find a matching sentence parser */
    /* this search is O(n). it has a lot of potential for optimization, at the expense of complexity, if you have a lot of sentences */
    /* binary search instead of linear (have to keep sentences in sorted order) O(NlogN) */
    /* OR, when sentences are added, create a TRIE structure to find the names with a constant time search O(5) */
    for(i=0;i<context->sentence_count;i++) {
        s = &context->sentence[i];
        assert(s != 0);
        if (strncmp(context->input_name,s->name,5) == 0) {
            /* found a match, call its parser */
            id = (*context->sentence[i].parser)(context,s);
            if (id > 0) {
                break;
            }
        }
    }
        
    return id;
}

/**
                 +-5-+              +---+
                 v   |              v   |
 +------+       +------+           +------+         +------+         +------+
 |   0  |--$--> |1-hdr |--alnum--> |2-data|----\r-->| 6-LF |---\n--->| done |--> 0
 +------+       +------+           +------+         +------+         +------+
                                    |                 ^
                                    *                 +--------\r-------+
                                    V                                   |
                                 +------+           +------+         +------+
                                 |3-cks |--xdigit-->|4-cks |-xdigit->| 5-CR |
                                 +------+           +------+         +------+
                                 
return to start conditions:
1. buffer overflow
2. invalid character for state

checksum calculation
two hex digits represent the XOR of all characters between, but not
including, the "$" and "*".  A checksum is required on some
sentences.

*/
int nmeap_parse(nmeap_context_t *context,char ch)
{
    int status = 0;
    
    /* check for input buffer overrun first to avoid duplicating code in the
    individual states
    */
    if (context->input_count >= (sizeof(context->input)-1)) {
        /* input buffer overrun, restart state machine */
        context->input_state = 0;
        /* reset input count */
        context->input_count = 0;
    }
    
    /* store the byte */
    context->input[context->input_count] = ch;
    
    /* next buffer position */
    context->input_count++;
    
    /* run it through the lexical scanner */
    switch(context->input_state) {
    /* LOOKING FOR $ */
    case 0:
        if (ch == '$') {
            /*look for id */
            context->input_state = 1;
            context->ccks        = 0;
			context->icks        = 0;
        }
        else {
            /* header error, start over */
            context->err_hdr++;
            context->input_state = 0;
            context->input_count = 0;
        }
        break;
    /* LOOKING FOR 5 CHARACTER SENTENCE ID */
    case 1:
        /* allow numbers even though it isn't usually done */
        /* a proprietary id might have a numeral */
        if (isalnum(ch)) {
            /* store name separately */
            context->input_name[context->input_count - 2] = ch;
            /* checksum */
            context->ccks ^= ch;
            /* end of header? */
            if (context->input_count >= 6) {
                /* yes, get body */
                context->input_state = 2;
            }
        }
        else {
            /* bad character, start over */
            context->err_id++;
            context->input_state = 0;
            context->input_count = 0;
        }
        break;
    /* LOOKING FOR CR OR CHECKSUM INDICATOR */
    case 2:
        if (ch == '*') {
            /* this sentence has a checksum */
            context->input_state = 3;
        }
        else if (ch == '\r') {
            /* carriage return, no checksum, force a match */
			context->icks = 0;
			context->ccks = 0;
            context->input_state = 6;
        }
	//add by lqy	
        else if (isalnum(ch)  ) {
            /* continue accumulating data */
            /* checksum */
            context->ccks ^= ch;
        }
	 else if ( (ch==',') || (ch=='.') || (ch=='-')  || (ch=='_') ) {
            /* continue accumulating data */
            /* checksum */
            context->ccks ^= ch;
        }
	else {
	    memset(errinput,0,sizeof(errinput));
	    strcpy((char*)errinput, (char*)context->input);
	    kal_prompt_trace(MOD_GPS, "nmeap_parse input=%s",context->input);	
	    //Bd_Log(3);	 // by lqy for test
            /* bad character, start over */
            context->err_id++;
            context->input_state = 0;
            context->input_count = 0;
	}	
	// add by lqy 	
        break;
    /* LOOKING FOR FIRST CHECKSUM CHARACTER */
    case 3:
        /* must be upper case hex digit */
        if (isxdigit(ch) && (ch <= 'F')) {
            /* got first checksum byte */
            context->input_state = 4;
            context->icks = HEXTOBIN(ch) << 4;
        }
        else {
            /* input error, restart */
            context->err_cks++;
            context->input_state = 0;
            context->input_count = 0;
        }
        break;
        /* LOOKING FOR SECOND CHECKSUM CHARACTER */
    case 4:
        /* must be upper case hex digit */
        if (isxdigit(ch) && (ch <= 'F')) {
            /* got second checksum byte */
            context->input_state = 5;
            context->icks += HEXTOBIN(ch);
        }
        else {
            /* input error, restart */
            context->err_cks++;
            context->input_state = 0;
            context->input_count = 0;
        }
        break;
    /* LOOKING FOR CR */
    case 5:
        if (ch == '\r') {
            /* carriage return */
            context->input_state = 6;
        }
        else {
            /* input error, restart */
            context->err_crl++;
            context->input_state = 0;
            context->input_count = 0;
        }
        break;
    /* LOOKING FOR LINE FEED */
    case 6:
        if (ch == '\n') {
            /* linefeed, line complete */
            
            /* delimit buffer */
            context->input[context->input_count] = 0;
            
            /* if the checksums match, process the sentence */
			if (context->ccks == context->icks) {
				/* process */
				status = nmeap_process(context);
				
				/* count good messages */
				context->msgs++;
			}
			else {
				memset(errinput,0,sizeof(errinput));
	                      strcpy((char*)errinput, (char*)context->input);
	                      kal_prompt_trace(MOD_GPS, " checksum errors  input=%s",context->input);	
	                      //Bd_Log(2);	 // by lqy for test
				/* count checksum errors */
				context->err_cks++;
			}
            
            /* restart next time */
            context->input_state = 0;
            context->input_count = 0;
        }
        else {
            /* input error, restart */
            context->err_crl++;
            context->input_state = 0;
            context->input_count = 0;
        }
        break;
    default:
        context->err_unk++;
        context->input_state = 0;
        break;
    }
            
	return status;
}

/** 
 * parse a buffer of nmea data
 */
int nmeap_parseBuffer(nmeap_context_t *context,const char *buffer,int *length)
{
    int  i;
    int  status;
    int  rem;
    int  tlen;
    
    tlen   = *length;
    rem    = *length;
    status = 0;
    /* for each byte in the buffer */
    for(i=0;i<tlen;i++) {
        /* decrement remaining byte count */
        rem--;
        /* parse the byte */
        status = nmeap_parse(context,buffer[i]);
        if (status != 0) {
            /* message found or error */
            break;
        }
    }
	
    /* return remaining byte count */
    *length = rem;
	
    return status;
}

/** 
 * standard GNTXT sentence parser 
 */
int nmeap_gntxt(nmeap_context_t *context,nmeap_sentence_t *sentence)
{
#ifndef NDEBUG	
    int i;
#endif
    
    /* get pointer to sentence data */
    nmeap_txt_t *txt = (nmeap_txt_t *)sentence->data;
    
    /* if there is a data element, extract data from the tokens */
    if (txt != 0) {
		memset(txt->hed_string, 0x00, sizeof(txt->hed_string));
		txt->hed_0  = atoi(context->token[1]);
		txt->hed_1  = atoi(context->token[2]);
		txt->hed_2  = atoi(context->token[3]);
		strcpy(txt->hed_string, context->token[4]);
    }
    kal_prompt_trace(MOD_GPS, "nmeap_gntxt token[1]=%s, token[2]=%s, token[3]=%s, token[4]=%s",context->token[1],context->token[2],context->token[3],context->token[4]);
    kal_prompt_trace(MOD_GPS, "nmeap_gntxt debug_input=%s,txt->hed_0 = %d, %d, %d, txt->hed_string = %s",context->debug_input, txt->hed_0, txt->hed_1, txt->hed_2, txt->hed_string);

    /* print some validation data */
    kal_prompt_trace(MOD_GPS, "nmeap_gntxt input_name,name=%s,%s,icks,ccks=%x,%x",context->input_name,sentence->name,context->icks,context->ccks);

    if (sentence->callout != 0) {
        (*sentence->callout)(context,txt,context->user_data);
    }
    
    return NMEAP_GNTXT;
}

/** 
 * standard GNGGA sentence parser 
 */
int nmeap_gngga(nmeap_context_t *context,nmeap_sentence_t *sentence)
{
#ifndef NDEBUG	
    int i;
#endif
    
    /* get pointer to sentence data */
    nmeap_gga_t *gga = (nmeap_gga_t *)sentence->data;
    
    /* if there is a data element, extract data from the tokens */
	if (gga != 0) {
		gga->latitude  = nmeap_latitude(context->token[2],context->token[3]);
		gga->longitude = nmeap_longitude(context->token[4],context->token[5]);
		gga->altitude  = nmeap_altitude(context->token[9],context->token[10]);
		gga->time       = atoi(context->token[1]);
		gga->satellites = atoi(context->token[7]);
		gga->quality    = atoi(context->token[6]);
		gga->east_west = (kal_int8)*context->token[5];
		gga->north_south = (kal_int8)*context->token[3];	
		gga->hdop       = atof(context->token[8]);
		gga->geoid      = nmeap_altitude(context->token[11],context->token[12]);
	}

      //by lqy for test 
      /*
      if ( (gga->latitude<28) && (gga->quality>=1) )
	{
	    memset(errinput,0,sizeof(errinput));
	    strcpy((char*)errinput, (char*)context->debug_input);
	    kal_prompt_trace(MOD_GPS, "nmeap_gngga debug_input=%s,quality = %d",context->debug_input,gga->quality);	
	    Bd_Log(1);
      	}	
      if ((gga->longitude<120)  && (gga->quality>=1)  )	
	{ 
	    memset(errinput,0,sizeof(errinput));
	     kal_prompt_trace(MOD_GPS, "nmeap_gngga debug_input=%s,quality = %d",context->debug_input,gga->quality);		
	    strcpy((char*)errinput, (char*)context->debug_input);
	    Bd_Log(1);
      	}*/	
         
    kal_prompt_trace(MOD_GPS, "nmeap_gngga debug_input=%s",context->debug_input);

    /* print some validation data */
    kal_prompt_trace(MOD_GPS, "nmeap_gngga input_name,name=%s,%s,icks,ccks=%x,%x",context->input_name,sentence->name,context->icks,context->ccks);
	
    /* print the tokens */
   /*	
    for(i=0;i<context->tokens;i++) {
        //kal_prompt_trace(MOD_GPS, "nmeap_gngga i,tokens=%d:%s",i,context->token[i]);
    }*/

    /* if the sentence has a callout, call it */
    if (sentence->callout != 0) {
        (*sentence->callout)(context,gga,context->user_data);
    }
    
    return NMEAP_GNGGA;
}

/** 
 * standard GNRMC sentence parser 
 */
int nmeap_gnrmc(nmeap_context_t *context,nmeap_sentence_t *sentence)
{
#ifndef NDEBUG	
    int i;
#endif

    /* get pointer to sentence data */
    nmeap_rmc_t *rmc = (nmeap_rmc_t *)sentence->data;
    
	/* if there is a data element, use it */
	if (rmc != 0) {
		/* extract data from the tokens */
		rmc->time       = atoi(context->token[1]);
		rmc->warn       = *context->token[2];
		rmc->latitude  = nmeap_latitude(context->token[3],context->token[4]);
		rmc->longitude = nmeap_longitude(context->token[5],context->token[6]);
		rmc->speed      = atof(context->token[7]);
		rmc->course     = atof(context->token[8]);
		rmc->date       = atoi(context->token[9]);
		rmc->magvar     = atof(context->token[10]);
	}

    kal_prompt_trace(MOD_GPS, "nmeap_gnrmc debug_input=%s",context->debug_input);
	
    /* print some validation data */
    kal_prompt_trace(MOD_GPS, "nmeap_gnrmc input_name,name=%s,%s,icks,ccks=%x,%x",context->input_name,sentence->name,context->icks,context->ccks);

   	
    /* print the tokens */
   /*	
    for(i=0;i<context->tokens;i++) {
   //     kal_prompt_trace(MOD_GPS, "nmeap_gnrmc i,tokens=%d:%s",i,context->token[i]);
    }*/

    /* if the sentence has a callout, call it */
    if (sentence->callout != 0) {
        (*sentence->callout)(context,rmc,context->user_data);
    }
    
    return NMEAP_GNRMC;
}

/** //add by lqy
 * standard GPGSV sentence parser 
 */
int nmeap_gpgsv(nmeap_context_t *context,nmeap_sentence_t *sentence)
{
#ifndef NDEBUG	
    int i;
#endif

    /* get pointer to sentence data */
    nmeap_gpgsv_t *gpgsv = (nmeap_gpgsv_t *)sentence->data;
    
	/* if there is a data element, use it */
	if (gpgsv != 0) {
		/* extract data from the tokens */
		gpgsv->pack_count           = atoi(context->token[1]);
		gpgsv->pack_index           = atoi(context->token[2]);
		gpgsv->sat_count            = atoi(context->token[3]);
		gpgsv->sat_data[0].id       = atoi(context->token[4]);
		gpgsv->sat_data[0].elv      = atoi(context->token[5]);
		gpgsv->sat_data[0].azimuth  = atoi(context->token[6]);
		gpgsv->sat_data[0].sig      = atoi(context->token[7]);
		gpgsv->sat_data[1].id       = atoi(context->token[8]);
		gpgsv->sat_data[1].elv      = atoi(context->token[9]);
		gpgsv->sat_data[1].azimuth  = atoi(context->token[10]);
		gpgsv->sat_data[1].sig      = atoi(context->token[11]);
		gpgsv->sat_data[2].id       = atoi(context->token[12]);
		gpgsv->sat_data[2].elv      = atoi(context->token[13]);
		gpgsv->sat_data[2].azimuth  = atoi(context->token[14]);
		gpgsv->sat_data[2].sig      = atoi(context->token[15]);
		gpgsv->sat_data[3].id       = atoi(context->token[16]);
		gpgsv->sat_data[3].elv      = atoi(context->token[17]);
		gpgsv->sat_data[3].azimuth  = atoi(context->token[18]);
		gpgsv->sat_data[3].sig      = atoi(context->token[19]);
	}

    kal_prompt_trace(MOD_GPS, "nmeap_gpgsv debug_input=%s",context->debug_input);
	
    /* print some validation data */
    kal_prompt_trace(MOD_GPS, "nmeap_gpgsv input_name,name=%s,%s,icks,ccks=%x,%x",context->input_name,sentence->name,context->icks,context->ccks);
	
    /* print the tokens */
   /*	
    for(i=0;i<context->tokens;i++) {
        kal_prompt_trace(MOD_GPS, "nmeap_gpgsv i,tokens=%d:%s",i,context->token[i]);
    }*/

    /* if the sentence has a callout, call it */
    if (sentence->callout != 0) {
        (*sentence->callout)(context,gpgsv,context->user_data);
    }
    
    return NMEAP_GPGSV;
}

/**  //add by lqy
 * standard BDGSV sentence parser 
 */
int nmeap_bdgsv(nmeap_context_t *context,nmeap_sentence_t *sentence)
{
#ifndef NDEBUG	
    int i;
#endif

    /* get pointer to sentence data */
    nmeap_bdgsv_t *bdgsv = (nmeap_bdgsv_t *)sentence->data;
    
	/* if there is a data element, use it */
	if (bdgsv != 0) {
		/* extract data from the tokens */
		bdgsv->pack_count           = atoi(context->token[1]);
		bdgsv->pack_index           = atoi(context->token[2]);
		bdgsv->sat_count            = atoi(context->token[3]);
		bdgsv->sat_data[0].id       = atoi(context->token[4]);
		bdgsv->sat_data[0].elv      = atoi(context->token[5]);
		bdgsv->sat_data[0].azimuth  = atoi(context->token[6]);
		bdgsv->sat_data[0].sig      = atoi(context->token[7]);
		bdgsv->sat_data[1].id       = atoi(context->token[8]);
		bdgsv->sat_data[1].elv      = atoi(context->token[9]);
		bdgsv->sat_data[1].azimuth  = atoi(context->token[10]);
		bdgsv->sat_data[1].sig      = atoi(context->token[11]);
		bdgsv->sat_data[2].id       = atoi(context->token[12]);
		bdgsv->sat_data[2].elv      = atoi(context->token[13]);
		bdgsv->sat_data[2].azimuth  = atoi(context->token[14]);
		bdgsv->sat_data[2].sig      = atoi(context->token[15]);
		bdgsv->sat_data[3].id       = atoi(context->token[16]);
		bdgsv->sat_data[3].elv      = atoi(context->token[17]);
		bdgsv->sat_data[3].azimuth  = atoi(context->token[18]);
		bdgsv->sat_data[3].sig      = atoi(context->token[19]);
	}

    kal_prompt_trace(MOD_GPS, "nmeap_bdgsv debug_input=%s",context->debug_input);
	
    /* print some validation data */
    kal_prompt_trace(MOD_GPS, "nmeap_bdgsv input_name,name=%s,%s,icks,ccks=%x,%x",context->input_name,sentence->name,context->icks,context->ccks);
	
    /* print the tokens */
   /*	
    for(i=0;i<context->tokens;i++) {
        kal_prompt_trace(MOD_GPS, "nmeap_bdgsv i,tokens=%d:%s",i,context->token[i]);
    }*/

    /* if the sentence has a callout, call it */
    if (sentence->callout != 0) {
        (*sentence->callout)(context,bdgsv,context->user_data);
    }
    
    return NMEAP_BDGSV;
}

/** 
 * standard GNVTG sentence parser 
 */
int nmeap_gnvtg(nmeap_context_t *context,nmeap_sentence_t *sentence)
{
#ifndef NDEBUG	
    int i;
#endif

    /* get pointer to sentence data */
    nmeap_vtg_t *vtg = (nmeap_vtg_t *)sentence->data;
    
	/* if there is a data element, use it */
	if (vtg != 0) {
		/* extract data from the tokens */
		vtg->dir        = atof(context->token[1]);
		vtg->dir_t      = *context->token[2];
		vtg->dec        = atof(context->token[3]);
		vtg->dec_m      = *context->token[4];
		vtg->spn        = atof(context->token[5]);
		vtg->spn_n      = *context->token[6];
		vtg->spk        = atof(context->token[7]);
		vtg->spk_k      = *context->token[8];
	}
     
#ifndef NDEBUG    
    /* print raw input string */
    kal_prompt_trace(MOD_GPS, "nmeap_dngsv debug_input=%s",context->debug_input);
    
    /* print some validation data */
    kal_prompt_trace(MOD_GPS, "nmeap_dngsv input_name,name=%s,%s,icks,ccks=%x,%x",context->input_name,sentence->name,context->icks,context->ccks);
    
    /* print the tokens */
   /*	
    for(i=0;i<context->tokens;i++) {
        kal_prompt_trace(MOD_GPS, "nmeap_dngsv i,tokens=%d:%s",i,context->token[i]);
    }*/
#endif   

    /* if the sentence has a callout, call it */
    if (sentence->callout != 0) {
        (*sentence->callout)(context,vtg,context->user_data);
    }
    
    return NMEAP_GNVTG;
}


/***************************************************************************
  Function:      Bd_Log 
  Description:    write Bd_Log to file
  Calls:          
  Called By:      
  Table Accessed:
  Table Updated:  
  Input:          
                  
  Output:         
  Return:         
  Others:         
  Author:       lqy2014-12-17
****************************************************************************/    
static void Bd_Log(U8 logtype)
{
if(0)// by lqy for disable
{
    Bird_log_type p_soc;
    applib_time_struct dt;
    //char  temp[NMEAP_MAX_SENTENCE_LENGTH+1];
    unsigned short                 file[256];
    S32                 hFile;
    kal_prompt_trace(MOD_GPS,"Bd_Log entry");   
    p_soc.Log_data_len = 0;
    memset(p_soc.Log_buffer, 0, 512);
    applib_dt_get_date_time(&dt);
    sprintf((char*)p_soc.Log_buffer, "[%04d-%02d-%02d %02d:%02d:%02d]", dt.nYear, dt.nMonth,dt.nDay,dt.nHour, dt.nMin, dt.nSec);
    p_soc.Log_data_len = strlen((const char *)p_soc.Log_buffer);
    kal_prompt_trace(MOD_GPS,"Bd_Log data %s",p_soc.Log_buffer);   
   
    /*LOG*/
   if (logtype == 1)
   {
        strcat((char*)p_soc.Log_buffer, " nmeap_gngga:");
        p_soc.Log_data_len += strlen(" nmeap_gngga:");
   }

   if (logtype == 2)
   {
        strcat((char*)p_soc.Log_buffer, " checksum error:");
        p_soc.Log_data_len += strlen(" checksum error:");
   }

   if (logtype == 3)
   {
        strcat((char*)p_soc.Log_buffer, " nmeap_parse error:");
        p_soc.Log_data_len += strlen(" nmeap_parse error:");
   }
   
    strcat((char*)p_soc.Log_buffer, (char*)errinput);
    p_soc.Log_data_len += strlen(errinput);

    strcat((char*)p_soc.Log_buffer, " \r\n");
    p_soc.Log_data_len += strlen(" \r\n");

    kal_prompt_trace(MOD_GPS,"Bd_Log data %s",p_soc.Log_buffer);   
    kal_wsprintf((kal_wchar*) file,BIRD_LOG_FILE_PATH,(S16)MMI_CARD_DRV);
    //对比url和文件名称
    hFile = FS_Open((U16 *)file, FS_CREATE | FS_READ_WRITE);
    if (hFile >= FS_NO_ERROR)
    {    
       
        UINT len;		
        //UINT fsize;
        //FS_GetFileSize(hFile,&fsize);
	 //kal_prompt_trace(MOD_GPS,"Bird_Log fsize=%d",fsize);   
        FS_Seek( hFile, 0, FS_FILE_END);
        FS_Write(hFile, p_soc.Log_buffer, p_soc.Log_data_len, &len);
        FS_Close(hFile);
    }
}
}



//add by lqy


