#include "sys.h"
#include "delay.h"
#include "usart.h"

int main(void)
 {	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();
	uart_init(115200);
	while(1)
	{

	}	 
} 

