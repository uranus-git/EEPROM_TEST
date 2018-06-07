#include "S13EE_X64Y64.h"
#include <string.h>
#include <math.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include <main.h>
#include <stdio.h>

//#define DEVELOP_BOARD
//#ifndef ENABLE
//#define ENABLE 1
//#endif
//#ifndef DISABLE
//#define DISABLE 0
//#endif
/*
 * HADDR - addr[13:12] - PG[ 9:8]  - GPIOG_ODR 0x40020814
 * LADDR - addr[11: 4] - PG[ 7:0]
 * ODATA - dout[15: 0] - PE[15:0]  - GPIOE_ODR 0x40021010
 * IDATA - din [15: 0] - PF[15:0]  - GPIOF_IDR 0x40021414
 * READ  - read        - PD[9]     - bitband addr 0x42418280 +  9 * 4 = 0x424182A4
 * PGM   - pgm         - PD[10]    - bitband addr 0x42418280 + 10 * 4 = 0x424182A8
 * ERASE - erase       - PD[14]    - bitband addr 0x42418280 + 14 * 4 = 0x424182B8
 * TESTMR- margin_rd   - PD[12]    - bitband addr 0x42418280 + 12 * 4 = 0x424182B0
 * SYNC  - sync        - PA[6]     - bitband addr 0x42400280 +  6 * 4 = 0x42400298
 * CLK   - clk         - PA[7]     - bitband addr 0x42400280 +  6 * 4 = 0x4240029C
 * BUFEST- burfst      - PD[13]    - bitband addr 0x42418280 + 13 * 4 = 0x424182B4
 * LOAD  - loaden      - PD[15]    - bitband addr 0x42418280 + 13 * 4 = 0x424182BC
 * DBY2  - dby2        - PD[11]    - bitband addr 0x42418280 + 13 * 4 = 0x424182AC
 */
#define NOP  (__nop)
#ifndef DEVELOP_BOARD
#define HADDR_BUS(val)            *((__IO uint32_t  *)(0x40021814)) = ((*((__IO uint32_t  *)(0x40021814))) & 0xff ) | ((val & 0x03) << 8)
#define LADDR_BUS(val)            *((__IO uint32_t  *)(0x40021814)) = ((*((__IO uint32_t  *)(0x40021814))) & 0x300) | (val & 0xff)
#define ODATA_BUS_READ()          *((__IO uint32_t  *)(0x40021010))
#define IDATA_BUS(val)            *((__IO uint32_t  *)(0x40021414)) = (val)
#define SIGNAL_READ(val)          *((__IO uint32_t  *)(0x424182A4)) = (val)
#define SIGNAL_PGM(val)           *((__IO uint32_t  *)(0x424182A8)) = (val)
#define SIGNAL_ERASE(val)         *((__IO uint32_t  *)(0x424182B8)) = (val)
#define SIGNAL_TESTMR(val)        *((__IO uint32_t  *)(0x424182B0)) = (val)
#define SIGNAL_SYNC(val)          *((__IO uint32_t  *)(0x42400298)) = (val)
#define SIGNAL_CLK(val)           TIM_Cmd(TIM14, val)
#define SIGNAL_BUFRST(val)        *((__IO uint32_t  *)(0x424182B4)) = (val)
#define SIGNAL_LOAD(val)          *((__IO uint32_t  *)(0x424182BC)) = (val)
#define SIGNAL_DBY2(val)          *((__IO uint32_t  *)(0x424182AC)) = (val)
#else
/* HADDR - addr[13:12] - PC[ 9:8]  - GPIOC_ODR 0x40021814
 * LADDR - addr[11: 4] - PC[ 7:0]
 * ODATA - dout[15: 0] - PD[15:0]  - GPIOD_ODR 0x40020C10
 * IDATA - din [15: 0] - PG[15:0]  - GPIOG_IDR 0x40021814
 * READ  - read        - PF[0]     - bitband addr 0x42428280 +  0 * 4 = 0x42428280
 * PGM   - pgm         - PF[1]     - bitband addr 0x42428280 +  1 * 4 = 0x42428284
 * ERASE - erase       - PF[2]     - bitband addr 0x42428280 +  2 * 4 = 0x42428288
 * TESTMR- margin_rd   -
 * SYNC  - sync        - PF[3]     - bitband addr 0x42428280 +  3 * 4 = 0x4242828C
 * CLK   - clk         - PF[9]     - bitband addr 0x42428280 +  9 * 4 = 0x424282A4
 * BUFEST- burfst      - PF[4]     - bitband addr 0x42428280 +  4 * 4 = 0x42428290
 * LOAD  - loaden      - PF[5]     - bitband addr 0x42428280 +  5 * 4 = 0x42428294
 * DBY2  - dby2        -
 */
#define HADDR_BUS(val)            *((__IO uint32_t  *)(0x40020814)) = ((*((__IO uint32_t  *)(0x40020814))) & 0xff ) | ((val & 0x03) << 8)
#define LADDR_BUS(val)            *((__IO uint32_t  *)(0x40020814)) = ((*((__IO uint32_t  *)(0x40020814))) & 0x300) | (val & 0xff)
#define ODATA_BUS_READ()          *((__IO uint32_t  *)(0x40020C10))
#define IDATA_BUS(val)            *((__IO uint32_t  *)(0x40021814)) = (val)
#define SIGNAL_READ(val)          *((__IO uint32_t  *)(0x42428280)) = (val)
#define SIGNAL_PGM(val)           *((__IO uint32_t  *)(0x42428284)) = (val)
#define SIGNAL_ERASE(val)         *((__IO uint32_t  *)(0x42428288)) = (val)
#define SIGNAL_TESTMR(val)
#define SIGNAL_SYNC(val)          *((__IO uint32_t  *)(0x4242828C)) = (val)
#define SIGNAL_CLK(val)           TIM_Cmd(TIM14, val)
#define SIGNAL_BUFRST(val)        *((__IO uint32_t  *)(0x42428290)) = (val)
#define SIGNAL_LOAD(val)          *((__IO uint32_t  *)(0x42428294)) = (val)
#define SIGNAL_DBY2(val)
#endif /* DEVELOP_BOARD */

/*
 * addr<13:4>   I   Address input bus¡ª¡ªaddr<13:12> for all words-select ;addr<11:6> for X-dec; addr<5:4> for Y-dec
 * din<15:0>    I   Parallel data inputs for write operation
 * dout<15:0>   O   Memory bits output
 * read         I   Read enable signal
 * pgm          I   Program enable signal
 * erase        I   Erase enable signal
 * testmr       I   Test mode¡ªMargin Read
 * sync         I   Synchronous signal for data loading and read operation
 * clk          I   Clock signal for driving high voltage charge pump
 * bufrst       I   Pulse signal to clear the data input buffers in data loading
 * loaden       I   Enable signal for write data loading
 * dby2         I   Clk frequency will be divided by 2 when dby2=¡®1¡¯
 * vdd          Power   0.8~1.65V power supply for memory operation
 * gnd          Power   Power ground
 */

typedef struct
{
    uint8_t haddr;
    uint8_t laddr;
    uint16_t din;
    uint8_t read;
    uint8_t pgm;
    uint8_t erase;
    uint8_t testmr;
    uint8_t sync;
    uint8_t clk;
    uint8_t bufrst;
    uint8_t loaden;
    uint8_t dby2;
}S13EE_OPIN_VALUE_LIST;

typedef struct
{
    char *name;
    int32_t nsDelay;
    int32_t parameter;
    void (*delayFunc)(int32_t);
}S13EE_DELAY;

/*
typedef struct
{
    uint8_t *name;
    S13EE_DELAY *
}S13EE_DELAY_INFO;
*/
static void nsDelay(uint32_t nsDelay);
static void nsDelay_141_24(int32_t nsDelay);

/* Buffer Reset and Data Loading Timing */
static S13EE_DELAY tw_b     = {"tw_b", 		200, 200, nsDelay_141_24};
static S13EE_DELAY tsu_bx   = {"tsu_bx", 	200, 200, nsDelay_141_24};
static S13EE_DELAY tdsu_xs  = {"tdsu_xs", 	200, 200, nsDelay_141_24};
static S13EE_DELAY tdh_xs   = {"tdh_xs", 	200, 200, nsDelay_141_24};
static S13EE_DELAY tdsu_as  = {"tdsu_as", 	200, 200, nsDelay_141_24};
static S13EE_DELAY tdh_as   = {"tdh_as", 	200, 200, nsDelay_141_24};
static S13EE_DELAY tdsu_ds  = {"tdsu_ds", 	200, 200, nsDelay_141_24};
static S13EE_DELAY tdh_ds   = {"tdh_ds", 	200, 200, nsDelay_141_24};
static S13EE_DELAY tdw_s_h  = {"tdw_s_h", 	200, 200, nsDelay_141_24};
static S13EE_DELAY tdw_s_l  = {"tdw_s_l", 	200, 200, nsDelay_141_24};
static S13EE_DELAY tdcyc_s  = {"tdcyc_s", 	400, 400, nsDelay_141_24};
/* Write-Erase and Program-Timing */
static S13EE_DELAY tsu_ae   = {"tsu_ae", 	200, 200, nsDelay_141_24};
static S13EE_DELAY tsu_ew   = {"tsu_ew", 	200000, 200000, nsDelay_141_24};
static S13EE_DELAY th_aw    = {"th_aw", 	100000, 100000, nsDelay_141_24};
static S13EE_DELAY tw_e     = {"tw_e", 		2400000, 2400000, nsDelay_141_24};
static S13EE_DELAY tw_w     = {"tw_w", 		2400000, 2400000, nsDelay_141_24};
static S13EE_DELAY tw_c_l   = {"tw_c_l", 	260, 260, nsDelay_141_24};
static S13EE_DELAY tw_c_h   = {"tw_c_h", 	260, 260, nsDelay_141_24};
static S13EE_DELAY tcyc_c   = {"tcyc_c", 	520, 520, nsDelay_141_24};
static S13EE_DELAY tsu_c    = {"tsu_c", 	200, 200, nsDelay_141_24};
static S13EE_DELAY th_c     = {"th_c", 		200, 200, nsDelay_141_24};
/* Read Timing */
static S13EE_DELAY tsu_wr   = {"tsu_wr", 	200000, 200000, nsDelay_141_24};
static S13EE_DELAY tsu_rs   = {"tsu_rs", 	200, 200, nsDelay_141_24};
static S13EE_DELAY th_rs    = {"th_rs", 	200, 200, nsDelay_141_24};
static S13EE_DELAY trw_s_h  = {"trw_s_h", 	750, 750, nsDelay_141_24};
static S13EE_DELAY trw_s_l  = {"trw_s_l", 	750, 750, nsDelay_141_24};
static S13EE_DELAY trcyc_s  = {"trcyc_s", 	1500, 1500, nsDelay_141_24};
static S13EE_DELAY tsac     = {"tsac", 		150, 150, nsDelay_141_24};
static S13EE_DELAY trsu_as  = {"trsu_as", 	200, 200, nsDelay_141_24};
static S13EE_DELAY trh_as   = {"trh_as", 	200, 200, nsDelay_141_24};

static const char *_errToString(S13EE_STATUS result)
{
    switch(result)
    {
        case S13EE_SUCCESS:
            return "SUCCESS";
        case S13EE_PRAM_ERR:
            return "INPUT PARAM ERR";
        default:
            return "UNKOWN ERR";
    }
}

static const S13EE_DELAY* s13eeDelayArray[] =
{
    /* Buffer Reset and Data Loading Timing */
    &tw_b,	&tsu_bx,&tdsu_xs,	&tdh_xs,	&tdsu_as,	&tdh_as,	&tdsu_ds,	&tdh_ds,	&tdw_s_h,	&tdw_s_l,	&tdcyc_s,
    /* Write-Erase and Program-Timing */
    &tsu_ae,&tsu_ew,&th_aw,		&tw_e,		&tw_w,		&tw_c_l,	&tw_c_h,	&tcyc_c,	&tsu_c,		&th_c,
    /* Read Timing */
    &tsu_wr,&tsu_rs,&th_rs,		&trw_s_h,	&trw_s_l,	&trcyc_s,	&tsac,		&trsu_as,	&trh_as
};


static void nsDelay(uint32_t nsDelay)
{
    while(nsDelay--);
}

static void nsDelay_60_37(uint32_t nsDelay)
{
    while(nsDelay--);
}

static void nsDelay_141_24(int32_t nsDelay)
{
    nsDelay -= 141;

    while(nsDelay > 0)
        nsDelay -= 24;
}

static void miniDelay(uint32_t nsDelay)
{

}

static S13EE_DELAY test_time = {"test_time", 165, 165, nsDelay_141_24};

static void testDelay(void)
{
    #define TEST_COUNT  700
    GPIO_InitTypeDef  GPIO_InitStructure;
	/* HADDR & LADDR */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

    while(1)
    {
        SIGNAL_BUFRST(1);
        test_time.delayFunc(test_time.parameter);
        SIGNAL_BUFRST(0);
        test_time.delayFunc(test_time.parameter);
    }
}
#if 0
void delayManage(void)
{
    uint32_t index = 0;
    uint32_t size = sizeof(s13eeDelayArray) / sizeof(s13eeDelayArray[0]);
    uint8_t buffer[128];
    uint32_t len;
    uint32_t nsDelayCount;

    while(index < size)
    {
        printf("%s - %8d(ns) :", s13eeDelayArray[index]->name, s13eeDelayArray[index]->nsDelay);
        memset(buffer, 0, sizeof(buffer));
        len = scanf("%s",buffer);
        if(len > 2)
        {
            nsDelayCount = atoi(buffer);
            s13eeDelayArray[index]->nsDelay = nsDelayCount;
        }
        index++;
    }
}
#endif

static void clkInit(uint32_t nsCycle)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

    SIGNAL_CLK(DISABLE);

#ifndef DEVELOP_BOARD
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM14);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#else
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
#endif

	TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 42-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);//3

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = 21;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM14, ENABLE);
}

static void pinValueInit(const S13EE_OPIN_VALUE_LIST *pPinValueList)
{
    HADDR_BUS(pPinValueList->haddr);
    LADDR_BUS(pPinValueList->laddr);
    IDATA_BUS(pPinValueList->din);
    SIGNAL_READ(pPinValueList->read);
    SIGNAL_PGM(pPinValueList->pgm);
    SIGNAL_ERASE(pPinValueList->erase);
    SIGNAL_TESTMR(pPinValueList->testmr);
    SIGNAL_SYNC(pPinValueList->sync);
    SIGNAL_CLK(DISABLE);
    SIGNAL_BUFRST(pPinValueList->bufrst);
    SIGNAL_LOAD(pPinValueList->loaden);
    SIGNAL_DBY2(pPinValueList->dby2);
}

static const S13EE_OPIN_VALUE_LIST bufRstDataLoadPinValueList =
{
    .read = 0,
    .erase = 0,
    .pgm = 0,
    .clk = 0,
    .haddr = 0,
    .bufrst = 0,
    .loaden = 0,
    .sync = 0
};

static S13EE_STATUS bufferResetDataLoad(uint8_t addr, uint16_t *u16Buffer, uint16_t cnt)
{
    uint16_t index = 0;

    pinValueInit(&bufRstDataLoadPinValueList);

    SIGNAL_BUFRST(1);
    tw_b.delayFunc(tw_b.parameter);
    SIGNAL_BUFRST(0);
    tsu_bx.delayFunc(tsu_bx.parameter);
    SIGNAL_LOAD(1);
    while(index < cnt)
    {
        LADDR_BUS(addr);
        IDATA_BUS(u16Buffer[index++]);
        tdw_s_l.delayFunc(tdw_s_l.parameter);
        SIGNAL_SYNC(1);
        tdw_s_h.delayFunc(tdw_s_h.parameter);
        SIGNAL_SYNC(0);
        addr++;
    }
    tdh_xs.delayFunc(tdh_xs.parameter);
    SIGNAL_LOAD(0);

    return S13EE_SUCCESS;
}

static const S13EE_OPIN_VALUE_LIST wrErasePinValueList =
{
    .read = 0,
    .loaden = 0,
    .bufrst = 0,
    .sync = 0,
    .haddr = 0,
    .erase = 0,
    .pgm = 0,
    .clk = 0
};

static S13EE_STATUS _write(uint8_t addr, uint16_t *u16Buffer, uint16_t cnt)
{
    S13EE_STATUS result;
    uint16_t step = (cnt >= 4) ? 4 : (cnt % 4);
#ifdef CHECK_PARAM
    if(!((cnt > 0) && (cnt < 5)))
    {
        assert_param(0);
        return S13EE_PRAM_ERR;
    }
#endif

    while(cnt)
    {
    if(S13EE_SUCCESS != (result = bufferResetDataLoad(addr, u16Buffer, step)))
        return result;

    pinValueInit(&wrErasePinValueList);

    LADDR_BUS(addr);
    SIGNAL_CLK(ENABLE);
    tsu_ae.delayFunc(tsu_ae.parameter);
    SIGNAL_ERASE(1);
    tw_e.delayFunc(tw_e.parameter);
    SIGNAL_ERASE(0);
    tsu_ew.delayFunc(tsu_ew.parameter);
    SIGNAL_PGM(1);
    tw_w.delayFunc(tw_w.parameter);
    SIGNAL_PGM(0);
    th_aw.delayFunc(th_aw.parameter);
    SIGNAL_CLK(DISABLE);

    cnt -= step;
    step = (cnt >= 4) ? 4 : (cnt % 4);
    }

    return S13EE_SUCCESS;
}

static S13EE_STATUS _erase(uint8_t addr, uint16_t cnt)
{
    S13EE_STATUS result;
    uint16_t u16Buffer[4] = {0xffff, 0xffff, 0xffff, 0xffff};
#ifdef CHECK_PARAM
    if(!((cnt > 0) && (cnt < 5)))
    {
        assert_param(0);
        return S13EE_PRAM_ERR;
    }
#endif

    if(S13EE_SUCCESS != (result = bufferResetDataLoad(addr, u16Buffer, cnt)))
        return result;

    pinValueInit(&wrErasePinValueList);

    LADDR_BUS(addr);
    SIGNAL_CLK(ENABLE);
    tsu_ae.delayFunc(tsu_ae.parameter);
    SIGNAL_ERASE(1);
    tw_e.delayFunc(tw_e.parameter);
    SIGNAL_ERASE(0);
    tsu_ew.delayFunc(tsu_ew.parameter);
#if 0
    SIGNAL_PGM(1);
    tw_w.delayFunc(tw_w.parameter);
    SIGNAL_PGM(0);
    th_aw.delayFunc(th_aw.parameter);
#endif
    SIGNAL_CLK(DISABLE);

    return S13EE_SUCCESS;
}


static const S13EE_OPIN_VALUE_LIST readPinValueList =
{

    .loaden = 0,
    .bufrst = 0,
    .din = 0,
    .clk = 0,
    .haddr = 0,
    .erase = 0,
    .read = 0,
    .pgm = 0,
    .sync = 0,
};

static S13EE_STATUS _read(uint8_t addr, uint16_t *u16Buffer, uint16_t cnt)
{
    uint16_t index = 0;

#ifdef CHECK_PARAM
    if(!((cnt > 0) && (cnt <= 256)))
    {
        assert_param(0);
        return S13EE_PRAM_ERR;
    }
#endif

    pinValueInit(&readPinValueList);

    SIGNAL_READ(1);
    tsu_rs.delayFunc(tsu_rs.parameter);

    while(index < cnt)
    {
        LADDR_BUS(addr);
        trsu_as.delayFunc(trsu_as.parameter);
        SIGNAL_SYNC(1);
        trw_s_h.delayFunc(trw_s_h.parameter);
        SIGNAL_SYNC(0);
        trw_s_l.delayFunc(trw_s_l.parameter);
        u16Buffer[index++] = ODATA_BUS_READ();
        addr += 1;
    }

    if(trw_s_l.nsDelay < th_rs.nsDelay)
        th_rs.delayFunc(th_rs.parameter);

    SIGNAL_READ(0);

    return S13EE_SUCCESS;
}

/* test mode */
static const S13EE_OPIN_VALUE_LIST testModeBufRstDataLoadPinValueList =
{
    .read = 0,
    .erase = 0,
    .pgm = 0,
    .clk = 0,
    .haddr = 0,
    .bufrst = 0,
    .loaden = 0,
    .sync = 0
};

static S13EE_STATUS testModeBufferResetDataLoad(uint8_t addr, uint16_t *u16Buffer, uint16_t cnt)
{
    uint8_t index = 0;

    pinValueInit(&testModeBufRstDataLoadPinValueList);

    HADDR_BUS(3);
    SIGNAL_BUFRST(1);
    tw_b.delayFunc(tw_b.parameter);
    SIGNAL_BUFRST(0);
    tsu_bx.delayFunc(tsu_bx.parameter);
    SIGNAL_LOAD(1);
    while(index < cnt)
    {
        LADDR_BUS(addr);
        IDATA_BUS(u16Buffer[index++]);
        tdw_s_l.delayFunc(tdw_s_l.parameter);
        SIGNAL_SYNC(1);
        tdw_s_h.delayFunc(tdw_s_h.parameter);
        SIGNAL_SYNC(0);
        addr++;
    }
    tdh_xs.delayFunc(tdh_xs.parameter);
    SIGNAL_LOAD(0);

    return S13EE_SUCCESS;
}

static const S13EE_OPIN_VALUE_LIST testModeWrErasePinValueList =
{
    .read = 0,
    .erase = 0,
    .pgm = 0,
    .clk = 0,
    .haddr = 0,
    .bufrst = 0,
    .loaden = 0,
    .sync = 0
};

static S13EE_STATUS _chipErase (void)
{
    S13EE_STATUS result;
    uint16_t u16Arry[4] = {0xffff, 0xffff, 0xffff, 0xffff};

    if(S13EE_SUCCESS != (result = testModeBufferResetDataLoad(0, u16Arry, 4)))
        return result;

    pinValueInit(&testModeWrErasePinValueList);

    HADDR_BUS(3);
    SIGNAL_CLK(ENABLE);
    tsu_ae.delayFunc(tsu_ae.parameter);
    SIGNAL_ERASE(1);
    tw_e.delayFunc(tw_e.parameter);
    SIGNAL_ERASE(0);
#if 0
    tsu_ew.delayFunc(tsu_ew.parameter);
    SIGNAL_PGM(1);
    tw_w.delayFunc(tw_w.parameter);
    SIGNAL_PGM(0);
#endif
    th_aw.delayFunc(th_aw.parameter);
    SIGNAL_CLK(DISABLE);

    return S13EE_SUCCESS;
}

static S13EE_STATUS _chipWrite (uint16_t (*u16Arry)[4])
{
    S13EE_STATUS result;
#ifdef CHECK_PARAM
    if(NULL == u16Arry)
    {
        assert_param(0);
        return S13EE_PRAM_ERR;
    }
#endif

    if(S13EE_SUCCESS != (result = testModeBufferResetDataLoad(0, (uint16_t *)u16Arry, 4)))
        return result;

    pinValueInit(&testModeWrErasePinValueList);

    HADDR_BUS(3);
    tsu_ae.delayFunc(tsu_ae.parameter);
    SIGNAL_CLK(ENABLE);
    tcyc_c.delayFunc(tcyc_c.parameter);
    tsu_c.delayFunc(tsu_c.parameter);
    SIGNAL_ERASE(1);
    tw_e.delayFunc(tw_e.parameter);
    SIGNAL_ERASE(0);
    tsu_ew.delayFunc(tsu_ew.parameter);
    SIGNAL_PGM(1);
    tw_w.delayFunc(tw_w.parameter);
    SIGNAL_PGM(0);
    th_aw.delayFunc(th_aw.parameter);
    SIGNAL_CLK(DISABLE);
    return S13EE_SUCCESS;
}

static S13EE_STATUS _halfWrite(uint16_t (*u16Arry)[4], uint8_t isUpper)
{
    S13EE_STATUS result;
#ifdef CHECK_PARAM
    if(NULL == u16Arry)
    {
        assert_param(0);
        return S13EE_PRAM_ERR;
    }
#endif

    if(S13EE_SUCCESS != (result = testModeBufferResetDataLoad(0, (uint16_t *)u16Arry, 4)))
        return result;

    pinValueInit(&testModeWrErasePinValueList);

    if(isUpper)
        HADDR_BUS(2);
    else
        HADDR_BUS(1);

    SIGNAL_CLK(ENABLE);
    tsu_ae.delayFunc(tsu_ae.parameter);
    SIGNAL_ERASE(1);
    tw_e.delayFunc(tw_e.parameter);
    SIGNAL_ERASE(0);
    tsu_ew.delayFunc(tsu_ew.parameter);
    SIGNAL_PGM(1);
    tw_w.delayFunc(tw_w.parameter);
    SIGNAL_PGM(0);
    th_aw.delayFunc(th_aw.parameter);
    SIGNAL_CLK(DISABLE);

    return S13EE_SUCCESS;
}

S13EE * S13EE_INIT (S13EE * pS13EE)
{
#ifdef CHECK_PARAM
    if(NULL == pS13EE)
    {
        assert_param(0);
        return NULL;
    }
#endif

    pS13EE->chipErase = _chipErase;
    pS13EE->chipWrite = _chipWrite;
    pS13EE->erase = _erase;
    pS13EE->read = _read;
    pS13EE->write = _write;
    pS13EE->halfWrite = _halfWrite;
    pS13EE->errToString = _errToString;
#ifndef DEVELOP_BOARD
    /* gpio config */
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* HADDR & LADDR */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| \
        GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* ODATA */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6|
        GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* IDATA */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6|
        GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

    /* READ PGM ERASE SYNC CLK BUFEST LOAD DBY2*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_15 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
    /* TESTMR */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
    /* SYNC */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#else
    /* gpio config */
	GPIO_InitTypeDef  GPIO_InitStructure;
	/* HADDR & LADDR */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| \
        GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* ODATA */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6|
        GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* IDATA */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6|
        GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

    /* READ PGM ERASE SYNC CLK BUFEST LOAD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
#if 0
    /* TESTMR */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
    /* SYNC */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#endif
    clkInit(0);

//    testDelay();

    return pS13EE;
}

S13EE_STATUS S13EE_UNINIT (S13EE * pS13EE)
{
    return S13EE_SUCCESS;
}

void gpio_test(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_6);
    GPIO_ResetBits(GPIOC, GPIO_Pin_6);
    while(1);
}
void uart3_test(void)
{
    while(1)
    {
    USART_SendData(USART3, 'h');
    nsDelay(100000000);
    }
}
