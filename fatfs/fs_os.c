/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : fs_os.c
* Author             : Tiko Zhong
* Date First Issued  : 11/18/2016
* Description        : 
*
********************************************************************************
* History:
* 11/18/2016: V0.0
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "fs_os.h"
#include "stdarg.h"
#include "fatfsCmd.h"

/* Public variables ---------------------------------------------------------*/
extern osMailQId UartTaskMailId;

/* Private define ------------------------------------------------------------*/
#define FUN_LEN 64
#define FUNC_MAX 128

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	char devName[DEV_NAME_LEN];
	TASK_BONDING_T* p;
}fsTaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const char FS_T[] = {"fs_t"};

/* Private function prototypes -----------------------------------------------*/
static void StartTaskFs(void * argument);
static s8 createTaskFs(osPriority priority, ...);
static s8 createTaskFsByCmd(osPriority priority, void* packet);
fsTaskArgument_t taskArgFs = {0};
TASK_BASE_T TaskInfoFs = {
	FS_T,							//	const u8* DEV_TYPE;
	FS_HELP,					//	const u8* HELP;
	createTaskFs,			//	s8 (*CreateTask)	(u8 argc, ...);
	createTaskFsByCmd,
	0
};

/*******************************************************************************
* Function Name  : StartTaskFs
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void StartTaskFs(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
//	osMailQId UartTaskMailId = devInfo[0].MailId;
	osMailQId owner = NULL;
//	char str[32]={0};
	fsTaskArgument_t* arg = (fsTaskArgument_t*)argument;	
	FILE_DEV_T fs = {0};
	
	//prepare hardware
	FileDevSetup(&fs, NULL);
	//prepare system using
	arg->p->base->count++;		//increase a device count
	//new local mail component
	osMailQDef(x1TaskMail, 1, Mail_T);	
	while(arg->p->MailId == NULL){
		arg->p->MailId = osMailCreate(osMailQ(x1TaskMail), NULL);
		owner = arg->p->MailId;
	}
	devInfo_RegName(arg->p, fs.rsrc.name, arg->devName);
	print("start %s...ok\n", arg->devName);
	
	for(;;){
		//deal mail
		evt = osMailGet(owner, 0);        // wait for mail
		if(evt.status == osEventMail){
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(owner, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				FatfsCmd(&fs, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);  // reply Mail
			}
			else	newSend(owner, pMailRcv->productor, 0,  "%s", "+err@FsOsMailCAllocFfail\r\n");
			osMailFree(owner, pMailRcv);
		}
		osDelay(4);
	}
}

static s8 createTaskFs(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArgFs, 0, sizeof(fsTaskArgument_t));
	for(i=0;i<MAX_INSTANCE;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoFs;
			taskArgFs.p = &devInfo[i];
			break;
		}
	}	
	if(taskArgFs.p == NULL)	return -1;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArgFs.devName, pStr);
	va_end(ap);	

	xTaskCreate( StartTaskFs, "fsTask", 1200, &taskArgFs, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgFs.p->TaskID);		//stack size in 4bytes	
	while(taskArgFs.p->MailId == NULL)	osDelay(1);
	return 0;
}

static s8 createTaskFsByCmd(osPriority priority, void* p){
	PAKET_T* packet = p;
	if(packetIsMatch(packet,"%s%s") && isSameStr(packet->addr[0], FS_T)){
		createTaskFs(priority, packet->addr[1]);
		return 0;
	}
	return -1;
}

////sys.newAdcTask()
//u8 newFsTask(void){
//	if(fsTaskHandle == NULL){
//		osThreadDef(fsTask, StartFsTask, osPriorityNormal, 0, 2000);	//1500
//		fsTaskHandle = osThreadCreate(osThread(fsTask), NULL);
//		if(fsTaskHandle != NULL)	return 1;
//	}
//	return 0;
//}

//static u8 taskCmd(const u8* S, PAKET_T* Content){
//	u8 sn[36];
//	u8* rtnStr;
//	u16 rtnStrSz;
//	CompiLer_Dev_T eDev;
//	CompiLer_Rsrc_T* e;
//	
//	compilerSetup(&eDev);
//	e = &eDev.rsrc;
//	if(eDev.comply(e, S, NULL, 0)<0)	return 0;
//	if(eDev.isMatchDevName(e, "sys") != PASS)	return 0;
//	
//	if(pFsDev == NULL)	return 0;
//	rtnStr = Content->pool;
//	rtnStrSz = PACKE_POOL_SZ;
//	
////	//sys.getsn()
////	strCpy(funStr, FUN_LEN, "sys.getsn");
////	if(isSameStr(e->funName, funStr) == 1 ){
////		if(tmpS8 == 0){
////			if(pFsDev->ReadRecord(&pFsDev->rsrc, "board.ini", NULL, NULL, "sys.sn", sn, 36) >= 0){
////				tmpS8 = thsComplier->Comply(e, sn, NULL, 0);
////				if(tmpS8 == 1){
////					strCpy(rtnStr, rtnStrSz, "+ok@");
////					strCatX(rtnStr, funStr, "(\"", e->params[0]);
////					strCatX(rtnStr, "\")", endStr, NULL);					
////				}
////				else{
////					pFsDev->WriteRecord(&pFsDev->rsrc, "board.ini", NULL, NULL, "sys.sn", "(\"undefined\")");
////					strcat((char*)rtnStr, (const char*)"undefined\")");
////					strcat((char*)rtnStr, (const char*)endStr);
////				}
////			}
////			else{
////				pFsDev->WriteRecord(&pFsDev->rsrc, "board.ini", NULL, NULL, "sys.sn", "(\"undefined\")");
////				strcat((char*)rtnStr, (const char*)"undefined\")");
////				strcat((char*)rtnStr, (const char*)endStr);
////			}				
////		}
////		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, S);
////		return 1;
////	}
//	
//	//sys.setsn()
//	if(eDev.isMatchFuncName(e, "setsn") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,STR) == PASS){	//match format too
//			strCpy(sn,36, "(\"");
//			strCatX(sn, e->params[0], "\")", NULL);
//			if(pFsDev->WriteRecord(&pFsDev->rsrc, "board.ini", NULL, NULL, "sys.sn", sn) >= 0)
//				RESPONSE_X(OK, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], NULL);
//			else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)S);
//		}
//		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, (const char*)S);
//		return 1;
//	}
//	
////	//start.play()
////	strCpy(funStr, FUN_LEN, "start.play");
////	if(isSameStr(e->funName, funStr) == 1 ){
////		if(tmpS8 == 0){
////			startEn = 1;
////			RESPONSE_X(OK, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], NULL);			
////		}
////		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, S);
////		return 1;
////	}
////	//start.stop()
////	strCpy(funStr, FUN_LEN, "start.stop");
////	if(isSameStr(e->funName, funStr) == 1 ){
////		if(tmpS8 == 0){
////			startEn = 0;
////			RESPONSE_X(OK, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], NULL);
////		}
////		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, S);
////		return 1;
////	}
////	//start.reset()
////	strCpy(funStr, FUN_LEN, "start.reset");
////	if(isSameStr(e->funName, funStr) == 1 ){
////		if(tmpS8 == 0){
////			complyScript(AUTO_RUN_FILE);
////			startEn = 1;
////			start.rsrc.pc = 0;
////			RESPONSE_X(OK, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], NULL);
////		}
////		else	RESPONSE_X(ERR, rtnStr, rtnStrSz, NULL, NULL, S);
////		return 1;
////	}
//	
//	return 0;
//}

////128@MAX LABEL MARK
////1024@MAX LINE
//static s16 complyScript(const char* FILE){
//	FIL* pf = &file;     /* File object */	
//	u16 beforeRead;
//	u16 bytesRead,i,j,labelIndx,pc;
//	u8 buff0[100],label[16],tmp81,tmp82;
//	
//	s16 s1;	//tmp
//	s16 indx0;	//index of first char of a line
//	s16 indx1;	//index of first char of "//"
//	s16 indx2;	//index of first char of ":"
//	s16 indx3;	//index of first char of ";"
//	s16 indx0_;	//index of first char of a line


//	if(f_open(pf, FILE, FA_READ) == FR_OK){
//		//ERASE SECTORS
////		for(i=EXE_LINE_TAB_INDX;i<EXE_LINE_TAB_COUNT;i++)	W25Q32.Sector4kErase(&W25Q32.rsrc, i);
//		for(i=0;i<CMD_INFO_TAB_COUNT;i++){
//			pW25Q32->Sector4kErase(&pW25Q32->rsrc, i+CMD_INFO_TAB_INDX);
//			osDelay(5);
//		}
//		for(i=0;i<LBL_INFO_TAB_COUNT;i++){
//			pW25Q32->Sector4kErase(&pW25Q32->rsrc, i+LBL_INFO_TAB_INDX);
//			osDelay(5);
//		}
//		//initial compute, count how many labels, how many lines
//		f_lseek(pf, 0);
//		beforeRead = 0;
//		for(labelIndx=0,pc=0;;){
//			memset(buff0, 0, 100);
//			beforeRead = f_tell(pf);
//			f_read(pf, buff0, 99, (UINT*)&bytesRead);
//			if(bytesRead == 0)	break;
//			//fetch lines
//			indx0=0;
//			for(indx0=0;;){
//				s1 = indexOf_L(&buff0[indx0], bytesRead-indx0, "\r\n");
//				//point to next line in file
//				if(s1 < 0 ){
//					f_lseek(pf, beforeRead+indx0);
//					break;
//				}
//				
//				indx0_ = indx0;	//backup indx0
//				indx0 += s1+2;	//this is the next line indx in buff0		
//				if(s1 == 0)	continue;

//				indx1 = indexOf_L(&buff0[indx0_], indx0-indx0_, "//");
//				if(indx1 == 0)	continue;
//				//if there is a "//"
//				if(indx1>0){
//					indx1 += indx0_;
//					indx2 = indexOf_L(&buff0[indx0_], indx1-indx0_, ":");
//					indx3 = indexOf_L(&buff0[indx0_], indx1-indx0_, ";");
//				}
//				else{
//					indx2 = indexOf_L(&buff0[indx0_], indx0-indx0_, ":");
//					indx3 = indexOf_L(&buff0[indx0_], indx0-indx0_, ";");		
//				}
//				if(indx2==0 || indx3==0)	continue;
//				if(indx2>0 && indx3>0){
//					if(indx2 > indx3)	continue;
//				}
//				
//				//if there is a ":"
//				if(indx2>0){
//					indx2 += indx0_;
//					memset(label,0,16);
//					for(i=indx0_,tmp81=0,tmp82=0;i<=indx2;i++){
//						if(buff0[i] == ':' || buff0[i] == 0 || tmp81>=14)	break;
//						if(tmp82){
//							if(buff0[i]>=33 && buff0[i]<=126){
//								label[tmp81++] = buff0[i];
//							}
//							else
//								break;
//						}
//						else{
//							if(buff0[i]>=33 && buff0[i]<=126){
//								label[tmp81++] = buff0[i];
//								tmp82 = 1;
//							}
//						}
//					}
//					wrtLabelInfo(labelIndx++, pc, label);
//				}
//				//if there is a ";"
//				if(indx3>0){
//					indx3 += indx0_;
//					i = (indx2>0?(indx2+1):indx0_);
//					j = indexOfFirstPrntChar(&buff0[i]);
//					if(j && (j+i)<indx3){
//						j += i;
//						wrtCommandInfo(pc, j+beforeRead, indx3-j);
//						pc++;
//					}
//				}
//			}
//			if(bytesRead < 99)	break;	//read to the last line
//		}
//		f_close(&file);
//	}
//	else{
//		s1 = -1;
////		dev.PrintStr(&dev.Rsrc,"open file fail\r\n");
//	}
//	return s1;
//}

////8 bytes in a label line:	SSSSSSSSSSSSSSNN
////SSSSSSSSSSSSSS: label
////NN: pC index in command line info table
//static s8 wrtLabelInfo(u16 indx, u16 pcIndx, const u8* LABEL){
//	u8 tmp[16]={0};
//	u32 addr_byte;
//	u16 pc = pcIndx;
//	
//	if(indx >= 512)	return -1;
//	
//	strCpy(tmp,14,LABEL);
////strCpy(s,32,tmp);
////strcat((char*)s,":");
////CONVERT.Uint32ToStrCatDecN(s,indx,0);
////MAIL.Send_SSS(FsTaskMailId, UartTaskMailId, SUB_PRNT_ONLY, "+wrtLabel@",s,"\r\n");osDelay(16);

//	tmp[14] = pc&0xff;	pc >>=8;
//	tmp[15] = pc&0xff;
//	addr_byte = LBL_INFO_TAB_INDX*(W25QXX_BYTES_PER_PG*W25QXX_PAGES_PER_SECTOR)+indx*16;
//	pW25Q32->PageWrite(&pW25Q32->rsrc, tmp, addr_byte, 16);
//	return 0;
//}

////4 bytes in a command line:	NNLx
////NN: command line index of file, 16bit
////L: command line lenght
//static s8 wrtCommandInfo(u16 pcIndx, u16 commandIndx, u8 commandLen){
//	u8 tmp[4]={0};
//	u32 addr_byte;
////	u16 pc = pcIndx;
//	u16 cmdIndx = commandIndx;
//	
//	if(pcIndx >= 4096)	return -1;
//	
//	tmp[0] = cmdIndx&0xff;	cmdIndx >>=8;
//	tmp[1] = cmdIndx&0xff;	
//	tmp[2] = commandLen;
//	addr_byte = CMD_INFO_TAB_INDX*(W25QXX_BYTES_PER_PG*W25QXX_PAGES_PER_SECTOR)+pcIndx*4;
//	pW25Q32->PageWrite(&pW25Q32->rsrc,tmp,addr_byte,4);
//	return 0;
//}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
