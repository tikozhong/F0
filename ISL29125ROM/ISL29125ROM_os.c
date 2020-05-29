/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : isl29125rom.c
* Author             : Tiko Zhong
* Date First Issued  : 12/28/2016
* Description        : 
*                      
********************************************************************************
* History:
* 12/28/2016: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "isl29125rom_os.h"
#include "stdarg.h"
#include "ISL29125ROM_CMD.h"

/* Public variables ---------------------------------------------------------*/

//extern osMailQId FsTaskMailId;

/* Private define ------------------------------------------------------------*/
#define CMD_RTN_LEN	128
#define ISL29125_CNT	3
#define ISL29125_TASK_HIGH_SPEED 4		//when there is device to run
#define ISL29125_TASK_LOW_SPEED 16		//when there is on a device

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	char devName[DEV_NAME_LEN];
	TASK_BONDING_T* p;
	const DEV_PORT* pPort;
}isl29125TaskArgument_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char ISL29125_T[] = {"isl29125_t"};
const char ISL29125_FILE[] = {"board.ini"};
const char ISL29125_HEAD[] = {"<isl29125>"};
const char ISL29125_TAIL[] = {"</isl29125>"};
/* Private function prototypes -----------------------------------------------*/
static void StartTaskIsl29125(void * argument);
static s8 createTaskIsl29125(osPriority priority, ...);
static s8 createTaskIsl29125ByCmd(osPriority priority, void* packet);
isl29125TaskArgument_t taskArgDict = {0};
TASK_BASE_T TaskInfoIsl29125 = {
	ISL29125_T,							//	const u8* DEV_TYPE;
	ISL29125_HELP,					//	const u8* HELP;
	createTaskIsl29125,			//	s8 (*CreateTask)	(u8 argc, ...);
	createTaskIsl29125ByCmd,
	0
};

//u8 isl29125TaskCmd(const u8* S, STR_PIECE_T* Content, COMPLIER_DEV_T* thsComplier);
//u8 disposeIsl29125DevByName(const u8* NAME);
//u8 disposeIsl29125DevAll(void);
//u8 disposeIsl29125Dev(ISL291259_Dev_T* pDev);
//u8 devCountIsl29125(DEV_MEMPOOL_T *pPool);					//return how many devices
////delegate for dev
static osMailQId curMailID = NULL;
void streamOutMsg(const char* MSG);
//s8 isl29125ReadCalCfg(const u8* RecName, CAL32_T *pCal);
//s8 isl29125WriteCalCfg	(const u8* RecName, CAL32_T cal);

//void paramDevPoolRecycle(DEV_MEMPOOL_T* paramPool, DEV_MEMPOOL_T* devPool, u8 devCnt);
/*******************************************************************************
* Function Name  : inputFetch
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void StartTaskIsl29125(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	ISL291259_Dev_T dev = {0};
	TASK_BONDING_T* p = NULL;
//	char str[32]={0};
	isl29125TaskArgument_t* arg = (isl29125TaskArgument_t*)argument;

	isl29125DevSetup(&dev, arg->pPort, streamOutMsg);	
	p = arg->p;
	p->base->count++;	
	//new local mail component
	osMailQDef(isl29125TaskMail, 1, Mail_T);
	while(arg->p->MailId == NULL){
		arg->p->MailId = osMailCreate(osMailQ(isl29125TaskMail), NULL);
	}
	devInfo_RegName(p, (char*)dev.rsrc.name, arg->devName);
	print("start %s...ok\n", arg->devName);

//	u8 tick0=0,tick1=0,tick2=0;
//	while(1){
//		tick0++;
//		tick1++;
//		tick2++;
//		if(tick0>1){
//			tick0 = 0;
//			HAL_GPIO_TogglePin(arg->pPort->PA0.GPIOx, arg->pPort->PA0.GPIO_Pin);		
//		}
//		if(tick1>2){
//			tick1 = 0;
//			HAL_GPIO_TogglePin(arg->pPort->PA1.GPIOx, arg->pPort->PA1.GPIO_Pin);		
//		}
//		if(tick2>4){
//			tick2 = 0;
//			HAL_GPIO_TogglePin(arg->pPort->PA2.GPIOx, arg->pPort->PA2.GPIO_Pin);		
//		}
//		osDelay(1);
//	}
	
	for(;;){
		curMailID = arg->p->MailId;
		evt = osMailGet(p->MailId, 0);        // wait for mail
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = osMailCAlloc(pMailRcv->productor, osWaitForever);       // Allocate memory
			if(pMailSnd != NULL){
				pMailSnd->productor = p->MailId;
				pMailSnd->traceIndex = pMailRcv->traceIndex;
				if(ISL29125Cmd(&dev, &pMailRcv->Content, &pMailSnd->Content) == 0){
					RESPONSE_ORG(&pMailSnd->Content, ERR, &pMailRcv->Content);
				}
				osMailPut(pMailRcv->productor, pMailSnd);                         // reply Mail
			}
			else	newSend(arg->p->MailId, pMailRcv->productor, 0,  "%s", "+err@RGB Sensor osMailCAlloc_fail\r\n");
			osMailFree(arg->p->MailId, pMailRcv);
		}
		//polling task
		dev.Polling(&dev.rsrc);
		osDelay(16);
	}
}

static s8 createTaskIsl29125ByCmd(osPriority priority, void* p){
	PAKET_T* packet = p;
	if(packetIsMatch(packet,"%s%s%d") && isSameStr(packet->addr[0], ISL29125_T)){
		createTaskIsl29125(priority, packet->addr[1], *(u32*)packet->addr[2]);
		return 0;
	}
	return -1;
}

//this function is run outside this task
static s8 createTaskIsl29125(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArgDict, 0, sizeof(isl29125TaskArgument_t));
	for(i=0;i<MAX_INSTANCE;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoIsl29125;
			taskArgDict.p = &devInfo[i];
			break;
		}
	}	
	if(taskArgDict.p == NULL)	return -1;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArgDict.devName, pStr);
	i = va_arg(ap, u32);
	va_end(ap);	
	if(i>=DEV_PORT_SUM)	return -2;
	taskArgDict.pPort = &DevPort[i];

	xTaskCreate( StartTaskIsl29125, "isl29125", 200, &taskArgDict, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgDict.p->TaskID);		//stack size in 4bytes	
	while(taskArgDict.p->MailId == NULL)	osDelay(1);
	return 0;
}

//s8 isl29125ReadCalCfg(const u8* RecName, CAL32_T *pCal){
//	s8 tmpS8,rtn;
////	u8 str[128] = {0}, strSz = 128;
//	PARAM_DEV_T* e;
//	if(pFsDev==NULL || devFsMutexHandle==NULL)	return -1;
//	osMutexWait(devFsMutexHandle, osWaitForever);
//	tmpS8 = pFsDev->ReadRecordVal(&pFsDev->rsrc, ISL29125_FILE, ISL29125_HEAD, ISL29125_TAIL, RecName);
//	if(tmpS8 == 3){
//		e = &pFsDev->rsrc.rec;
//		pCal->offset = *(u32*)e->rsrc.params[0];
//		pCal->gainMul = *(u32*)e->rsrc.params[1];
//		pCal->gainDiv = *(u32*)e->rsrc.params[2];	
//		rtn = 0;
//	}
//	else rtn = -2;
//	osMutexRelease(devFsMutexHandle);
//	return rtn;
//}

//s8 isl29125WriteCalCfg	(const u8* RecName, CAL32_T cal){
//	s8 rtn;
//	PARAM_DEV_T* e;
//	if(pFsDev==NULL || devFsMutexHandle==NULL)	return -1;
//	osMutexWait(devFsMutexHandle, osWaitForever);
//	e = &pFsDev->rsrc.rec;
//	e->reset(&e->rsrc);
//	strCpy(e->rsrc.funName, FUN_NAME_SZ, RecName);
//	e->addU32(&e->rsrc, cal.offset);
//	e->addU32(&e->rsrc, cal.gainMul);
//	e->addU32(&e->rsrc, cal.gainDiv);
//	rtn = pFsDev->WriteRecordVal(&pFsDev->rsrc, ISL29125_FILE, ISL29125_HEAD, ISL29125_TAIL);
//	osMutexRelease(devFsMutexHandle);
//	return rtn;
//}

void streamOutMsg(const char* MSG){
	newSend(curMailID, devInfo[0].MailId, 0,  "%s", MSG);
}

//u8 newIsl29125Task(void){
//	osThreadDef(isl29125Task, StartIsl29125Task, osPriorityNormal, 0, 350);			//192
//	if(isl29125TaskHandle == NULL){
//		isl29125TaskHandle = osThreadCreate(osThread(isl29125Task), NULL);
//		if(isl29125TaskHandle != NULL)	return 1;
//	}
//	return 0;
//}

//u8 isl29125TaskCmd(const u8* S, STR_PIECE_T* Content, COMPLIER_DEV_T* thsComplier){
//	s8 tmpS8;
//	u8 *rtnStr,rtnStrSz,i;
//	ISL291259_Dev_T* pDev;
//	PARAM_RSRC_T* e;
//	
//	rtnStr = Content->memPool;
//	rtnStrSz = BYTES_PER_MAIL;
//	e = &thsComplier->rsrc;
//	tmpS8 = thsComplier->Comply(e, S, NULL, 0);
//	//sys.newdev("isl29125_t", "devname", iicPortIndx);
//	if(isSameStr(e->funName, "sys.newdev") && tmpS8 == 3){
//		if(isSameStr(e->params[0], ISL29125_T) == 0)	return 0;	//reply nothing
//		else if(DEVNAME.IsAvalid(e->params[1]) < 0)		RESPONSE.Err_S(rtnStr, rtnStrSz, S, "DEV_NAME");
//		else if(*(u32*)e->params[2] >= DEV_PORT_SUM)	RESPONSE.Err_S(rtnStr, rtnStrSz, S, "DEV_PORT_INDEX");
//		else{
//			//apply memPool
//			pDev = NULL;
//			for(i=0;i<ISL29125_CNT;i++){
//				if(pIsl29125DevPool[i].pDev == NULL){
//					pIsl29125DevPool[i].pDev = (ISL291259_Dev_T*)osPoolCAlloc(pIsl29125DevPool[i].poolId);
//					pDev = pIsl29125DevPool[i].pDev;
//					break;
//				}
//			}
//			//setup dev		
//			if(pDev != NULL){
//				i = *(u32*)e->params[2];
//				if(isl29125DevSetup(pDev, e->params[1], devPort[i].PA1, devPort[i].PA2, devPort[i].PA0, streamOutMsg) >= 0 ){
//					RESPONSE.Ok_SS(rtnStr, rtnStrSz, e->funName, e->params[0], e->params[1]);
//				}
//				else{
//					disposeIsl29125Dev(pDev);
//					RESPONSE.Err_S(rtnStr, rtnStrSz, S, "setup");
//				}
//			}
//			else	RESPONSE.Err_S(rtnStr, rtnStrSz, S, "osPoolCAlloc");
//		}
//		return 1;
//	}
//	
//	//sys.disposedev("isl29125_t", "devname");	//dispose device named by "devname"
//	//sys.disposedev("isl29125_t");	//dispose all devices
//	if(isSameStr(e->funName, "sys.disposedev") && tmpS8 == 1){
//		if(disposeIsl29125DevByName(e->params[0]) == 0)	return 0;
//		RESPONSE.Ok_S(rtnStr, rtnStrSz, e->funName, e->params[0]);
//		return 1;
//	}
////	tmpS8 = fetchFun(S, &eDev, NULL, 0);
////	if(isSameStr(eDev.rsrc.funName, "sys.disposedev") && tmpS8 >= 1){
////		if(isSameStr(eDev.rsrc.params[0], ISL29125_T) == 0)	return 0;	//reply nothing
////		else{
////			if(tmpS8 == 1){		//dispose all devices
////				cnt = disposeIsl29125DevAll();
////				if(cnt)	responseOk_D(rtnStr, rtnStrSz, eDev.rsrc.funName, &cnt);
////				else	responseErr(rtnStr, rtnStrSz, S);
////			}
////			else if(tmpS8 == 2){	//dispose device named by "devname"
////				cnt = disposeIsl29125DevByName(eDev.rsrc.params[1]);	//
////				if(cnt)	responseOk_S(rtnStr, rtnStrSz, eDev.rsrc.funName, eDev.rsrc.params[1]);
////				else	responseErr(rtnStr, rtnStrSz, S);
////			}
////			else	RESPONSE.Err_S(rtnStr, rtnStrSz, eDev.rsrc.funName,"PARAM_CNT");
////			return 1;
////		}
////	}
//	
//	//sys.listdev("isl29125_t");	//list device name in this dev type
//	if(isSameStr(e->funName, "sys.listdev") && tmpS8 == 1){
//		if(isSameStr(e->params[0], ISL29125_T) == 0)	return 0;	//reply nothing
//		else{
//			//test if same name
//			strCpy(rtnStr, rtnStrSz,"+ok@sys.listdev(\"");
//			strcat((char*)rtnStr, (const char*)ISL29125_T);
//			strcat((char*)rtnStr, (const char*)"\",");
//			for(i=0;i<ISL29125_CNT;i++){
//				if(pIsl29125DevPool[i].pDev == NULL)	continue;
//				pDev = pIsl29125DevPool[i].pDev;
//				strcat((char*)rtnStr, (const char*)"\"");
//				strcat((char*)rtnStr, (char*)pDev->rsrc.name);
//				strcat((char*)rtnStr, (const char*)"\",");
//			}
//			strcat((char*)rtnStr, (const char*)")");
//			strReplace(rtnStr, rtnStrSz, ",)", ")");
//			strcat((char*)rtnStr, (const char*)endStr);
//			return 1;
//		}
//	}
//		
//	//sys.listdev("isl29125_t");	//list device name in this dev type
//	if(isSameStr(e->funName, "sys.help") && tmpS8 == 1){
//		if(isSameStr(e->params[0], ISL29125_T) == 0)	return 0;	//reply nothing
//		//test if same name
//		strCpy(rtnStr, rtnStrSz,"+ok@sys.help(\"");
//		strcat((char*)rtnStr, (const char*)ISL29125_T);
//		strcat((char*)rtnStr, (const char*)"\")\n");
//		Content->addr[1] = (u32)ISL29125_HELP;
//		return 1;
//	}	
//	
//	return 0;	
//}

//u8 disposeIsl29125DevAll(void){
//	u8 i,j;
//	ISL291259_Dev_T *pDev;
//	
//	for(i=0,j=0;i<ISL29125_CNT;i++){
//		pDev = (ISL291259_Dev_T*)pIsl29125DevPool[i].pDev;
//		if(pDev == NULL )	continue;
//		DEVNAME.Del(pDev->rsrc.name);
//		osPoolFree (pIsl29125DevPool[i].poolId, pIsl29125DevPool[i].pDev);
//		pIsl29125DevPool[i].pDev = NULL;
//		j++;
//	}
//	return j;
//}

//u8 disposeIsl29125DevByName(const u8* NAME){
//	u8 i;
//	ISL291259_Dev_T *pDev;
//	
//	for(i=0;i<ISL29125_CNT;i++){
//		pDev = (ISL291259_Dev_T*)pIsl29125DevPool[i].pDev;
//		if(pDev == NULL )	continue;
//		if(isSameStr(pDev->rsrc.name, NAME) ){
//			DEVNAME.Del(pDev->rsrc.name);
//			osPoolFree (pIsl29125DevPool[i].poolId, pIsl29125DevPool[i].pDev);
//			pIsl29125DevPool[i].pDev = NULL;
//			return 1;
//		}
//	}
//	return 0;
//}

//u8 disposeIsl29125Dev(ISL291259_Dev_T* pDev){
//	u8 i;
//	
//	for(i=0;i<ISL29125_CNT;i++){
//		if(pIsl29125DevPool[i].pDev == NULL )	continue;		
//		if(pDev == (ISL291259_Dev_T*)pIsl29125DevPool[i].pDev){
//			DEVNAME.Del(pDev->rsrc.name);
//			osPoolFree (pIsl29125DevPool[i].poolId, pIsl29125DevPool[i].pDev);
//			pIsl29125DevPool[i].pDev = NULL;
//			return 1;
//		}
//	}
//	return 0;
//}

//u8 devCountIsl29125(DEV_MEMPOOL_T *pPool){
//	u8 i,j;
//	for(i=0,j=0;i<ISL29125_CNT;i++){
//		if(pPool[i].pDev != NULL)	j++;
//	}
//	return j;
//}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
