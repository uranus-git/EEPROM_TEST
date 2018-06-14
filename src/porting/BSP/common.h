#ifndef __COMMON_H
#define __COMMON_H
#include "stm32f4xx.h"

//////////////////////////////////////////////////////////////////////////////////

typedef uint32_t  u32;
typedef uint16_t  u16;
typedef uint8_t   u8;

void NVIC_Configuration(void);

extern int htoi(uint8_t* p);




//void delay_init(void);
//void delay_ms(u16 nms);
//void delay_us(u32 nus);

#endif





























