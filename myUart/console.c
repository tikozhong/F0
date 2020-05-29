/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : console.c
* Author             : Tiko Zhong
* Date First Issued  : 11/18/2016
* Description        : 
*                      
********************************************************************************
* History:
* 11/18/2016: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "misc.h"
#include "publicRsrc.h"
#include "cmdDefine.h"
#include "stdarg.h"
#include "responseX.h"
#include "packet.h"
#include "compiler.h"

#define FUN_LEN 64
#define UART_DEBUG_PRNT_EN	0

#define UART_TICK 1

/* Public variables ---------------------------------------------------------*/


//extern SPI_HandleTypeDef hspi3;
/* Private define ------------------------------------------------------------*/
#define DEPTH_OF_TRACE	16
#define CMD_BUF_LEN 128

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	char *pCmd;
	u16 tick;
} mailTrace_t;

typedef struct{
	char devName[DEV_NAME_LEN];
	TASK_BONDING_T* p;
	PIN_T LED;
	u32 baudRate;
}uartTaskArgument_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static mailTrace_t mailTrace[DEPTH_OF_TRACE] = {0};
static const char UART_T[] = {	"uart_t" };

/* Private function prototypes -----------------------------------------------*/
static void StartTaskUart(void * argument);
static s8 createTaskUart(osPriority priority, ...);
/* Public variables ---------------------------------------------------------*/
TASK_BASE_T TaskInfoUart = {
	UART_T,						//	const u8* DEV_TYPE;
	SYS_HELP,					//	const u8* HELP;
	createTaskUart,		//	s8 (*CreateTask)	(u8 argc, ...);
	NULL,
	0
};

u32 uartErr = 0;
__IO ITStatus UartReady = RESET;

//devices' help
//static u32 devHelp[FRINED_CNT] = {NULL};
/* Private function prototypes -----------------------------------------------*/
//static void addHELP(u32 HELP_ADDR);
//static u8 StartUartTaskCmd(const u8* S);
void complyConstPointer(const u8* S, const u8* BEGIN_KEY, const u8* END_KEY, u8 *d, u16 dLen);
void StartUartDev(void const * argument);

static char* registerMail(const char* S);
static s8 unRegisterMail(char* s);
static u8 isNewLine(RINGBUFF_T* rb, char* line, u8 len);

static void debugPrint(const char*);
static void printPacket(PAKET_T*);
void printPacketDetail(PAKET_T* p);
static void printHelp(void);
static void printDevList(void);
static u8 sysCmd(PAKET_T *packetIn);

//mail trace
static char cmdStrPool[CMD_BUF_LEN*4] = {0};
static uartTaskArgument_t taskArgument = {0};
UART_DEV dev = {0};	

/*******************************************************************************
* Function Name  : StartUartTask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void StartTaskUart(void * argument){
	osMailQId owner;
	osEvent  evt;
	Mail_T *pMail;
	u8 i,j,iEn,ledTick=0,pollingTmr, pollingTick;
	s8 tmpS8;
	s32 bytes;
	PIN_T running;
	char cmdBuf[CMD_BUF_LEN] = {0};
	u32 subRcv;
	PAKET_T packet;
	char* pStr;
//	ShellType_t shellType = SHL_CPP;
	uartTaskArgument_t* arg = (uartTaskArgument_t*)argument;

	//kernel components
	osMailQDef(UartTaskMail, 4, Mail_T);
	while(arg->p->MailId == NULL)	arg->p->MailId = osMailCreate(osMailQ(UartTaskMail), NULL);	
	arg->p->QueenID = xQueueCreate(8, sizeof(void*));

	//setup a uart device
	strcpy(endStr, "\r\n");
	uartDevSetup(&dev, &huart1, arg->baudRate);
	devInfo_RegName(arg->p, dev.Rsrc.name, arg->devName);
	arg->p->base->count++;
	owner = arg->p->MailId;	
	
	running = arg->LED;
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(running);
	print("%s%s%s", SCH_VER, SRC_VER, INSTRUCTION_SET_VER);
	print("baudrate:%d\n", dev.Rsrc.baud);
	print("start %s...ok\n", arg->devName);

	pollingTmr = (DOUBLE_BUF_LEN * 8 * 1000)/arg->baudRate - 1;
	pollingTick = 0;
	osDelay(16);

	//run this task 
	memset(cmdBuf, 0, CMD_BUF_LEN);	
	for(;;){
		while(1){		//deal with all the mail in once
			evt = osMailGet(owner, 0);    // wait for mail
			if(evt.status == osEventMail){
				pMail = (Mail_T*)evt.value.p;
				subRcv = pMail->traceIndex;
				printPacket(&pMail->Content);
				if(unRegisterMail((char*)subRcv) > 0)	debugPrint("unRegisterMail@mailresponse\r\n");
				osMailFree(owner, pMail); 	// free memory allocated for mail
			}
			else break;
		}
		
		pollingTick += UART_TICK;
		if(pollingTick > pollingTmr){
			pollingTick = 0;
			bytes = dev.polling(&dev.Rsrc);
			if(bytes>0){	//receive some data
				while(isNewLine(&dev.Rsrc.rxRB_Obj, cmdBuf, CMD_BUF_LEN)){
					lowercase(cmdBuf);
					if(indexOf(cmdBuf, "help")==0){
						printHelp();
						continue;
					}					
					tmpS8 = cppStyleCompile(cmdBuf, &packet);
					if(sysCmd(&packet))	{}
					else if(tmpS8 <0 )	print("+unknown@%s", cmdBuf);
					else if((packet.format[0]&0x3f) == ((PACKET_STR<<PACKET_DATA_BW) | PACKET_STR) ){
						iEn = 1;
						for(i=0;i<MAX_INSTANCE;i++){
							if(devInfo[i].base == NULL || devInfo[i].MailId == NULL)	continue;
							if(isSameStr(packet.addr[0], devInfo[i].devName)){
								pStr = registerMail((char*)cmdBuf);
								pMail = newMail(owner, devInfo[i].MailId, (u32)pStr);
								if(pMail == NULL){
									print("+err@uartTask.newMail.fail\r\n");
									break;
								}
								pMail->Content = packet;
								osMailPut(devInfo[i].MailId, pMail);
								iEn = 0;
								break;
							}
						}
						if(iEn)	{	print("+unknown@%s", cmdBuf);	}
					}

////					
////					iEn = 1;
////					if(sysCmd(&packet)==0){
////						if((cppStyleCompile(cmdBuf, &packet)>=0) && ((packet.format[0]&0x3f) == ((PACKET_STR<<PACKET_DATA_BW) | PACKET_STR)) ){	//meet format of "dev.func"
////							for(i=0;i<MAX_INSTANCE;i++){
////								if(devInfo[i].base == NULL || devInfo[i].MailId == NULL)	continue;
////								if(isSameStr(packet.addr[0], devInfo[i].devName)){
////									pStr = registerMail((char*)cmdBuf);
////									pMail = newMail(owner, devInfo[i].MailId, (u32)pStr);
////									if(pMail == NULL){
////										print("+err@uartTask.newMail.fail\r\n");
////										break;
////									}
////									pMail->Content = packet;
////									osMailPut(devInfo[i].MailId, pMail);
////									iEn = 0;
////								}
////							}						
////						}
////					}
////					else	iEn = 0;
					
//					if((cppStyleCompile(cmdBuf, &packet)>=0) && ((packet.format[0]&0x3f) == ((PACKET_STR<<PACKET_DATA_BW) | PACKET_STR)) ){	//meet format of "dev.func"
//						if(sysCmd(&packet)==0){
//							for(i=0;i<MAX_INSTANCE;i++){
//								if(devInfo[i].base == NULL || devInfo[i].MailId == NULL)	continue;
//								if(isSameStr(packet.addr[0], devInfo[i].devName)){
//									pStr = registerMail((char*)cmdBuf);
//									pMail = newMail(owner, devInfo[i].MailId, (u32)pStr);
//									if(pMail == NULL){
//										print("+err@uartTask.newMail.fail\r\n");
//										break;
//									}
//									pMail->Content = packet;
//									osMailPut(devInfo[i].MailId, pMail);
//									iEn = 0;
//								}
//							}
//						}
//						else	iEn = 0;
//					}
////					if(iEn)	{
////						print("+unknown@%s", cmdBuf);
////						iEn = 0;
////					}
				}
			}
		}
		//check if subject timeout
		for(i=0; i<DEPTH_OF_TRACE; i++){
			if(mailTrace[i].pCmd == NULL)	continue;
			if((mailTrace[i].tick++) > 1000){
				print("+timeout@%s", mailTrace[i].pCmd);
				if(unRegisterMail(mailTrace[i].pCmd) > 0)	debugPrint("unRegisterMail@timeout\r\n");
			}
		}
		ledTick += UART_TICK;
		if(ledTick > 64){
			ledTick = 0;
			if(running.GPIOx != NULL)	HAL_GPIO_TogglePin(running.GPIOx, running.GPIO_Pin);
		}
//		adc.Polling(&adc.rsrc, UART_TICK);
		osDelay(UART_TICK);
	}
}

static s8 createTaskUart(osPriority priority, ...){
	va_list ap;
	u8 i;
	s8 rtn=0;
	char* pStr;

	memset(&taskArgument, 0, sizeof(uartTaskArgument_t));	
	for(i=0;i<MAX_INSTANCE;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoUart;
			taskArgument.p = &devInfo[i];
			break;
		}
	}	
	if(taskArgument.p == NULL)	return -1;
	
	rtn = 0;
	va_start(ap, priority);	//get last arg addr		
	pStr = va_arg(ap, char*);
	strcpy(taskArgument.devName, pStr);
	taskArgument.LED = *(PIN_T*)va_arg(ap, PIN_T*);
	taskArgument.baudRate = va_arg(ap, u32);
	va_end(ap);	
	if(rtn==0){
		xTaskCreate( StartTaskUart, "uartTask", 200, &taskArgument, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgument.p->TaskID);		//stack size in 2bytes	
		while(taskArgument.p->MailId == NULL)	osDelay(1);				
	}
	return rtn;
}

static char* registerMail(const char* S){
	char* pStr;
	u16 i,j,k,len;

	//assign a mem for cmd
	len = strlen(S);
	if(len==0)	return NULL;
	for(i=0,pStr=NULL; i<CMD_BUF_LEN*4; i++){
		if(cmdStrPool[i])	continue;
		for(j=i,k=0;j<CMD_BUF_LEN*4;j++){
			if(cmdStrPool[j]==0){
				if(++k > (len+2))	{	//make sure buf is enouth for whole String
					pStr = &cmdStrPool[i+1];
					break;
				}
			}
			else	break;
		}
		if(pStr != NULL)	break;
	}
	if(pStr == NULL)	return NULL;
	//find a empty trace node
	for(i=0;i<DEPTH_OF_TRACE;i++){
		if(mailTrace[i].pCmd == NULL){
			mailTrace[i].pCmd = pStr;
			strcpy(pStr, S);
			mailTrace[i].tick = 0;
			return mailTrace[i].pCmd;
		}
	}
	return NULL;
}

static s8 unRegisterMail(char* s){
	u16 i;
	if(s == NULL)	return -1;
	for(i=0;i<DEPTH_OF_TRACE;i++){
		if(mailTrace[i].pCmd == s){
			mailTrace[i].pCmd = 0;
			for(i=0;1;i++){
				if(s[i]==0)	break;
				s[i] = 0;
			}			
			return 1;
		}
	}
	return 0;
}

static void debugPrint(const char* S){
	if(UART_DEBUG_PRNT_EN)	print(S);
}

void printS(const char* STRING){
	s16 bytes = strlen(STRING);
	if(bytes>0){
		xSemaphoreTake( xSemaphoreUART, portMAX_DELAY);
		dev.send(&dev.Rsrc, (const u8*)STRING, bytes);
		xSemaphoreGive( xSemaphoreUART);
	}
}

void print(const char* FORMAT_ORG, ...){
	va_list ap;
	s16 bytes;
	char pTmp[256]={0};
	//take string
	va_start(ap, FORMAT_ORG);
	bytes = vsnprintf(pTmp, 256, FORMAT_ORG, ap);
	va_end(ap);
	//send out
	if(bytes>0){
		xSemaphoreTake( xSemaphoreUART, portMAX_DELAY);
		dev.send(&dev.Rsrc, (const u8*)pTmp, bytes);
		xSemaphoreGive( xSemaphoreUART);
	}
}

static void printPacket(PAKET_T* p){
	char str[200];
	s16 rtn;

	if(p->style == PAKET_STYLE_ORG){
		rtn = packetToStrLine(str, 200, p);
		if(rtn > 0)		printS(str);
		else	print("@PACKET_ERROR%d 0x%x %x\n", rtn, p->format[1], p->format[0]);
	}
	else if(p->style == PAKET_STYLE_C){
		rtn = packetToStrC(str, 200, p);
		if(rtn > 0)		printS(str);
		else	print("@PACKET_ERROR%d 0x%x %x\n", rtn, p->format[1], p->format[0]);
	}
	else if(p->style == PAKET_STYLE_CPP){
		rtn = packetToStrCpp(str, 200, p);
		if(rtn > 0)		printS(str);
		else	print("@PACKET_ERROR%d 0x%x %x\n", rtn, p->format[1], p->format[0]);
	}
	else{	/*printPacketDetail(p);*/	}
}

void printPacketDetail(PAKET_T* p){
	char str[200];
	s16 rtn;
	rtn = packetToStrLine(str, 200, p);
	if(rtn > 0)	print("0x%x%x, %s", p->format[1], p->format[0], str);
	else	print("@PACKET_ERROR%d 0x%x %x\n", rtn, p->format[1], p->format[0]);
}

static u8 sysCmd(PAKET_T *packetIn){
//  u32 tmp;
	if(isSameStr(packetIn->addr[0], "sys")){
		//%sys.help()
		if(isSameStr(packetIn->addr[1], "help") && packetIsMatch(packetIn, "%s%s")){
			printHelp();
			return 1;
		}
		//%sys.list()
		else if(isSameStr(packetIn->addr[1], "listdev") && packetIsMatch(packetIn, "%s%s")){
			printDevList();
			return 1;
		}
		//%sys.about()
		else if(isSameStr(packetIn->addr[1], "about") && packetIsMatch(packetIn, "%s%s")){
			print("%s%s%s",SCH_VER,SRC_VER,INSTRUCTION_SET_VER);
			return 1;
		}
		//%sys.fwversiong()
		else if(isSameStr(packetIn->addr[1], "fwversion") && packetIsMatch(packetIn, "%s%s")){
			printS(SRC_VER);
			return 1;
		}

		
	}
	return 0;
}

static void printHelp(void){
	u8 i;
	for(i=0;i<MAX_INSTANCE;i++){
		if(devInfo[i].base->HELP != NULL)	printS(devInfo[i].base->HELP);
	}
}

static void printDevList(void){
	u8 i,j,k;
	for(i=0;i<MAX_TASK;i++){
		if(taskInfo[i] != NULL){
			print("%s%s", taskInfo[i]->DEV_TYPE, "(");
			k = 0;
			for(j=0;j<MAX_INSTANCE && taskInfo[i]->count>0;j++){
				if(devInfo[j].base->DEV_TYPE == taskInfo[i]->DEV_TYPE && devInfo[j].devName!=NULL){
					printS(devInfo[j].devName);					
					if(++k == taskInfo[i]->count)		break;
					else	printS(", ");
				}
			}
			printS(")\n");
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(dev.Rsrc.huart->Instance == huart->Instance)
		dev.txCpltCallback(&dev.Rsrc);
}

static u8 isNewLine(RINGBUFF_T* rb, char* line, u8 len){
	RINGBUFF_T tmp;
	tmp = *rb;
	u8 byte;
	s32 count,i,j,min;
	
	memset(line, 0, len);
	j = -1;
	count = RingBuffer_GetCount(&tmp);
	for(i=0;i<count;i++){
		RingBuffer_Pop(&tmp, &byte);
		if(byte == '\r'){
			i++;
			RingBuffer_Pop(&tmp, &byte);
			if(i<count && byte == '\n'){
				i++;
				j = i;
				break;
			}
			j = i;
			break;
		}
		else if(byte == '\n'){
			i++;
			RingBuffer_Pop(&tmp, &byte);
			if(i<count){
				if(byte == '\r'){
					i++;
					j = i;
					break;
				}	
			}
			j = i;
			break;
		}
	}
	//copy to line
	if(j>0){
		min = (len>j?j:len);
		RingBuffer_PopMult(rb, line, min);
		return 1;
	}
	return 0;
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
