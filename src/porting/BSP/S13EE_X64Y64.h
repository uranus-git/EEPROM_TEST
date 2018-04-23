#ifndef __S13EE_H
#define __S13EE_H

#include <stm32f4xx.h>

typedef enum
{
    S13EE_SUCCESS = 0,
    S13EE_PRAM_ERR = -1,
}S13EE_STATUS;

typedef struct
{
    /* private data */

    /* object functions */
    /* user mode */
    S13EE_STATUS (*write) (uint8_t addr, uint16_t *u16Buffer, uint8_t cnt);
    S13EE_STATUS (*read)  (uint8_t addr, uint16_t *u16Buffer, uint8_t cnt);
    S13EE_STATUS (*erase) (uint8_t addr, uint8_t cnt);

    /* test mode */
    S13EE_STATUS (*chipErase)(void);
    S13EE_STATUS (*chipWrite)(uint16_t (*u16Arry)[4]);

}S13EE;


#endif


