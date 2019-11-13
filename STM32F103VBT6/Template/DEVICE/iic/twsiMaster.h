#ifndef __TWSIMASTER_H__
#define __TWSIMASTER_H__

#include "sys.h"
#include "delay.h"


#define SCL_O PBout(6)
#define SCL_I PBin(6)

#define SDA_O PBout(7)
#define SDA_I PBin(7)

#define  DDR_SDA   SDA_O        //输出  
#define  PIN_SDA   SDA_I         //输入  

#define  DDR_SCL   SCL_O            //输出
#define  PIN_SCL   SCL_I            //输入


#define TIME_OUT_COUNT  10    //10
#define TIME_DELAY      100 //100

#define ERROR_DATA_IN_TIME_OUT   1
#define ERROR_DATA_OUT_TIME_OUT  2
#define ERROR_NO_ACK             3

extern void TwsiConfig(void);

void DelayTime(void);
extern void InitTWSIMaster(void);
void SCL_H(void);
void SCL_L(void);
unsigned char SCL_IN(void);
void SDA_H(void);
void SDA_L(void);
unsigned char SDA_IN(void);
void ReleaseSDA(void);
void ReleaseSCL(void);

void SDA_OUT(unsigned char BitData);
unsigned char Bit_Out( unsigned char BitData );
unsigned char Bit_IN(void);

void SendACK(void);
void SendNACK(void);

unsigned char Byte_IN(void);

unsigned char Byte_OUT(unsigned char ByteData);
void Start(void);
void Stop(void);


unsigned char I2CMasterReadData( unsigned char DeviceAddr, unsigned char RegAddr, unsigned int count, unsigned char *DataBuffer);

unsigned char I2CMasterWriteData( unsigned char DeviceAddr, unsigned char RegAddr, unsigned int count, unsigned char *DataBuffer);

unsigned char I2CMasterCurrentReadData( unsigned char DeviceAddr, unsigned char count, unsigned char *DataBuffer);

#endif
