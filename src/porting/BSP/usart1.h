#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"
#include "common.h"

//////////////////////////////////////////////////////////////////////////////////

#define USART_BUFF_SIZE				100


extern unsigned char usartRecvFlag;
extern unsigned char usartBuf[USART_BUFF_SIZE];
extern unsigned int usartRecvCnt;

extern void RS232_Send_Data(unsigned char *send_buff,unsigned int length);
extern void USART_Configuration(void);
extern uint8_t USART1_getchar(void);
extern uint8_t *USART1_gets_nowait (void);
extern uint8_t * USART1_gets (void);

#endif


