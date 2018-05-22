#include "main.h"

extern void USART_Configuration(void);
extern void NVIC_Configuration(void);
extern void TIM14_PWM_Init(void);
extern void SysTick_Configuration(void);


void BSP_GPIO_Init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;
	/* Enable the GPIO_LED Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	/*初始化完后，关闭3个LED*/ 
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
}

void bsp_init(void)
{
    //SysTick_Configuration();
    //BSP_GPIO_Init();
    USART_Configuration();
    //NVIC_Configuration();
    //TIM14_PWM_Init();
}
