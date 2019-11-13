#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й����ʺ�STM32F10xϵ�У�
//����DelayUs,DelayMs
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/1/1
//�汾��V1.8
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.2�޸�˵��
//�������ж��е��ó�����ѭ���Ĵ���
//��ֹ��ʱ��׼ȷ,����do while�ṹ!
//V1.3�޸�˵��
//�����˶�UCOSII��ʱ��֧��.
//���ʹ��ucosII,InitDelay���Զ�����SYSTICK��ֵ,ʹ֮��ucos��TICKS_PER_SEC��Ӧ.
//DelayMs��DelayUsҲ���������ucos�ĸ���.
//DelayUs������ucos��ʹ��,����׼ȷ�Ⱥܸ�,����Ҫ����û��ռ�ö���Ķ�ʱ��.
//DelayMs��ucos��,���Ե���OSTimeDly����,��δ����ucosʱ,������DelayUsʵ��,�Ӷ�׼ȷ��ʱ
//����������ʼ������,��������ucos֮��DelayMs������ʱ�ĳ���,ѡ��OSTimeDlyʵ�ֻ���DelayUsʵ��.
//V1.4�޸�˵�� 20110929
//�޸���ʹ��ucos,����ucosδ������ʱ��,DelayMs���ж��޷���Ӧ��bug.
//V1.5�޸�˵�� 20120902
//��DelayUs����ucos��������ֹ����ucos���DelayUs��ִ�У����ܵ��µ���ʱ��׼��
//V1.6�޸�˵�� 20150109
//��DelayMs����OSLockNesting�жϡ�
//V1.7�޸�˵�� 20150319
//�޸�OS֧�ַ�ʽ,��֧������OS(������UCOSII��UCOSIII,����������OS������֧��)
//���:delay_osrunning/delay_ostickspersec/delay_osintnesting�����궨��
//���:delay_osschedlock/delay_osschedunlock/delay_ostimedly��������
//V1.8�޸�˵�� 20150519
//����UCOSIII֧��ʱ��2��bug��
//delay_tickspersec��Ϊ��delay_ostickspersec
//delay_intnesting��Ϊ��delay_osintnesting
////////////////////////////////////////////////////////////////////////////////// 
	 
void InitDelay(void);
void DelayMs(u16 nms);
void DelayUs(u32 nus);

#endif





























