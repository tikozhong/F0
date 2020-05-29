/**********************************************
filename:	sysStorage.c
**********************************************/
#include "sysStorage.h"

void cylinderTick(CLDR_RSRC_T* pRsrc);
void cylinderGoto(CLDR_RSRC_T* pRsrc, cldrPos_T pos, u16 timeout);

void cylinderSetup(CLDR_DEV_T *pDev, const u8 *NAME){
	if(NAME != NULL)	devRename(pDev->rsrc.name, NAME);
	pDev->TickTask = cylinderTick;
	pDev->GotoPos = cylinderGoto;
}

/*******************************************************************************
* Function Name  : cylinderTick
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void cylinderTick(CLDR_RSRC_T* pRsrc){
	cldrPos_T pos = POS_MOVING;
	cldrLogic_t resetSnsLogic = CLDR_L, actionSnsLogic = CLDR_L;
	INPUT_RSRC_T *pInputRsrc;
	OUTPUT_RSRC_T *pOutputRsrc;
	
	/* update curPosition */
	pInputRsrc = &pRsrc->pInputDev->rsrc;
	if(pRsrc->pInputDev->ReadPin(pInputRsrc, pRsrc->sensorReset.INDX)!=0)	
		resetSnsLogic = CLDR_H;
	if(pRsrc->pInputDev->ReadPin(pInputRsrc, pRsrc->sensorAction.INDX)!=0)	
		actionSnsLogic = CLDR_H;
	if(resetSnsLogic==pRsrc->sensorReset.resetPosLogic && actionSnsLogic==pRsrc->sensorAction.resetPosLogic )
		pos = POS_RESET;
	if(resetSnsLogic!=pRsrc->sensorReset.resetPosLogic && actionSnsLogic!=pRsrc->sensorAction.resetPosLogic )
		pos = POS_ACTION;
	if(pRsrc->curPos != pos){
		if((pos==POS_RESET)&&(pRsrc->newPosCallback!=NULL))		pRsrc->newPosCallback(pRsrc->name,POS_RESET);
		if((pos==POS_ACTION)&&(pRsrc->newPosCallback!=NULL))	pRsrc->newPosCallback(pRsrc->name,POS_ACTION);
		pRsrc->curPos = pos;
	}
	
	/* drive to position */
	pOutputRsrc = &pRsrc->pOutputDev->rsrc;
	if(pRsrc->curPos != pRsrc->tgtPos){
		if(pRsrc->tgtPos == POS_ACTION){
			if(pRsrc->drvReset.resetPosLogic == CLDR_L)
				pRsrc->pOutputDev->WritePin(pOutputRsrc, pRsrc->drvReset.INDX, PIN_SET);
			else
				pRsrc->pOutputDev->WritePin(pOutputRsrc, pRsrc->drvReset.INDX, PIN_RESET);
				
			if(pRsrc->drvAction.resetPosLogic == CLDR_L)
				pRsrc->pOutputDev->WritePin(pOutputRsrc, pRsrc->drvAction.INDX, PIN_SET);
			else
				pRsrc->pOutputDev->WritePin(pOutputRsrc, pRsrc->drvAction.INDX, PIN_RESET);
		}
		else{
			if(pRsrc->drvReset.resetPosLogic == CLDR_L)
				pRsrc->pOutputDev->WritePin(pOutputRsrc, pRsrc->drvReset.INDX, PIN_RESET);
			else
				pRsrc->pOutputDev->WritePin(pOutputRsrc, pRsrc->drvReset.INDX, PIN_SET);
			if(pRsrc->drvAction.resetPosLogic == CLDR_L)
				pRsrc->pOutputDev->WritePin(pOutputRsrc, pRsrc->drvAction.INDX, PIN_RESET);
			else
				pRsrc->pOutputDev->WritePin(pOutputRsrc, pRsrc->drvAction.INDX, PIN_SET);
		}
	}
	pRsrc->tick++;
	if((pRsrc->tick*pRsrc->tickUnit) >= pRsrc->timeout)	{
		if(pRsrc->timeOutCallback != NULL)	pRsrc->timeOutCallback(pRsrc->name);
		pRsrc->tick &= 0x7fff;
	}
}

/*******************************************************************************
* Function Name  : cylinderGotoUntil
* Description    : 
* Input          : None
* Output         : None
* Return         : CYLDR_TIMEOUT: timeout		CYLDR_GOTTO:reach pos within timeout
*******************************************************************************/
void cylinderGoto(CLDR_RSRC_T* pRsrc, cldrPos_T pos, u16 timeout){	
	pRsrc->tgtPos = pos;
	pRsrc->tick = 0;
	pRsrc->timeout = timeout;
}

