#include "twsimaster.h"

unsigned char TimeCount;

void TwsiConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);    //ʹ��PB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;	//SCL��SDA�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 	    //��©���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);                  
}


/*****************************************************************************
*
*  ����:   DelayTime
*
*  ����:   ��ʱ
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
*  ����:   InitTWSIMaster
*
*  ����:   ��ʼ�� ģ��I2C
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
*  ����:   SCL_H
*
*  ����:   SCL��
*
*****************************************************************************/

void SCL_H(void)
{
   DDR_SCL = 1;
}


/*****************************************************************************
*
*  ����:   SCL_H
*
*  ����:   SCL��
*
*****************************************************************************/

void SCL_L(void)
{
   DDR_SCL = 0;
}


/*****************************************************************************
*
*  ����:   SCL_IN
*
*  ����:   SCL��ƽ���
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
*  ����:   SDA_H
*
*  ����:   SDA��
*
*****************************************************************************/

void SDA_H(void)
{
   DDR_SDA = 1;
}


/*****************************************************************************
*
*  ����:   SDA_H
*
*  ����:   SDA��
*
*****************************************************************************/

void SDA_L(void)
{
   DDR_SDA = 0;
}


/*****************************************************************************
*
*  ����:   SDA_IN
*
*  ����:   SDA����
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
*  ����:   ReleaseSDA
*
*  ����:   �ͷ�SDA����
*
*****************************************************************************/

void ReleaseSDA(void)
{
   DDR_SDA = 1;   //�ͷ�SDA���ߣ�SDA��������ΪH
}


/*****************************************************************************
*
*  ����:   ReleaseSCL
*
*  ����:   �ͷ�SCL����
*
*****************************************************************************/

void ReleaseSCL(void)
{
   DDR_SCL = 1;
}

/*****************************************************************************
*
*  ����:   SDA_OUT
*
*  ����:   SDA���
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
*  ����:   Bit_Out
*
*  ����:   ���һ��bit����
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
         return ERROR_DATA_OUT_TIME_OUT;   // I2C ��ʱ
      }
   }while(!SCL_IN());
   
   SCL_L();
   DelayTime();
   
   return 0;
   
}


/*****************************************************************************
*
*  ����:   Bit_IN
*
*  ����:   ����һ��bit����
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
         //return ERROR_DATA_IN_TIME_OUT;   // I2C ��ʱ ����������ͨ�����������ݿ��������Կ��Բ��ô������
      }
   }while(!SCL_IN());
   
   
   BitData = SDA_IN();
   
   SCL_L();
   
   DelayTime();
   
   return BitData;
}


/*****************************************************************************
*
*  ����:   ACK
*
*  ����:   ����һ��Byte����
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
*  ����:   ACK
*
*  ����:   ����һ��Byte����
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
*  ����:   Byte_IN
*
*  ����:   ����һ��Byte����
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
*  ����:   Byte_OUT
*
*  ����:   ���һ��Byte����
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
*  ����:   Start
*
*  ����:   ���Ϳ�ʼ����
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
*  ����:   Stop
*
*  ����:   ����ֹͣ����
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
*  ����:   I2CMasterReadData
*
*  ����:   ������
*
*****************************************************************************/

unsigned char I2CMasterReadData( unsigned char DeviceAddr, unsigned char RegAddr, unsigned int count, unsigned char *DataBuffer)
{
   unsigned char SLA_W;
   unsigned char SLA_R;
   unsigned char ACK;
   
   SLA_W = DeviceAddr & 0xFE;  //8λ��ַ,���һλ�Ƕ�дָ��   0 д
   SLA_R = DeviceAddr | 0x01;  // 1 ��
   
   
   // ������ʼ����
   Start();                    //�������ӻ�������ʼ�ź�
   
   // ����������ַ
   if ( Byte_OUT(SLA_W) )      //���ʹӻ���ַ д
   {
      return ERROR_DATA_OUT_TIME_OUT;
   }
   
   ReleaseSDA();   
   
   ACK = Bit_IN();
   
   if (ACK)
   {
      return ERROR_NO_ACK;
   }
   
   // ���ͼĴ�����ַ
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
   
   
   // ���� �ظ� ��ʼ����
   Start();
   
   // ����������ַ
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
   
   // ��������
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
*  ����:   I2CMasterWriteData
*
*  ����:   д����
*
*****************************************************************************/

unsigned char I2CMasterWriteData( unsigned char DeviceAddr, unsigned char RegAddr, unsigned int count, unsigned char *DataBuffer)
{
   unsigned char SLA_W;
   unsigned char ACK;
   
   SLA_W = DeviceAddr & 0xFE;  //��ȡ�ӻ���ַ
   
   
   // ������ʼ����
   Start();
   
   // ����������ַ
   if ( Byte_OUT(SLA_W) )
   {
      return ERROR_DATA_OUT_TIME_OUT;
   }
   
   ReleaseSDA();         //�ͷ�������
   
   ACK = Bit_IN();      //�ж��Ƿ�ʱӦ��
   
   if (ACK)
   {
      return ERROR_NO_ACK;
   }
   
   // ���ͼĴ�����ַ
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
   
   // ��������
   while(count--)
   {
      // ��������
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
*  ����:   I2CMasterCurrentReadData
*
*  ����:   ������
*
*****************************************************************************/

unsigned char I2CMasterCurrentReadData( unsigned char DeviceAddr, unsigned char count, unsigned char *DataBuffer)
{
   unsigned char SLA_R;
   unsigned char ACK;
   
   SLA_R = DeviceAddr | 0x01;
   
   
   
   // ���� ��ʼ����
   Start();
   
   // ����������ַ
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
   
   // ��������
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






