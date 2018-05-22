#include "common.h"
 
 
/*********************************************************************************
************************STM32F407核心开发板******************************
**********************************************************************************
* 文件名称: common.c                                                             *
* 文件简述：各个工程所需调用的公共文件                                           *
             * 
**********************************************************************************
*********************************************************************************/	  

void NVIC_Configuration(void)
{
	
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	
	/* 2 bit for pre-emption priority, 2 bits for subpriority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
											
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

int htoi(uint8_t* p)  
{  
    int n = 0;  
    uint8_t *q = p;  
  
    /* reach its tail */  
    while(*q)  
        q++;  
  
    if(*p == '0' && *(p + 1) != 0)  
        /* skip "0x" or "0X" */  
        p += 2;  
  
    while(*p)  
    {  
        int c;  
        if(*p >= '0' && *p <= '9')  
            c = *p - '0';  
        else if(*p >= 'A' && *p <= 'F')  
            c = *p - 'A' + 0xA;  
        else if(*p >= 'a' && *p <= 'f')  
            c = *p - 'a' + 0xA;  
        else  
            /* invalid char */  
            return 0;  
  
        n += c << ((int)(q - p - 1) * 4);  
        p++;  
    }  
    return n;  
}   
