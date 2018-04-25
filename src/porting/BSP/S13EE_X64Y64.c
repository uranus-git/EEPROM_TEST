#include <S12EE_X64Y64.h> 
#include <string.h>
#include <math.h>

#ifndef ENABLE
#define ENABLE 1
#endif
#ifndef DISABLE
#define DISABLE 0
#endif

#define NOP  (__nop)
#define HADDR_BUS(val)            NOP  /* TODO */
#define LADDR_BUS(val)            NOP  /* TODO */
#define ODATA_BUS_READ()          NOP  /* TODO */
#define IDATA_BUS(val)            NOP  /* TODO */
#define SIGNAL_READ(val)          NOP  /* TODO */
#define SIGNAL_PGM(val)           NOP  /* TODO */
#define SIGNAL_ERASE(val)         NOP  /* TODO */
#define SIGNAL_TESTMR(val)        NOP  /* TODO */
#define SIGNAL_SYNC(val)          NOP  /* TODO */
#define SIGNAL_CLK(val)           NOP  /* TODO */
#define SIGNAL_BUFRST(val)        NOP  /* TODO */
#define SIGNAL_LOAD(val)          NOP  /* TODO */
#define SIGNAL_DBY2(val)          NOP  /* TODO */

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
    uint8_t *name;
    uint32_t nsDelay;
    uint32_t parameter;
    void (*delayFunc)(uint32_t);
}S13EE_DELAY;

/*
typedef struct
{
    uint8_t *name;
    S13EE_DELAY *
}S13EE_DELAY_INFO;
*/
static void nsDelay(uint32_t nsDelay);

/* Buffer Reset and Data Loading Timing */
static S13EE_DELAY tw_b     = {"tw_b", 		200, 200, nsDelay};
static S13EE_DELAY tsu_bx   = {"tsu_bx", 	200, 200, nsDelay};
static S13EE_DELAY tdsu_xs  = {"tdsu_xs", 	200, 200, nsDelay};
static S13EE_DELAY tdh_xs   = {"tdh_xs", 	200, 200, nsDelay};
static S13EE_DELAY tdsu_as  = {"tdsu_as", 	200, 200, nsDelay};
static S13EE_DELAY tdh_as   = {"tdh_as", 	200, 200, nsDelay};
static S13EE_DELAY tdsu_ds  = {"tdsu_ds", 	200, 200, nsDelay};
static S13EE_DELAY tdh_ds   = {"tdh_ds", 	200, 200, nsDelay};
static S13EE_DELAY tdw_s_h  = {"tdw_s_h", 	200, 200, nsDelay};
static S13EE_DELAY tdw_s_l  = {"tdw_s_l", 	200, 200, nsDelay};
static S13EE_DELAY tdcyc_s  = {"tdcyc_s", 	400, 400, nsDelay};
/* Write-Erase and Program-Timing */
static S13EE_DELAY tsu_ae   = {"tsu_ae", 	200, 200, nsDelay};
static S13EE_DELAY tsu_ew   = {"tsu_ew", 	100000, 100000, nsDelay};
static S13EE_DELAY th_aw    = {"th_aw", 	100000, 100000, nsDelay};
static S13EE_DELAY tw_e     = {"tw_e", 		2400000, 2400000, nsDelay};
static S13EE_DELAY tw_w     = {"tw_w", 		2400000, 2400000, nsDelay};
static S13EE_DELAY tw_c_l   = {"tw_c_l", 	260, 260, nsDelay};
static S13EE_DELAY tw_c_h   = {"tw_c_h", 	260, 260, nsDelay};
static S13EE_DELAY tcyc_c   = {"tcyc_c", 	520, 520, nsDelay};
static S13EE_DELAY tsu_c    = {"tsu_c", 	200, 200, nsDelay};
static S13EE_DELAY th_c     = {"th_c", 		200, 200, nsDelay};
/* Read Timing */
static S13EE_DELAY tsu_wr   = {"tsu_wr", 	200000, 200000, nsDelay};
static S13EE_DELAY tsu_rs   = {"tsu_rs", 	200, 200, nsDelay};
static S13EE_DELAY th_rs    = {"th_rs", 	200, 200, nsDelay};
static S13EE_DELAY trw_s_h  = {"trw_s_h", 	750, 750, nsDelay};
static S13EE_DELAY trw_s_l  = {"trw_s_l", 	750, 750, nsDelay};
static S13EE_DELAY trcyc_s  = {"trcyc_s", 	1500, 1500, nsDelay};
static S13EE_DELAY tsac     = {"tsac", 		150, 150, nsDelay};
static S13EE_DELAY trsu_as  = {"trsu_as", 	200, 200, nsDelay};
static S13EE_DELAY trh_as   = {"trh_as", 	200, 200, nsDelay};

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
    while(nsDelay)
    {
        nsDelay -= 30;
    }
}

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

static void inline pinValueInit(S13EE_OPIN_VALUE_LIST *pPinValueList)
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
    SIGNAL_LOAD(pPinValueList->load);
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

static S13EE_STATUS inline bufferResetDataLoad(uint8_t addr, uint16_t *u16Buffer, uint8_t cnt)
{
    uint8_t index = 0;

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

static S13EE_STATUS _write(uint8_t addr, uint16_t *u16Buffer, uint8_t cnt)
{
    S13EE_STATUS result;
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
    SIGNAL_PGM(1);
    tw_w.delayFunc(tw_w.parameter);
    SIGNAL_PGM(0);
    th_aw.delayFunc(th_aw.parameter);
    SIGNAL_CLK(DISABLE);

    return S13EE_SUCCESS;
}

static S13EE_STATUS _erase(uint8_t addr, uint8_t cnt)
{
    S13EE_STATUS result;
    uint16_t u16Buffer[4] = {0xff, 0xff, 0xff, 0xff};
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

static S13EE_STATUS _read(uint8_t addr, uint16_t *u16Buffer, uint8_t cnt)
{
    uint8_t index = 0;
    S13EE_STATUS result;

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

static S13EE_STATUS inline testModeBufferResetDataLoad(uint8_t addr, uint16_t *u16Buffer, uint8_t cnt)
{
    uint8_t index = 0;

    pinValueInit(&testModeBufRstDataLoadPinValueList);

    SIGNAL_BUFRST(1);
    tw_b.delayFunc(tw_b.parameter);
    SIGNAL_BUFRST(0);
    tsu_bx.delayFunc(tsu_bx.parameter);
    SIGNAL_LOAD(1);
    while(index < cnt)
    {
        HADDR_BUS(3);
        LADDR_BUS(addr);
        IDATA_BUS(u16Buffer[index++]);
        tdw_s_l.delayFunc(tdw_s_l.parameter);
        SIGNAL_SYNC(1);
        tdw_s_h.delayFunc(tdw_s_h.parameter);
        SIGNAL_SYNC(0);
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
    uint16_t u16Arry[4] = {0xff, 0xff, 0xff, 0xff};

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

    if(S13EE_SUCCESS != (result = testModeBufferResetDataLoad(0, u16Arry, 4)))
        return result;

    pinValueInit(&testModeWrErasePinValueList);

    HADDR_BUS(3);
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

    if(S13EE_SUCCESS != (result = testModeBufferResetDataLoad(0, u16Arry, 4)))
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

    return pS13EE;
}

S13EE_STATUS S13EE_UNINIT (S13EE * pS13EE)
{
    return S13EE_SUCCESS;
}

