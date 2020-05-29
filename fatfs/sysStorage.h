#ifndef _SYS_STORAGE_H
#define _SYS_STORAGE_H

/**********************************************
filename:	sysStorage.h
**********************************************/
#include "publicRsrc.h"
#include "fatfs.h"
/**********************************************
 @ public defines
**********************************************/
#define FILE_BUFF_SZ 1024	//2048
/**********************************************
 @ typedefs
**********************************************/
typedef struct{
	char name[DEV_NAME_LEN];
	FATFS fatFs;
	char* pBuff;
	void (*format)	(void);
}FILE_RSRC_T;

typedef struct{
	FILE_RSRC_T rsrc;
	DEV_STATUS (*WriteEnd)	(FILE_RSRC_T *pRsrc, const char *FILENAME, const char *buf0, const char *buf1);
	DEV_STATUS (*WriteHead)	(FILE_RSRC_T *pRsrc, const char *FILENAME, const char *buf0, const char* buf1);
	s32 (*Read)(FILE_RSRC_T *pRsrc, const char *FILENAME);
	s32 (*GetSize)(FILE_RSRC_T *pRsrc, const char *FILENAME);
	s16 (*ReadBetween)(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL, char *str, u8 strSz);
	s16 (*ReadRecord)(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL, const char* RecName, char *str, u8 strSz);
	s16 (*WriteRecord)(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL, const char* RecName, const char* LINE);
	s8 (*ReadRecordVal)(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL, const char* RecName);
	s8 (*WriteRecordVal)(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL);
	s8 (*ReadLineOf)(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL, char *str, u8 strSz);
	s8 (*Remove)(FILE_RSRC_T *pRsrc, const char *FILENAME);
}FILE_DEV_T;	//sizeof(FILE_DEV_T)3092

//extern FIL file;     /* File object */

DEV_STATUS FileDevSetup(FILE_DEV_T *pDev, void (*format)(void));

#endif
