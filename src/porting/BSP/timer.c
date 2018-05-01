﻿/*
 * timer.c
 *
 */
 
 #include "main.h"	

void Timer_Init(void)
{		 					 
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  	//TIM14����?����1?��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//��1?��PORTF����?��	
	
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); //GPIOF9?�䨮??a?������?��14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //?�䨮?1|?��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//?��?��100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //��?����?�䨮?��?3?
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //��?��-
	GPIO_Init(GPIOF,&GPIO_InitStructure);              //3?��??��PF9
	  
	TIM_TimeBaseStructure.TIM_Prescaler= 1 - 1;  //?������?�¡�??��
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //?����???��y?�꨺?
	TIM_TimeBaseStructure.TIM_Period=42-1;   //��??��??����???��
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//3?��??��?������?��14
	
	//3?��??��TIM14 Channel1 PWM?�꨺?	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //?????������?��?�꨺?:TIM??3??��?���̡�???�꨺?2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //����??��?3?��1?��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //��?3???D?:TIM��?3?����????D?�̨�
	TIM_OCInitStructure.TIM_Pulse = 21;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //?��?YT???����?2?��y3?��??����a����TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //��1?��TIM14?��CCR1��?��??�����????��??��
 
  TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE��1?�� 
	
	TIM_Cmd(TIM14, ENABLE);  //��1?��TIM14
										  
}  


