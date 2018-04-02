
#ifndef __RA8875_H
#define __RA8875_H

#include "stm32f10x.h"
#define LCD_70_HEIGHT	480		/* 7.0寸宽屏 高度，单位：像素 */
#define LCD_70_WIDTH	800		/* 7.0寸宽屏 宽度，单位：像素 */
#define RA8875_BASE		((uint32_t)(0x60000000 | 0x00000000))
#define RA8875_REG		*(__IO uint16_t *)(RA8875_BASE + (1 << 19))	  //a18作为RS
#define RA8875_RAM		*(__IO uint16_t *)(RA8875_BASE )
static __IO uint8_t s_ucRA8875Busy = 0;
static __IO uint16_t s_WinX, s_WinY, s_WinHeight, s_WinWidth;
uint16_t g_LcdHeight = 480;			/* 显示屏分辨率-高度 */
uint16_t g_LcdWidth = 800;			/* 显示屏分辨率-宽度 */
uint8_t  s_ucBright;					/* 背光亮度参数 */

  /* LCD 颜色代码，CL_是Color的简写 */

static void RA8875_Delaly1us(void);
static void RA8875_Delaly1ms(void);
static void RA8875_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue);
static void RA8875_SetCursor(uint16_t _usX, uint16_t _usY);
void RA8875_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);
void RA8875_InitHard(void);
static void BTE_SetTarBlock(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t _ucLayer);
static void BTE_SetOperateCode(uint8_t _ucOperate);
void RA8875_SetFrontColor(uint16_t _usColor);
void RA8875_SetBackColor(uint16_t _usColor);
static void BTE_Start(void);
void RA8875_ClrScr(uint16_t _usColor);
#endif
