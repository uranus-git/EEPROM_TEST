#include "S13EE_X64Y64.h"
//#include <S13EE_X64Y64_TestCase.h>
#include <string.h>
#include "usart1.H"
#include <stdlib.h>

char inputBuffer[];
int inputIntSignal;

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

static void EEP_InitWakeUp(S13EE *s13ee)
{
    uint16_t count = 0, index, addr = 0;
    static uint16_t writeBuffer[4] = {0x5A5A, 0xA5A5, 0x0000, 0xFFFF};
    uint16_t data[4], rdata;
    uint8_t *buffer;
    S13EE_STATUS ret;
    char ch;

    S13EE_PRINTF("初始化和唤醒测试\r\n");
    S13EE_PRINTF("writeBuffer [0x%04x 0x%04x 0x%04x 0x%04x]\r\n",
        writeBuffer[0], writeBuffer[1], writeBuffer[2], writeBuffer[3]);
    S13EE_PRINTF("Do you want to change (y/n): ");

    buffer = S13EE_GETLINE;
    ch = buffer[0];
    if((ch == 'y') || (ch == 'Y'))
    {
        for(index = 0; index < 4; index++)
        {
            S13EE_PRINTF("writeBuffer[%d] 0x%04x : 0x", index, writeBuffer[index]);
            buffer = S13EE_GETLINE;
            if(strlen(buffer) > 0)
                writeBuffer[index] = htoi((const char *)buffer);
        }
        S13EE_PRINTF("writeBuffer [0x%04x 0x%04x 0x%04x 0x%04x]\r\n",
            writeBuffer[0], writeBuffer[1], writeBuffer[2], writeBuffer[3]);
    }

    S13EE_PRINTF("TEST IN PROCESSING %02d\%%.", count * 2);
    for(count = 0; count < 50; count++)
    {
        S13EE_PRINTF("\b\b\b\b%02d\%%.", count * 2);
        for(index = 0; index < 4; index++)
        {
            data[0] = writeBuffer[index];data[1] = writeBuffer[index];
            data[2] = writeBuffer[index];data[3] = writeBuffer[index];

            if(S13EE_SUCCESS != (ret = s13ee->write(addr, &writeBuffer[index], 1)))
                goto s13eeInitWakeUpErr;

            if(S13EE_SUCCESS != (ret = s13ee->read(addr, &rdata, 1)))
                goto s13eeInitWakeUpErr;

            if(rdata != writeBuffer[index])
            {
                S13EE_PRINTF("\r\n\r\n%s : read addr[%d] = 0x%04x, not match the value 0x%04x(count = %d, index = %d)\r\n",
                    __FUNCTION__, addr, rdata, writeBuffer[index], count, index);
                return;
            }
        }
    }
    S13EE_PRINTF("\b\b\b\b%02d\%%.\r\n", count * 2);
    S13EE_PRINTF("\r\n%s SUCCUSS.\n", __FUNCTION__);
    return;

s13eeInitWakeUpErr:
    S13EE_PRINTF("\r\n\r\nEEP_T1 FAILED : %s.\r\n\r\n", s13ee->errToString(ret));
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
        S13EE_PRINTF("EEP_T1 SUCCUSS.\r\n");

    return;

EEP_T1_ERR:
    S13EE_PRINTF("\r\n\r\nEEP_T1 FAILED : %s.\r\n\r\n", s13ee->errToString(ret));
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

    S13EE_PRINTF("%s - 200 次全0测试.\r\n", __FUNCTION__);
    S13EE_PRINTF("TEST IN PROCESSING %02d\%%.", times/2);
    for(times = 0; times < 200; times++)
    {
        S13EE_PRINTF("\b\b\b\b%02d\%%.", times/2);
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

    S13EE_PRINTF("\b\b\b\b%02d\%%\r\n.", times/2);
    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
}

static void EEP_T6(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint32_t times = 0;
    uint16_t addr = 0;
    uint16_t u16ReadBuffer[S13EE_WORD_MAX];
    const uint16_t u16Value = 0xFFFF;

    S13EE_PRINTF("%s - 200 次全1测试.\r\n", __FUNCTION__);
    S13EE_PRINTF("TEST IN PROCESSING %02d\%%.", times/2);
    for(times = 0; times < 200; times++)
    {
        S13EE_PRINTF("\b\b\b\b%02d\%%.", times/2);
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
                printbuffer(u16ReadBuffer, S13EE_WORD_MAX);
                S13EE_PRINTF("EEP_T6 times %d compare addr %d failed.\r\n", times, addr);
                return;
            }
        }
    }

    S13EE_PRINTF("\b\b\b\b%02d\%%.\r\n", times/2);
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
    S13EE_PRINTF("TEST IN PROCESSING %02d\%%.", count);

    for(count = 0; count < 100; count++)
    {
        S13EE_PRINTF("\b\b\b\b%02d\%%.", count);

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
                S13EE_PRINTF("\r\n2.1 Read addr %d, data 0x%04x isn't 0x0000.\r\n", addr, rdata);
                return;
            }

            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
                goto EEP_T7_ERR;
            /*****************************/
            if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)(addr + 1), &rdata, 1)))
                goto EEP_T7_ERR;

            if(rdata != 0x0000)
            {
                S13EE_PRINTF("\r\n2.2 Read addr %d, data 0x%04x isn't 0x0000.\r\n", (addr + 1), rdata);
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
                S13EE_PRINTF("\r\n3.1 Read addr %d, data 0x%04x isn't 0x%04x.\r\n", addr, temp, rdata);
                return;
            }

            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
                goto EEP_T7_ERR;
            /*****************************/
            if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)(addr + 1), &temp, 1)))
                goto EEP_T7_ERR;

            if(temp != rdata)
            {
                S13EE_PRINTF("\r\n3.2 Read addr %d, data 0x%04x isn't 0x%04x.\r\n", (addr + 1), temp, rdata);
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
                S13EE_PRINTF("\r\n4.1 Read addr %d, data 0x%04x isn't 0x%04x.\r\n", addr, temp, rdata);
                return;
            }

            if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)(addr + 1), &temp, 1)))
                goto EEP_T7_ERR;

            if(rdata != temp)
            {
                S13EE_PRINTF("\r\n4.2 Read addr %d, data 0x%04x isn't 0x%04x.\r\n", addr+1, temp, rdata);
                return;
            }
         }
    }

    S13EE_PRINTF("\b\b\b\b%02d\%%.\r\n", count);
    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;

EEP_T7_ERR:
    S13EE_PRINTF("EEP_T7 FAILED : %s.\r\n", s13ee->errToString(ret));
}

static void EEP_T8(S13EE *s13ee)
{
    uint16_t addr;
    uint16_t wdata = 0x5a5a, rdata;
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

    S13EE_PRINTF("%s - PERFORMANCE TEST.\r\n", __FUNCTION__);
    S13EE_PRINTF("Write 2kbit data with 0x%04x 0x%04x 0x%04x 0x%04x\r\n",
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
    uint8_t *buffer;

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.8V , Read opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

    do
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(0, readBuffer, S13EE_WORD_MAX)))
            goto EEP_T22_ERR;
        buffer = S13EE_GETLINE_NOWAIT;
    }while(!(buffer && (buffer[0] == 'q')));

    S13EE_PRINTF("\r\n%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T22_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T23(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t readBuffer[S13EE_WORD_MAX];
    uint8_t *buffer;

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.5V , Read opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

    do
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(0, readBuffer, S13EE_WORD_MAX)))
            goto EEP_T23_ERR;

        buffer = S13EE_GETLINE_NOWAIT;
    }while(!(buffer && (buffer[0] == 'q')));

    S13EE_PRINTF("\r\n%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T23_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T24(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t readBuffer[S13EE_WORD_MAX];
    char *buffer;

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.0V , Read opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

    do
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(0, readBuffer, S13EE_WORD_MAX)))
            goto EEP_T24_ERR;

        buffer = S13EE_GETLINE_NOWAIT;
    }while(!(buffer && (*buffer == 'q')));

    S13EE_PRINTF("\r\n%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T24_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T25(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t readBuffer[S13EE_WORD_MAX];
    uint8_t *buffer;

    S13EE_PRINTF("%s - T = 25℃, vcca = 0.8/0.7V , Read opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

    do
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(0, readBuffer, S13EE_WORD_MAX)))
            goto EEP_T25_ERR;
        buffer = S13EE_GETLINE_NOWAIT;
    }while(!(buffer && (buffer[0] == 'q')));

    S13EE_PRINTF("\r\n%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T25_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T26(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t writeBuffer[S13EE_WORD_MAX];
    uint8_t *buffer;

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.8V , Write opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

    do
    {
        if(S13EE_SUCCESS != (ret = s13ee->write(0, writeBuffer, S13EE_WORD_MAX)))
            goto EEP_T26_ERR;
        buffer = S13EE_GETLINE_NOWAIT;
    }while(!(buffer && (buffer[0] == 'q')));

    S13EE_PRINTF("\r\n%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T26_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T27(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t writeBuffer[S13EE_WORD_MAX];
    uint8_t *buffer;

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.5V , Write opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

    do
    {
        if(S13EE_SUCCESS != (ret = s13ee->write(0, writeBuffer, S13EE_WORD_MAX)))
            goto EEP_T27_ERR;
        buffer = S13EE_GETLINE_NOWAIT;
    }while(!(buffer && (buffer[0] == 'q')));

    S13EE_PRINTF("\r\n%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T27_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T28(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t writeBuffer[S13EE_WORD_MAX];
    uint8_t *buffer;

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.2V , Write opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

    do
    {
        if(S13EE_SUCCESS != (ret = s13ee->write(0, writeBuffer, S13EE_WORD_MAX)))
            goto EEP_T28_ERR;
        buffer = S13EE_GETLINE_NOWAIT;
    }while(!(buffer && (buffer[0] == 'q')));

    S13EE_PRINTF("\r\n%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T28_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T29(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t writeBuffer[S13EE_WORD_MAX];
    uint8_t *buffer;

    S13EE_PRINTF("%s - T = 25℃, vcca = 1.1V , Write opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

    do
    {
        if(S13EE_SUCCESS != (ret = s13ee->write(0, writeBuffer, S13EE_WORD_MAX)))
            goto EEP_T29_ERR;
        buffer = S13EE_GETLINE_NOWAIT;
    }while(!(buffer && (buffer[0] == 'q')));

    S13EE_PRINTF("\r\n%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T29_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

enum
{
    INITIALIZE_AND_WAKEUP_TEST = 1,
    BASIC_FUNCTION_TEST,
    ERASE_READ_WRITE_TEST,
    PERFORMANCE_TEST,
    ALLROW_TEST,
    TEMPERATURE_TEST,
    POWER_COMSUMPTION_TEST,
    TIMING_TEST,
    RELIABILITY_TEST
};

enum
{
    EEP_TEST1 = 1,
    EEP_TEST2,
    EEP_TEST3,
    EEP_TEST4,
    EEP_TEST5,
    EEP_TEST6,
    EEP_TEST7,
    EEP_TEST8,
    EEP_TEST9,
    EEP_TEST10,
    EEP_TEST11,
    EEP_TEST12,
    EEP_TEST13,
    EEP_TEST14,
    EEP_TEST15,
    EEP_TEST16,
    EEP_TEST17,
    EEP_TEST18,
    EEP_TEST19,
    EEP_TEST20,
    EEP_TEST21,
    EEP_TEST22,
    EEP_TEST23,
    EEP_TEST24,
    EEP_TEST25,
    EEP_TEST26,
    EEP_TEST27,
    EEP_TEST28,
    EEP_TEST29,
    EEP_EXIT = 99
};

static uint32_t getOption(void)
{
    uint32_t option;
    uint8_t * buffer;

    S13EE_PRINTF("\r\n请输入选项编号:");
    buffer = S13EE_GETLINE;
    option = atoi((const char*)buffer);

    return option;
}

static void basicFunctionTest(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("%2d. EEP_T%d - 与EEPROM通讯正常，能正确发指令\r\n", EEP_TEST1, EEP_TEST1);
        S13EE_PRINTF("%2d. EEP_T%d - 读操作，序号2～6测试基于1.5v\r\n", EEP_TEST2, EEP_TEST2);
        S13EE_PRINTF("%2d. EEP_T%d - 写操作\r\n", EEP_TEST3, EEP_TEST3);
        S13EE_PRINTF("%2d. EEP_T%d - 擦除操作\r\n", EEP_TEST4, EEP_TEST4);
        S13EE_PRINTF("%2d. 退出\r\n", EEP_EXIT);

        option = getOption();

        switch(option)
        {
            case EEP_TEST1:
                EEP_T1(s13ee);
                break;
            case EEP_TEST2:
                EEP_T2(s13ee);
                break;
            case EEP_TEST3:
                EEP_T3(s13ee);
                break;
            case EEP_TEST4:
                EEP_T4(s13ee);
                break;
        }
    }while(option != EEP_EXIT);
}

static void eraseReadWriteTest(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("%2d. EEP_T%d - 连续擦除读写200次，对比读写数据，写数据为00，地址递增(全0测试)\r\n", EEP_TEST5, EEP_TEST5);
        S13EE_PRINTF("%2d. EEP_T%d - 连续擦除读写200次，对比读写数据，写数据为FF，地址递增(全1测试)\r\n", EEP_TEST6, EEP_TEST6);
        S13EE_PRINTF("%2d. EEP_T%d - 连续100次march算法测试\r\n", EEP_TEST7, EEP_TEST7);
        S13EE_PRINTF("%2d. EEP_T%d - 擦读写，对比读写数据，写数据为5A，地址变化按行方向递增顺序\r\n", EEP_TEST8, EEP_TEST8);
        S13EE_PRINTF("%2d. EEP_T%d - 擦读写，对比读写数据，写数据为5A，地址变化方向按左斜线\r\n", EEP_TEST9, EEP_TEST9);
        S13EE_PRINTF("%2d. EEP_T%d - 擦读写，对比读写数据，写数据为5A，地址变化方向按右斜线\r\n", EEP_TEST10, EEP_TEST10);
        S13EE_PRINTF("%2d. 退出\r\n", EEP_EXIT);

        option = getOption();

        switch(option)
        {
            case EEP_TEST5:
                EEP_T5(s13ee);
                break;
            case EEP_TEST6:
                EEP_T6(s13ee);
                break;
            case EEP_TEST7:
                EEP_T7(s13ee);
                break;
            case EEP_TEST8:
                EEP_T8(s13ee);
                break;
            case EEP_TEST9:
                EEP_T9(s13ee);
                break;
            case EEP_TEST10:
                EEP_T10(s13ee);
                break;
        }
    }while(option != EEP_EXIT);
}

static void performanceTest(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("%2d. EEP_T%d - 电压，1.5v时，调用EEP_T7测试程序\r\n", EEP_TEST11, EEP_TEST11);
        S13EE_PRINTF("%2d. EEP_T%d - 电压，1.3v时，调用EEP_T7测试程序\r\n", EEP_TEST12, EEP_TEST12);
        S13EE_PRINTF("%2d. EEP_T%d - 电压，1.2v时，调用EEP_T7测试程序\r\n", EEP_TEST13, EEP_TEST13);
        S13EE_PRINTF("%2d. EEP_T%d - 电压，1.1v时，调用EEP_T7测试程序\r\n", EEP_TEST14, EEP_TEST14);
        S13EE_PRINTF("%2d. EEP_T%d - 电压，1.0v时，调用EEP_T7测试程序\r\n", EEP_TEST15, EEP_TEST15);
        S13EE_PRINTF("%2d. 退出\r\n", EEP_EXIT);

        option = getOption();

        switch(option)
        {
            case EEP_TEST11:
                EEP_T11(s13ee);
                break;
            case EEP_TEST12:
                EEP_T12(s13ee);
                break;
            case EEP_TEST13:
                EEP_T13(s13ee);
                break;
            case EEP_TEST14:
                EEP_T14(s13ee);
                break;
            case EEP_TEST15:
                EEP_T15(s13ee);
                break;
        }
    }while(option != EEP_EXIT);
}

static void temperatureTest(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("%2d. EEP_T%d - T=-20℃，调用EEP_T11~EEP_T16测试程序，读写性能测试\r\n", EEP_TEST17, EEP_TEST17);
        S13EE_PRINTF("%2d. EEP_T%d - T=0℃，调用EEP_T11~EEP_T16测试程序，读写性能测试\r\n", EEP_TEST18, EEP_TEST18);
        S13EE_PRINTF("%2d. EEP_T%d - T=25℃，调用EEP_T11~EEP_T16测试程序，读写性能测试\r\n", EEP_TEST19, EEP_TEST19);
        S13EE_PRINTF("%2d. EEP_T%d - T=45℃，调用EEP_T11~EEP_T16测试程序，读写性能测试\r\n", EEP_TEST20, EEP_TEST20);
        S13EE_PRINTF("%2d. EEP_T%d - T=85℃，调用EEP_T11~EEP_T16测试程序，读写性能测试\r\n", EEP_TEST21, EEP_TEST21);
        S13EE_PRINTF("%2d. 退出\r\n", EEP_EXIT);

        option = getOption();

        switch(option)
        {
            case EEP_TEST17:
                EEP_T17(s13ee);
                break;
            case EEP_TEST18:
                EEP_T18(s13ee);
                break;
            case EEP_TEST19:
                EEP_T19(s13ee);
                break;
            case EEP_TEST20:
                EEP_T20(s13ee);
                break;
            case EEP_TEST21:
                EEP_T21(s13ee);
                break;
        }
    }while(option != EEP_EXIT);
}

static void powerConsumptionTest(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("%2d. EEP_T%d - T=25℃，vcca=1.8v时，EEPROM读操作时电流(测试vcca!端口)\r\n", EEP_TEST22, EEP_TEST22);
        S13EE_PRINTF("%2d. EEP_T%d - T=25℃，vcca=1.5v时，EEPROM读操作时电流(测试vcca!端口)\r\n", EEP_TEST23, EEP_TEST23);
        S13EE_PRINTF("%2d. EEP_T%d - T=25℃，vcca=1.0v时，EEPROM读操作时电流(测试vcca!端口)\r\n", EEP_TEST24, EEP_TEST24);
        S13EE_PRINTF("%2d. EEP_T%d - T=25℃，vcca=0.8/0.7v时，EEPROM读操作时电流(测试vcca!端口)\r\n", EEP_TEST25, EEP_TEST25);
        S13EE_PRINTF("%2d. EEP_T%d - T=25℃，vcca=1.8v时，EEPROM写操作时电流(测试vcca!端口)\r\n", EEP_TEST26, EEP_TEST26);
        S13EE_PRINTF("%2d. EEP_T%d - T=25℃，vcca=1.5v时，EEPROM写操作时电流(测试vcca!端口)\r\n", EEP_TEST27, EEP_TEST27);
        S13EE_PRINTF("%2d. EEP_T%d - T=25℃，vcca=1.2v时，EEPROM写操作时电流(测试vcca!端口)\r\n", EEP_TEST28, EEP_TEST28);
        S13EE_PRINTF("%2d. EEP_T%d - T=25℃，vcca=1.1v时，EEPROM写操作时电流(测试vcca!端口)\r\n", EEP_TEST29, EEP_TEST29);
        S13EE_PRINTF("%2d. 退出\r\n", EEP_EXIT);

        option = getOption();

        switch(option)
        {
            case EEP_TEST22:
                EEP_T2(s13ee);
                break;
            case EEP_TEST23:
                EEP_T23(s13ee);
                break;
            case EEP_TEST24:
                EEP_T24(s13ee);
                break;
            case EEP_TEST25:
                EEP_T25(s13ee);
                break;
            case EEP_TEST26:
                EEP_T26(s13ee);
                break;
            case EEP_TEST27:
                EEP_T27(s13ee);
                break;
            case EEP_TEST28:
                EEP_T28(s13ee);
                break;
            case EEP_TEST29:
                EEP_T29(s13ee);
                break;
        }
    }while(option != EEP_EXIT);
}

void EEP_T_MAIN(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("\r\n\r\nS13EE_X64Y64 测试目录:\r\n\r\n");
        S13EE_PRINTF("%2d. 初始化和唤醒测试.\r\n", INITIALIZE_AND_WAKEUP_TEST);
        S13EE_PRINTF("%2d. 基本功能测试.\r\n", BASIC_FUNCTION_TEST);
        S13EE_PRINTF("%2d. 擦除/读写功能测试.\r\n", ERASE_READ_WRITE_TEST);
        S13EE_PRINTF("%2d. 性能测试.\r\n", PERFORMANCE_TEST);
        S13EE_PRINTF("%2d. ALLROW测试.\r\n", ALLROW_TEST);
        S13EE_PRINTF("%2d. 高低温测试.\r\n", TEMPERATURE_TEST);
        S13EE_PRINTF("%2d. 功耗测试.\r\n", POWER_COMSUMPTION_TEST);
        S13EE_PRINTF("%2d. 时序测试.\r\n", TIMING_TEST);
        S13EE_PRINTF("%2d. 可靠性测试.\r\n", RELIABILITY_TEST);

        option = getOption();

        switch(option)
        {
            case INITIALIZE_AND_WAKEUP_TEST:
                EEP_InitWakeUp(s13ee);
                break;
            case BASIC_FUNCTION_TEST:
                basicFunctionTest(s13ee);
                break;
            case ERASE_READ_WRITE_TEST:
                eraseReadWriteTest(s13ee);
                break;
            case PERFORMANCE_TEST:
                performanceTest(s13ee);
                break;
            case ALLROW_TEST:
                EEP_T16(s13ee);
                break;
            case TEMPERATURE_TEST:
                temperatureTest(s13ee);
                break;
            case POWER_COMSUMPTION_TEST:
                powerConsumptionTest(s13ee);
                break;
            case TIMING_TEST:
                break;
            case RELIABILITY_TEST:
                break;
        }
    }while(1);
}

