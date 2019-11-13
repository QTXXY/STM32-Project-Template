#include "sys.h"
#include "delay.h"
#include "usart.h"

int main(void)
 {	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();
	uart_init(115200);
	while(1)
	{

	}	 
} 

