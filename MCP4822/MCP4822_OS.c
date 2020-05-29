/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : tm1638_os.c
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
#include "mcp4822_os.h"
#include "responseX.h"

/* Public variables ---------------------------------------------------------*/
extern osMailQId UartTaskMailId;

/* Private define ------------------------------------------------------------*/
//#define CMD_RTN_LEN	128
#define MCP4822_CNT	8

/* Private typedef -----------------------------------------------------------*/
typedef struct Node{    
	MCP4822Dev_t dev;
	struct Node *pNxt;
}Node_T;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const u8 MCP4822_T[] = {"mcp4822_t"};
const u8 MCP4822_FILE[] = {"board.ini"};
const u8 MCP4822_HEAD[] = {"<mcp4822>"};
const u8 MCP4822_TAIL[] = {"</mcp4822>"};

osThreadId MCP4822TaskHandle = NULL;
osMailQId MCP4822TaskMailId = NULL;
static osPoolId PoolID_lnk;
static Node_T* pDevList = NULL;

/* Private function prototypes -----------------------------------------------*/
static MCP4822Dev_t* pDev = NULL;
static u8 MCP4822TaskCmd(Mail_T* rcvr, Mail_T *sndr);

static MCP4822Dev_t* insertLastList(Node_T **pNode);
static s8 sizeList(Node_T *pHead);

//delegate for dev
//static void streamOutMsg(const u8* MSG);
/*******************************************************************************
* Function Name  : devtask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void StartMCP4822Task(void const * argument){
	osMailQId owner;
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	Node_T *pTmp; 
	
	//new local mail component
	osMailQDef(Mcp4822TaskMail, 1, PAKET_T);
	owner = osMailCreate(osMailQ(Mcp4822TaskMail), NULL);

	osPoolDef (MemPoolDef_lnk, MCP4822_CNT, Node_T);
	PoolID_lnk = osPoolCreate (osPool (MemPoolDef_lnk));
	
	//wait outside env ready
//	while(eDevMutexHandle == NULL)	{	osDelay(16);}
	while(UartTaskMailId == NULL || owner == NULL)		{ osDelay(4);}	//wait uart ready		
	//ready go
//	MAIL.Send_S(owner, UartTaskMailId, 0XFF, (const u8*)"start tm1638 task.. ok\r\n");		// Send Mail
//	//to book mail from uartTask
//	MAIL.MakeFriend(owner, UartTaskMailId, MCP4822_HELP);	//ower want to be a friend of UartTaskMailId

	MCP4822TaskMailId = owner;
	//wait a mail to setup a device
	for(pDev=NULL;;){
		evt = osMailGet(owner, 0);        // wait for mail
		if (evt.status == osEventMail){
			pMailRcv = evt.value.p;			
			pMailSnd = osMailCAlloc(pMailRcv->productor, osWaitForever);       // Allocate memory
			pMailSnd->traceIndex = pMailRcv->traceIndex;
			if(pMailSnd != NULL){
				MAIL.Reset(pMailSnd);
				pMailSnd->productor = owner;
				MCP4822TaskCmd(pMailRcv, pMailSnd);
				osMailPut(pMailRcv->productor, pMailSnd);                         // reply Mail
			}
			else	MAIL.Send_S(owner, pMailRcv->productor, 0xff, (const u8*)"+err@MCP4822_Dev_osMailCAlloc_fail\r\n");
			osMailFree(owner, pMailRcv);
			if(sizeList(pDevList))  	break;
		}
		osDelay(4);
	}
	MAIL.Send_S(owner, UartTaskMailId, 0xff, (const u8*)"start MCP4822 task.. ok\r\n");		// Send Mail

	pDev = &pDevList->dev;
	pDev->SetVolt_mV(&pDev->rsrc,0,2000);
	pDev->SetVolt_mV(&pDev->rsrc,1,2000);
	
	for(;;){
		evt = osMailGet(owner, 0);        // wait for mail
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = osMailCAlloc(pMailRcv->productor, osWaitForever);       // Allocate memory
			if(pMailSnd != NULL){
				MAIL.Reset(pMailSnd);
				pMailSnd->productor = owner;
				pMailSnd->traceIndex = pMailRcv->traceIndex;
				if(MCP4822TaskCmd(pMailRcv, pMailSnd) == 0){
					for(pTmp = pDevList; pTmp != NULL; pTmp = pTmp->pNxt){
						if(mcp4822Cmd(&pTmp->dev, (const u8*)pMailRcv->Content.addr[0], &pMailSnd->Content) )	break;
					}				
				}
				osMailPut(pMailRcv->productor, pMailSnd);                         // reply Mail
			}
			else	MAIL.Send_S(owner, pMailRcv->productor, 0xff, (const u8*)"+err@MCP4822 osMailCAlloc_fail\r\n");
			osMailFree(owner, pMailRcv);
		}
	
		
//		pDev = &pDevList->dev;
//		if(pDev){
//			HAL_GPIO_TogglePin(pDev->rsrc.CS.GPIOx, pDev->rsrc.CS.GPIO_Pin);
//			HAL_GPIO_TogglePin(pDev->rsrc.SCK.GPIOx, pDev->rsrc.SCK.GPIO_Pin);
//			HAL_GPIO_TogglePin(pDev->rsrc.SDI.GPIOx, pDev->rsrc.SDI.GPIO_Pin);
//		}
//		
//		MAIL.Send_S(owner, UartTaskMailId, 0xff, (const u8*)"MCP4822 Task running\r\n");
		osDelay(100);
	}
}

s8 newMCP4822Task(u8 argc, ...){
	osThreadDef(MCP4822Task, StartMCP4822Task, osPriorityNormal, 0, 256);			//192
	if(MCP4822TaskHandle == NULL){
		MCP4822TaskHandle = osThreadCreate(osThread(MCP4822Task), NULL);
		if(MCP4822TaskHandle != NULL)	return 1;
	}
	return 0;
}

//static u8 MCP4822TaskCmd(osMailQId sndr, osMailQId rcvr, const u8* S, STR_PIECE_T* Content, PARAM_DEV_T* e)
static u8 MCP4822TaskCmd(Mail_T* rcvr, Mail_T *sndr){
	u8 *rtnStr, rtnStrSz, i;
	u8* S;
	CompiLer_Dev_T eDev;
	CompiLer_Rsrc_T* e;

	rtnStr = (u8*)sndr->Content.addr[0];
	rtnStrSz = PACKE_POOL_SZ;
	S = (u8*)rcvr->Content.addr[0];	
	
	compilerSetup(&eDev);
	e = &eDev.rsrc;
	if(eDev.comply(e, S, NULL, 0)<0)	return 0;
	if(eDev.isMatchDevName(e, (const u8*)"sys") != PASS)	return 0;

	//sys.newdev("dev_t", "devname", iicPortIndx);
	if(eDev.isMatchFuncName(e, (const u8*)"newdev") == PASS){	//match function name
		if(eDev.isMatchFormat(e,STR,STR,U32) == PASS){	//match format too
			if(isSameStr(e->params[0], MCP4822_T) == 0)	return 0;	//reply nothing
			else if(isDevNameAvalid(e->params[0], e->params[1]) == 0)		RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", (const u8*)"DEV_NAME");
			else if(*(u32*)e->params[2] >= DEV_PORT_SUM)	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", (const u8*)"PORTS_INDEX");
			else{
				//apply memPool
				pDev = insertLastList(&pDevList);
				//setup dev		
				if(pDev != NULL){
					i = *(u32*)e->params[2];
					//board stage config
//					mcp4822DevSetup(pDev, e->params[1], devPort[i].PB1, devPort[i].PB0, devPort[i].PB2);
					mcp4822DevSetup(pDev, e->params[1]);
					taskTabAddDevName(MCP4822_T, pDev->rsrc.name, e->params[1]);
					RESPONSE_X(OK, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s%s", e->params[0], e->params[1]);
				}
				else	RESPONSE_X(ERR, rtnStr, rtnStrSz, e->funName, &e->funName[e->funcNameIndx], "%s", (const u8*)"DEV_NAME");
			}
			return 1;
		}
	}
	
	//sys.disposedev("dev_t", "devname");	//dispose device named by "devname"
	//sys.disposedev("dev_t");	//dispose all devices
//	if(eDev.isMatchFuncName(e, (const u8*)"disposedev") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,STR) == PASS){	//match format too
//			if(isSameStr(e->params[0], MCP4822_T) == 0){
//				if(disposeMCP4822DevAll() == 0)	return 0;
//				RESPONSE.Ok_S(rtnStr, rtnStrSz, e->funName, e->params[0]);
//			}
//			else{
//				if(disposeMCP4822DevByName(e->params[0]) == 0)	return 0;
//				RESPONSE.Ok_S(rtnStr, rtnStrSz, e->funName, e->params[0]);			
//			}
//			return 1;
//		}
//	}

	return 0;	
}

//static u8 disposeMCP4822DevAll(void){
//	u8 i,j;
//	MCP4822Dev_t *pDev;
//	
//	for(i=0,j=0;i<MCP4822_CNT;i++){
//		pDev = (MCP4822Dev_t*)pTm1638DevPool[i].pDev;
//		if(pDev == NULL )	continue;
//		DEVNAME.Del(pDev->rsrc.name);
//		taskTabDelDevName(MCP4822_T, pDev->rsrc.name);
//		osPoolFree (pTm1638DevPool[i].poolId, pTm1638DevPool[i].pDev);
//		pTm1638DevPool[i].pDev = NULL;
//		j++;
//	}
//	return j;
//}

//static u8 disposeMCP4822DevByName(const u8* NAME){
//	u8 i;
//	MCP4822Dev_t *pDev;
//	
//	for(i=0;i<MCP4822_CNT;i++){
//		pDev = (MCP4822Dev_t*)pTm1638DevPool[i].pDev;
//		if(pDev == NULL )	continue;
//		if(isSameStr(pDev->rsrc.name, NAME) ){
//			DEVNAME.Del(pDev->rsrc.name);
//			taskTabDelDevName(MCP4822_T, pDev->rsrc.name);
//			osPoolFree (pTm1638DevPool[i].poolId, pTm1638DevPool[i].pDev);
//			pTm1638DevPool[i].pDev = NULL;
//			return 1;
//		}
//	}
//	return 0;
//}

//static u8 disposeMCP4822Dev(MCP4822Dev_t* pDev){
//	u8 i;
//	
//	for(i=0;i<MCP4822_CNT;i++){
//		if(pTm1638DevPool[i].pDev == NULL )	continue;		
//		if(pDev == (MCP4822Dev_t*)pTm1638DevPool[i].pDev){
//			DEVNAME.Del(pDev->rsrc.name);
//			taskTabDelDevName(MCP4822_T, pDev->rsrc.name);
//			osPoolFree (pTm1638DevPool[i].poolId, pTm1638DevPool[i].pDev);
//			pTm1638DevPool[i].pDev = NULL;
//			return 1;
//		}
//	}
//	return 0;
//}

//static MCP4822Dev_t* insertHeadList(Node_T **pNode)
//{
//	Node_T *pInsert;
//	Node_T *pHead;
//	
//	pHead = *pNode;
//	if(pHead == NULL){
//		pHead = (Node_T*)osPoolCAlloc(PoolID_lnk);
//		memset(pHead,0,sizeof(Node_T));
//		return &pHead->dev;
//	}
//	
//	pInsert = (Node_T*)osPoolCAlloc(PoolID_lnk);
//	memset(pInsert,0,sizeof(Node_T));
//	pInsert->pNxt = *pNode;
//	*pNode = pInsert;
//	return &pInsert->dev;
//}

static MCP4822Dev_t* insertLastList(Node_T **pNode)
{
	Node_T *pInsert;
	Node_T *pHead;
	Node_T *pTmp; 
	
	pHead = *pNode;
	if(pHead == NULL){
		pHead = (Node_T*)osPoolCAlloc(PoolID_lnk);
		memset(pHead,0,sizeof(Node_T));
		*pNode = pHead;
		return &pHead->dev;
	}

	pTmp = pHead;
	pInsert = (Node_T*)osPoolCAlloc(PoolID_lnk);
	memset(pInsert,0,sizeof(Node_T));
	while(pHead->pNxt != NULL){
		pHead = pHead->pNxt;
	}
	pHead->pNxt = pInsert;
	*pNode = pTmp;

	return &pInsert->dev;
}

static s8 sizeList(Node_T *pHead)
{
	s8 size = 0;
	while(pHead != NULL){
		size++; 
		pHead = pHead->pNxt;
	}
	return size;    
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
