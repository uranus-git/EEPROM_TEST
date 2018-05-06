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

/* 连续100次march算法测试 */
static void EEP_T7(S13EE *s13ee)
{
    uint16_t addr;
    uint16_t rdata, wdata, temp;
    uint16_t count = 0;
    S13EE_STATUS ret;

    S13EE_PRINTF("VDD = 1.8V, 100 TIMES MARCH TEST.\r\n");
    S13EE_PRINTF("TEST IN PROCESSING %02d\%.", count);

    for(count = 0; count < 100; count++)
    {
        S13EE_PRINTF("\b\b\b\b%02d\%.", count);

        /* 1. 从起始位置开始,按照地址递增顺序把全部单元写为0x0000 */
        for(addr = 0, wdata = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
                goto EEP_T7_ERR;
        }

        /* 2. 从起始位置开始,按照地址递增顺序,读A0单元的0x0000，再改写为0x5555；
         *    读A1单元的0x0000，再改写为0x5555；读A2单元的0x0000，再改写为0xaaaa；
         *    读A3单元的0x0000，再改写为0xaaaa...直到An-1。这步骤结果之后，存储器
         *    就像被划分为棋盘一样，类似与进行checkerboard测试算法操作得到的结果一
         *    样；在此过程中，若读回的数据不为0x0000，则返回失败
         */
         for(addr = 0, wdata = 0x5555; addr < S13EE_WORD_MAX; addr += 2, wdata = ~wdata)
         {
            if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)addr, &rdata, 1)))
                goto EEP_T7_ERR;

            if(rdata != 0x0000)
            {
                S13EE_PRINTF("2.1 Read addr %d, data 0x%04x isn't 0x0000.\r\n", addr, rdata);
                return;
            }

            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
                goto EEP_T7_ERR;
            /*****************************/
            if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)(addr + 1), &rdata, 1)))
                goto EEP_T7_ERR;

            if(rdata != 0x0000)
            {
                S13EE_PRINTF("2.2 Read addr %d, data 0x%04x isn't 0x0000.\r\n", (addr + 1), rdata);
                return;
            }

            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)(addr + 1), &wdata, 1)))
                goto EEP_T7_ERR;
         }

         /* 3. 从最后一个word单元位置开始，按照地址降序，读An-1单元的0xaaaa，
          *    再改写为0x5555；读An-2单元的0xaaaa，再改写为0x5555；读An-3单元的0x5555，
          *    再改写为0xaaaa；读An-4单元的0x5555，再改写为0xaaaa；…直到A0
          *   （即读取每个存储单元中的值再写入相反值)。在此过程中，若读回的数据不一致，则返回失败
          */
         for(addr = S13EE_WORD_MAX - 1, rdata = 0xAAAA, wdata = 0x5555; \
            (addr >= 0) && (addr < S13EE_WORD_MAX); addr += 2, rdata = ~rdata, wdata = ~wdata)
         {
             if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)addr, &temp, 1)))
                goto EEP_T7_ERR;

            if(temp != rdata)
            {
                S13EE_PRINTF("3.1 Read addr %d, data 0x%04x isn't 0x%04x.\r\n", addr, temp, rdata);
                return;
            }

            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
                goto EEP_T7_ERR;
            /*****************************/
            if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)(addr + 1), &temp, 1)))
                goto EEP_T7_ERR;

            if(temp != rdata)
            {
                S13EE_PRINTF("3.2 Read addr %d, data 0x%04x isn't 0x%04x.\r\n", (addr + 1), temp, rdata);
                return;
            }

            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)(addr + 1), &wdata, 1)))
                goto EEP_T7_ERR;
        }
        /* 4. 从起始位置开始，按照地址递增顺序，读全部单元，若读回的数据不一致，则返回失败 */
        for(addr = 0, rdata = 0xAAAA; addr < S13EE_WORD_MAX; addr += 2, rdata = ~rdata)
         {
            if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)addr, &temp, 1)))
                goto EEP_T7_ERR;

            if(rdata != temp)
            {
                S13EE_PRINTF("4.1 Read addr %d, data 0x%04x isn't 0x%04x.\r\n", addr, temp, rdata);
                return;
            }

            if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)(addr + 1), &temp, 1)))
                goto EEP_T7_ERR;

            if(rdata != temp)
            {
                S13EE_PRINTF("4.2 Read addr %d, data 0x%04x isn't 0x%04x.\r\n", addr+1, temp, rdata);
                return;
            }
         }
    }

    S13EE_PRINTF("\b\b\b\b%02d\%.", count);
    S13EE_PRINTF("PASS\r\n");

    return;

EEP_T7_ERR:
    S13EE_PRINTF("EEP_T7 FAILED : %s.\r\n", s13ee->errToString(ret));
}

static void EEP_T8(S13EE *s13ee)
{
    uint16_t addr;
    const uint16_t wdata = 0x5a5a, rdata;
    S13EE_STATUS ret;

    /* 擦读写，对比读写数据，写数据为5A，地址变化按行方向递增顺序 */
    for(addr = 0; addr < S13EE_WORD_MAX; addr++)
    {
        if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
            goto EEP_T8_ERR;

        if(S13EE_SUCCESS!= (ret = s13ee->read((uint8_t)addr, &rdata, 1)))
                goto EEP_T8_ERR;

        if(rdata != wdata)
        {
            S13EE_PRINTF("2.1 Read addr %d, data 0x%04x isn't 0x0000.\r\n", addr, rdata);
            return;
        }
    }

EEP_T8_ERR:
    S13EE_PRINTF("EEP_T8 FAILED : %s.\r\n", s13ee->errToString(ret));
}

static void EEP_T9(S13EE *s13ee)
{
    uint16_t addr;
    const uint16_t wdata = 0x5a5a, rdata;
    S13EE_STATUS ret;

    /* TODO */
}

static void EEP_T10(S13EE *s13ee)
{
    uint16_t addr;
    const uint16_t wdata = 0x5a5a, rdata;
    S13EE_STATUS ret;

    /* TODO */

}

void EEP_T_MAIN(S13EE *s13ee)
{
    //GPIO_SetBits(GPIOC, GPIO_Pin_8|GPIO_Pin_9);
    EEP_T3(s13ee);
    //TIM_Cmd(TIM14, ENABLE);
    //*((__IO uint32_t  *)(0x40020814)) = ((*((__IO uint32_t  *)(0x40020814))) & 0x300) & (0xff & 0xff);
}

