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

unsigned char usartRecvFlag = 0;
unsigned char usartBuf[USART_BUFF_SIZE];
unsigned int usartRecvCnt = 0;

void USART_Configuration(void)
{
#if 0
	GPIO_InitTypeDef GPIO_InitStructure;

	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_DeInit(USART1);

	USART_StructInit(&USART_InitStructure);
	USART_ClockStructInit(&USART_ClockInitStruct);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_USART1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1);

	USART_ClockInit(USART1,&USART_ClockInitStruct);


	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ClearITPendingBit(USART1, USART_IT_TC);
	USART_Cmd(USART1,ENABLE);
#else
    GPIO_InitTypeDef GPIO_InitStructure;

	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	USART_DeInit(USART3);

	USART_StructInit(&USART_InitStructure);
	USART_ClockStructInit(&USART_ClockInitStruct);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3);

	USART_ClockInit(USART3,&USART_ClockInitStruct);


	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ClearITPendingBit(USART3, USART_IT_TC);
	USART_Cmd(USART3,ENABLE);
#endif
}


void RS232_Send_Data(unsigned char *send_buff,unsigned int length)
{
 	unsigned int i = 0;
	for(i = 0;i < length;i ++)
	{
		USART1->DR = send_buff[i];
		while((USART1->SR&0X40)==0);
	}
}

unsigned char USART1_putchar(unsigned char ch)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t) ch); /*????????*/

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)/*??????*/
  {

  }
  return ch;
}

/**
  * @brief  Test to see if a key has been pressed on the HyperTerminal
  * @param  key: The key pressed
  * @retval 1: Correct
  *         0: Error
  */
uint32_t USART1_ValidInput(void)
{

  if ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

uint8_t USART1_getchar(void)
{
  uint8_t key = 0;

  /* Waiting for user input */
  while (1)
  {
    if ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
    {
      key = (uint8_t)USART1->DR & 0xFF;
      break;
    }
  }
  return key;
}

/**
  * @brief  Get Input string from the HyperTerminal
  * @param  buffP: The input string
  * @retval None
  */
uint8_t *USART1_gets_nowait (void)
{
    static uint32_t bytes_read = 0;
    uint8_t c = 0;

    if(USART1_ValidInput())
    {
        c = USART1_getchar();
        if (c == '\r')
        {
          USART1_putchar('\n');
          USART1_putchar('\r');
          usartBuf[bytes_read] = '\0';
          bytes_read = 0;

          return usartBuf;
        }

        if (c == '\b') /* Backspace */
        {
          if (bytes_read > 0)
          {
            USART1_putchar('\b');
            USART1_putchar(' ');
            USART1_putchar('\b');
            bytes_read--;
          }
          return 0;
        }
        //if (bytes_read >= (CMD_STRING_SIZE))
        if (bytes_read >= (USART_BUFF_SIZE))
        {
          printf("Command string size overflow\r\n");
          bytes_read = 0;
          return 0;
        }
        if (c >= 0x20 && c <= 0x7E)
        {
          usartBuf[bytes_read++] = c;
          USART1_putchar(c);
        }
    }

    return 0;
}

uint8_t *USART1_gets (void)
{
    uint8_t *buffer;

    while(!(buffer = USART1_gets_nowait()));

    return buffer;
}

