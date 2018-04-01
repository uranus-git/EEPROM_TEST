#ifndef _LED_H_
#define _LED_H_

#define LED1_ON 		GPIO_ResetBits(GPIOF , GPIO_Pin_8)
#define LED2_ON 		GPIO_ResetBits(GPIOF , GPIO_Pin_9)
#define LED3_ON 		GPIO_ResetBits(GPIOF , GPIO_Pin_10)



#define LED1_OFF 		GPIO_SetBits(GPIOF , GPIO_Pin_8)
#define LED2_OFF 		GPIO_SetBits(GPIOF , GPIO_Pin_9)
#define LED3_OFF 		GPIO_SetBits(GPIOF , GPIO_Pin_10)


void LED_GPIO_Config(void);

#endif
