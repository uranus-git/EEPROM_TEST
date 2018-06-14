#include "S13EE_X64Y64.h"
#include "common.h"
//#include <S13EE_X64Y64_TestCase.h>
#include <string.h>
#include "usart1.H"
#include <stdlib.h>

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
    EEP_TEST30,
    EEP_TEST31,
    EEP_TEST32,
    EEP_TEST33,
    EEP_EXIT = 99
};

typedef enum
{
    INPUT_DATA_FORMAT_DEC,
    INPUT_DATA_FORMAT_HEX
}data_input_format_e;

char inputBuffer[];
int inputIntSignal;
static void checknWordChange(uint16_t *writeBuffer, uint32_t n);
static void checknWordChangeExp(char *name, uint16_t *resBuffer, uint32_t *n);

static uint32_t getOption(void);
static uint32_t DIAG_GetNumber(char * name, data_input_format_e format, uint32_t min, uint32_t max);
static void EEP_T32(S13EE *s13ee);




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

    S13EE_PRINTF("��ʼ���ͻ��Ѳ���\r\n");
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
            if(strlen((const char *)buffer) > 0)
                writeBuffer[index] = htoi(buffer);
        }
        S13EE_PRINTF("writeBuffer [0x%04x 0x%04x 0x%04x 0x%04x]\r\n",
            writeBuffer[0], writeBuffer[1], writeBuffer[2], writeBuffer[3]);
    }

    S13EE_PRINTF("TEST IN PROCESSING %02d%%.", count * 2);
    for(count = 0; count < 50; count++)
    {
        S13EE_PRINTF("\b\b\b\b%02d%%.", count * 2);
        for(index = 0; index < 4; index++)
        {
            data[0] = writeBuffer[index];
            data[1] = writeBuffer[index];
            data[2] = writeBuffer[index];
            data[3] = writeBuffer[index];

            if(S13EE_SUCCESS != (ret = s13ee->chipWrite((uint16_t (*)[4])data)))
                goto s13eeInitWakeUpErr;

            for(addr = 0; addr < S13EE_WORD_MAX; addr++)
            {
                if(S13EE_SUCCESS != (ret = s13ee->read(addr, &rdata, 1)))
                    goto s13eeInitWakeUpErr;

                if(rdata != writeBuffer[index])
                {
                    S13EE_PRINTF("\r\n�� %d �Σ�ȫƬд���� 0x%04x,����ַ %d ,�������� 0x%04x .\r\n", count,  writeBuffer[index], addr, rdata);
                    return;
                }
            }
        }
    }
    S13EE_PRINTF("\b\b\b\b%02d%%.\r\n", count * 2);
    S13EE_PRINTF("\r\n%s SUCCUSS.\n", __FUNCTION__);
    return;

s13eeInitWakeUpErr:
    S13EE_PRINTF("\r\n\r\nEEP_T1 FAILED : %s.\r\n\r\n", s13ee->errToString(ret));
}

static void EEP_T1(S13EE *s13ee)
{
    S13EE_STATUS ret;
    uint16_t wdata = 0xA5A5, rdata = 0;

    /* ��EEPROMͨѶ����������ȷ��ָ�� */
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
    static uint16_t writeBuffer[4] = {0xA5A5, 0xA5A5, 0xA5A5, 0xA5A5};
    uint16_t rdata[4], addr;
    uint8_t *buffer,ch;
    uint32_t index;
    S13EE_STATUS ret;

    S13EE_PRINTF("chipWrite.\r\n");
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
            if(strlen((const char *)buffer) > 0)
                writeBuffer[index] = htoi(buffer);
        }
        S13EE_PRINTF("writeBuffer [0x%04x 0x%04x 0x%04x 0x%04x]\r\n",
            writeBuffer[0], writeBuffer[1], writeBuffer[2], writeBuffer[3]);
    }

    if(S13EE_SUCCESS != (result = s13ee->chipWrite((uint16_t (*)[4])writeBuffer)))
    {
        S13EE_PRINTF("chipWrite failed(result = %d)\r\n", result);
        goto EEP_T3_ERR;
    }

    for(addr = 0; addr < S13EE_WORD_MAX; addr += 4)
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(addr, (uint16_t *)rdata, 4)))
            goto EEP_T3_ERR;

        for(index = 0; index < 4; index++)
        {
            if(rdata[index] != writeBuffer[index])
            {
                S13EE_PRINTF("\r\n����ַ %d ,�������� 0x%04x ,Ӧ��Ϊ 0x%04x.\r\n", addr + index, rdata[index], writeBuffer[index]);
                return;
            }
        }
    }

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T3_ERR:
    S13EE_PRINTF("\r\n\r\nEEP_T3_ERR FAILED : %s.\r\n\r\n", s13ee->errToString(ret));

}

static void EEP_T4(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint16_t addr, rdata;
    S13EE_STATUS ret;

    S13EE_PRINTF("%s chipErase.\r\n", __FUNCTION__);

    if(S13EE_SUCCESS != (result = s13ee->chipErase()))
    {
        S13EE_PRINTF("chipErase failed(result = %d)\r\n", result);
        goto EEP_T4_ERR;;
    }

    for(addr = 0; addr < S13EE_WORD_MAX; addr++)
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(addr, &rdata, 1)))
            goto EEP_T4_ERR;

        if(rdata != 0)
        {
            S13EE_PRINTF("\r\n����ַ %d ,�������� 0x%04x ,Ӧ��Ϊ 0x0000.\r\n", addr, rdata);
            return;
        }
    }

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;

EEP_T4_ERR:
    S13EE_PRINTF("\r\n\r\n%s FAILED : %s.\r\n\r\n",__FUNCTION__, s13ee->errToString(ret));

}

static void EEP_T5(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint32_t times = 0;
    uint16_t addr = 0;
    uint16_t u16ReadBuffer[S13EE_WORD_MAX];
    const uint16_t u16Value = 0;

    S13EE_PRINTF("%s - 200 ��ȫ0����.\r\n", __FUNCTION__);
    S13EE_PRINTF("TEST IN PROCESSING %02d%%.", times/2);
    for(times = 0; times < 200; times++)
    {
        S13EE_PRINTF("\b\b\b\b%02d%%.", times/2);
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

    S13EE_PRINTF("\b\b\b\b%02d%%.\r\n", times/2);
    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
}

static void EEP_T6(S13EE *s13ee)
{
    S13EE_STATUS result;
    uint32_t times = 0;
    uint16_t addr = 0;
    uint16_t u16ReadBuffer[S13EE_WORD_MAX];
    const uint16_t u16Value = 0xFFFF;

    S13EE_PRINTF("%s - 200 ��ȫ1����.\r\n", __FUNCTION__);
    S13EE_PRINTF("TEST IN PROCESSING %02d%%.", times/2);
    for(times = 0; times < 200; times++)
    {
        S13EE_PRINTF("\b\b\b\b%02d%%.", times/2);
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
                S13EE_PRINTF("\r\n");
                printbuffer(u16ReadBuffer, S13EE_WORD_MAX);
                S13EE_PRINTF("EEP_T6 times %d compare addr %d failed.\r\n", times, addr);
                return;
            }
        }
    }

    S13EE_PRINTF("\b\b\b\b%02d%%.\r\n", times/2);
    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
}

/* ����100��march�㷨���� */
static void EEP_T7(S13EE *s13ee)
{
    int16_t addr;
    uint16_t rdata, wdata, temp;
    uint16_t count = 0;
    S13EE_STATUS ret;

    S13EE_PRINTF("\r\n100 march�㷨����.\r\n\r\n");
    S13EE_PRINTF("march�㷨����˵�� :\r\n");
    S13EE_PRINTF("1. ����ʼλ�ÿ�ʼ,���յ�ַ����˳���ȫ����ԪдΪ0x0000.\r\n");
    S13EE_PRINTF("2. ����ʼλ�ÿ�ʼ,���յ�ַ����˳��,��A0��Ԫ��0x0000���ٸ�дΪ0x5555��"
         "\r\n   ��A1��Ԫ��0x0000���ٸ�дΪ0x5555����A2��Ԫ��0x0000���ٸ�дΪ0x5555��"
         "\r\n   ��A3��Ԫ��0x0000���ٸ�дΪ0x5555����A4��Ԫ��0x0000���ٸ�дΪ0xaaaa��"
         "\r\n   ��A5��Ԫ��0x0000���ٸ�дΪ0xaaaa����A6��Ԫ��0x0000���ٸ�дΪ0xaaaa��"
         "\r\n   ��A7��Ԫ��0x0000���ٸ�дΪ0xaaaa...ֱ��An-1���ⲽ����֮�󣬴洢��"
         "\r\n   ���񱻻���Ϊ����һ�������������checkerboard�����㷨�����õ��Ľ��һ"
         "\r\n   �����ڴ˹����У������ص����ݲ�Ϊ0x0000���򷵻�ʧ��.\r\n");
    S13EE_PRINTF("3. �����һ��word��Ԫλ�ÿ�ʼ�����յ�ַ���򣬶�An-1��Ԫ��0xaaaa��"
          "\r\n   �ٸ�дΪ0x5555����An-2��Ԫ��0xaaaa���ٸ�дΪ0x5555����An-3��Ԫ��0xaaaa��"
          "\r\n   �ٸ�дΪ0x5555����An-4��Ԫ��0xaaaa���ٸ�дΪ0x5555����An-5��Ԫ��0x5555��"
          "\r\n   �ٸ�дΪ0xaaaa����An-6��Ԫ��0x5555���ٸ�дΪ0xaaaa����An-7��Ԫ��0x5555��"
          "\r\n   �ٸ�дΪ0xaaaa����An-8��Ԫ��0x5555���ٸ�дΪ0xaaaa��ֱ��A0"
          "\r\n  ������ȡÿ���洢��Ԫ�е�ֵ��д���෴ֵ)���ڴ˹����У������ص����ݲ�һ�£�"
          "\r\n  �򷵻�ʧ��\r\n");
    S13EE_PRINTF("4. ����ʼλ�ÿ�ʼ�����յ�ַ����˳�򣬶�ȫ����Ԫ�������ص����ݲ�һ�£�"
        "\r\n  �򷵻�ʧ��\r\n");

    S13EE_PRINTF("������ %02d%%.", count);
#if 1
    for(count = 0; count < 100; count++)
    {
        S13EE_PRINTF("\b\b\b\b%02d%%.", count);

        /* 1. ����ʼλ�ÿ�ʼ,���յ�ַ����˳���ȫ����ԪдΪ0x0000 */
        for(addr = 0, wdata = 0; addr < S13EE_WORD_MAX; addr++)
        {
            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
                goto EEP_T7_ERR;
        }

        /* 2. ����ʼλ�ÿ�ʼ,���յ�ַ����˳��,��A0��Ԫ��0x0000���ٸ�дΪ0x5555��
         *    ��A1��Ԫ��0x0000���ٸ�дΪ0x5555����A2��Ԫ��0x0000���ٸ�дΪ0x5555��
         *    ��A3��Ԫ��0x0000���ٸ�дΪ0x5555����A4��Ԫ��0x0000���ٸ�дΪ0xaaaa��
         *    ��A5��Ԫ��0x0000���ٸ�дΪ0xaaaa����A6��Ԫ��0x0000���ٸ�дΪ0xaaaa��
         *    ��A7��Ԫ��0x0000���ٸ�дΪ0xaaaa...ֱ��An-1���ⲽ����֮�󣬴洢��
         *    ���񱻻���Ϊ����һ�������������checkerboard�����㷨�����õ��Ľ��һ
         *    �����ڴ˹����У������ص����ݲ�Ϊ0x0000���򷵻�ʧ��
         */
         for(addr = 0, wdata = 0x5555; addr < S13EE_WORD_MAX; addr += 4, wdata = ~wdata)
         {
            for(uint32_t index = 0; index < 4; index++)
            {
                if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)(addr + index), &rdata, 1)))
                goto EEP_T7_ERR;

                if(rdata != 0x0000)
                {
                    //S13EE_PRINTF("\r\n2. Read addr %d, data 0x%04x isn't 0x0000.\r\n", (addr + index), rdata);
                    S13EE_PRINTF("\r\n2. �� %d �Σ�����ַ %d ,�������� 0x%04x ,Ӧ��Ϊ 0x0000.\r\n", count, (addr + index), rdata);
                    return;
                }

                if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)(addr + index), &wdata, 1)))
                    goto EEP_T7_ERR;
            }
         }

         /* 3. �����һ��word��Ԫλ�ÿ�ʼ�����յ�ַ���򣬶�An-1��Ԫ��0xaaaa��
          *    �ٸ�дΪ0x5555����An-2��Ԫ��0xaaaa���ٸ�дΪ0x5555����An-3��Ԫ��0xaaaa��
          *    �ٸ�дΪ0x5555����An-4��Ԫ��0xaaaa���ٸ�дΪ0x5555����An-5��Ԫ��0x5555��
          *    �ٸ�дΪ0xaaaa����An-6��Ԫ��0x5555���ٸ�дΪ0xaaaa����An-7��Ԫ��0x5555��
          *    �ٸ�дΪ0xaaaa����An-8��Ԫ��0x5555���ٸ�дΪ0xaaaa��ֱ��A0
          *   ������ȡÿ���洢��Ԫ�е�ֵ��д���෴ֵ)���ڴ˹����У������ص����ݲ�һ�£��򷵻�ʧ��
          */
         for(addr = S13EE_WORD_MAX - 1, rdata = 0xAAAA, wdata = 0x5555; \
            (addr >= 0) && (addr < S13EE_WORD_MAX); addr -= 4, rdata = ~rdata, wdata = ~wdata)
         {
            for(uint32_t index = 0; index < 4; index++)
            {
                if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)(addr - index), &temp, 1)))
                goto EEP_T7_ERR;

                if(temp != rdata)
                {
                    //S13EE_PRINTF("\r\n3. Read addr %d, data 0x%04x isn't 0x%04x.\r\n", (addr - index), temp, rdata);
                    S13EE_PRINTF("\r\n3. �� %d �Σ�����ַ %d ,�������� 0x%04x ,Ӧ��Ϊ 0x%04x.\r\n", count, (addr - index), temp, rdata);
                    return;
                }

                if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)(addr - index), &wdata, 1)))
                    goto EEP_T7_ERR;
            }
        }
        /* 4. ����ʼλ�ÿ�ʼ�����յ�ַ����˳�򣬶�ȫ����Ԫ�������ص����ݲ�һ�£��򷵻�ʧ�� */
        for(addr = 0, rdata = 0xAAAA; addr < S13EE_WORD_MAX; addr += 4, rdata = ~rdata)
        {
            for(uint32_t index = 0; index < 4; index++)
            {
                if(S13EE_SUCCESS != (ret = s13ee->read((uint8_t)(addr + index), &temp, 1)))
                goto EEP_T7_ERR;

                if(temp != rdata)
                {
                    //S13EE_PRINTF("\r\n4. Read addr %d, data 0x%04x isn't 0x%04x.\r\n", (addr + index), temp, rdata);
                    S13EE_PRINTF("\r\n4. �� %d �Σ�����ַ %d ,�������� 0x%04x ,Ӧ��Ϊ 0x%04x.\r\n", count, (addr + index), temp, rdata);
                    return;
                }
            }
         }
    }
#endif
    S13EE_PRINTF("\b\b\b\b%02d%%.\r\n", count);
    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;

EEP_T7_ERR:
    S13EE_PRINTF("EEP_T7 FAILED : %s.\r\n", s13ee->errToString(ret));
}

static void EEP_T8(S13EE *s13ee)
{
    uint8_t *buffer, ch;
    uint16_t row, col;
    uint16_t addr, rdata;
    static uint16_t wdata = 0x5a5a;
    S13EE_STATUS ret;

    //S13EE_PRINTF("%s - ALL WRITE 0x%04x, ADDRESS increase continuous.\r\n", __FUNCTION__, wdata);
    S13EE_PRINTF("Do you want to change (y/n): ");

    buffer = S13EE_GETLINE;
    ch = buffer[0];
    if((ch == 'y') || (ch == 'Y'))
    {
        S13EE_PRINTF("write data 0x%04x : 0x", wdata);
        buffer = S13EE_GETLINE;
        if(strlen((const char *)buffer) > 0)
            wdata = htoi(buffer);
        S13EE_PRINTF("write data 0x%04x.\r\n", wdata);
    }
    /* ����д���Աȶ�д���ݣ�д����Ϊ5A����ַ�仯���з������˳�� */
    for(col = 0; col < 4; col++)
    {
        for(row = 0; row < 64; row++)
        {
            addr = row * 4 + col;

            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
                goto EEP_T8_ERR;

            if(S13EE_SUCCESS!= (ret = s13ee->read((uint8_t)addr, &rdata, 1)))
                    goto EEP_T8_ERR;

            if(rdata != wdata)
            {
                S13EE_PRINTF("Read addr %d, data 0x%04x isn't 0x%04x.\r\n", addr, rdata, wdata);
                return;
            }
        }
    }

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T8_ERR:
    S13EE_PRINTF("EEP_T8 FAILED : %s.\r\n", s13ee->errToString(ret));
}

static void EEP_T9(S13EE *s13ee)
{
    uint8_t *buffer, ch;
    int16_t row, row0, col, col0;
    uint16_t addr, rdata;
    static uint16_t wdata = 0x5a5a;
    S13EE_STATUS ret;

    //S13EE_PRINTF("%s - ALL WRITE 0x%04x, ADDRESS increase continuous.\r\n", __FUNCTION__, wdata);
    /* ����д���Աȶ�д���ݣ�д����Ϊ5A����ַ�仯���з������˳�� */
    S13EE_PRINTF("Do you want to change (y/n): ");

    buffer = S13EE_GETLINE;
    ch = buffer[0];
    if((ch == 'y') || (ch == 'Y'))
    {
        S13EE_PRINTF("write data 0x%04x : 0x", wdata);
        buffer = S13EE_GETLINE;
        if(strlen((const char *)buffer) > 0)
            wdata = htoi(buffer);
        S13EE_PRINTF("write data 0x%04x.\r\n", wdata);
    }

    for(col = 3; col >= 0; col--)
    {
        for(col0 = col, row0 = 0; (col0 < 4) && (row0 < 4); row0++, col0++)
        {
            addr = row0 * 4 + col0;

            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
                goto EEP_T8_ERR;

            if(S13EE_SUCCESS!= (ret = s13ee->read((uint8_t)addr, &rdata, 1)))
                    goto EEP_T8_ERR;

            if(rdata != wdata)
            {
                S13EE_PRINTF("Read addr %d, data 0x%04x isn't 0x%04x.\r\n", addr, rdata, wdata);
                return;
            }
        }
    }

    for(row = 1; row < 64; row++)
    {
        for(col0 = 0, row0 = row; (col0 < 4) && (row0 < 64); row0++, col0++)
        {
            addr = row0 * 4 + col0;

            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
                goto EEP_T8_ERR;

            if(S13EE_SUCCESS!= (ret = s13ee->read((uint8_t)addr, &rdata, 1)))
                    goto EEP_T8_ERR;

            if(rdata != wdata)
            {
                S13EE_PRINTF("Read addr %d, data 0x%04x isn't 0x%04x.\r\n", addr, rdata, wdata);
                return;
            }
        }
    }

    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T8_ERR:
    S13EE_PRINTF("EEP_T8 FAILED : %s.\r\n", s13ee->errToString(ret));
}

static void EEP_T10(S13EE *s13ee)
{
    uint8_t *buffer, ch;
    int16_t row, row0, col, col0;
    uint16_t addr, rdata;
    static uint16_t wdata = 0x5a5a;
    S13EE_STATUS ret;

    //S13EE_PRINTF("%s - ALL WRITE 0x%04x, ADDRESS increase continuous.\r\n", __FUNCTION__, wdata);
    S13EE_PRINTF("Do you want to change (y/n): ");

    buffer = S13EE_GETLINE;
    ch = buffer[0];
    if((ch == 'y') || (ch == 'Y'))
    {
        S13EE_PRINTF("write data 0x%04x : 0x", wdata);
        buffer = S13EE_GETLINE;
        if(strlen((const char *)buffer) > 0)
            wdata = htoi(buffer);
        S13EE_PRINTF("write data 0x%04x.\r\n", wdata);
    }
    /* ����д���Աȶ�д���ݣ�д����Ϊ5A����ַ�仯���з������˳�� */
    for(col = 0; col < 4; col++)
    {
        for(col0 = col, row0 = 0; (col0 >= 0) && (row0 < 64); row0++, col0--)
        {
            addr = row0 * 4 + col0;

            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
                goto EEP_T8_ERR;

            if(S13EE_SUCCESS!= (ret = s13ee->read((uint8_t)addr, &rdata, 1)))
                    goto EEP_T8_ERR;

            if(rdata != wdata)
            {
                S13EE_PRINTF("Read addr %d, data 0x%04x isn't 0x%04x.\r\n", addr, rdata, wdata);
                return;
            }
        }
    }

    for(row = 1; row < 64; row++)
    {
        for(col0 = 3, row0 = row; (col0 >= 0) && (row0 < 64); row0++, col0--)
        {
            addr = row0 * 4 + col0;

            if(S13EE_SUCCESS != (ret = s13ee->write((uint8_t)addr, &wdata, 1)))
                goto EEP_T8_ERR;

            if(S13EE_SUCCESS!= (ret = s13ee->read((uint8_t)addr, &rdata, 1)))
                    goto EEP_T8_ERR;

            if(rdata != wdata)
            {
                S13EE_PRINTF("Read addr %d, data 0x%04x isn't 0x%04x.\r\n", addr, rdata, wdata);
                return;
            }
        }
    }
    S13EE_PRINTF("%s SUCCUSS.\r\n", __FUNCTION__);
    return;
EEP_T8_ERR:
    S13EE_PRINTF("EEP_T8 FAILED : %s.\r\n", s13ee->errToString(ret));
}

static void EEP_T11(S13EE *s13ee)
{
    S13EE_PRINTF("%s - VDD = 1.5V, CALL EEP_T3.\r\n", __FUNCTION__);

    EEP_T3(s13ee);
}

static void EEP_T12(S13EE *s13ee)
{
    S13EE_PRINTF("%s - VDD = 1.3V, CALL EEP_T3.\r\n", __FUNCTION__);

    EEP_T3(s13ee);
}

static void EEP_T13(S13EE *s13ee)
{
    S13EE_PRINTF("%s - VDD = 1.2V, CALL EEP_T3.\r\n", __FUNCTION__);

    EEP_T3(s13ee);
}

static void EEP_T14(S13EE *s13ee)
{
    S13EE_PRINTF("%s - VDD = 1.1V, CALL EEP_T3.\r\n", __FUNCTION__);

    EEP_T3(s13ee);
}

static void EEP_T15(S13EE *s13ee)
{
    S13EE_PRINTF("%s - VDD = 1.0V, CALL EEP_T3.\r\n", __FUNCTION__);

    EEP_T3(s13ee);
}

static void EEP_T16(S13EE *s13ee)
{
    uint16_t addr;
    static uint16_t writeBuffer[4] = {0x5A5A, 0xA5A5, 0x0000, 0xFFFF};
    uint16_t readBuffer[4];
    S13EE_STATUS ret;
    uint32_t option;
    uint16_t startAddr, endAddr;

    S13EE_PRINTF("%s - �����ܲ���.\r\n", __FUNCTION__);

    checknWordChange((uint16_t *) writeBuffer, 4);

    do
    {
        S13EE_PRINTF("\r\n\r\n��ѡ�����Ŀ���ַ��Χ:\r\n\r\n");
        S13EE_PRINTF("%2d. ��2k bit.\r\n", 1);
        S13EE_PRINTF("%2d. ��2k bit.\r\n", 2);
        S13EE_PRINTF("%2d. ȫƬ.\r\n", 3);
        S13EE_PRINTF("%2d. EEP_T%d - ȫƬ����У��\r\n", EEP_TEST32, EEP_TEST32);
        S13EE_PRINTF("%2d. �˳�\r\n", EEP_EXIT);

        option = getOption();

        switch(option)
        {
            case 1:
                if(S13EE_SUCCESS != (ret = s13ee->halfWrite((uint16_t (*)[4])writeBuffer, 0)))
                    goto EEP_T16_ERR;
                startAddr = 0;
                endAddr = S13EE_WORD_MAX / 2;
                break;
            case 2:
                if(S13EE_SUCCESS != (ret = s13ee->halfWrite((uint16_t (*)[4])writeBuffer, 1)))
                    goto EEP_T16_ERR;
                endAddr = S13EE_WORD_MAX;
                startAddr = S13EE_WORD_MAX / 2;
                break;
            case 3:
                if(S13EE_SUCCESS != (ret = s13ee->chipWrite((uint16_t (*)[4])writeBuffer)))
                    goto EEP_T16_ERR;
                startAddr = 0;
                endAddr = S13EE_WORD_MAX;
                break;
            case EEP_TEST32:
                EEP_T32(s13ee);
                break;
        }

        if((option != EEP_EXIT) && (option != EEP_TEST32))
        {
            for(addr =  startAddr; addr < endAddr; addr += 4)
            {
                if(S13EE_SUCCESS != (ret = s13ee->read(addr, readBuffer, 4)))
                    goto EEP_T16_ERR;
                if((readBuffer[0] != writeBuffer[0]) || (readBuffer[1] != writeBuffer[1]) ||
                   (readBuffer[2] != writeBuffer[2]) || (readBuffer[3] != writeBuffer[3]))
                {
                    S13EE_PRINTF("%s - Addr %d compare failed [0x%04x 0x%04x 0x%04x 0x%04x].\r\n",
                        __FUNCTION__, addr, readBuffer[0], readBuffer[1], readBuffer[2], readBuffer[3]);
                    break;
                }
            }
            if(addr >= endAddr)
                S13EE_PRINTF("\r\n ���Գɹ�\r\n");
            else
                S13EE_PRINTF("\r\n ����ʧ��\r\n");
        }
    }while(option != EEP_EXIT);

    return;
EEP_T16_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

static void EEP_T17(S13EE *s13ee)
{
    S13EE_PRINTF("%s - T = -20��, EEP_T11 ~ EEP_T16 TEST.\r\n", __FUNCTION__);

    EEP_T11(s13ee);
    EEP_T12(s13ee);
    EEP_T13(s13ee);
    EEP_T14(s13ee);
    EEP_T15(s13ee);
    EEP_T16(s13ee);
}

static void EEP_T18(S13EE *s13ee)
{
    S13EE_PRINTF("%s - T = 0��, EEP_T11 ~ EEP_T16 TEST.\r\n", __FUNCTION__);

    EEP_T11(s13ee);
    EEP_T12(s13ee);
    EEP_T13(s13ee);
    EEP_T14(s13ee);
    EEP_T15(s13ee);
    EEP_T16(s13ee);
}

static void EEP_T19(S13EE *s13ee)
{
    S13EE_PRINTF("%s - T = 25��, EEP_T11 ~ EEP_T16 TEST.\r\n", __FUNCTION__);

    EEP_T11(s13ee);
    EEP_T12(s13ee);
    EEP_T13(s13ee);
    EEP_T14(s13ee);
    EEP_T15(s13ee);
    EEP_T16(s13ee);
}

static void EEP_T20(S13EE *s13ee)
{
    S13EE_PRINTF("%s - T = 45��, EEP_T11 ~ EEP_T16 TEST.\r\n", __FUNCTION__);

    EEP_T11(s13ee);
    EEP_T12(s13ee);
    EEP_T13(s13ee);
    EEP_T14(s13ee);
    EEP_T15(s13ee);
    EEP_T16(s13ee);
}

static void EEP_T21(S13EE *s13ee)
{
    S13EE_PRINTF("%s - T = 85��, EEP_T11 ~ EEP_T16 TEST.\r\n", __FUNCTION__);

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

    S13EE_PRINTF("%s - T = 25��, vcca = 1.8V , Read opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

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

    S13EE_PRINTF("%s - T = 25��, vcca = 1.5V , Read opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

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

    S13EE_PRINTF("%s - T = 25��, vcca = 1.0V , Read opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

    do
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(0, readBuffer, S13EE_WORD_MAX)))
            goto EEP_T24_ERR;

        buffer = (char *)S13EE_GETLINE_NOWAIT;
    }while(!(buffer && (*((char *)buffer) == 'q')));

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

    S13EE_PRINTF("%s - T = 25��, vcca = 0.8/0.7V , Read opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

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

    S13EE_PRINTF("%s - T = 25��, vcca = 1.8V , Write opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

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

    S13EE_PRINTF("%s - T = 25��, vcca = 1.5V , Write opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

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

    S13EE_PRINTF("%s - T = 25��, vcca = 1.2V , Write opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

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

    S13EE_PRINTF("%s - T = 25��, vcca = 1.1V , Write opreate current test.(Input \'q\' to exit) : ", __FUNCTION__);

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

static void EEP_T30(S13EE *s13ee)
{
    S13EE_STATUS ret;
    static uint16_t writeBuffer[S13EE_WORD_MAX] = {0xa5a5, 0x5a5a, 0x0000, 0xffff};
    uint8_t startAddr;
    int16_t i;
    uint16_t readBuffer[S13EE_WORD_MAX];
    static int32_t n, m, index;

    S13EE_PRINTF("%s - ��addr0~addr(n-1)��дdata0-data(n-1)���ظ�m��\r\n", __FUNCTION__);

    startAddr = DIAG_GetNumber("��ʼ��ַ", INPUT_DATA_FORMAT_DEC, 0, 255);
    n = DIAG_GetNumber("n��ֵ", INPUT_DATA_FORMAT_DEC, 0, S13EE_WORD_MAX - startAddr);
    if(n == 0)
        goto EEP_T30_SUCC;
    checknWordChange(writeBuffer, n);
    m = DIAG_GetNumber("m��ֵ", INPUT_DATA_FORMAT_DEC, 0, 0);
    if(m == 0)
        goto EEP_T30_SUCC;

    S13EE_PRINTF("������ %02d%%.", 0);
    for(i = 0 ;i < m; i++)
    {
        S13EE_PRINTF("\b\b\b\b%02d%%.", i * 100 / m);
#if 0
        if(S13EE_SUCCESS != (ret = s13ee->write(startAddr, writeBuffer, n)))
            goto EEP_T30_ERR;
        if(S13EE_SUCCESS != (ret = s13ee->read(startAddr, readBuffer, n)))
            goto EEP_T30_ERR;
#endif
        for(index = 0; index < n; index++)
        {
            if(S13EE_SUCCESS != (ret = s13ee->write(startAddr + index, &writeBuffer[index], 1)))
                goto EEP_T30_ERR;
        }
        for(index = 0; index < n; index++)
        {
            if(S13EE_SUCCESS != (ret = s13ee->read(startAddr + index, &readBuffer[index], 1)))
                goto EEP_T30_ERR;
        }

        for(index = 0; index < n; index++)
        {
            if(writeBuffer[index] != readBuffer[index])
            {
                S13EE_PRINTF("\r\n�� %d �飬����ַ %d ,�������� 0x%04x, Ӧ��Ϊ 0x%04x.\r\n", \
                    i, startAddr + index, readBuffer[index], writeBuffer[index]);
                goto EEP_T30_ERR;
            }
        }
    }
    S13EE_PRINTF("\b\b\b\b%02d%%.\r\n\r\n", i * 100 / m);

EEP_T30_SUCC:
    S13EE_PRINTF("%s SUCC .\r\n", __FUNCTION__);
    return;
EEP_T30_ERR:
    S13EE_PRINTF("%s FAILED : %s.\r\n", __FUNCTION__, s13ee->errToString(ret));
}

void EEP_T31(S13EE *s13ee)
{
    uint16_t index, addr = 0;
    static uint16_t writeBuffer[8] = {0x5A5A, 0xA5A5, 0x0000, 0xFFFF,
        0x5A5A, 0xA5A5, 0x0000, 0xFFFF};
    uint16_t readBuffer[8];
    uint8_t *buffer;
    S13EE_STATUS ret;
    char ch;

    S13EE_PRINTF("\r\n���̸����\r\n");
    S13EE_PRINTF("writeBuffer [0x%04x 0x%04x 0x%04x 0x%04x",
        writeBuffer[0], writeBuffer[1], writeBuffer[2], writeBuffer[3]);
    S13EE_PRINTF(" 0x%04x 0x%04x 0x%04x 0x%04x]\r\n",
        writeBuffer[4], writeBuffer[5], writeBuffer[6], writeBuffer[7]);
    S13EE_PRINTF("Do you want to change (y/n): ");

    buffer = S13EE_GETLINE;
    ch = buffer[0];
    if((ch == 'y') || (ch == 'Y'))
    {
        for(index = 0; index < 8; index++)
        {
            S13EE_PRINTF("writeBuffer[%d] 0x%04x : 0x", index, writeBuffer[index]);
            buffer = S13EE_GETLINE;
            if(strlen((const char *)buffer) > 0)
                writeBuffer[index] = htoi(buffer);
        }
        S13EE_PRINTF("writeBuffer [0x%04x 0x%04x 0x%04x 0x%04x",
        writeBuffer[0], writeBuffer[1], writeBuffer[2], writeBuffer[3]);
        S13EE_PRINTF(" 0x%04x 0x%04x 0x%04x 0x%04x]\r\n",
            writeBuffer[4], writeBuffer[5], writeBuffer[6], writeBuffer[7]);
    }

    for(addr = 0; addr < S13EE_WORD_MAX; addr += 8)
    {
        if(S13EE_SUCCESS != (ret = s13ee->write(addr, &writeBuffer[0], 8)))
            goto EEP_T31_err;
        if(S13EE_SUCCESS != (ret = s13ee->read(addr, &readBuffer[0], 8)))
            goto EEP_T31_err;

        for(index = 0; index < 8; index++)
        {
            if(readBuffer[index] != writeBuffer[index])
            {
                S13EE_PRINTF("\r\n\r\n%s : read addr[%d] = 0x%04x, not match the value 0x%04x\r\n",
                    __FUNCTION__, addr + index, readBuffer[index], writeBuffer[index]);
                return;
            }
        }
    }

    S13EE_PRINTF("\r\n%s SUCCUSS.\n", __FUNCTION__);
    return;

EEP_T31_err:
    S13EE_PRINTF("\r\n\r\nEEP_T30 FAILED : %s.\r\n\r\n", s13ee->errToString(ret));
}

static void EEP_T32(S13EE *s13ee)
{
    uint16_t index, addr = 0;
    static uint16_t resourceBuffer[8] = {0x5A5A, 0xA5A5, 0x0000, 0xFFFF,
        0x5A5A, 0xA5A5, 0x0000, 0xFFFF};
    static uint32_t n = 8;
    uint16_t readBuffer[8];
    S13EE_STATUS ret;

    S13EE_PRINTF("\r\n����У�飬����n���������ݣ�n <= 8��,���ζ�n�����ݣ�������n��У������Դ���жԱȣ������ʾ��������\r\n");
    checknWordChangeExp("resourceBuffer", resourceBuffer, &n);

    for(addr = 0; (addr < S13EE_WORD_MAX) && (n != 0); addr += n)
    {
        if(S13EE_SUCCESS != (ret = s13ee->read(addr, &readBuffer[0], n)))
            goto EEP_T32_err;

        for(index = 0; index < n; index++)
        {
            if(((addr + index) % 4) == 0)
                S13EE_PRINTF("\r\n");

            if(readBuffer[index] != resourceBuffer[index])
            {
                S13EE_PRINTF("��%04x��", readBuffer[index]);
            }
            else
            {
                S13EE_PRINTF("  %04x  ", readBuffer[index]);
            }
        }
    }

    S13EE_PRINTF("\r\n\n");
    return;

EEP_T32_err:
    S13EE_PRINTF("\r\n\r\nEEP_T32_err FAILED : %s.\r\n\r\n", s13ee->errToString(ret));
}

static void EEP_T33(S13EE *s13ee)
{
    S13EE_STATUS ret;
    static uint16_t writeBuffer[S13EE_WORD_MAX] = {0xa5a5, 0x5a5a, 0x0000, 0xffff};
    uint8_t startAddr;
    int16_t i;
    uint16_t readBuffer[S13EE_WORD_MAX];
    static int32_t n, m, index;

    S13EE_PRINTF("%s - ��addr0~addr(n-1)��дdata0-data(n-1)���ظ�m��(дһ�Σ���һ��)\r\n", __FUNCTION__);

    startAddr = DIAG_GetNumber("��ʼ��ַ", INPUT_DATA_FORMAT_DEC, 0, 255);
    n = DIAG_GetNumber("n��ֵ", INPUT_DATA_FORMAT_DEC, 0, S13EE_WORD_MAX - startAddr);
    if(n == 0)
        goto EEP_T30_SUCC;
    checknWordChange(writeBuffer, n);
    m = DIAG_GetNumber("m��ֵ", INPUT_DATA_FORMAT_DEC, 0, 0);
    if(m == 0)
        goto EEP_T30_SUCC;

    S13EE_PRINTF("������ %02d%%.", 0);
    for(i = 0 ;i < m; i++)
    {
        S13EE_PRINTF("\b\b\b\b%02d%%.", i * 100 / m);
#if 0
        if(S13EE_SUCCESS != (ret = s13ee->write(startAddr, writeBuffer, n)))
            goto EEP_T30_ERR;
        if(S13EE_SUCCESS != (ret = s13ee->read(startAddr, readBuffer, n)))
            goto EEP_T30_ERR;
#endif
        for(index = 0; index < n; index++)
        {
            if(S13EE_SUCCESS != (ret = s13ee->write(startAddr + index, &writeBuffer[index], 1)))
                goto EEP_T30_ERR;
            if(S13EE_SUCCESS != (ret = s13ee->read(startAddr + index, &readBuffer[index], 1)))
                goto EEP_T30_ERR;
            if(writeBuffer[index] != readBuffer[index])
            {
                S13EE_PRINTF("\r\n�� %d �飬����ַ %d ,�������� 0x%04x, Ӧ��Ϊ 0x%04x.\r\n", \
                    i, startAddr + index, readBuffer[index], writeBuffer[index]);
                return;
            }
        }
    }
    S13EE_PRINTF("\b\b\b\b%02d%%.\r\n\r\n", i * 100 / m);

EEP_T30_SUCC:
    S13EE_PRINTF("%s SUCC .\r\n", __FUNCTION__);
    return;
EEP_T30_ERR:
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
    RELIABILITY_TEST,
};



static uint32_t getOption(void)
{
    uint32_t option;
    uint8_t * buffer;

    S13EE_PRINTF("\r\n������ѡ����:");
    buffer = S13EE_GETLINE;
    option = atoi((const char*)buffer);

    return option;
}


static uint32_t DIAG_GetNumber(char * name, data_input_format_e format, uint32_t min, uint32_t max)
{
    uint32_t n = 0x7FFFFFFF;
    uint8_t *sInput;

    do
    {
        if(min < max)
            S13EE_PRINTF("������ %s (%d - %d) : %s", name, min, max, (INPUT_DATA_FORMAT_DEC == format) ? "" : "0x");
        else
            S13EE_PRINTF("������ %s : %s", name, (INPUT_DATA_FORMAT_DEC == format) ? "" : "0x");

        sInput = S13EE_GETLINE;

        if(INPUT_DATA_FORMAT_DEC == format)
            n = atoi((const char *)sInput);
//            iRet = sscanf(sInput, "%d", &n);
        else if(INPUT_DATA_FORMAT_HEX == format)
            n = htoi(sInput);
            //iRet = sscanf(sInput, "%x", &n);

        if((min < max) && ((n > max) || (n < min)))
            continue;

        break;
    }while(1);

    return n;
}


static void checknWordChange(uint16_t *writeBuffer, uint32_t n)
{
    uint8_t ch, *buffer;
    uint32_t index;

    if(n == 0)
        return;

    S13EE_PRINTF("\r\nwriteBuffer [ ");
    for(index = 0; index < n; index++)
        S13EE_PRINTF("%04x ", writeBuffer[index]);
    S13EE_PRINTF("]\r\n");

    S13EE_PRINTF("Do you want to change (y/n): ");

    buffer = S13EE_GETLINE;
    ch = buffer[0];
    if((ch == 'y') || (ch == 'Y'))
    {
        for(index = 0; index < n; index++)
        {
            S13EE_PRINTF("writeBuffer[%d] 0x%04x : 0x", index, writeBuffer[index]);
            buffer = S13EE_GETLINE;
            if(strlen((const char *)buffer) > 0)
                writeBuffer[index] = htoi(buffer);
        }
    }
    S13EE_PRINTF("\r\nwriteBuffer [ ");
    for(index = 0; index < n; index++)
        S13EE_PRINTF("%04x ", writeBuffer[index]);
    S13EE_PRINTF("]\r\n");
}

static void checknWordChangeExp(char *name, uint16_t *resBuffer, uint32_t *n)
{
    uint8_t ch, *buffer;
    uint32_t index;

    S13EE_PRINTF("\r\n%s [ ", name);
    for(index = 0; index < *n; index++)
        S13EE_PRINTF("%04x ", resBuffer[index]);
    S13EE_PRINTF("]\r\n");

    S13EE_PRINTF("Do you want to change (y/n): ");

    buffer = S13EE_GETLINE;
    ch = buffer[0];
    if((ch == 'y') || (ch == 'Y'))
    {
        *n = DIAG_GetNumber("n��ֵ", INPUT_DATA_FORMAT_DEC, 0, 8);
        for(index = 0; index < *n; index++)
        {
            S13EE_PRINTF("%s[%d] 0x%04x : 0x", name, index, resBuffer[index]);
            buffer = S13EE_GETLINE;
            if(strlen((const char *)buffer) > 0)
                resBuffer[index] = htoi(buffer);
        }
    }
    S13EE_PRINTF("\r\n%s [ ", name);
    for(index = 0; index < *n; index++)
        S13EE_PRINTF("%04x ", resBuffer[index]);
    S13EE_PRINTF("]\r\n");
}


static void basicFunctionTest(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("\r\n");
        S13EE_PRINTF("%2d. EEP_T%d - ��EEPROMͨѶ����������ȷ��ָ��\r\n", EEP_TEST1, EEP_TEST1);
        S13EE_PRINTF("%2d. EEP_T%d - �����������2��6���Ի���1.5v\r\n", EEP_TEST2, EEP_TEST2);
        S13EE_PRINTF("%2d. EEP_T%d - д����\r\n", EEP_TEST3, EEP_TEST3);
        S13EE_PRINTF("%2d. EEP_T%d - ��������\r\n", EEP_TEST4, EEP_TEST4);
        S13EE_PRINTF("%2d. EEP_T%d - ��addr0~addr(n-1)��дdata0-data(n-1)���ظ�m��\r\n", EEP_TEST30, EEP_TEST30);
        S13EE_PRINTF("%2d. EEP_T%d - ���̸����\r\n", EEP_TEST31, EEP_TEST31);
        S13EE_PRINTF("%2d. EEP_T%d - ȫƬ����У��\r\n", EEP_TEST32, EEP_TEST32);
        S13EE_PRINTF("%2d. EEP_T%d - ��addr0~addr(n-1)��дdata0-data(n-1)���ظ�m��(дһ�Σ���һ��)\r\n", EEP_TEST33, EEP_TEST33);
        S13EE_PRINTF("%2d. �˳�\r\n", EEP_EXIT);

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
            case EEP_TEST30:
                EEP_T30(s13ee);
                break;
            case EEP_TEST31:
                EEP_T31(s13ee);
                break;
            case EEP_TEST32:
                EEP_T32(s13ee);
                break;
            case EEP_TEST33:
                EEP_T33(s13ee);
                break;
        }
    }while(option != EEP_EXIT);
}

static void eraseReadWriteTest(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("%2d. EEP_T%d - ����������д200�Σ��Աȶ�д���ݣ�д����Ϊ00����ַ����(ȫ0����)\r\n", EEP_TEST5, EEP_TEST5);
        S13EE_PRINTF("%2d. EEP_T%d - ����������д200�Σ��Աȶ�д���ݣ�д����ΪFF����ַ����(ȫ1����)\r\n", EEP_TEST6, EEP_TEST6);
        S13EE_PRINTF("%2d. EEP_T%d - ����100��march�㷨����\r\n", EEP_TEST7, EEP_TEST7);
        S13EE_PRINTF("%2d. EEP_T%d - ����д���Աȶ�д���ݣ�д����Ϊ5A����ַ�仯���з������˳��\r\n", EEP_TEST8, EEP_TEST8);
        S13EE_PRINTF("%2d. EEP_T%d - ����д���Աȶ�д���ݣ�д����Ϊ5A����ַ�仯������б��\r\n", EEP_TEST9, EEP_TEST9);
        S13EE_PRINTF("%2d. EEP_T%d - ����д���Աȶ�д���ݣ�д����Ϊ5A����ַ�仯������б��\r\n", EEP_TEST10, EEP_TEST10);
        S13EE_PRINTF("%2d. EEP_T%d - ȫƬ����У��\r\n", EEP_TEST32, EEP_TEST32);
        S13EE_PRINTF("%2d. �˳�\r\n", EEP_EXIT);

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
            case EEP_TEST32:
                EEP_T32(s13ee);
                break;
        }
    }while(option != EEP_EXIT);
}

static void performanceTest(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("%2d. EEP_T%d - ��ѹ��1.5vʱ������EEP_T3���Գ���\r\n", EEP_TEST11, EEP_TEST11);
        S13EE_PRINTF("%2d. EEP_T%d - ��ѹ��1.3vʱ������EEP_T3���Գ���\r\n", EEP_TEST12, EEP_TEST12);
        S13EE_PRINTF("%2d. EEP_T%d - ��ѹ��1.2vʱ������EEP_T3���Գ���\r\n", EEP_TEST13, EEP_TEST13);
        S13EE_PRINTF("%2d. EEP_T%d - ��ѹ��1.1vʱ������EEP_T3���Գ���\r\n", EEP_TEST14, EEP_TEST14);
        S13EE_PRINTF("%2d. EEP_T%d - ��ѹ��1.0vʱ������EEP_T3���Գ���\r\n", EEP_TEST15, EEP_TEST15);
        S13EE_PRINTF("%2d. EEP_T%d - ȫƬ����У��\r\n", EEP_TEST32, EEP_TEST32);
        S13EE_PRINTF("%2d. �˳�\r\n", EEP_EXIT);

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
            case EEP_TEST32:
                EEP_T32(s13ee);
                break;
        }
    }while(option != EEP_EXIT);
}

static void temperatureTest(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("%2d. EEP_T%d - T=-20�棬����EEP_T11~EEP_T16���Գ��򣬶�д���ܲ���\r\n", EEP_TEST17, EEP_TEST17);
        S13EE_PRINTF("%2d. EEP_T%d - T=0�棬����EEP_T11~EEP_T16���Գ��򣬶�д���ܲ���\r\n", EEP_TEST18, EEP_TEST18);
        S13EE_PRINTF("%2d. EEP_T%d - T=25�棬����EEP_T11~EEP_T16���Գ��򣬶�д���ܲ���\r\n", EEP_TEST19, EEP_TEST19);
        S13EE_PRINTF("%2d. EEP_T%d - T=45�棬����EEP_T11~EEP_T16���Գ��򣬶�д���ܲ���\r\n", EEP_TEST20, EEP_TEST20);
        S13EE_PRINTF("%2d. EEP_T%d - T=85�棬����EEP_T11~EEP_T16���Գ��򣬶�д���ܲ���\r\n", EEP_TEST21, EEP_TEST21);
        S13EE_PRINTF("%2d. EEP_T%d - ȫƬ����У��\r\n", EEP_TEST32, EEP_TEST32);
        S13EE_PRINTF("%2d. �˳�\r\n", EEP_EXIT);

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
            case EEP_TEST32:
                EEP_T32(s13ee);
                break;
        }
    }while(option != EEP_EXIT);
}

static void powerConsumptionTest(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("%2d. EEP_T%d - T=25�棬vcca=1.8vʱ��EEPROM������ʱ����(����vcca!�˿�)\r\n", EEP_TEST22, EEP_TEST22);
        S13EE_PRINTF("%2d. EEP_T%d - T=25�棬vcca=1.5vʱ��EEPROM������ʱ����(����vcca!�˿�)\r\n", EEP_TEST23, EEP_TEST23);
        S13EE_PRINTF("%2d. EEP_T%d - T=25�棬vcca=1.0vʱ��EEPROM������ʱ����(����vcca!�˿�)\r\n", EEP_TEST24, EEP_TEST24);
        S13EE_PRINTF("%2d. EEP_T%d - T=25�棬vcca=0.8/0.7vʱ��EEPROM������ʱ����(����vcca!�˿�)\r\n", EEP_TEST25, EEP_TEST25);
        S13EE_PRINTF("%2d. EEP_T%d - T=25�棬vcca=1.8vʱ��EEPROMд����ʱ����(����vcca!�˿�)\r\n", EEP_TEST26, EEP_TEST26);
        S13EE_PRINTF("%2d. EEP_T%d - T=25�棬vcca=1.5vʱ��EEPROMд����ʱ����(����vcca!�˿�)\r\n", EEP_TEST27, EEP_TEST27);
        S13EE_PRINTF("%2d. EEP_T%d - T=25�棬vcca=1.2vʱ��EEPROMд����ʱ����(����vcca!�˿�)\r\n", EEP_TEST28, EEP_TEST28);
        S13EE_PRINTF("%2d. EEP_T%d - T=25�棬vcca=1.1vʱ��EEPROMд����ʱ����(����vcca!�˿�)\r\n", EEP_TEST29, EEP_TEST29);
        S13EE_PRINTF("%2d. EEP_T%d - ȫƬ����У��\r\n", EEP_TEST32, EEP_TEST32);
        S13EE_PRINTF("%2d. �˳�\r\n", EEP_EXIT);

        option = getOption();

        switch(option)
        {
            case EEP_TEST22:
                EEP_T22(s13ee);
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
            case EEP_TEST32:
                EEP_T32(s13ee);
                break;
        }
    }while(option != EEP_EXIT);
}

void EEP_T_MAIN(S13EE *s13ee)
{
    uint32_t option;

    do
    {
        S13EE_PRINTF("\r\n\r\nS13EE_X64Y64 ����Ŀ¼:\r\n\r\n");
        S13EE_PRINTF("%2d. ��ʼ���ͻ��Ѳ���.\r\n", INITIALIZE_AND_WAKEUP_TEST);
        S13EE_PRINTF("%2d. �������ܲ���.\r\n", BASIC_FUNCTION_TEST);
        S13EE_PRINTF("%2d. ����/��д���ܲ���.\r\n", ERASE_READ_WRITE_TEST);
        S13EE_PRINTF("%2d. ���ܲ���.\r\n", PERFORMANCE_TEST);
        S13EE_PRINTF("%2d. ALLROW����.\r\n", ALLROW_TEST);
        S13EE_PRINTF("%2d. �ߵ��²���.\r\n", TEMPERATURE_TEST);
        S13EE_PRINTF("%2d. ���Ĳ���.\r\n", POWER_COMSUMPTION_TEST);
        S13EE_PRINTF("%2d. ʱ�����.\r\n", TIMING_TEST);
        S13EE_PRINTF("%2d. �ɿ��Բ���.\r\n", RELIABILITY_TEST);
        S13EE_PRINTF("%2d. EEP_T%d - ȫƬ����У��\r\n", EEP_TEST32, EEP_TEST32);

        //S13EE_PRINTF("%2d. ��չ����.\r\n", EXPAND_TEST);

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
                delayManage();
                break;
            case RELIABILITY_TEST:
                break;
            case EEP_TEST32:
                EEP_T32(s13ee);
                break;
        }
    }while(1);
}

