/**********************************************
filename:	sysStorage.c
**********************************************/
#include "sysStorage.h"
#include "STM32FLASHDISK.h"
//#include "myuart_os.h"
extern osMailQId UartTaskMailId;
extern osMailQId FsTaskMailId;

static DEV_STATUS writeEnd(FILE_RSRC_T *pRsrc, const char *FILENAME, const char *str0, const char *str1);
static DEV_STATUS writeHead(FILE_RSRC_T *pRsrc, const char *FILENAME, const char *str, const char* str1);
s32 load(FILE_RSRC_T *pRsrc, const char *FILENAME);
static s32 read(FILE_RSRC_T *pRsrc, const char *FILENAME);
static s32 getSize(FILE_RSRC_T *pRsrc, const char *FILENAME);
static s16 readBetween(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* END, char *str, u8 strSz);
static s16 readLineBtn(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* END, const char* Begin, char *str, u8 strSz);
static s16 writeLineBtn(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL, const char* BEGIN, const char *LINE);

static s8 readRecValFromFile(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL, const char* RecName);
static s8 writeRecValToFile(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL);

static s8 removeFile(FILE_RSRC_T *pRsrc, const char *FILENAME);

//DEV_STATUS fetchRecord(FILE_RSRC_T *pRsrc, const u8 *FILENAME, const u8* beginWith, u8 *str, u8 strSz);
//DEV_STATUS writeRecord(FILE_RSRC_T *pRsrc, const u8 *FILENAME, const u8* beginWith, const u8 *str);
//s16 readSegLine(FILE_RSRC_T *pRsrc, const u8 *FILENAME, const u8* SEG_FR, const u8* SEG_TO, const u8* BEGIN, u8 *str, u8 strSz);
//s8 readLineOf(FILE_RSRC_T *pRsrc, const u8 *FILENAME, const u8* HEAD, const u8* TAIL, u8 indx, u8 *str, u16 strSz);
//s16 readBetweenBetween(FILE_RSRC_T *pRsrc, const u8 *FILENAME, const u8* H0, const u8* E0,  const u8* H1, const u8* E1, u8 *str, u8 strSz);
static char buff[FILE_BUFF_SZ];	//__align(4)
FIL file;     /* File object */
/*******************************************************************************
* Function Name  : sysStorageSetup
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
DEV_STATUS FileDevSetup(FILE_DEV_T *pDev, void (*format)(void)){
//	FIL file;     /* File object */	
	FRESULT res;
//	u16 bytesWrite;
	
	pDev->rsrc.pBuff = buff;
	pDev->rsrc.format = format;
	pDev->WriteHead = writeHead;
	pDev->WriteEnd = writeEnd;
	pDev->Read = read;
	pDev->GetSize = getSize;
	pDev->ReadBetween = readBetween;
	pDev->ReadRecord = readLineBtn;
	pDev->WriteRecord = writeLineBtn;
	pDev->ReadRecordVal = readRecValFromFile;
	pDev->WriteRecordVal = writeRecValToFile;
//	pDev->ReadLineOf = readLineOf;
	pDev->Remove = removeFile;

	res = f_mount(&pDev->rsrc.fatFs, (TCHAR const*)"0:", 1);
	if(res != FR_OK){
		if(res == FR_NO_FILESYSTEM){
//			if(f_mkfs((TCHAR const*)"0:", 1, 2048) != FR_OK)	return DEV_ERROR;	 /* USER_Path: USER logical drive path */
//			if(f_open(&file, (const char*)"start.txt", FA_WRITE|FA_OPEN_ALWAYS) == FR_OK){
//				f_write(&file, "//to run a loop script\r\n", strlen("//to run a loop script\r\n"), (UINT*)&bytesWrite);
//				f_close(&file);
//			}
//			else{	return DEV_ERROR;	}
		}
		else	return DEV_ERROR;
	}
	
	return DEV_SUCCESS;
}

/*******************************************************************************
* Function Name  : writetoend
* Description    : 
* Input          : None
* Output         : None
* Return         : CYLDR_TIMEOUT: timeout		CYLDR_GOTTO:reach pos within timeout
*******************************************************************************/
static DEV_STATUS writeEnd(FILE_RSRC_T *pRsrc, const char *FILENAME, const char *str0, const char *str1){
//	FIL file;     /* File object */
	u16 bytesWrite;

	if(f_open(&file, (const char*)FILENAME, FA_WRITE|FA_OPEN_ALWAYS) != FR_OK )	
		return DEV_ERROR;
	f_lseek (&file, file.fsize);
	if(str0 != NULL)	f_write(&file, str0, strlen((char*)str0), (UINT*)&bytesWrite);
	if(str1 != NULL)	f_write(&file, str1, strlen((char*)str1), (UINT*)&bytesWrite);
	f_close(&file);
	return DEV_SUCCESS;
}

static DEV_STATUS writeHead(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* str0, const char* str1){
//	FIL file;     /* File object */
	u16 bytesRead, bytesWrite, strL,strL0,strL1, posRd;
	
	strL0 = strlen((const char*)str0);
	strL1 = strlen((const char*)str1);
	strL = strL0+strL1;
	
	if(f_open(&file, (const char*)FILENAME, FA_READ|FA_WRITE|FA_OPEN_ALWAYS) != FR_OK)	return DEV_ERROR;
	posRd = f_size(&file);
	while(1){
		if(posRd == 0){
			f_lseek(&file, posRd);
			if(str0 != NULL)	f_write(&file, str0, strL0, (UINT*)&bytesWrite);
			if(str1 != NULL)	f_write(&file, str1, strL1, (UINT*)&bytesWrite);
			break;
		}
		else{	
			if(posRd >= FILE_BUFF_SZ)	posRd -= FILE_BUFF_SZ;
			else posRd = 0;
		}	
		f_lseek(&file, posRd);
		memset(pRsrc->pBuff, 0, FILE_BUFF_SZ);
		f_read(&file, pRsrc->pBuff, FILE_BUFF_SZ, (UINT*)&bytesRead);
		f_lseek(&file, posRd+strL);
		f_write(&file, pRsrc->pBuff, bytesRead, (UINT*)&bytesWrite);
	}
	f_close(&file);
	return DEV_SUCCESS;
}

/*******************************************************************************
* Function Name  : readFile
* Description    : 
* Input          : None
* Output         : None
* Return         : CYLDR_TIMEOUT: timeout		CYLDR_GOTTO:reach pos within timeout
*******************************************************************************/
static s32 read(FILE_RSRC_T *pRsrc, const char *FILENAME){
	u16 bytesRead;
//	FIL file;     /* File object */
	
	if(f_open(&file, (const char*)FILENAME, FA_READ) == FR_OK){
		f_rewind(&file);
		memset(pRsrc->pBuff, 0, FILE_BUFF_SZ);
		f_read(&file, pRsrc->pBuff, FILE_BUFF_SZ, (UINT*)&bytesRead);
		f_close(&file);
		return bytesRead;
	}
	else	return -1;
}

static s32 getSize(FILE_RSRC_T *pRsrc, const char *FILENAME){
	s32 sz=-1;
//	FIL file;     /* File object */
	if(f_open(&file, (const char*)FILENAME, FA_READ) == FR_OK){
		sz = f_size(&file);
		f_close(&file);
	}
	return sz;
}

/*******************************************************************************
* Function Name  : readBetween
* Description    : 
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/
static s16 readBetween(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD0, const char* END0, char *str, u8 strSz){
//	FIL file;     /* File object */
	char *buff;
	u16 bytesRead;
	s16 btw;
	
	buff = pRsrc->pBuff;	memset(buff, 0, FILE_BUFF_SZ);
	if(f_open(&file, (const char*)FILENAME, FA_READ) == FR_OK){
		f_rewind(&file);
		while(1){
			f_read(&file, buff, FILE_BUFF_SZ, (UINT*)&bytesRead);
			btw = strBetween(buff, HEAD0, END0, str, strSz);
			if(btw > 0){	f_close(&file);	return btw;}
			if(bytesRead < FILE_BUFF_SZ){	f_close(&file);		return -2;}		
		}
	}
	return -1;
}

static s16 readLineBtn(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL, const char* BEGIN, char *str, u8 strSz){
//	FIL file;     /* File object */
	char *buf0,*buf1,*buf2,*tmp;
	u8 stage;
	u16 bytesRead,step;
	s16 btw;

	step = FILE_BUFF_SZ/3;
	buf0 = &pRsrc->pBuff[step*0];
	buf1 = &pRsrc->pBuff[step*1];
	buf2 = &pRsrc->pBuff[step*2];
	stage = 0;

	if(f_open(&file, (const char*)FILENAME, FA_READ) == FR_OK){
		f_rewind(&file);
		while(1){
			strCpy(buf0, step, buf1);
			strCpy(buf1, step, buf2);
			memset(buf2,0,step);
			
			if(stage == 0)			tmp = buf2;
			else if(stage == 1)	tmp = buf1;
			else 								tmp = buf0;
			
			if(f_read(&file, buf2, step, (UINT*)&bytesRead) == FR_OK){
				btw = strBetweenLine(tmp, HEAD, TAIL, BEGIN, str, strSz);
				stage++;	stage &= 0x7f;
				if(btw >= 0){	f_close(&file);	return btw;}
				if(bytesRead < step){	f_close(&file);		return -2;}		//end of file					
			}
			else{	f_close(&file);		return -3;	}
		}
	}
	return -1;
}

//LINE should end with "\r\n", file should less than 2K byte
static s8 newRecord(char *pSrc, u16 pSrcSz, const char* HEAD, const char* TAIL, const char *RecName, const char *RecContent){
	s16 indxTAIL,indxRecName;
	u16 len0,len1,len2,i;
//	CONVERT_T cvnt;
//	u8 tmp[64]={0};
	
//	convertSetup(&cvnt);
	
	indxRecName = indexOf_Pair(pSrc, HEAD, TAIL, RecName);
	if(indxRecName < 0)	{	//
		if(isPair(pSrc, HEAD, TAIL) == 0){	//add HEAD..REC..TAIL
			strCatX(pSrc, HEAD, "\r\n", NULL);
			strCatX(pSrc, RecName, RecContent, "\r\n");
			strCatX(pSrc, TAIL, "\r\n", NULL);
			return 0;
		}
		else{	//only add REC
			indxTAIL = indexOf_HEAD(pSrc, HEAD, TAIL);
			if(indxTAIL < 0)	return -1;
			if(strInsert(pSrc, pSrcSz, indxTAIL, RecName, RecContent, "\r\n") <0 )	return -1;
		}
	}
	else{	//do replace
		if(TAIL==NULL)	indxTAIL = strlen((const char*)pSrc)-1;
		else	indxTAIL = indexOf_HEAD(pSrc, HEAD, TAIL);
		
		len0 = strlen((const char*)RecName);
		len1 = strlen((const char*)RecContent);

		for(len2=0,i=indxRecName+len0;i<indxTAIL;i++){
			if(pSrc[i] == '\r')	break;
			len2 ++;
		}
		
		if(len1>len2)	strMoveRightN(&pSrc[indxRecName+len0],(len1-len2));
		else if(len1<len2)	strMoveLeftN(&pSrc[indxRecName+len0],(len2-len1));
		
		for(i=0;i<len1;i++){
			pSrc[i+indxRecName+len0] = RecContent[i];
		}
	}
	return 0;
}
	
static s16 writeLineBtn(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL, const char* BEGIN, const char *LINE){
//	FIL file;     /* File object */
	u16 bytes,pos0,maxByteWrt;
//	__align(4) u8 tmp[20];

	
	if(f_open(&file, (const char*)FILENAME, FA_READ|FA_WRITE|FA_OPEN_ALWAYS) == FR_OK){
		//only read last 2K bytes
		memset(pRsrc->pBuff,0,FILE_BUFF_SZ);
		if(file.fsize < FILE_BUFF_SZ)	pos0 = 0;
		else	pos0 = file.fsize - FILE_BUFF_SZ;
		if( f_lseek(&file, pos0) != FR_OK)	{f_close(&file); return -1;}
		if(f_read(&file, pRsrc->pBuff, FILE_BUFF_SZ, (UINT*)&bytes) != FR_OK)	{f_close(&file); return -2;}
		//LINE replace record that begin with BEGIN
		maxByteWrt = strlen((const char*)pRsrc->pBuff);
//strCpy(tmp,20,"max=");	
//CONVERT.Uint32ToStrCatDecN(tmp,maxByteWrt,0);
//strcat((char*)tmp," ");
		
		if( newRecord(pRsrc->pBuff, FILE_BUFF_SZ, HEAD, TAIL, BEGIN, LINE) < 0)	{f_close(&file); return -3;}
		if( f_lseek(&file, pos0) != FR_OK)	{f_close(&file); return -1;}
		bytes = strlen((const char*)pRsrc->pBuff);
		
//CONVERT.Uint32ToStrCatDecN(tmp,bytes,0);
//strcat((char*)tmp," ");

		if(bytes > maxByteWrt)		maxByteWrt = bytes;
		
//CONVERT.Uint32ToStrCatDecN(tmp,maxByteWrt,0);
//MAIL.Send_SSS(FsTaskMailId, UartTaskMailId, 0xff, tmp,"\r\n",NULL);
		
		if(f_write(&file, pRsrc->pBuff, maxByteWrt, (UINT*)&bytes) != FR_OK)	{f_close(&file); return -4;}
		f_close(&file);
		return 0;
	}
	return -1;
}

/*******************************************************************************
* Function Name  : readRecValFromFile
* Description    : 
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/
static s8 readRecValFromFile(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL, const char* RecName){
	s8 tmpS8;
//	u8 str[PACKE_POOL_SZ] = {0};
//	if(readLineBtn(pRsrc, FILENAME, HEAD, TAIL, RecName, str, PACKE_POOL_SZ) <0 ) 	return -1;
//	tmpS8 = pRsrc->rec.Comply(&pRsrc->rec.rsrc, str, NULL, 0);
//	if( tmpS8< 0)	return -2;
	return tmpS8;
}

/*******************************************************************************
* Function Name  : writeRecValToFile
* Description    : 
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/
static s8 writeRecValToFile(FILE_RSRC_T *pRsrc, const char *FILENAME, const char* HEAD, const char* TAIL){
	s16 tmpS16;
//	PARAM_RSRC_T* e = &pRsrc->rec.rsrc;
//	u8 str[128],i;
//	//make record string
//	strCpy(str, 128, "(");
//	for(i=0;i<PARAM_SUM;i++){
//		if(e->params[i] == NULL)	break;;
//		if(e->format & (u32)1<<i)	strcat((char*)str, (char*)e->params[i]);
//		else	CONVERT.Uint32ToStrCatDecN(str, *(u32*)e->params[i], 0);
//		strcat((char*)str, (const char*)",");
//	}
//	strcat((char*)str, (const char*)")");
//	strReplace(str,128,",)",")");
//	//write it in file
//	tmpS16 = writeLineBtn(pRsrc, FILENAME, HEAD, TAIL, e->funName, str);
	//return 
	if(tmpS16 >= 0)	return 0;
	else return -1;
}

//s8 readLineOf(FILE_RSRC_T *pRsrc, const u8 *FILENAME, const u8* HEAD, const u8* TAIL, u8 indx, u8 *str, u16 strSz){
////	FIL file;     /* File object */
//	u8 *buf0,*buf1,*buf2,*tmp,stage;
//	u16 bytesRead,step,i,j;
//	s16 btw,x;

//	step = FILE_BUFF_SZ/3;
//	buf0 = &pRsrc->buff[step*0];
//	buf1 = &pRsrc->buff[step*1];
//	buf2 = &pRsrc->buff[step*2];
//	stage = 0;

//	if(f_open(&file, (const char*)FILENAME, FA_READ) == FR_OK){
//		f_rewind(&file);
//		while(1){
//			strCpy(buf0, step, buf1);
//			strCpy(buf1, step, buf2);
//			memset(buf2,0,step);
//			
//			if(stage == 0)			tmp = buf2;
//			else if(stage == 1)	tmp = buf1;
//			else 								tmp = buf0;
//			
//			if(f_read(&file, buf2, step, (UINT*)&bytesRead) == FR_OK){
//				btw = strBetween(tmp, HEAD, TAIL, pRsrc->buff, FILE_BUFF_SZ);
//				stage++;	stage &= 0x7f;
//				if(btw >= 0){	
//					f_close(&file);	
//					for(i=0,j=0;1;){
//						x = indexOf(&str[i], "\r\n");
//						if(x >= 0){
//							if(++j==indx){
//								;
//							}
//							i += (x+2);	//POINT TO END OF LINE
//						}
//						else{	
//							return -4;
//						}
//					}

//					for(i=0;i<strSz;i++){
//						if(str[i] == 0)	break;

//					}
//					return btw;
//				}
//				if(bytesRead < step){	f_close(&file);		return -2;}		//end of file
//			}
//			else{	f_close(&file);		return -3;	}
//		}
//	}
//	return -1;
//}

/*******************************************************************************
* Function Name  : _writeLineBtn
* Description    : 
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/
//work on large size file
//s16 _writeLineBtn(FILE_RSRC_T *pRsrc, const u8 *FILENAME, const u8* HEAD, const u8* TAIL, const u8* BEGIN, const u8 *LINE){
//	FIL file;     /* File object */
//	u8 *buf0,*buf1,*buf2,*tmp,stage;
//	u16 bytesRead,step,pos0,pos1,pos2,pos3,indx0,indx1,indx2,indx3,len;
//	s16 btw,posOffset;

//	if(f_open(&file, (const char*)FILENAME, FA_READ|FA_WRITE|FA_OPEN_ALWAYS) == FR_OK){
//		//if()	// file missing
//		step = FILE_BUFF_SZ/3;
//		buf0 = &pRsrc->buff[step*0];
//		buf1 = &pRsrc->buff[step*1];
//		buf2 = &pRsrc->buff[step*2];
//		stage = 0;
//		memset(pRsrc->buff,0,FILE_BUFF_SZ);
//		
//		f_rewind(&file);
//		while(1){
//			strCpy(buf0, step, buf1);
//			strCpy(buf1, step, buf2);
//			memset(buf2,0,step);
//			
//			if(stage == 0)			tmp = buf2;
//			else if(stage == 1)	tmp = buf1;
//			else 								tmp = buf0;
//			
//			pos0 = file.fptr;
//			if(f_read(&file, buf2, step, (UINT*)&bytesRead) == FR_OK){
//				pos1 = file.fptr;

//				if(meetBtnLine(tmp, &indx0, &indx1, &indx2, &indx3) >= 0){	//if meet record
//					//indx0: HEAD's index
//					//indx1: BEGIN's index
//					//indx2: "\r\n"'s index
//					//indx3: TAIL's index
//					len = strlen((const char*)LINE);
//					posOffset = len-(indx2-indx1+2);
////					moveFileStr(&file, pos0+indx1, posOffset);
//				}

//				
//				
//				
//				stage++;	stage &= 0x7f;
//				if(btw >= 0){	f_close(&file);	return btw;}
//				if(bytesRead < step){	f_close(&file);		return -2;}		//end of file		
//			}
//			else{	f_close(&file);		return -3;	}
//		}
//	}
//	return -1;
//}

/*******************************************************************************
* Function Name  : removeFile
* Description    : to delete a file
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/
static s8 removeFile(FILE_RSRC_T *pRsrc, const char *FILENAME){
	if(f_unlink ((const TCHAR*)FILENAME) == FR_OK)	return 0;
	return -1;
}

