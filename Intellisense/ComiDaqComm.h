#ifndef __COMIDAS_COMMON_H_
#define __COMIDAS_COMMON_H_

/* COMI-DAQ Device ID */
typedef enum{
	/* CP-Seriese */
	COMI_CP101=0xC101, COMI_CP201=0xC201, COMI_CP301=0xC301, COMI_CP302=0xC302, COMI_CP401=0xC401, COMI_CP501=0xC501, COMI_SD101=0xB101,
	/* SD-Seriese */
	COMI_SD102=0xB102, COMI_SD103=0xB103, COMI_SD104=0xB104, COMI_SD201=0xB201, COMI_SD202=0xB202, COMI_SD203=0xB203, COMI_SD301=0xB301,
	COMI_SD401=0xB401, COMI_SD402=0xB402, COMI_SD403=0xB403, COMI_SD404=0xB404, COMI_SD501=0xB501, COMI_SD502=0xB502, COMI_LX101=0xA101,
	/* LX-Seriese */
	COMI_LX102=0xA102, COMI_LX103=0xA103, COMI_LX201=0xA201, COMI_LX202=0xA202, COMI_LX203=0xA203, COMI_LX301=0xA301, COMI_LX401=0xA401,
	COMI_LX402=0xA402, 
	/* ST-Seriese */
	COMI_ST101=0xD101, COMI_ST201=0xD201, COMI_ST202=0xD202, COMI_ST203=0xD203, COMI_ST301=0xD301, COMI_ST401=0xD401, COMI_ST402=0xD402,
	/* MU-Seriese */
	COMI_MU101=0xE101, COMI_MU201=0xE201, COMI_MU301=0xE301, COMI_MU401=0xE401, COMI_MU402=0xE402, COMI_MU403=0xE403, COMI_MU501=0xE501,
	COMI_MU701=0xE701, 
	/* MB-Seriese */
	MB_DAC101=0x0101,  MB_DAC201=0x0201,  MB_DAC301=0x0301,  MB_DAC401=0x0401,  MB_DAC501=0x0501,  MB_DAC601=0x0601
}TCmDeviceID;

typedef enum {
	cmAI_DIFF=0,  /* Differential connection */
	cmAI_SINGLE=1 /* Single Ended connection */
}TCmAiInputType; /* Analog Input Signal Connection Method */

typedef enum {
	cmTRS_SINGLE=1,/* Single data transfer (low performance) */
	cmTRS_BLOCK=2  /* Block data transfer (high performance) */
}TCmAiScanTrs; /* A/I Scan Data Transfer Method */

typedef enum {
	cmTS_NONE=0,   /* No trigger event                                          */
	cmTS_ANALOG=1, /* Trigger event source => analog input signal(CH0)          */
	cmTS_DIGITAL=2 /* Trigger event source => Digital input signal(Trigger pin) */
}TCmAiEventSrc; /* A/I Scan Trigger Event Source */

typedef enum {
	cmTE_POSITIVE=0,/* Positive edge */
	cmTE_NEGATIVE=1 /* Negative edge */
}TCmEdgeType; /* A/I Scan Trigger Event Signal Edge Type */

typedef enum {
	cmDI_ONLY=0,/* All channels are digital input                                                */
	cmDI_DO=1,  /* The first half channels are input and the latter half channels are for output */
	cmDO_DI=2,  /* The first half channels are output and the latter half channels are for input */
	cmDO_ONLY=3 /* All channels are digital output                                               */
}TCmDioUsage; /* DIO Usage */

typedef enum {
	True=0,
	False=1
}TCmBool; 

typedef enum {
	cmCMODE0=0, 
	cmCMODE1=1,
	cmCMODE2=2, 
	cmCMODE3=3, 
	cmCMODE4=4, 
	cmCMODE5=5
}TCnt16Mode;

typedef enum {
	cmCOUNTER_LATCH=0,
	cmREAD_LOAD_MSB=1,
	cmREAD_LOAD_LSB=2,
	cmREAD_LOAD_WORD=3
}TCmRwMode;

typedef enum {
	cmBINARY=0, /* BCD counter                */
	cmBCD=1 /* Binary counter             */
}TBcdBin;

typedef enum {
	cmENCODER_1X=0, 
	cmENCODER_2X=1,
	cmENCODER_4X=2, 
}TEncMode;

//************* Error codes *************************************//
#define ERR_MEM_ALLOC_FAIL	-290	// Memory allocation fail
#define ERR_ISR_CONNEC_FAIL -310	// ISR registration fail
#define ERR_DIVIDE_BY_ZERO	-400	// Cause divide by zero error 
#define ERR_WORNG_NUM_DATA	-500	// Number of data is too small or too big

#define ERR_INVALID_DEVICE_ID		-1010 // Invalid device id => Load Device 또는 SetDeviceId()에서...
#define ERR_INVALID_HANDLE			-1020
#define ERR_UNSUPORTED_FUNC			-1030 
#define ERR_INVALID_PARAMETER		-1101
#define ERR_INVALID_INPUT_RANGE		-1111 // Invalid range value (AI, AO)
#define ERR_INVALID_FREQ_RANGE		-1121 // Invalid input or output frequency
#define	ERR_FILE_CREATE_FAIL	-1501 // File create fail 
#define ERR_FILE_OPEN_FAIL		-1511 // File open fail

#endif
