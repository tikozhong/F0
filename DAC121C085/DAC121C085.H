/**********************************************************
filename: dac121c085.h
**********************************************************/
#ifndef _DAC121C085_H_
#define _DAC121C085_H_

#include "misc.h"
#include "IIC_IO.h"

#define	DAC_VOL_MV(refVol, expectVol)	(expectVol*4096/refVol)	

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	/* config	*/
//	IIC_IO_Dev_T iic;
	PIN_T scl,sda;
	u8 devAddr;
	u16 lastUpdate;
	u16 refVol;	//in mV
}DAC121C085_Rsrc_T;

/*
	when page write more than 64 bytes,
	pBuf must point to a static pointer
*/
typedef struct{
	DAC121C085_Rsrc_T rsrc;
	void	(*SetVol)(DAC121C085_Rsrc_T*, u16 mv);
	void	(*SetRef)(DAC121C085_Rsrc_T*, u16 mv);
	u16	(*GetRef)(DAC121C085_Rsrc_T*);
	void 	(*PowerDown)(DAC121C085_Rsrc_T*, u8 mod);
}DAC121C085_Dev_T;

DEV_STATUS DAC121C085_Setup(
	DAC121C085_Dev_T *pDev, 
	const PIN_T SCL, 
	const PIN_T SDA, 
	const u8 devAddr,
	u16 refVol,
	u16 initVal
);
	
#endif
