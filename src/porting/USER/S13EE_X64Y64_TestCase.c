#include "S13EE_X64Y64.h"
//#include <S13EE_X64Y64_TestCase.h>
#include <string.h>

static void printbuffer (uint16_t *u16Buffer, uint32_t count)
{
    uint32_t index = 0;

    while(index < count)
    {
        S13EE_PRINTF("%04x ", u16Buffer[index++]);
        if((index % 4) == 0)
            S13EE_PRINTF("\r\n");
    }
}

static void EEP_T1(S13EE *s13ee)
{
}

static void EEP_T2(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint16_t u16Buffer[S13EE_WORD_MAX];

    S13EE_PRINTF("VDD = 1.5V, Read 4kbit.\r\n");

    if(S13EE_SUCCESS != (result = s13ee->read(0, u16Buffer, S13EE_WORD_MAX)))
        S13EE_PRINTF("read failed(result = %d)\r\n", result);

    printbuffer(u16Buffer, S13EE_WORD_MAX);
}

static void EEP_T3(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint16_t u16Buffer[4] = {0xA5A5, 0xA5A5, 0xA5A5, 0xA5A5};

    S13EE_PRINTF("VDD = 1.5V, chipWrite %#04x.\r\n", u16Buffer[0]);

    if(S13EE_SUCCESS != (result = s13ee->chipWrite((uint16_t (*)[4])u16Buffer)))
        S13EE_PRINTF("chipWrite failed(result = %d)\r\n", result);
}

static void EEP_T4(S13EE *s13ee)
{
    S13EE_STATUS result;
    S13EE_PRINTF("VDD = 1.5V, chipErase.\r\n");

    if(S13EE_SUCCESS != (result = s13ee->chipErase()))
        S13EE_PRINTF("chipErase failed(result = %d)\r\n", result);
}

static void EEP_T5(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint32_t times = 0;
    uint16_t addr = 0;
    uint16_t u16ReadBuffer[S13EE_WORD_MAX];
    const uint16_t u16Value = 0;

    S13EE_PRINTF("VDD = 1.5V, 200 TIMES All ZERO TEST.\r\n");

    for(times = 0; times < 200; times++)
    {
        /* chipErase */
        if(S13EE_SUCCESS != (result = s13ee->chipErase()))
        {
            S13EE_PRINTF("EEP_T5 times %d chipErase failed(result = %d)\r\n", times, result);
            return;
        }
        /* write */
        for(addr = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(S13EE_SUCCESS != (result = s13ee->write(addr, (uint16_t *)&u16Value, 1)))
            {
                S13EE_PRINTF("EEP_T5 times %d write addr %d failed(result = %d)\r\n", times, addr, result);
                return;
            }
        }
        /* read */
        memset(u16ReadBuffer, 0xee, sizeof(u16ReadBuffer));
        if(S13EE_SUCCESS != (result = s13ee->read(0, u16ReadBuffer, S13EE_WORD_MAX)))
        {
            S13EE_PRINTF("EEP_T5 times %d read failed(result = %d)\r\n", times, result);
            return;
        }
        /* compare */
        for(addr = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(u16ReadBuffer[addr] != u16Value)
            {
                S13EE_PRINTF("EEP_T5 times %d compare addr %d failed(result = %d)\r\n", times, addr, result);
                printbuffer(u16ReadBuffer, S13EE_WORD_MAX);
                return;
            }
        }
    }
}

static void EEP_T6(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint32_t times = 0;
    uint16_t addr = 0;
    uint16_t u16ReadBuffer[S13EE_WORD_MAX];
    const uint16_t u16Value = 0xFFFF;

    S13EE_PRINTF("VDD = 1.5V, 200 TIMES All FFFF TEST.\r\n");

    for(times = 0; times < 200; times++)
    {
        /* chipErase */
        if(S13EE_SUCCESS != (result = s13ee->chipErase()))
        {
            S13EE_PRINTF("EEP_T5 times %d chipErase failed(result = %d)\r\n", times, result);
            return;
        }
        /* write */
        for(addr = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(S13EE_SUCCESS != (result = s13ee->write((uint8_t)addr, (uint16_t *)&u16Value, 1)))
            {
                S13EE_PRINTF("EEP_T5 times %d write addr %d failed(result = %d)\r\n", times, addr, result);
                return;
            }
        }
        /* read */
        memset(u16ReadBuffer, 0xee, sizeof(u16ReadBuffer));
        if(S13EE_SUCCESS != (result = s13ee->read(0, u16ReadBuffer, S13EE_WORD_MAX)))
        {
            S13EE_PRINTF("EEP_T5 times %d read failed(result = %d)\r\n", times, result);
            return;
        }
        /* compare */
        for(addr = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(u16ReadBuffer[addr] != u16Value)
            {
                S13EE_PRINTF("EEP_T5 times %d compare addr %d failed(result = %d)\r\n", times, addr, result);
                printbuffer(u16ReadBuffer, S13EE_WORD_MAX);
                return;
            }
        }
    }
}

static void EEP_T7(S13EE *s13ee)
{
}

void EEP_T_MAIN(S13EE *s13ee)
{
    //GPIO_SetBits(GPIOC, GPIO_Pin_8|GPIO_Pin_9);
    EEP_T3(s13ee);
    //TIM_Cmd(TIM14, ENABLE);
    //*((__IO uint32_t  *)(0x40020814)) = ((*((__IO uint32_t  *)(0x40020814))) & 0x300) & (0xff & 0xff);
}

