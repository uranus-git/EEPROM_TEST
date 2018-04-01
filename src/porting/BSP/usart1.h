#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "common.h" 

//////////////////////////////////////////////////////////////////////////////////	 

#define RS232_REC_BUFF_SIZE				100
#define RS232_END_FLAG1	'#'			//RS232????????1 
#define RS232_END_FLAG2	'*'			//RS232????????2 


extern  unsigned char RS232_REC_Flag ;
extern  unsigned char RS232_buff[RS232_REC_BUFF_SIZE] ;//??????
extern  unsigned int RS232_rec_counter ;//??RS232????

void RS232_Send_Data(unsigned char *send_buff,unsigned int length);
void USART_Configuration(void);

#endif


