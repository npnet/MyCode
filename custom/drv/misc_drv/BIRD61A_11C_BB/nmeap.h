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

* 
 * 
 * modify by lqy for mtk and BD platform 2014-11-20
 *
 *

*/

#ifndef __NMEAP_H__
#define __NMEAP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* 
============================================
COMPILE TIME CONFIGURATION CONSTANTS
============================================
*/

/* these constants affect the size of the context object. tweak them as desired but know what you are doing */

/** maximum number of sentence parsers supported */
#define NMEAP_MAX_SENTENCES            8
/** length of sentence name. leave this at 5 unless you really know what you are doing */
#define NMEAP_MAX_SENTENCE_NAME_LENGTH 5
/** max length of a complete sentence. the standard says 82 bytes, but its probably better to go at least 128 since
 * some units don't adhere to the 82 bytes especially for proprietary sentences */
#define NMEAP_MAX_SENTENCE_LENGTH      255
/** max tokens in one sentence. 24 is enough for any standard sentence */
#define NMEAP_MAX_TOKENS               24
//add by lqy max SAT for bd and gps
#define NMEA_MAXSAT       24
#define NMEA_SATINPACK      4

/* predefined message ID's */

/* GGA MESSAGE ID */
#define NMEAP_GNGGA 1
/* RMC MESSAGE ID */
#define NMEAP_GNRMC 2

//add by lqy
/* GPGSV MESSAGE ID */
#define NMEAP_GPGSV 3
/* BDGSV MESSAGE ID */
#define NMEAP_BDGSV 4
/* GNVTG MESSAGE ID */
#define NMEAP_GNVTG 5
//add by lqy
#define NMEAP_GNTXT 6

/** user defined parsers should make ID numbers using NMEAP_USER as the base value, plus some increment */
#define NMEAP_USER  100

/* forward references */
struct nmeap_context;
struct nmeap_sentence;

/* 
============================================
CALLOUTS
============================================
*/

/**
 * sentence callout function type
 * a callout is fired for each registered sentence type
 * the callout gets the object context and a pointer to sentence specific data.
 * the callout must cast the 'sentence_data' to the appropriate type for that callout
 * @param context			nmea object context
 * @param sentence_data		sentence specific data
*/ 
typedef void (*nmeap_callout_t)(struct nmeap_context *context,void *sentence_data,void *user_data);

/**
 * sentence parser function type
 * stored in the object context and called internally when the sentence name matches
 * the specified value
 * the callout gets the object context and a pointer to sentence specific data.
 * the callout must cast the 'sentence_data' to the appropriate type for that callout
 * @param context			nmea object context
 * @param sentence_data		sentence specific data
 * @return id of sentence  (each sentence parser knows its own ID)
*/ 
typedef int (*nmeap_sentence_parser_t)(struct nmeap_context *context,struct nmeap_sentence *sentence);


/* ==== opaque types === */
#include "nmeap_def.h"


/* 
============================================
STANDARD SENTENCE DATA STRUCTURES
============================================
*/

/** extracted data from a TXT message */
struct nmeap_txt {
	int hed_0;
	int hed_1;
	int hed_2;
	char hed_string[12];
};
typedef struct nmeap_txt nmeap_txt_t;

/** extracted data from a GGA message */
struct nmeap_gga {
	double        latitude;
	double        longitude;
	double        altitude;
	unsigned long time;
	int           satellites;
	int           quality;
	kal_int8 east_west;
	kal_int8 north_south;
	double        hdop;
	double        geoid;
};
typedef struct nmeap_gga nmeap_gga_t;

/** extracted data from an RMC message */
struct nmeap_rmc {
	unsigned long time;
	char          warn;
	double        latitude;
	double        longitude;
	double        speed;
	double        course;
	unsigned long date;
	double        magvar;
};

typedef struct nmeap_rmc nmeap_rmc_t;

//add by lqy

/** * Information about satellite * @see nmeaSATINFO * @see nmeaGPGSV */
typedef struct _nmeaSATELLITE {    
       int     id;         /**< Satellite PRN number */    
	int     in_use;     /**< Used in position fix */    
	int     elv;        /**< Elevation in degrees, 90 maximum */    
	int     azimuth;    /**< Azimuth, degrees from true north, 000 to 359 */    
	int     sig;        /**< Signal, 00-99 dB */
} nmeaSATELLITE;

/** extracted data from an GPGSV message */
struct nmeap_gpgsv {
	int     pack_count; /**< Total number of messages of this type in this cycle */    
	int     pack_index; /**< Message number */    
	int     sat_count;  /**< Total number of satellites in view */    
	nmeaSATELLITE sat_data[NMEA_SATINPACK];
};

typedef struct nmeap_gpgsv nmeap_gpgsv_t;

struct nmeap_bdgsv {
	int     pack_count; /**< Total number of messages of this type in this cycle */    
	int     pack_index; /**< Message number */    
	int     sat_count;  /**< Total number of satellites in view */    
	nmeaSATELLITE sat_data[NMEA_SATINPACK];
};

typedef struct nmeap_bdgsv nmeap_bdgsv_t;

struct nmeap_vtg{    
	double  dir;        /**< True track made good (degrees) */    
	char    dir_t;      /**< Fixed text 'T' indicates that track made good is relative to true north */    
	double  dec;        /**< Magnetic track made good */    
	char    dec_m;      /**< Fixed text 'M' */    double  spn;        /**< Ground speed, knots */    
	char    spn_n;      /**< Fixed text 'N' indicates that speed over ground is in knots */    
	double  spk;        /**< Ground speed, kilometers per hour */    
	char    spk_k;      /**< Fixed text 'K' indicates that speed over ground is in kilometers/hour */
};

typedef struct nmeap_vtg nmeap_vtg_t;
//add by lqy


/* 
============================================
METHODS
============================================
*/

static void Bd_Log(); //by lqy
 

/**
 * initialize an NMEA parser. call this function to initialize a user allocated context object
 * @param context 		nmea object context. allocated by user statically or dynamically.
 * @param user_data 	pointer to user defined data
 * @return 0 if ok, -1 if initialization failed
 */
int nmeap_init(nmeap_context_t *context,void *user_data);

/**
 * register an NMEA sentence parser
 * @param context 		   nmea object context
 * @param sentence_name    string matching the sentence name for this parser. e.g. "GPGGA". not including the '$'
 * @param sentence_parser  parser function for this sentence
 * @param sentence_callout callout triggered when this sentence is received and parsed. 
 *                         if null, no callout is triggered for this sentence
 * @param sentence_data    user allocated sentence specific data defined by the application. the parser uses
                           this data item to store the extracted data. This data object needs to persist over the life
						   of the parser, so be careful if allocated on the stack. 
 * @return 0 if registered ok, -1 if registration failed
 */
int nmeap_addParser(nmeap_context_t         *context,
					 const char             *sentence_name,
					 nmeap_sentence_parser_t sentence_parser,
					 nmeap_callout_t         sentence_callout,
					 void                   *sentence_data
					 );

/** 
 * parse a buffer of nmea data.
 * @param context 		   nmea object context
 * @param buffer          buffer of input characters
 * @param length          [in,out] pointer to length of buffer. on return, contains number of characters not used for
 *                        the current sentence
 * @return -1 if error, 0 if the data did not complete a sentence, sentence code if a sentence was found in the stream
 */
int nmeap_parseBuffer(nmeap_context_t *context,const char *buffer,int *length);

/** 
 * parse one character of nmea data.
 * @param context 		   nmea object context
 * @param ch              input character
 * @return -1 if error, 0 if the data did not complete a sentence, sentence code if a sentence was found in the stream  
 */
int nmeap_parse(nmeap_context_t *context,char ch);

/** 
 * built-in parser for TXT sentences.
 * @param context 		   nmea object context
 * @param sentence         sentence object for this parser
  */
int nmeap_gntxt(nmeap_context_t *context,nmeap_sentence_t *sentence);
/** 
 * built-in parser for GGA sentences.
 * @param context 		   nmea object context
 * @param sentence         sentence object for this parser
  */
int nmeap_gngga(nmeap_context_t *context,nmeap_sentence_t *sentence);

/**   
 * built-in parser for RMC sentences.
 * @param context 		   nmea object context
 * @param sentence         sentence object for this parser
 */
int nmeap_gnrmc(nmeap_context_t *context,nmeap_sentence_t *sentence);

/**  add by lqy
 * built-in parser for BD GSV sentences.
 * @param context 		   nmea object context
 * @param sentence         sentence object for this parser
 */
int nmeap_bdgsv(nmeap_context_t *context,nmeap_sentence_t *sentence);


/**  add by lqy
 * built-in parser for GPS GSV sentences.
 * @param context 		   nmea object context
 * @param sentence         sentence object for this parser
 */
int nmeap_gpgsv(nmeap_context_t *context,nmeap_sentence_t *sentence);

/**  add by lqy
 * built-in parser for  VTG sentences.
 * @param context 		   nmea object context
 * @param sentence         sentence object for this parser
 */
int nmeap_gnvtg(nmeap_context_t *context,nmeap_sentence_t *sentence);

/**
 * extract latitude from 2 tokens in ddmm.mmmm,h format.
 * @param plat pointer to token with numerical latitude
 * @param phem pointer to token with hemisphere
 * @return latitude in degrees and fractional degrees
 */
double nmeap_latitude(const char *plat,const char *phem);


/**
 * extract longitude from 2 tokens in ddmm.mmmm,h format.
 * @param plat pointer to token with numerical longitude
 * @param phem pointer to token with hemisphere
 * @return longitude in degrees and fractional degrees
 */
double nmeap_longitude(const char *plat,const char *phem);

#ifdef __cplusplus
} // extern C
#endif


#endif

