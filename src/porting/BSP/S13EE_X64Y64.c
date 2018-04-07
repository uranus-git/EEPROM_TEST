#include "stm32f4xx.h"
#include "common.h"
#include "stdio.H"
#include "usart1.H"
/*********************************************************************************
**********************STM32F407核心开发板******************************
**********************************************************************************
* 文件名称: usart1.c                                                             *
* 文件简述：USART1使用                                                           *
                   * 
**********************************************************************************
*********************************************************************************/	

 unsigned char RS232_REC_Flag = 0;
 unsigned char RS232_buff[RS232_REC_BUFF_SIZE];//??????
 unsigned int RS232_rec_counter = 0;//??RS232????




void USART_Configuration(void)
{ 
	
	GPIO_InitTypeDef GPIO_InitStructure;

	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOD, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	USART_DeInit(USART1);
	
	USART_StructInit(&USART_InitStructure);
	USART_ClockStructInit(&USART_ClockInitStruct);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    //??
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);        //??PA9???USART1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;        
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                                                                                                 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	USART_ClockInit(USART1,&USART_ClockInitStruct);


	USART_InitStructure.USART_BaudRate = 115200; //?????
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No; //???
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure); 

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);        ///////??????
	USART_ClearITPendingBit(USART1, USART_IT_TC);//????TC?
	USART_Cmd(USART1,ENABLE);//??????
}
//????????
void USART1_IRQHandler(void)  
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{	
		RS232_buff[RS232_rec_counter] = USART1->DR;//
		RS232_rec_counter ++;
/********?RS232_END_FLAG1?RS232_END_FLAG2????????????????************/
		if(RS232_rec_counter >= 2)	//?????2?????????
		{
			if(RS232_buff[RS232_rec_counter - 2] == RS232_END_FLAG1 && RS232_buff[RS232_rec_counter - 1] == RS232_END_FLAG2) 	//?????   
			{
				RS232_REC_Flag = 1;
			}
		}
		if(RS232_rec_counter > RS232_REC_BUFF_SIZE)//?????????
		{
			RS232_rec_counter = 0;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(USART1, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
    }	
}

//??????
void RS232_Send_Data(unsigned char *send_buff,unsigned int length)
{
 	unsigned int i = 0;
	for(i = 0;i < length;i ++)
	{			
		USART1->DR = send_buff[i];
		while((USART1->SR&0X40)==0);	
	}	
}


