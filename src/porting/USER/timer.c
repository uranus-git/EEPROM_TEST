/*
 * timer.c
 *
 */
 
 #include "main.h"	

void Timer_Init(void)
{		 					 
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14那㊣?車那1?邦    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//那1?邦PORTF那㊣?車	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9?∩車??a?“那㊣?¯14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //?∩車?1|?邦
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//?迄?豕100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //赤?赤足?∩車?那?3?
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //谷?角-
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //3?那??‘PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler= 1 - 1;  //?“那㊣?¯﹞??米
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //?辰谷???那y?㏒那?
	TIM_TimeBaseStructure.TIM_Period=42-1;   //℅??‘??℅～???米
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//3?那??‘?“那㊣?¯14
	
	//3?那??‘TIM14 Channel1 PWM?㏒那?	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //?????“那㊣?¯?㏒那?:TIM??3??赤?豕米¯???㏒那?2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //㊣豕??那?3?那1?邦
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //那?3???D?:TIM那?3?㊣豕????D?米赤
	TIM_OCInitStructure.TIM_Pulse = 21;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //?迄?YT???“米?2?那y3?那??‘赤a谷豕TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //那1?邦TIM14?迆CCR1谷?米??∟℅～????∩??¯
 
  TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE那1?邦 
	
	TIM_Cmd(TIM14, ENABLE);  //那1?邦TIM14
										  
}  


