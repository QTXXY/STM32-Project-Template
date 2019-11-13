#include "twsimaster.h"

unsigned char TimeCount;

void TwsiConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);    //使能PB端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//SCL和SDA端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 	    //开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);                  
}


/*****************************************************************************
*
*  名称:   DelayTime
*
*  描述:   延时
*
*****************************************************************************/

void DelayTime(void)
{
   int i;
   
   for ( i = 0; i < TIME_DELAY; i++ )
   {
      ;
   }
	
//	while(i--)
//	{
//		delay_us(TIME_DELAY); //100us  f = 10K
//	}
}

/*****************************************************************************
*
*  名称:   InitTWSIMaster
*
*  描述:   初始化 模拟I2C
*
*****************************************************************************/

void InitTWSIMaster(void)
{
	TwsiConfig();
	
	DDR_SDA  = 0;   
	DDR_SCL  = 0;
}

/*****************************************************************************
*
*  名称:   SCL_H
*
*  描述:   SCL高
*
*****************************************************************************/

void SCL_H(void)
{
   DDR_SCL = 1;
}


/*****************************************************************************
*
*  名称:   SCL_H
*
*  描述:   SCL低
*
*****************************************************************************/

void SCL_L(void)
{
   DDR_SCL = 0;
}


/*****************************************************************************
*
*  名称:   SCL_IN
*
*  描述:   SCL电平检测
*
*****************************************************************************/

unsigned char SCL_IN(void)
{
   if (PIN_SCL)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

/*****************************************************************************
*
*  名称:   SDA_H
*
*  描述:   SDA高
*
*****************************************************************************/

void SDA_H(void)
{
   DDR_SDA = 1;
}


/*****************************************************************************
*
*  名称:   SDA_H
*
*  描述:   SDA低
*
*****************************************************************************/

void SDA_L(void)
{
   DDR_SDA = 0;
}


/*****************************************************************************
*
*  名称:   SDA_IN
*
*  描述:   SDA输入
*
*****************************************************************************/

unsigned char SDA_IN(void)
{
   if (PIN_SDA)
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

/*****************************************************************************
*
*  名称:   ReleaseSDA
*
*  描述:   释放SDA总线
*
*****************************************************************************/

void ReleaseSDA(void)
{
   DDR_SDA = 1;   //释放SDA总线，SDA被上拉，为H
}


/*****************************************************************************
*
*  名称:   ReleaseSCL
*
*  描述:   释放SCL总线
*
*****************************************************************************/

void ReleaseSCL(void)
{
   DDR_SCL = 1;
}

/*****************************************************************************
*
*  名称:   SDA_OUT
*
*  描述:   SDA输出
*
*****************************************************************************/

void SDA_OUT(unsigned char BitData)
{
   if (BitData)
   {
      SDA_H();
   }
   else
   {
      SDA_L();
   }
}


/*****************************************************************************
*
*  名称:   Bit_Out
*
*  描述:   输出一个bit数据
*
*****************************************************************************/

unsigned char Bit_Out( unsigned char BitData )
{ 
   SDA_OUT(BitData);
   
   DelayTime();
   
   SCL_H();
   
   TimeCount = TIME_OUT_COUNT;
   
   do
   {
      DelayTime();
      TimeCount--;
      if ( TimeCount == 0 )
      {
         return ERROR_DATA_OUT_TIME_OUT;   // I2C 超时
      }
   }while(!SCL_IN());
   
   SCL_L();
   DelayTime();
   
   return 0;
   
}


/*****************************************************************************
*
*  名称:   Bit_IN
*
*  描述:   输入一个bit数据
*
*****************************************************************************/

unsigned char Bit_IN(void)
{
   unsigned  char BitData;
   
   
   SCL_H();
   
   TimeCount = TIME_OUT_COUNT;
      
   do
   {
      DelayTime();
      TimeCount--;
      if ( TimeCount == 0 )
      {
         //return ERROR_DATA_IN_TIME_OUT;   // I2C 超时 输入出错可以通过读出的数据看出，所以可以不用错误代码
      }
   }while(!SCL_IN());
   
   
   BitData = SDA_IN();
   
   SCL_L();
   
   DelayTime();
   
   return BitData;
}


/*****************************************************************************
*
*  名称:   ACK
*
*  描述:   输入一个Byte数据
*
*****************************************************************************/

void SendACK(void)
{
   SDA_L();
   DelayTime();
   SCL_H();
   DelayTime();
   SCL_L();
}


/*****************************************************************************
*
*  名称:   ACK
*
*  描述:   输入一个Byte数据
*
*****************************************************************************/

void SendNACK(void)
{
   SDA_H();
   DelayTime();
   SCL_H();
   DelayTime();
   SCL_L();
}


/*****************************************************************************
*
*  名称:   Byte_IN
*
*  描述:   输入一个Byte数据
*
*****************************************************************************/

unsigned char Byte_IN(void)
{
   unsigned  char ByteData;
   int i;

   ByteData = 0;
   
   ReleaseSDA();
   
   for ( i = 0; i < 8; i++ )
   {  
      ByteData<<=1;
      ByteData |= Bit_IN();
   }
   
 
   //Bit_Out(ACK);
   
   return ByteData;
}


/*****************************************************************************
*
*  名称:   Byte_OUT
*
*  描述:   输出一个Byte数据
*
*****************************************************************************/

unsigned char Byte_OUT(unsigned char ByteData)
{
   int i;
   //unsigned char ACK;
   
   
   for ( i = 0; i < 8; i++ )
   {  
      if ( ByteData & 0x80)
      {
         if ( Bit_Out(1) )
         {
            return ERROR_DATA_OUT_TIME_OUT;
         }
      }
      else
      {
         if ( Bit_Out(0) )
         {
            return ERROR_DATA_OUT_TIME_OUT;
         } 
      }
      
      ByteData<<=1;
   }
  /* 
   ReleaseSDA();
   
   ACK = Bit_IN();
   
   if (ACK)
   {
      return ERROR_NO_ACK;
   }
   */
   return 0;
}


/*****************************************************************************
*
*  名称:   Start
*
*  描述:   发送开始条件
*
*****************************************************************************/

void Start(void)
{

   SCL_L();
   DelayTime();
   SDA_H();
   DelayTime();
   SCL_H();
   DelayTime(); 
   SDA_L();
   DelayTime();
   SCL_L();  
   DelayTime();
}


/*****************************************************************************
*
*  名称:   Stop
*
*  描述:   发送停止条件
*
*****************************************************************************/

void Stop(void)
{

   SCL_L();
   DelayTime();
   SDA_L();
   DelayTime();
   SCL_H();
   DelayTime();
   SDA_H();   
   DelayTime();
   SCL_L();  
   DelayTime();
}

/*****************************************************************************
*
*  名称:   I2CMasterReadData
*
*  描述:   读数据
*
*****************************************************************************/

unsigned char I2CMasterReadData( unsigned char DeviceAddr, unsigned char RegAddr, unsigned int count, unsigned char *DataBuffer)
{
   unsigned char SLA_W;
   unsigned char SLA_R;
   unsigned char ACK;
   
   SLA_W = DeviceAddr & 0xFE;  //8位地址,最后一位是读写指令   0 写
   SLA_R = DeviceAddr | 0x01;  // 1 读
   
   
   // 发送起始条件
   Start();                    //主机给从机发送起始信号
   
   // 发送器件地址
   if ( Byte_OUT(SLA_W) )      //发送从机地址 写
   {
      return ERROR_DATA_OUT_TIME_OUT;
   }
   
   ReleaseSDA();   
   
   ACK = Bit_IN();
   
   if (ACK)
   {
      return ERROR_NO_ACK;
   }
   
   // 发送寄存器地址
   if ( Byte_OUT(RegAddr) )
   {
      return ERROR_DATA_OUT_TIME_OUT;
   }
   
   
   ReleaseSDA();   

   ACK = Bit_IN();
   
   if (ACK)
   {
      return ERROR_NO_ACK;
   }
   
   
   // 发送 重复 起始条件
   Start();
   
   // 发送器件地址
   if ( Byte_OUT(SLA_R) )
   {
      return ERROR_DATA_OUT_TIME_OUT;
   }
   
   ReleaseSDA();   

   ACK = Bit_IN();
   
   if (ACK)
   {
      return ERROR_NO_ACK;
   }
   
   // 返回数据
   *DataBuffer = Byte_IN();
   DataBuffer++;
   
   count--;
   
   while(count--)
   {
      SendACK();
      *DataBuffer = Byte_IN();
      DataBuffer++;
   }
   
   SendNACK();
   
   Stop();

   return 0;   
}



/*****************************************************************************
*
*  名称:   I2CMasterWriteData
*
*  描述:   写数据
*
*****************************************************************************/

unsigned char I2CMasterWriteData( unsigned char DeviceAddr, unsigned char RegAddr, unsigned int count, unsigned char *DataBuffer)
{
   unsigned char SLA_W;
   unsigned char ACK;
   
   SLA_W = DeviceAddr & 0xFE;  //获取从机地址
   
   
   // 发送起始条件
   Start();
   
   // 发送器件地址
   if ( Byte_OUT(SLA_W) )
   {
      return ERROR_DATA_OUT_TIME_OUT;
   }
   
   ReleaseSDA();         //释放数据线
   
   ACK = Bit_IN();      //判断是否超时应答
   
   if (ACK)
   {
      return ERROR_NO_ACK;
   }
   
   // 发送寄存器地址
   if ( Byte_OUT(RegAddr) )
   {
      return ERROR_DATA_OUT_TIME_OUT;
   }
   
   
   ReleaseSDA();   
   
   ACK = Bit_IN();
   
   if (ACK)
   {
      return ERROR_NO_ACK;
   }
   
   // 发送数据
   while(count--)
   {
      // 发送数据
      if ( Byte_OUT(*DataBuffer) )
      {
         return ERROR_DATA_OUT_TIME_OUT;
      }
	  
      DataBuffer++;
      ReleaseSDA();   
      
      ACK = Bit_IN();
      
      if (ACK)
      {
         return ERROR_NO_ACK;
      } 
   }
   
   Stop();

   return 0;   
}




/*****************************************************************************
*
*  名称:   I2CMasterCurrentReadData
*
*  描述:   读数据
*
*****************************************************************************/

unsigned char I2CMasterCurrentReadData( unsigned char DeviceAddr, unsigned char count, unsigned char *DataBuffer)
{
   unsigned char SLA_R;
   unsigned char ACK;
   
   SLA_R = DeviceAddr | 0x01;
   
   
   
   // 发送 起始条件
   Start();
   
   // 发送器件地址
   if ( Byte_OUT(SLA_R) )
   {
      return ERROR_DATA_OUT_TIME_OUT;
   }
   ReleaseSDA();   
   
   ACK = Bit_IN();
   
   if (ACK)
   {
      return ERROR_NO_ACK;
   }
   
   // 返回数据
   *DataBuffer = Byte_IN();
   DataBuffer++;
   
   count--;
   
   while(count--)
   {
      SendACK();
      *DataBuffer = Byte_IN();
      DataBuffer++;
   }
   
   SendNACK();
   
   Stop();

   return 0;   
}






