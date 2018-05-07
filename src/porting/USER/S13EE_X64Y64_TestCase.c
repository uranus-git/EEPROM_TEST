#include "S13EE_X64Y64.h"
//#include <S13EE_X64Y64_TestCase.h>
#include <string.h>

extern char inputBuffer[];
extern int inputIntSignal;

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

static void s13eeInitWakeUp(S13EE *s13ee)
{
    uint16_t count, index, addr;
    uint16_t writeBuffer[4] = {0x5A5A, 0xA5A5, 0x0000, 0xFFFF};
    uint16_t data[4], rdata;
    char buffer[128];
    S13EE_STATUS ret;
    char ch;

    S13EE_PRINTF("%s : Initialize and Wake up test\r\n", __FUNCTION__);
    S13EE_PRINTF("writeBuffer [0x%04x 0x%04x 0x%04x 0x%04x]\r\n",
        writeBuffer[0], writeBuffer[1], writeBuffer[2], writeBuffer[3]);
    S13EE_PRINTF("Do you want to change (y/n): ");

    ch = S13EE_GETC;
    if((ch == 'y') || (ch == 'Y'))
    {
        for(index = 0; index < 4; index++)
        {
            S13EE_PRINTF("writeBuffer[%d] 0x%04x : 0x", index, writeBuffer[index]);
            memset(buffer, 0, sizeof(buffer));
            S13EE_GETLINE(buffer);
            writeBuffer[index] = atoi(buffer);
        }
        S13EE_PRINTF("writeBuffer [0x%04x 0x%04x 0x%04x 0x%04x]\r\n",
            writeBuffer[0], writeBuffer[1], writeBuffer[2], writeBuffer[3]);
    }

    S13EE_PRINTF("TEST IN PROCESSING %02d\%.", count * 2);
    for(count = 0; count < 50; count++)
    {
        S13EE_PRINTF("\b\b\b\b%02d\%.", count * 2);
        for(index = 0; index < 4; index++)
        {
            data[0] = writeBuffer[index];data[1] = writeBuffer[index];
            data[2] = writeBuffer[index];data[3] = writeBuffer[index];

            if(S13EE_SUCCESS != (ret = s13ee->chipWrite((uint16_t (*)[4]) data)))
                goto s13eeInitWakeUpErr;

            for(addr = 0; addr < S13EE_WORD_MAX; addr++)
            {
                if(S13EE_SUCCESS != (ret = s13ee->read(addr, &rdata, 1)))
                    goto s13eeInitWakeUpErr;
                if(rdata != writeBuffer[index])
                {
                    S13EE_PRINTF("%s : read addr %d = 0x%04x, not match the value 0x%04x\r\n",
                        __FUNCTION__, addr, rdata, writeBuffer[index]);
                    return;
                }
            }
        }
    }
    S13EE_PRINTF("\b\b\b\b%02d\%.", count * 2);
    S13EE_PRINTF("%s SUCCUSS.\n", __FUNCTION__);
    return;

s13eeInitWakeUpErr:
    S13EE_PRINTF("EEP_T1 FAILED : %s.\r\n", s13ee->errToString(ret));
}

static void EEP_T1(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t wdata = 0xA5A5, rdata = 0;

    /* 与EEPROM通讯正常，能正确发指令 */
    if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)0, &wdata, 1)))
        goto EEP_T1_ERR;

    if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)0, &rdata, 1)))
        goto EEP_T1_ERR;

    if(wdata != rdata)
        S13EE_PRINTF("EEP_T1 FAILED : read data [0x%04x] and write data [0x%04x] isn't match !!!\r\n", rdata, wdata);
    else
        S13EE_PRINTF("EEP_T1 SUCCUSS.\n");

    return;

EEP_T1_ERR:
    S13EE_PRINTF("EEP_T1 FAILED : %s.\r\n", s13ee->errToString(ret));
}

static void EEP_T2(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint16_t u16Buffer[S13EE_WORD_MAX];

    S13EE_PRINTF("%s - Read 4kbit.\r\n", __FUNCTION__);

    if(S13EE_SUCCESS != (result = s13ee->read(0, u16Buffer, S13EE_WORD_MAX)))
    {
        S13EE_PRINTF("read failed(result = %d)\r\n", result);
        return;
    }

    printbuffer(u16Buffer, S13EE_WORD_MAX);
    S13EE_PRINTF("\r\n");
    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
}

static void EEP_T3(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint16_t u16Buffer[4] = {0xA5A5, 0xA5A5, 0xA5A5, 0xA5A5};

    S13EE_PRINTF("%s - chipWrite %#04x.\r\n", __FUNCTION__, u16Buffer[0]);

    if(S13EE_SUCCESS != (result = s13ee->chipWrite((uint16_t (*)[4])u16Buffer)))
    {
        S13EE_PRINTF("chipWrite failed(result = %d)\r\n", result);
        return;
    }

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
}

static void EEP_T4(S13EE *s13ee)
{
    S13EE_STATUS result;
    S13EE_PRINTF("%s chipErase.\r\n", __FUNCTION__);

    if(S13EE_SUCCESS != (result = s13ee->chipErase()))
    {
        S13EE_PRINTF("chipErase failed(result = %d)\r\n", result);
        return;
    }

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
}

static void EEP_T5(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint32_t times = 0;
    uint16_t addr = 0;
    uint16_t u16ReadBuffer[S13EE_WORD_MAX];
    const uint16_t u16Value = 0;

    S13EE_PRINTF("%s - 200 TIMES All ZERO TEST.\r\n", __FUNCTION__);
    S13EE_PRINTF("TEST IN PROCESSING %02d\%.", times/2);
    for(times = 0; times < 200; times++)
    {
        S13EE_PRINTF("\b\b\b\b%02d\%.", times/2);
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

    S13EE_PRINTF("\b\b\b\b%02d\%.", times/2);
    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
}

static void EEP_T6(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint32_t times = 0;
    uint16_t addr = 0;
    uint16_t u16ReadBuffer[S13EE_WORD_MAX];
    const uint16_t u16Value = 0xFFFF;

    S13EE_PRINTF("%s - 200 TIMES All FFFF TEST.\r\n", __FUNCTION__);
    S13EE_PRINTF("TEST IN PROCESSING %02d\%.", times/2);
    for(times = 0; times < 200; times++)
    {
        S13EE_PRINTF("\b\b\b\b%02d\%.", times/2);
        /* chipErase */
        if(S13EE_SUCCESS != (result = s13ee->chipErase()))
        {
            S13EE_PRINTF("EEP_T6 times %d chipErase failed(result = %d)\r\n", times, result);
            return;
        }
        /* write */
        for(addr = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(S13EE_SUCCESS != (result = s13ee->write((uint8_t)addr, (uint16_t *)&u16Value, 1)))
            {
                S13EE_PRINTF("EEP_T6 times %d write addr %d failed(result = %d)\r\n", times, addr, result);
                return;
            }
        }
        /* read */
        memset(u16ReadBuffer, 0xee, sizeof(u16ReadBuffer));
        if(S13EE_SUCCESS != (result = s13ee->read(0, u16ReadBuffer, S13EE_WORD_MAX)))
        {
            S13EE_PRINTF("EEP_T6 times %d read failed(result = %d)\r\n", times, result);
            return;
        }
        /* compare */
        for(addr = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(u16ReadBuffer[addr] != u16Value)
            {
                S13EE_PRINTF("EEP_T6 times %d compare addr %d failed(result = %d)\r\n", times, addr, result);
                printbuffer(u16ReadBuffer, S13EE_WORD_MAX);
                return;
            }
        }
    }

    S13EE_PRINTF("\b\b\b\b%02d\%.", times/2);
    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
}

/* 连续100次march算法测试 */
static void EEP_T7(S13EE *s13ee)
{
    uint16_t addr;
    uint16_t rdata, wdata, temp;
    uint16_t count = 0;
    S13EE_STATUS ret;

    S13EE_PRINTF("%s - 100 TIMES MARCH TEST.\r\n", __FUNCTION__);
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
    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;

EEP_T7_ERR:
    S13EE_PRINTF("EEP_T7 FAILED : %s.\r\n", s13ee->errToString(ret));
}

static void EEP_T8(S13EE *s13ee)
{
    uint16_t addr;
    const uint16_t wdata = 0x5a5a, rdata;
    S13EE_STATUS ret;

    S13EE_PRINTF("%s - ALL WRITE 0X5A, ADDRESS increase continuous.\r\n", __FUNCTION__);
    /* 擦读写，对比读写数据，写数据为5A，地址变化按行方向递增顺序 */
    for(addr = 0; addr < S13EE_WORD_MAX; addr++)
    {
        if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
            goto EEP_T8_ERR;

        if(S13EE_SUCCESS!= (ret = s13ee->read((uint8_t)addr, &rdata, 1)))
                goto EEP_T8_ERR;

        if(rdata != wdata)
        {
            S13EE_PRINTF("Read addr %d, data 0x%04x isn't 0x0000.\r\n", addr, rdata);
            return;
        }
    }

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
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

static void EEP_T11(S13EE *s13ee)
{
    S13EE_PRINTF("%s - VDD = 1.5V, CALL EEP_T7.\r\n", __FUNCTION__);

    EEP_T7(s13ee);
}

static void EEP_T12(S13EE *s13ee)
{
    S13EE_PRINTF("%s - VDD = 1.3V, CALL EEP_T7.\r\n", __FUNCTION__);

    EEP_T7(s13ee);
}

static void EEP_T13(S13EE *s13ee)
{
    S13EE_PRINTF("%s - VDD = 1.2V, CALL EEP_T7.\r\n", __FUNCTION__);

    EEP_T7(s13ee);
}

static void EEP_T14(S13EE *s13ee)
{
    S13EE_PRINTF("%s - VDD = 1.1V, CALL EEP_T7.\r\n", __FUNCTION__);

    EEP_T7(s13ee);
}

static void EEP_T15(S13EE *s13ee)
{
    S13EE_PRINTF("%s - VDD = 1.0V, CALL EEP_T7.\r\n", __FUNCTION__);

    EEP_T7(s13ee);
}

static void EEP_T16(S13EE *s13ee)
{
    uint16_t addr;
    uint16_t writeBuffer[4] = {0x5A5A, 0xA5A5, 0x0000, 0xFFFF};
    uint16_t readBuffer[4];
    S13EE_STATUS ret;
    char ch;
    uint16_t addr;

    S13EE_PRINTF("%s - PERFORMANCE TEST.\r\n", __FUNCTION__);
    S13EE_PRINTF("Write 2kbit data with 0x%04x 0x%04x 0x%04x 0x%04x\n",
        writeBuffer[0], writeBuffer[1], writeBuffer[2], writeBuffer[3]);

    do
    {
        for(addr = 0; addr < S13EE_WORD_MAX; addr += 4)
        {
            if(S13EE_SUCCESS != (ret = s13ee->read(addr, readBuffer, 4)))
                goto EEP_T16_ERR;
            if((readBuffer[0] != writeBuffer[0]) || (readBuffer[1] != writeBuffer[1]) ||
               (readBuffer[2] != writeBuffer[2]) || (readBuffer[3] != writeBuffer[3]))
            {
                S13EE_PRINTF("%s - Addr %d compare failed [0x%04x 0x%04x 0x%04x 0x%04x].\r\n",
                    __FUNCTION__, addr, readBuffer[0], readBuffer[1], readBuffer[2], readBuffer[3]);
                return;
            }
        }

        S13EE_PRINTF("%s - Read 2kbit & compare OK.(Input \'q\' to exit)\r\n", __FUNCTION__);

        ch = S13EE_GETC;
    }while(ch != 'q');

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T16_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T17(S13EE *s13ee)
{
    S13EE_PRINTF("%s - T = -20℃, EEP_T11 ~ EEP_T16 TEST.\r\n", __FUNCTION__);

    EEP_T11(s13ee);
    EEP_T12(s13ee);
    EEP_T13(s13ee);
    EEP_T14(s13ee);
    EEP_T15(s13ee);
    EEP_T16(s13ee);
}

static void EEP_T18(S13EE *s13ee)
{
    S13EE_PRINTF("%s - T = 0℃, EEP_T11 ~ EEP_T16 TEST.\r\n", __FUNCTION__);

    EEP_T11(s13ee);
    EEP_T12(s13ee);
    EEP_T13(s13ee);
    EEP_T14(s13ee);
    EEP_T15(s13ee);
    EEP_T16(s13ee);
}

static void EEP_T19(S13EE *s13ee)
{
    S13EE_PRINTF("%s - T = 25℃, EEP_T11 ~ EEP_T16 TEST.\r\n", __FUNCTION__);

    EEP_T11(s13ee);
    EEP_T12(s13ee);
    EEP_T13(s13ee);
    EEP_T14(s13ee);
    EEP_T15(s13ee);
    EEP_T16(s13ee);
}

static void EEP_T20(S13EE *s13ee)
{
    S13EE_PRINTF("%s - T = 45℃, EEP_T11 ~ EEP_T16 TEST.\r\n", __FUNCTION__);

    EEP_T11(s13ee);
    EEP_T12(s13ee);
    EEP_T13(s13ee);
    EEP_T14(s13ee);
    EEP_T15(s13ee);
    EEP_T16(s13ee);
}

static void EEP_T21(S13EE *s13ee)
{
    S13EE_PRINTF("%s - T = 85℃, EEP_T11 ~ EEP_T16 TEST.\r\n", __FUNCTION__);

    EEP_T11(s13ee);
    EEP_T12(s13ee);
    EEP_T13(s13ee);
    EEP_T14(s13ee);
    EEP_T15(s13ee);
    EEP_T16(s13ee);
}

static void EEP_T22(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t readBuffer[S13EE_WORD_MAX];

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.8V , Read opreate current test.(Input \'q\' to exit)\r\n", __FUNCTION__);

    while(!(inputIntSignal && (inputBuffer[0] == 'q')))
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(0, readBuffer, S13EE_WORD_MAX)))
            goto EEP_T22_ERR;
    }
    inputIntSignal  = 0;

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T22_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T23(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t readBuffer[S13EE_WORD_MAX];

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.5V , Read opreate current test.(Input \'q\' to exit)\r\n", __FUNCTION__);

    while(!(inputIntSignal && (inputBuffer[0] == 'q')))
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(0, readBuffer, S13EE_WORD_MAX)))
            goto EEP_T23_ERR;
    }
    inputIntSignal  = 0;

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T23_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T24(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t readBuffer[S13EE_WORD_MAX];

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.0V , Read opreate current test.(Input \'q\' to exit)\r\n", __FUNCTION__);

    while(!(inputIntSignal && (inputBuffer[0] == 'q')))
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(0, readBuffer, S13EE_WORD_MAX)))
            goto EEP_T24_ERR;
    }
    inputIntSignal  = 0;

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T24_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T25(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t readBuffer[S13EE_WORD_MAX];

    S13EE_PRINTF("%s - T = 25℃, vcca = 0.8/0.7V , Read opreate current test.(Input \'q\' to exit)\r\n", __FUNCTION__);

    while(!(inputIntSignal && (inputBuffer[0] == 'q')))
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(0, readBuffer, S13EE_WORD_MAX)))
            goto EEP_T25_ERR;
    }
    inputIntSignal  = 0;

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T25_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T26(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t writeBuffer[S13EE_WORD_MAX];

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.8V , Write opreate current test.(Input \'q\' to exit)\r\n", __FUNCTION__);

    while(!(inputIntSignal && (inputBuffer[0] == 'q')))
    {
        if(S13EE_SUCCESS != (ret = s13ee->write(0, writeBuffer, S13EE_WORD_MAX)))
            goto EEP_T26_ERR;
    }
    inputIntSignal  = 0;

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T26_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T27(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t writeBuffer[S13EE_WORD_MAX];

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.5V , Write opreate current test.(Input \'q\' to exit)\r\n", __FUNCTION__);

    while(!(inputIntSignal && (inputBuffer[0] == 'q')))
    {
        if(S13EE_SUCCESS != (ret = s13ee->write(0, writeBuffer, S13EE_WORD_MAX)))
            goto EEP_T27_ERR;
    }
    inputIntSignal  = 0;

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T27_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T28(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t writeBuffer[S13EE_WORD_MAX];

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.2V , Write opreate current test.(Input \'q\' to exit)\r\n", __FUNCTION__);

    while(!(inputIntSignal && (inputBuffer[0] == 'q')))
    {
        if(S13EE_SUCCESS != (ret = s13ee->write(0, writeBuffer, S13EE_WORD_MAX)))
            goto EEP_T28_ERR;
    }
    inputIntSignal  = 0;

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T28_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T29(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t writeBuffer[S13EE_WORD_MAX];

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.1V , Write opreate current test.(Input \'q\' to exit)\r\n", __FUNCTION__);

    while(!(inputIntSignal && (inputBuffer[0] == 'q')))
    {
        if(S13EE_SUCCESS != (ret = s13ee->write(0, writeBuffer, S13EE_WORD_MAX)))
            goto EEP_T29_ERR;
    }
    inputIntSignal  = 0;

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T29_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

void EEP_T_MAIN(S13EE *s13ee)
{

}

