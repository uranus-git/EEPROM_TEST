#include <S12EE_X64Y64.h>


#ifndef ENABLE
#define ENABLE 1
#endif
#ifndef DISABLE
#define DISABLE 0
#endif

#define NOP  (__nop)
#define HADDR_BUS(val)            NOP  /* TODO */
#define LADDR_BUS(val)            NOP  /* TODO */
#define DATA_BUS(val)             NOP  /* TODO */
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
    uint32_t nsDelay;
    uint32_t parameter;
    void (*delayFunc)(uint32_t);
}S13EE_DELAY;

static void nsDelay(uint32_t nsDelay);

/* Buffer Reset and Data Loading Timing */
static S13EE_DELAY tw_b = {200, 200, nsDelay};
static S13EE_DELAY tsu_bx = {200, 200, nsDelay};
static S13EE_DELAY tdsu_xs = {200, 200, nsDelay};
static S13EE_DELAY tdh_xs = {200, 200, nsDelay};
static S13EE_DELAY tdsu_as = {200, 200, nsDelay};
static S13EE_DELAY tdh_as = {200, 200, nsDelay};
static S13EE_DELAY tdsu_ds = {200, 200, nsDelay};
static S13EE_DELAY tdh_ds = {200, 200, nsDelay};
static S13EE_DELAY tdw_s_h = {200, 200, nsDelay};
static S13EE_DELAY tdw_s_l = {200, 200, nsDelay};
static S13EE_DELAY tdcyc_s = {400, 400, nsDelay};
/* Write-Erase and Program-Timing */
static S13EE_DELAY tsu_ae = {200, 200, nsDelay};
static S13EE_DELAY tsu_ew = {100000, 100000, nsDelay};
static S13EE_DELAY tsu_aw = {100000, 100000, nsDelay};
static S13EE_DELAY tw_e = {2400000, 2400000, nsDelay};
static S13EE_DELAY tw_w = {2400000, 2400000, nsDelay};
static S13EE_DELAY tw_c_l = {260, 260, nsDelay};
static S13EE_DELAY tw_c_h = {260, 260, nsDelay};
static S13EE_DELAY tcyc_c = {520, 520, nsDelay};
static S13EE_DELAY tsu_c = {200, 200, nsDelay};
static S13EE_DELAY th_c = {200, 200, nsDelay};
/* Read Timing */
static S13EE_DELAY tsu_wr = {200000, 200000, nsDelay};
static S13EE_DELAY tsu_rs = {200, 200, nsDelay};
static S13EE_DELAY th_rs = {200, 200, nsDelay};
static S13EE_DELAY trw_s_h = {750, 750, nsDelay};
static S13EE_DELAY trw_s_l = {750, 750, nsDelay};
static S13EE_DELAY trcyc_s = {1500, 1500, nsDelay};
static S13EE_DELAY tsac = {150, 150, nsDelay};
static S13EE_DELAY trsu_as = {200, 200, nsDelay};
static S13EE_DELAY trh_as = {200, 200, nsDelay};

static const S13EE_OPIN_VALUE_LIST bufRstDataLoadPinValueList =
    {
        .read = 0,
        .erase = 0,
        .pgm = 0,
        .clk = 0,
        .addr1312 = 0,
        .bufrst = 0,
        .loaden = 0,
        .sync = 0
    };

static void nsDelay(uint32_t nsDelay)
{
    /* TODO */
}
static void inline pinValueInit(S13EE_OPIN_VALUE_LIST *pPinValueList)
{
    HADDR_BUS(pPinValueList->haddr);
    LADDR_BUS(pPinValueList->laddr);
    DATA_BUS(pPinValueList->din);
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

static S13EE_STATUS inline bufferResetDataLoad(uint8_t addr, uint16_t *u16Buffer, uint8_t cnt)
{
    uint8_t index = 0;

#ifdef CHECK_PARAM
    if(!((cnt > 0) && (cnt < 5)))
    {
        assert_param(0);
        return S13EE_PRAM_ERR;
    }
#endif

    pinValueInit(&bufRstDataLoadPinValueList);

    SIGNAL_BUFRST(1);
    tw_b.delayFunc(tw_b.parameter);
    SIGNAL_BUFRST(0);
    tsu_bx.delayFunc(tsu_bx.parameter);
    SIGNAL_LOAD(1);
    while(index < cnt)
    {
        LADDR_BUS(addr);
        DATA_BUS(u16Buffer[index++]);
        tdw_s_l.delayFunc(tdw_s_l.parameter);
        SIGNAL_SYNC(1);
        tdw_s_h.delayFunc(tdw_s_h.parameter);
        SIGNAL_SYNC(0);
    }
    tdh_xs.delayFunc(tdh_xs.parameter);
    SIGNAL_LOAD(0);

    return S13EE_SUCCESS;
}

static S13EE_STATUS _write(uint8_t addr, uint16_t *u16Buffer, uint8_t cnt)
{

}

static S13EE_STATUS _read(uint8_t addr, uint16_t *u16Buffer, uint8_t cnt)
{

}

static S13EE_STATUS _erase(uint8_t addr, uint16_t *u16Buffer, uint8_t cnt)
{

}

static S13EE_STATUS _chipErase (void)
{

}

static S13EE_STATUS _chipWrite (uint16_t (*u16Arry)[4])
{

}

S13EE * S13EE_INIT (S13EE * pS13EE)
{
    pS13EE->chipErase = _chipErase;
    pS13EE->chipWrite = _chipErase;
    pS13EE->erase = _erase;
    pS13EE->read = _read;
    pS13EE->write = _write;

    return pS13EE;
}

S13EE_STATUS S13EE_UNINIT (S13EE * pS13EE)
{
    return S13EE_SUCCESS;
}

