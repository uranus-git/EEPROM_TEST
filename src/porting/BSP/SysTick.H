#ifndef _SYSTICK_H_
#define _SYSTICK_H_
extern unsigned char time_flag;
void SysTick_Configuration(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif 
