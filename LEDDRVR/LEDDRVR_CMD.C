/**********************************************************
filename: leddrvr_cmd.c
**********************************************************/

/*********************************************************/
#include "leddrvr_cmd.h"
#include "myString.h"
#include "string.h"
#include "compiler.h"

//#include "myuart_os.h"

const u8 LEDDRVR_HELP[] = {
	"LEDDRVR Task Commands:"
	"\n sys.help(\"leddrvr_t\")"
	"\nLedDrvr Task Commands:"
	"\n eeprom.writeCell(index, \"content\")"
	"\n eeprom.readCell(index)"
	"\n eeprom.format()/(initVal)"
	"\n eeprom.info()"
	"\n eeprom.saveAsCsv(\"eeprom.csv\")\r\n"
};

//#include "myUart.h"
//extern UART_DEV dev;

//static s8 fetchNum(const u8 *NUMSTR, u32* dat);

s8 ledDrvrCmd(LedDrvrDev_t *pDev, PAKET_T* packetIn, PAKET_T* packetRtn){
	u8 str[64] = {0};
	u16 config;

	if(cppStyleIsMatchDev(packetIn, (const char*)pDev->rsrc.name) < 0 )	return -1;
	packetReset(packetRtn);
	packetRtn->option0 = 2;	
	//setBrightness
	if(cppStyleIsMatchFunc(packetIn, "setbrightness")){
		if(cppStyleIsMatchArgu(packetIn, "%u%u")){
			if(pDev->SetBrightness(&pDev->rsrc, *(u32*)packetIn->addr[2], *(u32*)packetIn->addr[3])>=0)
				strCatX(str, "+ok@", (const u8*)packetIn->addr[0], NULL);			
			else	strCatX(str, "+err@", (const u8*)packetIn->addr[0], NULL);		
			packetAddTo(packetRtn,"%s", str);
			packetAddTo(packetRtn,"%s", packetIn->addr[1]);
			packetAddTo(packetRtn,"%u", *(u32*)packetIn->addr[2]);
			packetAddTo(packetRtn,"%u", *(u32*)packetIn->addr[3]);
			return 0;
		}
		else if(cppStyleIsMatchArgu(packetIn, "%u")){
			pDev->SetAllBrightness(&pDev->rsrc, *(u32*)packetIn->addr[2]);
			strCatX(str, "+ok@", (const u8*)packetIn->addr[0], NULL);
			packetAddTo(packetRtn,"%s", str);
			packetAddTo(packetRtn,"%s", packetIn->addr[1]);
			packetAddTo(packetRtn,"%u", *(u32*)packetIn->addr[2]);
			return 0;
		}
		strCatX(str, "+err.argu@", (const u8*)packetIn->addr[0], NULL);
		packetAddTo(packetRtn,"%s", str);
		packetAddTo(packetRtn,"%s", packetIn->addr[1]);
		packetAddTo(packetRtn,"%s", "unknown arguments");
		return 0;
	}
	//setResolution
	if(cppStyleIsMatchFunc(packetIn, "setresolution")){
		if(cppStyleIsMatchArgu(packetIn, "%u")){
			pDev->SetLedResolution(&pDev->rsrc, *(u32*)packetIn->addr[2]);
			strCatX(str, "+ok@", (const u8*)packetIn->addr[0], NULL);			
			packetAddTo(packetRtn,"%s", str);
			packetAddTo(packetRtn,"%s", packetIn->addr[1]);
			packetAddTo(packetRtn,"%u", *(u32*)packetIn->addr[2]);
			return 0;
		}
		strCatX(str, "+err.argu@", (const u8*)packetIn->addr[0], NULL);
		packetAddTo(packetRtn,"%s", str);
		packetAddTo(packetRtn,"%s", packetIn->addr[1]);
		packetAddTo(packetRtn,"%s", "unknown arguments");
		return 0;
	}
	
	//getBrightness
	else if(cppStyleIsMatchFunc(packetIn, "getbrightness") == 0 ){
	}
	//setSwitch
	if(cppStyleIsMatchFunc(packetIn, "switch")){
		if(cppStyleIsMatchArgu(packetIn, NULL)){
			pDev->ToggleLed(&pDev->rsrc);
			strCatX(str, "+ok@", (const u8*)packetIn->addr[0], NULL);
			packetAddTo(packetRtn,"%s", str);
			packetAddTo(packetRtn,"%s", packetIn->addr[1]);
			return 0;
		}
		strCatX(str, "+err.argu@", (const u8*)packetIn->addr[0], NULL);
		packetAddTo(packetRtn,"%s", str);
		packetAddTo(packetRtn,"%s", packetIn->addr[1]);
		packetAddTo(packetRtn,"%s", "unknown arguments");
		return 0;
	}
	//setConfig
	if(cppStyleIsMatchFunc(packetIn, "setconfig")){
		if(cppStyleIsMatchArgu(packetIn, "%u%u")){
			if(pDev->SetConfig(&pDev->rsrc, *(u32*)packetIn->addr[2], *(u32*)packetIn->addr[3]) >=0 )
				strCatX(str, "+ok@", (const u8*)packetIn->addr[0], NULL);
			else	strCatX(str, "+err@", (const u8*)packetIn->addr[0], NULL);
			packetAddTo(packetRtn,"%s", str);
			packetAddTo(packetRtn,"%s", packetIn->addr[1]);
			packetAddTo(packetRtn,"%u", *(u32*)packetIn->addr[2]);
			packetAddTo(packetRtn,"%u", *(u32*)packetIn->addr[3]);
			return 0;
		}
		strCatX(str, "+err.argu@", (const u8*)packetIn->addr[0], NULL);
		packetAddTo(packetRtn,"%s", str);
		packetAddTo(packetRtn,"%s", packetIn->addr[1]);
		packetAddTo(packetRtn,"%s", "unknown arguments");
		return 0;
	}
	
	//getConfig
	if(cppStyleIsMatchFunc(packetIn, "getconfig")){
		if(cppStyleIsMatchArgu(packetIn, "%u")){
			if(pDev->GetConfig(&pDev->rsrc, *(u32*)packetIn->addr[2], &config) >=0 )
				strCatX(str, "+ok@", (const u8*)packetIn->addr[0], NULL);
			else	strCatX(str, "+err@", (const u8*)packetIn->addr[0], NULL);
			packetAddTo(packetRtn,"%s", str);
			packetAddTo(packetRtn,"%s", packetIn->addr[1]);
			packetAddTo(packetRtn,"%u", *(u32*)packetIn->addr[2]);
			return 0;
		}
		strCatX(str, "+err.argu@", (const u8*)packetIn->addr[0], NULL);
		packetAddTo(packetRtn,"%s", str);
		packetAddTo(packetRtn,"%s", packetIn->addr[1]);
		packetAddTo(packetRtn,"%s", "unknown arguments");
		return 0;
	}

//	//setMaxCurrent
//	if(cppStyleIsMatchFunc(packetIn, "setmaxcurrent")){
//		if(cppStyleIsMatchArgu(packetIn, "%u")){
//			if(pDev->s(&pDev->rsrc, *(u32*)packetIn->addr[2], *(u32*)packetIn->addr[3])>=0)
//				strCatX(str, "+ok@", (const u8*)packetIn->addr[0], NULL);			
//			else	strCatX(str, "+err@", (const u8*)packetIn->addr[0], NULL);		
//			packetAddTo(packetRtn,"%s", str);
//			packetAddTo(packetRtn,"%s", packetIn->addr[1]);
//			packetAddTo(packetRtn,"%u", *(u32*)packetIn->addr[2]);
//			packetAddTo(packetRtn,"%u", *(u32*)packetIn->addr[3]);
//			return 0;
//		}
//		strCatX(str, "+err.argu@", (const u8*)packetIn->addr[0], NULL);
//		packetAddTo(packetRtn,"%s", str);
//		packetAddTo(packetRtn,"%s", packetIn->addr[1]);
//		packetAddTo(packetRtn,"%s", "unknown arguments");
//		return 0;
//	}

	strCatX(str, "+unknown@", (const u8*)packetIn->addr[0], NULL);
	packetAddTo(packetRtn,"%s", str);
	packetAddTo(packetRtn,"%s", packetIn->addr[1]);
	packetAddTo(packetRtn,"%s", "unknown method");
	return 0;
}



//s8 ledDrvrCmdIntelligent(LedDrvrDev_t *dev, const u8* CMD, u8* pRtn, u8 sz){
//	u8 str[64] = {0};
//	u16 config;
//	s16 indx;
//	u32 rnt[4];
//	//WRITE M LED [CHAN] [LED] [DRIVE VAL] - Set LED Drive Value
//	indx = indexOf(CMD, (const u8*)"write m led ");
//	if(indx == 0){
//		fetchNum(&CMD[indx+strlen("write m led ")], rnt);
//		dev->SetBrightness(&dev->rsrc, rnt[0]*17 + rnt[1], rnt[2]);
//		
//		strCpy(pRtn, sz, (const u8*)"ok ");

//		CONVERT.Uint32ToDecStrN(&pRtn[3], rnt[0], 4);
//		strcat((char*)pRtn, ",");
//		
//		CONVERT.Uint32ToDecStrN(&pRtn[8], rnt[1], 4);
//		strcat((char*)pRtn, ",");

//		CONVERT.Uint32ToDecStrN(&pRtn[13], rnt[2], 4);
//		strcat((char*)pRtn, "\r\n");

//		return 1;
//	}
//	
//	indx = indexOf(CMD, (const u8*)"write a led ");
//	if(indx == 0){
//		return 1;
//	}
//	
//	indx = indexOf(CMD, (const u8*)"write cal ");
//	if(indx == 0){
//		return 1;
//	}
//	
//	indx = indexOf(CMD, (const u8*)"read cal ");
//	if(indx == 0){
//		return 1;
//	}
//	
//	indx = indexOf(CMD, (const u8*)"write l eeprom ");
//	if(indx == 0){
//		return 1;
//	}
//	
//	indx = indexOf(CMD, (const u8*)"read l eeprom ");
//	if(indx == 0){
//		return 1;
//	}
//	
//	indx = indexOf(CMD, (const u8*)"write sn ");
//	if(indx == 0){
//		return 1;
//	}
//	
//	indx = indexOf(CMD, (const u8*)"read sn ");
//	if(indx == 0){
//		return 1;
//	}
//	
//	indx = indexOf(CMD, (const u8*)"read cal ");
//	if(indx == 0){
//		return 1;
//	}
//	
//	return 0;
//}


//static s8 fetchNum(const u8 *NUMSTR, u32* dat){
//	s8 indx;
//	u8 indx0 = 0;
//	u32 tmp;

//	indx = CONVERT.StrToUint32(NUMSTR, &tmp);
//	if(indx >= 0){
//		dat[0] = tmp;
//		indx0 += indx+1;
//	}
//	else return 0;
////	dev.PrintStr(&dev.Rsrc, &NUMSTR[indx0]);

//	indx = CONVERT.StrToUint32(&NUMSTR[indx0], &tmp);
//	if(indx >= 0){
//		dat[1] = tmp;
//		indx0 += indx+1;
//	}
//	else return 1;
////	dev.PrintStr(&dev.Rsrc, &NUMSTR[indx0]);
//	
//	indx = CONVERT.StrToUint32(&NUMSTR[indx0], &tmp);
//	if(indx >= 0){
//		dat[2] = tmp;
//		indx0 += indx+1;
//	}
//	else return 2;
////	dev.PrintStr(&dev.Rsrc, &NUMSTR[indx0]);
//	
//	indx = CONVERT.StrToUint32(&NUMSTR[indx0], &tmp);
//	if(indx >= 0){
//		dat[3] = tmp;
//		return 4;
//	}
//	else return 3;
//}

/**********************************************************
 == THE END ==
**********************************************************/
