#ifndef __S13EE_H
#define __S13EE_H

#include "stm32f4xx.h"

typedef enum
{
    S13EE_SUCCESS = 0,
    S13EE_PRAM_ERR = 1
}S13EE_STATUS;

#define S13EE_WORD_MAX    256
#define DEBUG
#ifdef DEBUG
#define S13EE_PRINTF(format, ...) \
do\
{\
	(void)printf(format, ##__VA_ARGS__); \
}while (0)
#else
#define S13EE_PRINTF(format, ...)
#endif
#define S13EE_GETC USART1_getchar()
#define S13EE_GETLINE USART1_gets()
#define S13EE_GETLINE_NOWAIT USART1_gets_nowait()



typedef struct
{
    /* private data */

    /* object functions */
    /* user mode */
    S13EE_STATUS (*write) (uint8_t addr, uint16_t *u16Buffer, uint16_t cnt);
    S13EE_STATUS (*read)  (uint8_t addr, uint16_t *u16Buffer, uint16_t cnt);
    S13EE_STATUS (*erase) (uint8_t addr, uint16_t cnt);

    /* test mode */
    S13EE_STATUS (*chipErase)(void);
    S13EE_STATUS (*chipWrite)(uint16_t (*u16Arry)[4]);
    S13EE_STATUS (*halfWrite)(uint16_t (*u16Arry)[4], uint8_t isUpper);
    const char * (*errToString)(S13EE_STATUS result);
}S13EE;

extern S13EE * S13EE_INIT (S13EE * pS13EE);
extern S13EE_STATUS S13EE_UNINIT (S13EE * pS13EE);
extern void delayManage(void);


#endif


