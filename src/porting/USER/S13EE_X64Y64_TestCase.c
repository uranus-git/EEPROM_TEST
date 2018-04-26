#include <S13EE_X64Y64.h>
#include <S13EE_X64Y64_TestCase.h>
#include <string.h>

static S13EE s13ee;

void EEP_T_INIT(void)
{
    if(NULL == S13EE_INIT(&s13ee))
        printf("EEP_T_INIT failed\n");
}

static void printbuffer (uint16 *u16Buffer, uint32_t count)
{
    uint32_t index = 0;

    while(index < count)
    {
        printf("%04x ", u16Buffer[index++]);
        if((index % 4) == 0)
            printf("\r\n");
    }
}

static void EEP_T1(void)
{
}

static void EEP_T2(void)
{
    S13EE_STATUS result;
    uint16_t u16Buffer[S13EE_WORD_MAX];

    printf("VDD = 1.5V, Read 4kbit.\r\n");

    if(S13EE_SUCCESS != (result = s13ee.read(0, u16Buffer, S13EE_WORD_MAX)))
        printf("read failed(result = %d)\r\n", result);

    printbuffer(u16Buffer, S13EE_WORD_MAX);
}

static void EEP_T3(void)
{
    S13EE_STATUS result;
    uint16_t u16Buffer[4] = {0xA5A5, 0xA5A5, 0xA5A5, 0xA5A5};

    printf("VDD = 1.5V, chipWrite %#04x.\r\n", u16Buffer[0]);

    if(S13EE_SUCCESS != (result = s13ee.chipWrite(u16Buffer)))
        printf("chipWrite failed(result = %d)\r\n", result);
}

static void EEP_T4(void)
{
    S13EE_STATUS result;
    printf("VDD = 1.5V, chipErase.\r\n");

    if(S13EE_SUCCESS != (result = s13ee.chipErase()))
        printf("chipErase failed(result = %d)\r\n", result);
}

static void EEP_T5(void)
{
    S13EE_STATUS result;
    uint32_t times = 0;
    uint8_t addr = 0;
    uint16_t u16ReadBuffer[S13EE_WORD_MAX];
    const uint16_t u16Value = 0;

    printf("VDD = 1.5V, 200 TIMES All ZERO TEST.\r\n");

    for(times = 0; times < 200; times++)
    {
        /* chipErase */
        if(S13EE_SUCCESS != (result = s13ee.chipErase()))
        {
            printf("EEP_T5 times %d chipErase failed(result = %d)\r\n", times, result);
            return;
        }
        /* write */
        for(addr = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(S13EE_SUCCESS != (result = s13ee.write(addr, &u16Value, 1)))
            {
                printf("EEP_T5 times %d write addr %d failed(result = %d)\r\n", times, addr, result);
                return;
            }
        }
        /* read */
        memset(u16ReadBuffer, 0xee, sizeof(u16ReadBuffer));
        if(S13EE_SUCCESS != (result = s13ee.read(0, u16ReadBuffer, S13EE_WORD_MAX)))
        {
            printf("EEP_T5 times %d read failed(result = %d)\r\n", times, result);
            return;
        }
        /* compare */
        for(addr = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(u16ReadBuffer[addr] != u16Value)
            {
                printf("EEP_T5 times %d compare addr %d failed(result = %d)\r\n", times, addr, result);
                printbuffer(u16ReadBuffer, S13EE_WORD_MAX);
                return;
            }
        }
    }
}

static void EEP_T6(void)
{
    S13EE_STATUS result;
    uint32_t times = 0;
    uint8_t addr = 0;
    uint16_t u16ReadBuffer[S13EE_WORD_MAX];
    const uint16_t u16Value = 0xFFFF;

    printf("VDD = 1.5V, 200 TIMES All FFFF TEST.\r\n");

    for(times = 0; times < 200; times++)
    {
        /* chipErase */
        if(S13EE_SUCCESS != (result = s13ee.chipErase()))
        {
            printf("EEP_T5 times %d chipErase failed(result = %d)\r\n", times, result);
            return;
        }
        /* write */
        for(addr = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(S13EE_SUCCESS != (result = s13ee.write(addr, &u16Value, 1)))
            {
                printf("EEP_T5 times %d write addr %d failed(result = %d)\r\n", times, addr, result);
                return;
            }
        }
        /* read */
        memset(u16ReadBuffer, 0xee, sizeof(u16ReadBuffer));
        if(S13EE_SUCCESS != (result = s13ee.read(0, u16ReadBuffer, S13EE_WORD_MAX)))
        {
            printf("EEP_T5 times %d read failed(result = %d)\r\n", times, result);
            return;
        }
        /* compare */
        for(addr = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(u16ReadBuffer[addr] != u16Value)
            {
                printf("EEP_T5 times %d compare addr %d failed(result = %d)\r\n", times, addr, result);
                printbuffer(u16ReadBuffer, S13EE_WORD_MAX);
                return;
            }
        }
    }
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}

static void EEP_T1(void)
{
}


