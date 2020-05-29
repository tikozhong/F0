/**********************************************************
filename: dictionary.c
**********************************************************/

/************************包含头文件***********************/
#include "dictionary.h"
#include "myString.h"
/**********************************************************
 Private function
**********************************************************/
static s8 dictWriteStr	(DictRsrc_t* pRsrc, const char* segName, const char* content, u8 len);
static s8 dictReadStr		(DictRsrc_t* pRsrc, const char* segName, char* content, u8 len, u32* cycTz);
static void dictReadStrOut(DictRsrc_t* pRsrc);
//static s8 dictWriteNum	(DictRsrc_t* pRsrc, const char* segName, s32 dec);
//static s8 dictReadNum		(DictRsrc_t* pRsrc, const char* segName, s32* dec);
static s8 dictRemove		(DictRsrc_t* pRsrc, const char* segName);
static s8 dictRemoveAll(DictRsrc_t* pRsrc);
//static u32 dictGetCycle	(DictRsrc_t* pRsrc, const char* segName);
static u16 dictVerify(DictRsrc_t* pRsrc);
static u8 dictFetchLine(DictRsrc_t* pRsrc, u16 lineIndx, char* key, char* val);

//lower
static void dictMakeExtra(char* buf, const char* content, u8 len, u32 cyc);
static u32 fetchCycTz(const char* buf);

/**********************************************************
 Public function
**********************************************************/
void DictSetup(
	DictDev_T *pDev,
	u32 romStartAddr,
	u16 romLen,
	u32 cycTimes,
	s8 (*ioWrite)(u16 addr, const u8 *pDat, u16 nBytes),
	s8 (*ioRead)(u16 addr, u8 *pDat, u16 nBytes)
){
	//register resource
	pDev->rsrc.romStartAddr = romStartAddr;
	pDev->rsrc.romLen = romLen;
	pDev->rsrc.cycTimesUpper = cycTimes;
	pDev->rsrc.ioWrite = ioWrite;
	pDev->rsrc.ioRead = ioRead;
	pDev->rsrc.lines = romLen/DICT_LINE_SZ;	
	//register op
	pDev->WriteStr = dictWriteStr;
	pDev->ReadStr = dictReadStr;
	pDev->ReadStrOut = dictReadStrOut;
//	pDev->WriteNum = dictWriteNum;
//	pDev->ReadNum = dictReadNum;
	pDev->Remove = dictRemove;
	pDev->RemoveAll = dictRemoveAll;
//	pDev->GetCycle = dictGetCycle;
	pDev->Verify = dictVerify;
	pDev->FetchLine = dictFetchLine;
//	dictVerify(&pDev->rsrc);
}

/**********************************************************
 Public function
**********************************************************/
static s8 dictWriteStr	(DictRsrc_t* pRsrc, const char* segName, const char* content, u8 len){
	char x[DICT_LINE_SZ] = {0}, tmpU8, chk;
	u16 i,addr, emptyAddr;
	u32 cycTz;

	addr = pRsrc->romStartAddr;
	emptyAddr = 0xffff;
	for(i=0; i<pRsrc->lines; i++){
		pRsrc->ioRead(addr, (u8*)x, DICT_LINE_SZ);
		for(chk=0xca,tmpU8=0; tmpU8<31; tmpU8++)	chk ^= x[tmpU8];
		if(chk==x[31]){
			if(x[0]==0 && emptyAddr==0xffff){
				cycTz = fetchCycTz(x);
				if(cycTz < pRsrc->cycTimesUpper)	emptyAddr = addr;	//this is a empty line
			}
			else{
				for(tmpU8=0;tmpU8<4;tmpU8++){
					if(segName[tmpU8]==0 || x[tmpU8]==0)	break;
					if(segName[tmpU8] == x[tmpU8])				continue;	//case sensive
					else	break;
				}
				if(tmpU8==strlen(segName)||tmpU8==4){				//found
					dictMakeExtra(x, content, len, fetchCycTz(x)+1);
					pRsrc->ioWrite(addr, (u8*)x, DICT_LINE_SZ);
					return DICT_RTN_PASS;
				}
			}
		}
		else{	//meet a uninitial line
			emptyAddr = addr;
			cycTz = 0;
		}
		addr += DICT_LINE_SZ;
	}
	if(emptyAddr != 0xffff){
		//generate verify code
		memset(x,0,DICT_LINE_SZ);
		for(tmpU8=0;tmpU8<4;tmpU8++){
			if(segName[tmpU8]==0)	break;
			x[tmpU8] = segName[tmpU8];
		}
		cycTz++;
		dictMakeExtra(x,content,len,cycTz);
		pRsrc->ioWrite(emptyAddr, (u8*)x, DICT_LINE_SZ);
		return DICT_RTN_PASS; 
	}
	return DICT_RTN_OVER1;
}

static void dictReadStrOut(DictRsrc_t* pRsrc){
	u8 x[DICT_LINE_SZ] = {0}, tmpU8, chk;
	u16 i,addr;
	char key[5] = {0};
	
	addr = pRsrc->romStartAddr;
	for(i=0; i<pRsrc->lines; i++){
		pRsrc->ioRead(addr, x, DICT_LINE_SZ);
		for(chk=0xca,tmpU8=0; tmpU8<31; tmpU8++)	chk ^= x[tmpU8];
		if(chk == x[31] && strlen((char*)x)>0){	//check pass
			for(tmpU8=0;tmpU8<4;tmpU8++)
				key[tmpU8] = x[tmpU8];
			pRsrc->printOut((const char*)pRsrc->name,"read",key,(char*)&x[4]);
		}
		addr += DICT_LINE_SZ;
	}
}

static s8 dictReadStr(DictRsrc_t* pRsrc, const char* KEY, char* val, u8 len, u32* cycTz){
	u8 x[DICT_LINE_SZ] = {0}, tmpU8, chk;
	u16 i,addr;

	addr = pRsrc->romStartAddr;
	for(i=0; i<pRsrc->lines; i++){
		pRsrc->ioRead(addr, x, DICT_LINE_SZ);
		for(tmpU8=0;tmpU8<4;tmpU8++){
			if(KEY[tmpU8]==0 || x[tmpU8]==0)	break;
			if(KEY[tmpU8] == x[tmpU8])	continue;	//case sensive
			else	break;
		}
		if(tmpU8==strlen(KEY)||tmpU8==4){	//found
			for(chk=0xca,tmpU8=0; tmpU8<31; tmpU8++)	chk ^= x[tmpU8];
			if(chk == x[31]){	//check pass
				memcpy(val,&x[4],(len>24?24:len));
				if(cycTz != NULL)	*cycTz = (*(u32*)&x[4+24])&0x00ffffff;
				return DICT_RTN_PASS;
			}
			return DICT_RTN_CHK_FAIL;
		}
		addr += DICT_LINE_SZ;
	}
	return DICT_RTN_NO_SEG;
}

static s8 dictRemove(DictRsrc_t* pRsrc, const char* KEY){
	char x[DICT_LINE_SZ] = {0}, tmpU8,chk;
	u16 i,addr;
	u32 cycTz;

	addr = pRsrc->romStartAddr;
	for(i=0; i<pRsrc->lines; i++){
		pRsrc->ioRead(addr, (u8*)x, DICT_LINE_SZ);
		for(chk=0xca,tmpU8=0; tmpU8<31; tmpU8++)	chk ^= x[tmpU8];
		//if(chk!=x[31])	return DICT_RTN_CHK_FAIL;
		for(tmpU8=0;tmpU8<4;tmpU8++){
			if(KEY[tmpU8]==0 || x[tmpU8]==0)	break;
			if(KEY[tmpU8] == x[tmpU8])	continue;	//case sensive
			else	break;
		}
		if(tmpU8==strlen(KEY)||tmpU8==4){	//found
			cycTz = fetchCycTz(x)+1;
			memset(x,0,DICT_LINE_SZ);
			dictMakeExtra(x,NULL,0,cycTz);
			pRsrc->ioWrite(addr, (u8*)x, DICT_LINE_SZ);			
			return DICT_RTN_PASS;
		}
		addr += DICT_LINE_SZ;
	}
	return DICT_RTN_NO_SEG;
}

static s8 dictRemoveAll(DictRsrc_t* pRsrc){
	char x[DICT_LINE_SZ] = {0}, tmpU8, chk;
	u16 i,addr;
	u32 cycTz;
	s8 rtn = DICT_RTN_NO_SEG;

	addr = pRsrc->romStartAddr;
	for(i=0; i<pRsrc->lines; i++){
		pRsrc->ioRead(addr, (u8*)x, DICT_LINE_SZ);
		for(chk=0xca,tmpU8=0; tmpU8<31; tmpU8++)	chk ^= x[tmpU8];
		cycTz = 1;
		if(chk == x[31])	cycTz += fetchCycTz(x);
		memset(x,0,DICT_LINE_SZ);
		dictMakeExtra(x,NULL,0,cycTz);
		pRsrc->ioWrite(addr, (u8*)x, DICT_LINE_SZ);
		rtn =  DICT_RTN_PASS;
		addr += DICT_LINE_SZ;
	}
	return rtn;
}

//static s8 dictWriteNum	(DictRsrc_t* pRsrc, const char* segName, s32 dec){
//	return DICT_RTN_PASS;
//}
//static s8 dictReadNum		(DictRsrc_t* pRsrc, const char* segName, s32* dec){
//	return DICT_RTN_PASS;
//}

//static u32 dictGetCycle	(DictRsrc_t* pRsrc, const char* segName){
//	return 0;
//}

static void dictMakeExtra(char* buf, const char* content, u8 len, u32 cycTz){
	u8 tmpU8,dictLen;
	//replace content
	memset(&buf[4], 0, 24);
	if(content != NULL){
		dictLen = (len>24?24:len);
		for(tmpU8=0; tmpU8<dictLen; tmpU8++)	buf[4+tmpU8] = content[tmpU8];
	}
	//cyctimes
	buf[4+24+0] = cycTz & 0xff;	cycTz >>=8;
	buf[4+24+1] = cycTz & 0xff;	cycTz >>=8;
	buf[4+24+2] = cycTz & 0xff;	
	//generate verify code
	for(buf[4+24+3]=0xca,tmpU8=0; tmpU8<31; tmpU8++)	buf[4+24+3] ^= buf[tmpU8];
}

static u32 fetchCycTz(const char* buf){
	u32 cycTz = *(u32*)&buf[4+24]&0x00ffffff;
	return cycTz;
}

static u16 dictVerify(DictRsrc_t* pRsrc){
	u8 x[DICT_LINE_SZ] = {0}, tmpU8, chk;
	u16 i,addr,rtn=0;
	addr = pRsrc->romStartAddr;
	for(i=0; i<pRsrc->lines; i++){
		pRsrc->ioRead(addr, x, DICT_LINE_SZ);
		for(chk=0xca,tmpU8=0; tmpU8<31; tmpU8++)	chk ^= x[tmpU8];
		if(chk != x[31]){	//check fail
			rtn ++;
			memset(x,0,32);
			x[4+24] = 1;
			for(x[31]=0xca,tmpU8=0; tmpU8<31; tmpU8++)	x[31] ^= x[tmpU8];
			pRsrc->ioWrite(addr, x, DICT_LINE_SZ);
		}
		addr += DICT_LINE_SZ;
	}
	return rtn;
}

static u8 dictFetchLine(DictRsrc_t* pRsrc, u16 lineIndx, char* key, char* val){
	u8 x[DICT_LINE_SZ] = {0}, tmpU8, chk;

	pRsrc->ioRead(pRsrc->romStartAddr+lineIndx*DICT_LINE_SZ, x, DICT_LINE_SZ);
	for(chk=0xca,tmpU8=0; tmpU8<31; tmpU8++)	chk ^= x[tmpU8];
	if(chk == x[31]){	//check pass
		memcpy(key, x, 4);
		memcpy(val,&x[4],24);
		return 1;
	}
	return 0;
}


/**********************************************************
 == THE END ==
**********************************************************/
