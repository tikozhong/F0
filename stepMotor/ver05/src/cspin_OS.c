/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : L6480_os.c
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
#include "publicRsrc.h"
#include "cspin_os.h"
#include "cspin_command.h"

/* Public variables ---------------------------------------------------------*/
extern osMailQId UartTaskMailId;

/* Private define ------------------------------------------------------------*/
//#define CMD_RTN_LEN	128
#define L6480_CNT	2

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const u8 L6480_T[] = {"stepper_t"};
const u8 L6480_FILE[] = {"board.ini"};
const u8 L6480_HEAD[] = {"<stepper>"};
const u8 L6480_TAIL[] = {"</stepper>"};

osThreadId L6480TaskHandle = NULL;
osPoolDef (MemPoolCspinDevDef, 1, cSPIN_DEV_T);
DEV_MEMPOOL_T pL6480DevPool[L6480_CNT] = { NULL };
osMailQId L6480TaskMailId = NULL;

/* Private function prototypes -----------------------------------------------*/
static u8 L6480TaskCmd(Mail_T* rcvr, Mail_T *sndr, COMPLIER_DEV_T* complier);
static u8 disposeL6480DevByName(const u8* NAME);	
static u8 disposeL6480DevAll(void);
static u8 disposeL6480Dev(cSPIN_DEV_T* pDev);

//delegate for dev
//static void streamOutMsg(const u8* MSG);
/*******************************************************************************
* Function Name  : devtask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void StartL6480Task(void const * argument){
	osMailQId owner;
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	u8 i;
	cSPIN_DEV_T* pDev = NULL;
	
	//new local mail component
	osMailQDef(L6480TaskMail, 1, Mail_T);
	L6480TaskMailId = osMailCreate(osMailQ(L6480TaskMail), NULL);
	for(i=0; i<L6480_CNT; i++){	//new mempoolId
		pL6480DevPool[i].poolId = osPoolCreate (osPool (MemPoolCspinDevDef));
		pL6480DevPool[i].pDev = NULL;
	}
	//wait outside env ready
//	while(eDevMutexHandle == NULL)	{	osDelay(16);}
	while(UartTaskMailId == NULL)		{ osDelay(16);}	//wait uart ready		
	while(L6480TaskMailId == NULL)	{	osDelay(16);}
	//ready go
	owner = L6480TaskMailId;
	MAIL.Send_S(owner, UartTaskMailId, 0XFF, (const u8*)"start stepper task.. ok\r\n");		// Send Mail
	//to book mail from uartTask
	MAIL.MakeFriend(owner, UartTaskMailId, L6480_HELP);	//ower want to be a friend of UartTaskMailId
	
	for(;;){
		devPort[3].F0(NULL);
		evt = osMailGet(owner, 0);        // wait for mail
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = osMailCAlloc(pMailRcv->productor, osWaitForever);       // Allocate memory
			if(pMailSnd != NULL){
				MAIL.Reset(pMailSnd);
				pMailSnd->productor = owner;
				pMailSnd->traceIndex = pMailRcv->traceIndex;
				osMutexWait(eDevMutexHandle, osWaitForever);	//get eDev resource, and be careful with using other mutex before this release							
				if(L6480TaskCmd(pMailRcv, pMailSnd, &COMPLIER) == 0){
					for(i=0; i<L6480_CNT; i++){
						if(pL6480DevPool[i].pDev == NULL)	continue;
						pDev = (cSPIN_DEV_T*)pL6480DevPool[i].pDev;
						if(l6480Cmd(pDev, (const u8*)pMailRcv->Content.addr[0], &pMailSnd->Content, &COMPLIER) )	break;
					}
				}
				osMutexRelease(eDevMutexHandle);
				osMailPut(pMailRcv->productor, pMailSnd);                         // reply Mail
			}
			else	MAIL.Send_S(owner, pMailRcv->productor, 0xff, (const u8*)"+err@stepper osMailCAlloc_fail\r\n");
			osMailFree(owner, pMailRcv);
		}
		//device exe
		for(i=0; i<L6480_CNT; i++){
			if(pL6480DevPool[i].pDev == NULL)	continue;
			pDev = (cSPIN_DEV_T*)pL6480DevPool[i].pDev;
			pDev->tickExe(&pDev->rsrc);
		}
		osDelay(4);
	}
}

u8 newL6480Task(void){
	osThreadDef(L6480Task, StartL6480Task, osPriorityAboveNormal, 0, 512);			//192
	if(L6480TaskHandle == NULL){
		L6480TaskHandle = osThreadCreate(osThread(L6480Task), NULL);
		if(L6480TaskHandle != NULL)	return 1;
	}
	return 0;
}

//static u8 L6480TaskCmd(osMailQId sndr, osMailQId rcvr, const u8* S, STR_PIECE_T* Content, PARAM_DEV_T* e)
static u8 L6480TaskCmd(Mail_T* rcvr, Mail_T *sndr, COMPLIER_DEV_T* complier){
	s8 tmpS8;
	u8 *rtnStr, rtnStrSz, i;
	cSPIN_DEV_T* pDev;
	u8* S;
	PARAM_RSRC_T* e = &complier->rsrc;

	rtnStr = sndr->Content.memPool;
	rtnStrSz = BYTES_PER_MAIL;
	S = rcvr->Content.memPool;

	//fetch
	tmpS8 = complier->Comply(e, S, NULL, 0);
	//sys.help()
	//sys.help("dev_t")
	if(isSameStr(e->funName, (const u8*)"sys.help") ){
		if(tmpS8 == 1){
			if(isSameStr(e->params[0], L6480_T) == 0)	return 0;	//reply nothing
			strCpy(rtnStr, rtnStrSz,(const u8*)"+ok@sys.help(\"");
			strcat((char*)rtnStr, (const char*)L6480_T);
			strcat((char*)rtnStr, (const char*)"\")\n");
			sndr->Content.addr[1] = (u32)L6480_HELP;
			return 1;
		}
		else	return 0;
	}

	//sys.newdev("dev_t", "devname", iicPortIndx);
	if(isSameStr(e->funName, (const u8*)"sys.newdev") && tmpS8 == 3){
		if(isSameStr(e->params[0], L6480_T) == 0)	return 0;	//reply nothing
		else if(DEVNAME.IsAvalid(e->params[1]) < 0)		RESPONSE.Err_S(rtnStr, rtnStrSz, S, (const u8*)"DEV_NAME");
		else if(*(u32*)e->params[2] >= DEV_PORT_SUM)	RESPONSE.Err_S(rtnStr, rtnStrSz, S, (const u8*)"PORTS_INDEX");
		else{
			//apply memPool
			pDev = NULL;
			for(i=0;i<L6480_CNT;i++){
				if(pL6480DevPool[i].pDev == NULL){
					pL6480DevPool[i].pDev = (cSPIN_DEV_T*)osPoolCAlloc(pL6480DevPool[i].poolId);
					pDev = pL6480DevPool[i].pDev;
					break;
				}
			}
			//setup dev		
			if(pDev != NULL){
				i = *(u32*)e->params[2];
				cSPIN_Setup(pDev, e->params[1], 
					&devPort[i].SPI_HANDLE, 
					devPort[i].PB1, 
					devPort[i].PC0,
					devPort[i].PC1,
					devPort[i].PA0,
					devPort[i].PA1,
					devPort[i].PA2,
					delayMs
				);
				taskTabAddDevName(L6480_T, pDev->rsrc.name);
				RESPONSE.Ok_SS(rtnStr, rtnStrSz, e->funName, e->params[0], e->params[1]);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, S, (const u8*)"osPoolCAlloc");
		}
		return 1;
	}
	
	//sys.disposedev("dev_t", "devname");	//dispose device named by "devname"
	//sys.disposedev("dev_t");	//dispose all devices
	if(isSameStr(e->funName, (const u8*)"sys.disposedev") && tmpS8 == 1){
		if(isSameStr(e->params[0], L6480_T) == 0){
			if(disposeL6480DevAll() == 0)	return 0;
			RESPONSE.Ok_S(rtnStr, rtnStrSz, e->funName, e->params[0]);
		}
		else{
			if(disposeL6480DevByName(e->params[0]) == 0)	return 0;
			RESPONSE.Ok_S(rtnStr, rtnStrSz, e->funName, e->params[0]);			
		}
		return 1;
	}

	//sys.listdev("isl29125_t");	//list device name in this dev type
	if(isSameStr(e->funName, (const u8*)"sys.listdev") && tmpS8 == 1){
		if(isSameStr(e->params[0], L6480_T) == 0)	return 0;	//reply nothing
		else{
			//test if same name
			strCpy(rtnStr, rtnStrSz,(const u8*)"+ok@sys.listdev(\"");
			strcat((char*)rtnStr, (const char*)L6480_T);
			strcat((char*)rtnStr, (const char*)"\",");
			for(i=0;i<L6480_CNT;i++){
				if(pL6480DevPool[i].pDev == NULL)	continue;
				pDev = pL6480DevPool[i].pDev;
				strcat((char*)rtnStr, (const char*)"\"");
				strcat((char*)rtnStr, (char*)pDev->rsrc.name);
				strcat((char*)rtnStr, (const char*)"\",");
			}
			strcat((char*)rtnStr, (const char*)")");
			strReplace(rtnStr, rtnStrSz, (const u8*)",)", (const u8*)")");
			strcat((char*)rtnStr, (const char*)endStr);
			return 1;
		}
	}

	return 0;	
}

static u8 disposeL6480DevAll(void){
	u8 i,j;
	cSPIN_DEV_T *pDev;
	
	for(i=0,j=0;i<L6480_CNT;i++){
		pDev = (cSPIN_DEV_T*)pL6480DevPool[i].pDev;
		if(pDev == NULL )	continue;
		DEVNAME.Del(pDev->rsrc.name);
		taskTabDelDevName(L6480_T, pDev->rsrc.name);
		osPoolFree (pL6480DevPool[i].poolId, pL6480DevPool[i].pDev);
		pL6480DevPool[i].pDev = NULL;
		j++;
	}
	return j;
}

static u8 disposeL6480DevByName(const u8* NAME){
	u8 i;
	cSPIN_DEV_T *pDev;
	
	for(i=0;i<L6480_CNT;i++){
		pDev = (cSPIN_DEV_T*)pL6480DevPool[i].pDev;
		if(pDev == NULL )	continue;
		if(isSameStr(pDev->rsrc.name, NAME) ){
			DEVNAME.Del(pDev->rsrc.name);
			taskTabDelDevName(L6480_T, pDev->rsrc.name);
			osPoolFree (pL6480DevPool[i].poolId, pL6480DevPool[i].pDev);
			pL6480DevPool[i].pDev = NULL;
			return 1;
		}
	}
	return 0;
}

static u8 disposeL6480Dev(cSPIN_DEV_T* pDev){
	u8 i;
	
	for(i=0;i<L6480_CNT;i++){
		if(pL6480DevPool[i].pDev == NULL )	continue;		
		if(pDev == (cSPIN_DEV_T*)pL6480DevPool[i].pDev){
			DEVNAME.Del(pDev->rsrc.name);
			taskTabDelDevName(L6480_T, pDev->rsrc.name);
			osPoolFree (pL6480DevPool[i].poolId, pL6480DevPool[i].pDev);
			pL6480DevPool[i].pDev = NULL;
			return 1;
		}
	}
	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
