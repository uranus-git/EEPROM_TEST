#include "main.h"

#define DEBUG

#ifdef DEBUG
__IO uint32_t singleSysTickInt = 0;
#endif
__IO uint32_t TimingDelay;
unsigned char time_flag = 0;



//SysTick定时器配置1毫秒中断一次
void SysTick_Configuration(void)
{
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		/* Capture error */
		while (1);
	}
}


//延时函数 单位：毫秒
void Delay(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);
}
//延时计数
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}
//SysTick中断函数
void SysTick_Handler(void)
{
#ifdef DEBUG
    singleSysTickInt = !singleSysTickInt;
#endif
	TimingDelay_Decrement();//用于延时
}
