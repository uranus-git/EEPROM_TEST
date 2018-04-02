/
*********************************************************************************************************
*	                                  
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V2.0
*	说    明 : 驱动LED指示灯
*	修改记录 :
*		版本号  日期       作者    说明
*		v0.1    2009-12-27 armfly  创建该文件，ST固件库版本为V3.1.2
*		v1.0    2011-01-11 armfly  ST固件库升级到V3.4.0版本。
*       v2.0    2011-10-16 armfly  ST固件库升级到V3.5.0版本。
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "stm32f10x.h"
#include <stdio.h>

#include "bsp_led.h"

/*
	安富莱STM32F103ZE-EK开发板LED指示灯口线分配：
	LED1 : PF6  低电平点亮
	LED2 : PF7  低电平点亮	
	LED3 : PF8  低电平点亮
	LED4 : PF9  低电平点亮		
*/
#define GPIO_PORT_LED1	GPIOF
#define GPIO_PORT_LED2	GPIOF
#define GPIO_PORT_LED3	GPIOF
#define GPIO_PORT_LED4	GPIOF

#define GPIO_PIN_LED1	GPIO_Pin_6
#define GPIO_PIN_LED2	GPIO_Pin_7
#define GPIO_PIN_LED3	GPIO_Pin_8
#define GPIO_PIN_LED4	GPIO_Pin_9

#define GPIO_CLK_LED1	RCC_APB2Periph_GPIOF
#define GPIO_CLK_LED2	RCC_APB2Periph_GPIOF
#define GPIO_CLK_LED3	RCC_APB2Periph_GPIOF
#define GPIO_CLK_LED4	RCC_APB2Periph_GPIOF

/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 初始化LED指示灯
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitLed(void)
{FOD
	GPIO_InitTypeDef GPIO_InitStructure;

#if 1	/* 采用宏定义的方式初始化GPIO，以便于修改GPIO口线 */
	/* 打开GPIOF的时钟 */
	RCC_APB2PeriphClockCmd(GPIO_CLK_LED1 | GPIO_CLK_LED2 | GPIO_CLK_LED3 | GPIO_CLK_LED4, ENABLE);

	/* 配置所有的LED指示灯GPIO为推挽输出模式 */
	/* 由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，因此会驱动LED点亮
		这是我不希望的，因此在改变GPIO为输出前，先修改输出寄存器的值为1 */
	GPIO_SetBits(GPIO_PORT_LED1,  GPIO_PIN_LED1);
	GPIO_SetBits(GPIO_PORT_LED2,  GPIO_PIN_LED2);
	GPIO_SetBits(GPIO_PORT_LED3,  GPIO_PIN_LED3);
	GPIO_SetBits(GPIO_PORT_LED4,  GPIO_PIN_LED4);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED1;
	GPIO_Init(GPIO_PORT_LED1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED2;
	GPIO_Init(GPIO_PORT_LED2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED3;
	GPIO_Init(GPIO_PORT_LED3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_PIN_LED4;
	GPIO_Init(GPIO_PORT_LED4, &GPIO_InitStructure);
#else
	/* 打开GPIOF的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

	/* 配置所有的LED指示灯GPIO为推挽输出模式 */
	/* 由于将GPIO设置为输出时，GPIO输出寄存器的值缺省是0，因此会驱动LED点亮
		这是我不希望的，因此在改变GPIO为输出前，先修改输出寄存器的值为1 */
	GPIO_SetBits(GPIOF,  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
#endif
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参：_no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	_no--;
	
	if (_no == 0)
	{
		GPIO_PORT_LED1->BRR = GPIO_PIN_LED1;
	}
	else if (_no == 1)
	{
		GPIO_PORT_LED2->BRR = GPIO_PIN_LED2;
	}
	else if (_no == 2)
	{
		GPIO_PORT_LED3->BRR = GPIO_PIN_LED3;
	}
	else if (_no == 3)
	{
		GPIO_PORT_LED4->BRR = GPIO_PIN_LED4;
	}		
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参：_no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{
	_no--;
	
	if (_no == 0)
	{
		GPIO_PORT_LED1->BSRR = GPIO_PIN_LED1;
	}
	else if (_no == 1)
	{
		GPIO_PORT_LED2->BSRR = GPIO_PIN_LED2;
	}
	else if (_no == 2)
	{
		GPIO_PORT_LED3->BSRR = GPIO_PIN_LED3;
	}
	else if (_no == 3)
	{
		GPIO_PORT_LED4->BSRR = GPIO_PIN_LED4;
	}		
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参：_no : 指示灯序号，范围 1 - 4
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	_no--;
	
	if (_no == 0)
	{
		GPIO_PORT_LED1->ODR ^= GPIO_PIN_LED1;
	}
	else if (_no == 1)
	{
		GPIO_PORT_LED2->ODR ^= GPIO_PIN_LED2;
	}
	else if (_no == 2)
	{
		GPIO_PORT_LED3->ODR ^= GPIO_PIN_LED3;
	}
	else if (_no == 3)
	{
		GPIO_PORT_LED4->ODR ^= GPIO_PIN_LED4;
	}		
}

/****************************************************************************************************************************************************************************

                                                                                                                   模块名称 : 定时器模块
	                                                                                                           文件名称 : bsp_timer.c
*****************************************************************************************************************************************************************************

/*
*********************************************************************************************************
*	                                  
*	模块名称 : 定时器模块
*	文件名称 : bsp_timer.c
*	版    本 : V2.0
*	说    明 : 设置systick中断，实现若干个软件定时器
*	修改记录 :
*		版本号  日期       作者    说明
*		v0.1    2009-12-27 armfly  创建该文件，ST固件库版本为V3.1.2
*		v1.0    2011-01-11 armfly  ST固件库升级到V3.4.0版本。
*       v2.0    2011-10-16 armfly  ST固件库升级到V3.5.0版本。
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "stm32f10x.h"
#include <stdio.h>

#include "bsp_timer.h"
#include "bsp_button.h"

/* 
	在此定义若干个软件定时器全局变量
	注意，必须增加__IO 即 volatile，因为这个变量在中断和主程序中同时被访问，有可能造成编译器错误优化。
*/
#define TMR_COUNT	4		/* 软件定时器的个数，第1个保留给bsp_DelayMS()使用 */

SOFT_TMR g_Tmr[TMR_COUNT];

/* 全局运行时间，单位10ms，用于uIP */
__IO int32_t g_iRunTime = 0;

static void bsp_SoftTimerDec(SOFT_TMR *_tmr);

void bsp_InitTimer(void)
{
	uint8_t i;
	
	/* 清零所有的软件定时器 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		g_Tmr[i].count = 0;
		g_Tmr[i].flag = 0;
	}
	
	/* 
		配置systic中断周期为1ms，并启动systick中断。
    	这个函数在 \Libraries\CMSIS\CM3\CoreSupport\core_cm3.h 
    	
    	Systick中断向量在(\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm\
    		startup_stm32f10x_hd.s 文件中定义为 SysTick_Handler。
    	SysTick_Handler函数的实现在stm32f10x_it.c 文件。
    	SysTick_Handler函数调用了SysTick_ISR()函数，在本文件末尾。
    */	
	SysTick_Config(SystemCoreClock / 1000);
}

/*
*********************************************************************************************************
*	函 数 名: SysTick_ISR
*	功能说明: SysTick中断服务程序，每隔1ms进入1次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
extern void TOUCH_Scan(void);
void SysTick_ISR(void)
{
	static uint8_t s_count = 0;
	uint8_t i;

	for (i = 0; i < TMR_COUNT; i++)
	{
		bsp_SoftTimerDec(&g_Tmr[i]);
	}

	g_iRunTime++;	/* 全局运行时间每1ms增1 */	
	if (g_iRunTime == 0x7FFFFFFF)
	{
		g_iRunTime = 0;
	}
		
	TOUCH_Scan();	/* 触摸板扫描程序 */
	if (++s_count >= 10)
	{
		s_count = 0;



		/* 
			下面的代码实现所有按键的检测。按键检测每隔10ms一次就行了，一般做40ms的滤波处理就可以
			有效过滤掉机械动作造成的按键抖动。
		*/
	//	bsp_KeyPro();		/* 该函数在 bsp_button.c 中实现 */
	}
	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SoftTimerDec
*	功能说明: 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
*	形    参：_tmr : 定时器变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->flag == 0)
	{
		if (_tmr->count > 0)
		{
			/* 如果定时器变量减到1则设置定时器到达标志 */
			if (--_tmr->count == 0)
			{
				_tmr->flag = 1;
			}
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DelayMS
*	功能说明: ms级延迟，延迟精度为正负1ms
*	形    参：n : 延迟长度，单位1 ms。 n 应大于2
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t n)
{
	/* 避免 n = 1 出现主程序死锁 */
	if (n <= 1)
	{
		n = 2;
	}
	
	__set_PRIMASK(1);  		/* 关中断 */
	g_Tmr[0].count = n;
	g_Tmr[0].flag = 0;
	__set_PRIMASK(0);  		/* 开中断 */

	while (1)
	{
		CPU_IDLE();	/* 此处是空操作。用户可以定义，让CPU进入IDLE状态，以降低功耗；或实现喂狗 */

		/* 等待延迟时间到 */
		if (g_Tmr[0].flag == 1)
		{
			break;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StartTimer
*	功能说明: 启动一个定时器，并设置定时周期。
*	形    参：	_id     : 定时器ID，值域【1,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*						  定时器ID = 0 已用于bsp_DelayMS()函数
*				_period : 定时周期，单位1ms
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); 死机 */
		return;
	}

	__set_PRIMASK(1);  		/* 关中断 */
	g_Tmr[_id].count = _period;
	g_Tmr[_id].flag = 0;
	__set_PRIMASK(0);  		/* 开中断 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_CheckTimer
*	功能说明: 检测定时器是否超时
*	形    参：	_id     : 定时器ID，值域【1,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*						  0 用于
*				_period : 定时周期，单位1ms
*	返 回 值: 返回 0 表示定时未到， 1表示定时到
*********************************************************************************************************
*/
uint8_t bsp_CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (g_Tmr[_id].flag == 1)
	{
		g_Tmr[_id].flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetRunTime
*	功能说明: 获取CPU运行时间，单位1ms
*	形    参：无
*	返 回 值: CPU运行时间，单位1ms
*********************************************************************************************************
*/
int32_t bsp_GetRunTime(void)
{
	int runtime; 

	__set_PRIMASK(1);  		/* 关中断 */
	
	runtime = g_iRunTime;	/* 由于在Systick中断被改写，因此关中断进行保护 */
		
	__set_PRIMASK(0);  		/* 开中断 */

	return runtime;
}
/****************************************************************************************************************************************************************************

                                                                                                                   模块名称 : 按键驱动模块
  	                                                                                                           文件名称 : bsp_button.c
*****************************************************************************************************************************************************************************
/*
*********************************************************************************************************
*	                                  
*	模块名称 : 按键驱动模块
*	文件名称 : bsp_button.c
*	版    本 : V2.0
*	说    明 : 实现按键的检测，具有软件滤波机制，可以检测如下事件：
*				(1) 按键按下
*				(2) 按键弹起
*				(3) 长按键
*				(4) 长按时自动连发
*				(5) 组合键
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v0.1    2009-12-27 armfly  创建该文件，ST固件库版本为V3.1.2
*		v1.0    2011-01-11 armfly  ST固件库升级到V3.4.0版本。
*       v2.0    2011-10-16 armfly  ST固件库升级到V3.5.0版本。
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "stm32f10x.h"
#include <stdio.h>

#include "bsp_button.h"

static BUTTON_T s_BtnUser;		/* USER 键 */
static BUTTON_T s_BtnTamper;	/* TAMPER 键 */
static BUTTON_T s_BtnWakeUp;	/* WAKEUP 键 */
static BUTTON_T s_BtnUp;		/* 摇杆UP键 */
static BUTTON_T s_BtnDown;		/* 摇杆DOWN键 */
static BUTTON_T s_BtnLeft;		/* 摇杆LEFT键 */
static BUTTON_T s_BtnRight;		/* 摇杆RIGHT键 */
static BUTTON_T s_BtnOk;		/* 摇杆OK键 */

static BUTTON_T s_BtnUserTamper;/* 组合键，USER和TAMPER键 */

static KEY_FIFO_T s_Key;		/* 按键FIFO变量,结构体 */

static void bsp_InitButtonVar(void);
static void bsp_InitButtonHard(void);
static void bsp_DetectButton(BUTTON_T *_pBtn);

/*
	安富莱STM32F103ZE-EK 按键口线分配：
	USER键     : PG8  (低电平表示按下)
	TAMPEER键  : PC13 (低电平表示按下)
	WKUP键     : PA0  (!!!高电平表示按下)
	摇杆UP键   : PG15 (低电平表示按下)
	摇杆DOWN键 : PD3  (低电平表示按下)
	摇杆LEFT键 : PG14 (低电平表示按下)
	摇杆RIGHT键: PG13 (低电平表示按下)
	摇杆OK键   : PG7 (低电平表示按下)

	定义函数判断按键是否按下，返回值1 表示按下，0表示未按下
*/
static uint8_t IsKeyDownUser(void) 		{if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_8) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownTamper(void) 	{if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownWakeUp(void) 	{if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET) return 1; return 0;}
static uint8_t IsKeyDownUp(void) 		{if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_15) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownDown(void) 		{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownLeft(void) 		{if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownRight(void) 	{if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownOk(void) 		{if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDownUserTamper(void) {if (IsKeyDownUser() && IsKeyDownTamper()) return 1; return 0;}	/* 组合键 */

/*
*********************************************************************************************************
*	函 数 名: bsp_InitButton
*	功能说明: 初始化按键
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitButton(void)
{
	bsp_InitButtonVar();		/* 初始化按键变量 */
	bsp_InitButtonHard();		/* 初始化按键硬件 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参：_KeyCode : 按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_Key.Buf[s_Key.Write] = _KeyCode;

	if (++s_Key.Write  >= KEY_FIFO_SIZE)
	{
		s_Key.Write = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_Key.Read == s_Key.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_Key.Buf[s_Key.Read];

		if (++s_Key.Read >= KEY_FIFO_SIZE)
		{
			s_Key.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyState
*	功能说明: 读取按键的状态
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t bsp_KeyState(uint8_t _ucKeyID)
{
	uint8_t ucState = 0;

	switch (_ucKeyID)
	{
		case KID_TAMPER:
			ucState = s_BtnTamper.State;
			break;

		case KID_WAKEUP:
			ucState = s_BtnWakeUp.State;
			break;

		case KID_USER:
			ucState = s_BtnUser.State;
			break;

		case KID_JOY_UP:
			ucState = s_BtnUp.State;
			break;

		case KID_JOY_DOWN:
			ucState = s_BtnDown.State;
			break;

		case KID_JOY_LEFT:
			ucState = s_BtnLeft.State;
			break;

		case KID_JOY_RIGHT:
			ucState = s_BtnRight.State;
			break;

		case KID_JOY_OK:
			ucState = s_BtnOk.State;
			break;
	}

	return ucState;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitButtonHard
*	功能说明: 初始化按键硬件
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitButtonHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*
	安富莱STM32F103ZE-EK 按键口线分配：
	USER键     : PG8  (低电平表示按下)
	TAMPEER键  : PC13 (低电平表示按下)
	WKUP键     : PA0  (!!!高电平表示按下)
	摇杆UP键   : PG15 (低电平表示按下)
	摇杆DOWN键 : PD3  (低电平表示按下)
	摇杆LEFT键 : PG14 (低电平表示按下)
	摇杆RIGHT键: PG13 (低电平表示按下)
	摇杆OK键   : PG7 (低电平表示按下)
	*/
			
	/* 第1步：打开GPIOA GPIOC GPIOD GPIOF GPIOG的时钟
	   注意：这个地方可以一次性全打开
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC
			| RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG, ENABLE);
	
	/* 第2步：配置所有的按键GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	/* PA0 */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	/* PC13 */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	/* PD3 */
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_13
					  | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOG, &GPIO_InitStructure);	/* PG7,8,13,14,15 */
}
	
/*
*********************************************************************************************************
*	函 数 名: bsp_InitButtonVar
*	功能说明: 初始化按键变量
*	形    参：strName : 例程名称字符串
*			  strDate : 例程发布日期
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitButtonVar(void)
{
	/* 对按键FIFO读写指针清零 */
	s_Key.Read = 0;
	s_Key.Write = 0;

	/* 初始化USER按键变量，支持按下、弹起、长按 */
	s_BtnUser.IsKeyDownFunc = IsKeyDownUser;		/* 判断按键按下的函数 */
	s_BtnUser.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */
	s_BtnUser.LongTime = BUTTON_LONG_TIME;			/* 长按时间 */
	s_BtnUser.Count = s_BtnUser.FilterTime / 2;		/* 计数器设置为滤波时间的一半 */
	s_BtnUser.State = 0;							/* 按键缺省状态，0为未按下 */
	s_BtnUser.KeyCodeDown = KEY_DOWN_USER;			/* 按键按下的键值代码 */
	s_BtnUser.KeyCodeUp = KEY_UP_USER;				/* 按键弹起的键值代码 */
	s_BtnUser.KeyCodeLong = KEY_LONG_USER;			/* 按键被持续按下的键值代码 */
	s_BtnUser.RepeatSpeed = 0;						/* 按键连发的速度，0表示不支持连发 */
	s_BtnUser.RepeatCount = 0;						/* 连发计数器 */		

	/* 初始化TAMPER按键变量，支持按下 */
	s_BtnTamper.IsKeyDownFunc = IsKeyDownTamper;	/* 判断按键按下的函数 */
	s_BtnTamper.FilterTime = BUTTON_FILTER_TIME;	/* 按键滤波时间 */
	s_BtnTamper.LongTime = 0;						/* 长按时间, 0表示不检测 */
	s_BtnTamper.Count = s_BtnTamper.FilterTime / 2;	/* 计数器设置为滤波时间的一半 */
	s_BtnTamper.State = 0;							/* 按键缺省状态，0为未按下 */
	s_BtnTamper.KeyCodeDown = KEY_DOWN_TAMPER;		/* 按键按下的键值代码 */
	s_BtnTamper.KeyCodeUp = KEY_UP_TAMPER;			/* 按键弹起的键值代码 */
	s_BtnTamper.KeyCodeLong = 0;					/* 按键被持续按下的键值代码 */
	s_BtnTamper.RepeatSpeed = 0;					/* 按键连发的速度，0表示不支持连发 */
	s_BtnTamper.RepeatCount = 0;					/* 连发计数器 */	

	/* 初始化WAKEUP按键变量，支持按下 */
	s_BtnWakeUp.IsKeyDownFunc = IsKeyDownWakeUp;	/* 判断按键按下的函数 */
	s_BtnWakeUp.FilterTime = BUTTON_FILTER_TIME;	/* 按键滤波时间 */
	s_BtnWakeUp.LongTime = 0;						/* 长按时间 */
	s_BtnWakeUp.Count = s_BtnWakeUp.FilterTime / 2;	/* 计数器设置为滤波时间的一半 */
	s_BtnWakeUp.State = 0;							/* 按键缺省状态，0为未按下 */
	s_BtnWakeUp.KeyCodeDown = KEY_DOWN_WAKEUP;		/* 按键按下的键值代码 */
	s_BtnWakeUp.KeyCodeUp = KEY_UP_WAKEUP;			/* 按键弹起的键值代码，0表示不检测 */
	s_BtnWakeUp.KeyCodeLong = 0;					/* 按键被持续按下的键值代码，0表示不检测 */
	s_BtnWakeUp.RepeatSpeed = 0;					/* 按键连发的速度，0表示不支持连发 */
	s_BtnWakeUp.RepeatCount = 0;					/* 连发计数器 */	

	/* 初始化UP按键变量，支持按下、连发（周期10ms） */
	s_BtnUp.IsKeyDownFunc = IsKeyDownUp;			/* 判断按键按下的函数 */
	s_BtnUp.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */
	s_BtnUp.LongTime = 20;							/* 长按时间 */
	s_BtnUp.Count = s_BtnUp.FilterTime / 2;			/* 计数器设置为滤波时间的一半 */
	s_BtnUp.State = 0;								/* 按键缺省状态，0为未按下 */
	s_BtnUp.KeyCodeDown = KEY_DOWN_JOY_UP;			/* 按键按下的键值代码 */
	s_BtnUp.KeyCodeUp = 0;							/* 按键弹起的键值代码，0表示不检测 */
	s_BtnUp.KeyCodeLong = 0;						/* 按键被持续按下的键值代码，0表示不检测 */
	s_BtnUp.RepeatSpeed = 2;						/* 按键连发的速度，0表示不支持连发 */
	s_BtnUp.RepeatCount = 0;						/* 连发计数器 */		

	/* 初始化DOWN按键变量，支持按下、连发（周期10ms） */
	s_BtnDown.IsKeyDownFunc = IsKeyDownDown;		/* 判断按键按下的函数 */
	s_BtnDown.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */
	s_BtnDown.LongTime = 20;							/* 长按时间 */
	s_BtnDown.Count = s_BtnDown.FilterTime / 2;		/* 计数器设置为滤波时间的一半 */
	s_BtnDown.State = 0;							/* 按键缺省状态，0为未按下 */
	s_BtnDown.KeyCodeDown = KEY_DOWN_JOY_DOWN;		/* 按键按下的键值代码 */
	s_BtnDown.KeyCodeUp = 0;						/* 按键弹起的键值代码，0表示不检测 */
	s_BtnDown.KeyCodeLong = 0;						/* 按键被持续按下的键值代码，0表示不检测 */
	s_BtnDown.RepeatSpeed = 2;						/* 按键连发的速度，0表示不支持连发 */
	s_BtnDown.RepeatCount = 0;						/* 连发计数器 */		

	/* 初始化LEFT按键变量，支持按下 */
	s_BtnLeft.IsKeyDownFunc = IsKeyDownLeft;		/* 判断按键按下的函数 */
	s_BtnLeft.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */
	s_BtnLeft.LongTime = 20;							/* 长按时间 */
	s_BtnLeft.Count = s_BtnLeft.FilterTime / 2;		/* 计数器设置为滤波时间的一半 */
	s_BtnLeft.State = 0;							/* 按键缺省状态，0为未按下 */
	s_BtnLeft.KeyCodeDown = KEY_DOWN_JOY_LEFT;		/* 按键按下的键值代码 */
	s_BtnLeft.KeyCodeUp = 0;						/* 按键弹起的键值代码，0表示不检测 */
	s_BtnLeft.KeyCodeLong = 0;						/* 按键被持续按下的键值代码，0表示不检测 */
	s_BtnLeft.RepeatSpeed = 2;						/* 按键连发的速度，0表示不支持连发 */
	s_BtnLeft.RepeatCount = 0;						/* 连发计数器 */	

	/* 初始化RIGHT按键变量，支持按下 */
	s_BtnRight.IsKeyDownFunc = IsKeyDownRight;		/* 判断按键按下的函数 */
	s_BtnRight.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */
	s_BtnRight.LongTime = 20;						/* 长按时间 */
	s_BtnRight.Count = s_BtnRight.FilterTime / 2;	/* 计数器设置为滤波时间的一半 */
	s_BtnRight.State = 0;							/* 按键缺省状态，0为未按下 */
	s_BtnRight.KeyCodeDown = KEY_DOWN_JOY_RIGHT;	/* 按键按下的键值代码 */
	s_BtnRight.KeyCodeUp = 0;						/* 按键弹起的键值代码，0表示不检测 */
	s_BtnRight.KeyCodeLong = 0;						/* 按键被持续按下的键值代码，0表示不检测 */
	s_BtnRight.RepeatSpeed = 2;						/* 按键连发的速度，0表示不支持连发 */
	s_BtnRight.RepeatCount = 0;						/* 连发计数器 */	

	/* 初始化OK按键变量，支持按下 */
	s_BtnOk.IsKeyDownFunc = IsKeyDownOk;			/* 判断按键按下的函数 */
	s_BtnOk.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */
	s_BtnOk.LongTime = 0;							/* 长按时间 */
	s_BtnOk.Count = s_BtnOk.FilterTime / 2;			/* 计数器设置为滤波时间的一半 */
	s_BtnOk.State = 0;								/* 按键缺省状态，0为未按下 */
	s_BtnOk.KeyCodeDown = KEY_DOWN_JOY_OK;			/* 按键按下的键值代码 */
	s_BtnOk.KeyCodeUp = KEY_UP_JOY_OK;				/* 按键弹起的键值代码，0表示不检测 */
	s_BtnOk.KeyCodeLong = 0;						/* 按键被持续按下的键值代码，0表示不检测 */
	s_BtnOk.RepeatSpeed = 0;						/* 按键连发的速度，0表示不支持连发 */
	s_BtnOk.RepeatCount = 0;						/* 连发计数器 */	

	/* 初始化组合按键变量，支持按下 */
	s_BtnUserTamper.IsKeyDownFunc = IsKeyDownUserTamper;	/* 判断按键按下的函数 */
	s_BtnUserTamper.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */
	s_BtnUserTamper.LongTime = 0;							/* 长按时间 */
	s_BtnUserTamper.Count = s_BtnUserTamper.FilterTime / 2;	/* 计数器设置为滤波时间的一半 */
	s_BtnUserTamper.State = 0;								/* 按键缺省状态，0为未按下 */
	s_BtnUserTamper.KeyCodeDown = KEY_DOWN_USER_TAMPER;		/* 按键按下的键值代码 */
	s_BtnUserTamper.KeyCodeUp = 0;							/* 按键弹起的键值代码，0表示不检测 */
	s_BtnUserTamper.KeyCodeLong = 0;						/* 按键被持续按下的键值代码，0表示不检测 */
	s_BtnUserTamper.RepeatSpeed = 0;						/* 按键连发的速度，0表示不支持连发 */
	s_BtnUserTamper.RepeatCount = 0;						/* 连发计数器 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DetectButton
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参：按键结构变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_DetectButton(BUTTON_T *_pBtn)
{
	/* 如果没有初始化按键函数，则报错
	if (_pBtn->IsKeyDownFunc == 0)
	{
		printf("Fault : DetectButton(), _pBtn->IsKeyDownFunc undefine");
	}
	*/

	if (_pBtn->IsKeyDownFunc())
	{
		if (_pBtn->Count < _pBtn->FilterTime)
		{
			_pBtn->Count = _pBtn->FilterTime;
		}
		else if(_pBtn->Count < 2 * _pBtn->FilterTime)
		{
			_pBtn->Count++;
		}
		else
		{
			if (_pBtn->State == 0)
			{
				_pBtn->State = 1;

				/* 发送按钮按下的消息 */
				if (_pBtn->KeyCodeDown > 0)
				{
					/* 键值放入按键FIFO */
					bsp_PutKey(_pBtn->KeyCodeDown);
				}
			}

			if (_pBtn->LongTime > 0)
			{
				if (_pBtn->LongCount < _pBtn->LongTime)
				{
					/* 发送按钮持续按下的消息 */
					if (++_pBtn->LongCount == _pBtn->LongTime)
					{
						/* 键值放入按键FIFO */
						bsp_PutKey(_pBtn->KeyCodeLong);						
					}
				}
				else
				{
					if (_pBtn->RepeatSpeed > 0)
					{
						if (++_pBtn->RepeatCount >= _pBtn->RepeatSpeed)
						{
							_pBtn->RepeatCount = 0;
							/* 常按键后，每隔10ms发送1个按键 */
							bsp_PutKey(_pBtn->KeyCodeDown);												
		
						}
					}
				}
			}
		}
	}
	else
	{
		if(_pBtn->Count > _pBtn->FilterTime)
		{
			_pBtn->Count = _pBtn->FilterTime;
		}
		else if(_pBtn->Count != 0)
		{
			_pBtn->Count--;
		}
		else
		{
			if (_pBtn->State == 1)
			{
				_pBtn->State = 0;

				/* 发送按钮弹起的消息 */
				if (_pBtn->KeyCodeUp > 0)
				{
					/* 键值放入按键FIFO */
					bsp_PutKey(_pBtn->KeyCodeUp);			
				}
			}
		}

		_pBtn->LongCount = 0;
		_pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyPro
*	功能说明: 检测所有按键。非阻塞状态，必须被周期性的调用。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_KeyPro(void)
{
	bsp_DetectButton(&s_BtnUser);		/* USER 键 */
	bsp_DetectButton(&s_BtnTamper);		/* TAMPER 键 */
	bsp_DetectButton(&s_BtnWakeUp);		/* WAKEUP 键 */
	bsp_DetectButton(&s_BtnUp);			/* 摇杆UP键 */
	bsp_DetectButton(&s_BtnDown);		/* 摇杆DOWN键 */
	bsp_DetectButton(&s_BtnLeft);		/* 摇杆LEFT键 */
	bsp_DetectButton(&s_BtnRight);		/* 摇杆RIGHT键 */
	bsp_DetectButton(&s_BtnOk);			/* 摇杆OK键 */
	bsp_DetectButton(&s_BtnUserTamper);	/* 组合键 */
}
/****************************************************************************************************************************************************************************

                                                                                                                   模块名称 : TFT液晶显示器驱动模块
	                                                                                                           文件名称 : bsp_tft_lcd.c
*****************************************************************************************************************************************************************************
/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : bsp_tft_lcd.c
*	版    本 : V2.3
*	说    明 : 安富莱开发板标配的TFT显示器分辨率为240x400，3.0寸宽屏，带PWM背光调节功能。
*				支持的LCD内部驱动芯片型号有：SPFD5420A、OTM4001A、R61509V
*				驱动芯片的访问地址为:  0x60000000
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-08-21 armfly  ST固件库版本 V3.5.0版本。
*					a) 取消访问寄存器的结构体，直接定义
*		V2.0    2011-10-16 armfly  增加R61509V驱动，实现图标显示函数
*		V2.1    2012-07-06 armfly  增加RA8875驱动，支持4.3寸屏
*		V2.2    2012-07-13 armfly  改进LCD_DispStr函数，支持12点阵字符;修改LCD_DrawRect,解决差一个像素问题
*		V2.3    2012-08-08 armfly  将底层芯片寄存器操作相关的函数放到单独的文件，支持RA8875
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

/*
	友情提示：
	TFT控制器和一般的12864点阵显示器的控制器最大的区别在于引入了窗口绘图的机制，这个机制对于绘制局部图形
	是非常有效的。TFT可以先指定一个绘图窗口，然后所有的读写显存的操作均在这个窗口之内，因此它不需要CPU在
	内存中保存整个屏幕的像素数据。
*/

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "bsp_timer.h"
#include "bsp_tft_lcd.h"
#include "LCD_SPFD5420.h"
#include "LCD_RA8875.h"
#include "fonts.h"

/* 下面3个变量，主要用于使程序同时支持不同的屏 */
uint16_t g_ChipID = IC_4001;		/* 驱动芯片ID */
uint16_t g_LcdHeight = 240;			/* 显示屏分辨率-高度 */
uint16_t g_LcdWidth = 400;			/* 显示屏分辨率-宽度 */
uint8_t s_ucBright;					/* 背光亮度参数 */

static void LCD_CtrlLinesConfig(void);
static void LCD_FSMCConfig(void);

/*
*********************************************************************************************************
*	函 数 名: LCD_InitHard
*	功能说明: 初始化LCD
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_InitHard(void)
{
	uint16_t id;

	/* 配置LCD控制口线GPIO */
	LCD_CtrlLinesConfig();

	/* 配置FSMC接口，数据总线 */
	LCD_FSMCConfig();

	/* FSMC重置后必须加延迟才能访问总线设备  */
	bsp_DelayMS(20);

	id = SPFD5420_ReadID();  	/* 读取LCD驱动芯片ID */
	if ((id == 0x5420) || (id ==  0xB509))
	{
		SPFD5420_InitHard();	/* 初始化5420和4001屏硬件 */
		/* g_ChipID 在函数内部设置了 */
	}
	else
	{
		if (RA8875_ReadID() == 0x75)
		{
			g_ChipID = IC_8875;
			RA8875_InitHard();	/* 初始化RA8875芯片 */
		}
		else
		{
			/* 如果未能成功识别，则缺省按4001屏处理 */
			g_ChipID = IC_4001;
			g_LcdHeight = LCD_30_HEIGHT;
			g_LcdWidth = LCD_30_WIDTH;
		}
	}

	/* 清除显存 */
	LCD_ClrScr(CL_BLACK);	/* 黑色 */
}


/*
*********************************************************************************************************
*	函 数 名: LCD_GetChipDescribe
*	功能说明: 读取LCD驱动芯片的描述符号，用于显示
*	形    参：char *_str : 描述符字符串填入此缓冲区
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_GetChipDescribe(char *_str)
{
	switch (g_ChipID)
	{
		case IC_5420:
			strcpy(_str, CHIP_STR_5420);
			break;

		case IC_4001:
			strcpy(_str, CHIP_STR_4001);
			break;

		case IC_61509:
			strcpy(_str, CHIP_STR_61509);
			break;

		case IC_8875:
			strcpy(_str, CHIP_STR_8875);
			break;

		default:
			strcpy(_str, "Unknow");
			break;
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetHeight
*	功能说明: 读取LCD分辨率之高度
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t LCD_GetHeight(void)
{
	return g_LcdHeight;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetWidth
*	功能说明: 读取LCD分辨率之宽度
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t LCD_GetWidth(void)
{
	return g_LcdWidth;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DispOn
*	功能说明: 打开显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispOn(void)
{
	if (g_ChipID == IC_8875)
	{
		RA8875_DispOn();
	}
	else	/* 61509, 5420, 4001 */
	{
		SPFD5420_DispOn();
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DispOff
*	功能说明: 关闭显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispOff(void)
{
	if (g_ChipID == IC_8875)
	{
		RA8875_DispOff();
	}
	else	/* 61509, 5420, 4001 */
	{
		SPFD5420_DispOff();
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_ClrScr
*	功能说明: 根据输入的颜色值清屏
*	形    参：_usColor : 背景色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_ClrScr(uint16_t _usColor)
{
	if (g_ChipID == IC_8875)	/* RA8875 分支 */
	{
		RA8875_ClrScr(_usColor);
	}
	else	/* 5420，4001，61509 分支 */
	{
		SPFD5420_ClrScr(_usColor);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DispStr
*	功能说明: 在LCD指定坐标（左上角）显示一个字符串
*	形    参：
*		_usX : X坐标，对于3.0寸宽屏，范围为【0 - 399】
*		_usY : Y坐标，对于3.0寸宽屏，范围为 【0 - 239】
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
{
	uint32_t i;
	uint8_t code1;
	uint8_t code2;
	uint32_t address;
	uint8_t buf[24 * 24 / 8];	/* 最大支持24点阵汉字 */
	uint8_t m, width;
	uint8_t font_width,font_height, font_bytes;
	uint16_t x, y;
	const uint8_t *pAscDot;
	const uint8_t *pHzDot;

	/* 如果字体结构为空指针，则缺省按16点阵 */
	if (_tFont->usFontCode == FC_ST_12)
	{
		font_height = 12;
		font_width = 12;
		font_bytes = 24;
		pAscDot = g_Ascii12;
		pHzDot = g_Hz12;
	}
	else
	{
		/* 缺省是16点阵 */
		font_height = 16;
		font_width = 16;
		font_bytes = 32;
		pAscDot = g_Ascii16;
		pHzDot = g_Hz16;
	}

	/* 开始循环处理字符 */
	while (*_ptr != 0)
	{
		code1 = *_ptr;	/* 读取字符串数据， 该数据可能是ascii代码，也可能汉字代码的高字节 */
		if (code1 < 0x80)
		{
			/* 将ascii字符点阵复制到buf */
			memcpy(buf, &pAscDot[code1 * (font_bytes / 2)], (font_bytes / 2));
			width = font_width / 2;
		}
		else
		{
			code2 = *++_ptr;
			if (code2 == 0)
			{
				break;
			}

			/* 计算16点阵汉字点阵地址
				ADDRESS = [(code1-0xa1) * 94 + (code2-0xa1)] * 32
				;
			*/
			#ifdef USE_SMALL_FONT
				m = 0;
				while(1)
				{
					address = m * (font_bytes + 2);
					m++;
					if ((code1 == pHzDot[address + 0]) && (code2 == pHzDot[address + 1]))
					{
						address += 2;
						memcpy(buf, &pHzDot[address], font_bytes);
						break;
					}
					else if ((pHzDot[address + 0] == 0xFF) && (pHzDot[address + 1] == 0xFF))
					{
						/* 字库搜索完毕，未找到，则填充全FF */
						memset(buf, 0xFF, font_bytes);
						break;
					}
				}
			#else	/* 用全字库 */
				/* 此处需要根据字库文件存放位置进行修改 */
				address = ((code1-0xa1) * 94 + (code2-0xa1)) * font_bytes + HZK16_ADDR;
				memcpy(buf, (const uint8_t *)address, font_bytes);
			#endif

				width = font_width;
		}

		y = _usY;
		/* 开始刷LCD */
		for (m = 0; m < font_height; m++)	/* 字符高度 */
		{
			x = _usX;
			for (i = 0; i < width; i++)	/* 字符宽度 */
			{
				if ((buf[m * ((2 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
				{
					LCD_PutPixel(x, y, _tFont->usTextColor);	/* 设置像素颜色为文字色 */
				}
				else
				{
					LCD_PutPixel(x, y, _tFont->usBackColor);	/* 设置像素颜色为文字背景色 */
				}

				x++;
			}
			y++;
		}

		if (_tFont->usSpace > 0)
		{
			/* 如果文字底色按_tFont->usBackColor，并且字间距大于点阵的宽度，那么需要在文字之间填充(暂时未实现) */
		}
		_usX += width + _tFont->usSpace;	/* 列地址递增 */
		_ptr++;			/* 指向下一个字符 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_PutPixel
*	功能说明: 画1个像素
*	形    参：
*			_usX,_usY : 像素坐标
*			_usColor  ：像素颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	if (g_ChipID == IC_8875)
	{
		RA8875_PutPixel(_usX, _usY, _usColor);
	}
	else
	{
		SPFD5420_PutPixel(_usX, _usY, _usColor);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetPixel
*	功能说明: 读取1个像素
*	形    参：
*			_usX,_usY : 像素坐标
*			_usColor  ：像素颜色
*	返 回 值: RGB颜色值
*********************************************************************************************************
*/
uint16_t LCD_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint16_t usRGB;

	if (g_ChipID == IC_8875)
	{
		usRGB = RA8875_GetPixel(_usX, _usY);
	}
	else
	{
		usRGB = SPFD5420_GetPixel(_usX, _usY);
	}

	return usRGB;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawLine
*	功能说明: 采用 Bresenham 算法，在2点间画一条直线。
*	形    参：
*			_usX1, _usY1 ：起始点坐标
*			_usX2, _usY2 ：终止点Y坐标
*			_usColor     ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor)
{
	if (g_ChipID == IC_8875)
	{
		RA8875_DrawLine(_usX1 , _usY1 , _usX2, _usY2 , _usColor);
	}
	else
	{
		SPFD5420_DrawLine(_usX1 , _usY1 , _usX2, _usY2 , _usColor);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawPoints
*	功能说明: 采用 Bresenham 算法，绘制一组点，并将这些点连接起来。可用于波形显示。
*	形    参：
*			x, y     ：坐标数组
*			_usColor ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor)
{
	uint16_t i;

	for (i = 0 ; i < _usSize - 1; i++)
	{
		LCD_DrawLine(x[i], y[i], x[i + 1], y[i + 1], _usColor);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawRect
*	功能说明: 绘制水平放置的矩形。
*	形    参：
*			_usX,_usY：矩形左上角的坐标
*			_usHeight ：矩形的高度
*			_usWidth  ：矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	if (g_ChipID == IC_8875)
	{
		RA8875_DrawRect(_usX, _usY, _usHeight, _usWidth, _usColor);
	}
	else
	{
		SPFD5420_DrawRect(_usX, _usY, _usHeight, _usWidth, _usColor);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawCircle
*	功能说明: 绘制一个圆，笔宽为1个像素
*	形    参：
*			_usX,_usY  ：圆心的坐标
*			_usRadius  ：圆的半径
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	if (g_ChipID == IC_8875)
	{
		RA8875_DrawCircle(_usX, _usY, _usRadius, _usColor);
	}
	else
	{
		SPFD5420_DrawCircle(_usX, _usY, _usRadius, _usColor);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawBMP
*	功能说明: 在LCD上显示一个BMP位图，位图点阵扫描次序：从左到右，从上到下
*	形    参：
*			_usX, _usY : 图片的坐标
*			_usHeight  ：图片高度
*			_usWidth   ：图片宽度
*			_ptr       ：图片点阵指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	if (g_ChipID == IC_8875)
	{
		RA8875_DrawBMP(_usX, _usY, _usHeight, _usWidth, _ptr);
	}
	else
	{
		SPFD5420_DrawBMP(_usX, _usY, _usHeight, _usWidth, _ptr);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawIcon
*	功能说明: 在LCD上绘制一个图标，四角自动切为弧脚
*	形    参：
*			_usX, _usY : 图片的坐标
*			_usHeight  ：图片高度
*			_usWidth   ：图片宽度
*			_ptr       ：图片点阵指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawIcon(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	const uint16_t *p;
	uint16_t usNewRGB;
	uint16_t x, y;		/* 用于记录窗口内的相对坐标 */

	p = _ptr;
	for (y = 0; y < _usHeight; y++)
	{
		for (x = 0; x < _usWidth; x++)
		{
			usNewRGB = *p++;	/* 读取图标的颜色值后指针加1 */
			/* 将图标的4个直角切割为弧角，弧角外是背景图标 */
			if ((y == 0 && (x < 6 || x > _usWidth - 7)) ||
				(y == 1 && (x < 4 || x > _usWidth - 5)) ||
				(y == 2 && (x < 3 || x > _usWidth - 4)) ||
				(y == 3 && (x < 2 || x > _usWidth - 3)) ||
				(y == 4 && (x < 1 || x > _usWidth - 2)) ||
				(y == 5 && (x < 1 || x > _usWidth - 2))	||

				(y == _usHeight - 1 && (x < 6 || x > _usWidth - 7)) ||
				(y == _usHeight - 2 && (x < 4 || x > _usWidth - 5)) ||
				(y == _usHeight - 3 && (x < 3 || x > _usWidth - 4)) ||
				(y == _usHeight - 4 && (x < 2 || x > _usWidth - 3)) ||
				(y == _usHeight - 5 && (x < 1 || x > _usWidth - 2)) ||
				(y == _usHeight - 6 && (x < 1 || x > _usWidth - 2))
				)
			{
				;
			}
			else
			{
				LCD_PutPixel(x + _usX, y + _usY, usNewRGB);
			}
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawIconActive
*	功能说明: 在LCD上绘制一个激活选中的图标
*	形    参：
*			_usX, _usY : 图片的坐标
*			_usHeight  ：图片高度
*			_usWidth   ：图片宽度
*			_ptr       ：图片点阵指针
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawIconActive(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	const uint16_t *p;
	uint16_t usNewRGB;
	uint16_t x, y;		/* 用于记录窗口内的相对坐标 */

	p = _ptr;
	for (y = 0; y < _usHeight; y++)
	{
		for (x = 0; x < _usWidth; x++)
		{
			usNewRGB = *p++;	/* 读取图标的颜色值后指针加1 */
			if ((y == 0 && (x < 6 || x > _usWidth - 7)) ||
				(y == 1 && (x < 4 || x > _usWidth - 5)) ||
				(y == 2 && (x < 3 || x > _usWidth - 4)) ||
				(y == 3 && (x < 2 || x > _usWidth - 3)) ||
				(y == 4 && (x < 1 || x > _usWidth - 2)) ||
				(y == 5 && (x < 1 || x > _usWidth - 2))	||

				(y == _usHeight - 1 && (x < 6 || x > _usWidth - 7)) ||
				(y == _usHeight - 2 && (x < 4 || x > _usWidth - 5)) ||
				(y == _usHeight - 3 && (x < 3 || x > _usWidth - 4)) ||
				(y == _usHeight - 4 && (x < 2 || x > _usWidth - 3)) ||
				(y == _usHeight - 5 && (x < 1 || x > _usWidth - 2)) ||
				(y == _usHeight - 6 && (x < 1 || x > _usWidth - 2))
				)
			{
				;
			}
			else
			{
				/* 降低原始像素的亮度，实现图标被激活选中的效果 */
				uint16_t R,G,B;
				uint16_t bright = 15;

				/* rrrr rggg gggb bbbb */
				R = (usNewRGB & 0xF800) >> 11;
				G = (usNewRGB & 0x07E0) >> 5;
				B =  usNewRGB & 0x001F;
				if (R > bright)
				{
					R -= bright;
				}
				else
				{
					R = 0;
				}
				if (G > 2 * bright)
				{
					G -= 2 * bright;
				}
				else
				{
					G = 0;
				}
				if (B > bright)
				{
					B -= bright;
				}
				else
				{
					B = 0;
				}

				LCD_PutPixel(_usX + x, _usY + y, (R << 11) + (G << 5) + B);
			}
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_CtrlLinesConfig
*	功能说明: 配置LCD控制口线，FSMC管脚设置为复用功能
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void LCD_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	/* 使能 FSMC, GPIOD, GPIOE 和 AFIO 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
	                     RCC_APB2Periph_AFIO, ENABLE);

	/* 设置 PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
	 PD.10(D15), PD.14(D0), PD.15(D1) 为复用推挽输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
	                            GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* 设置 PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
	 PE.14(D11), PE.15(D12) 为复用推挽输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* 设置 PD.13(A18 (RS))  为复用推挽输出 ,改为A16*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* 设置 PD.7(NE1 (LCD/CS)) 为复用推挽输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_FSMCConfig
*	功能说明: 配置FSMC并口访问时序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void LCD_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  init;
	FSMC_NORSRAMTimingInitTypeDef  timing;

	/*-- FSMC Configuration ------------------------------------------------------*/
	/*----------------------- SRAM Bank 4 ----------------------------------------*/
	/* FSMC_Bank1_NORSRAM4 configuration */
	timing.FSMC_AddressSetupTime = 1;
	timing.FSMC_AddressHoldTime = 0;
	timing.FSMC_DataSetupTime = 2;
	timing.FSMC_BusTurnAroundDuration = 0;
	timing.FSMC_CLKDivision = 0;
	timing.FSMC_DataLatency = 0;
	timing.FSMC_AccessMode = FSMC_AccessMode_A;

	/*
	 LCD configured as follow:
	    - Data/Address MUX = Disable
	    - Memory Type = SRAM
	    - Data Width = 16bit
	    - Write Operation = Enable
	    - Extended Mode = Enable
	    - Asynchronous Wait = Disable
	*/
	init.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	init.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	init.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	init.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	init.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	init.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;	/* 注意旧库无这个成员 */
	init.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	init.FSMC_WrapMode = FSMC_WrapMode_Disable;
	init.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	init.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	init.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	init.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	init.FSMC_WriteBurst = FSMC_WriteBurst_Disable;

	init.FSMC_ReadWriteTimingStruct = &timing;
	init.FSMC_WriteTimingStruct = &timing;

	FSMC_NORSRAMInit(&init);

	/* - BANK 3 (of NOR/SRAM Bank 1~4) is enabled */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_SetBackLight
*	功能说明: 初始化控制LCD背景光的GPIO,配置为PWM模式。
*			当关闭背光时，将CPU IO设置为浮动输入模式（推荐设置为推挽输出，并驱动到低电平)；将TIM3关闭 省电
*	形    参：_bright 亮度，0是灭，255是最亮
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_SetBackLight(uint8_t _bright)
{
	s_ucBright =  _bright;	/* 保存背光值 */

	if (g_ChipID == IC_8875)
	{
		RA8875_SetBackLight(_bright);
	}
	else
	{
		SPFD5420_SetBackLight(_bright);
	}
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetBackLight
*	功能说明: 获得背光亮度参数
*	形    参：无
*	返 回 值: 背光亮度参数
*********************************************************************************************************
*/
uint8_t LCD_GetBackLight(void)
{
	return s_ucBright;
}

/****************************************************************************************************************************************************************************

	                                                                                      模块名称 : TFT液晶显示器驱动模块
	                                                                                      文件名称 : LCD_tft_lcd.h
*****************************************************************************************************************************************************************************
/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : LCD_tft_lcd.h
*	版    本 : V2.0
*	说    明 : 头文件
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-08-21 armfly  ST固件库版本 V3.5.0版本。
*		v2.0    2011-10-16 armfly  ST固件库版本 V3.5.0版本。规范函数接口，优化内部结构
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/


#ifndef _BSP_TFT_LCD_H
#define _BSP_TFT_LCD_H

#include "stm32f10x.h"


/* 定义LCD显示区域的分辨率 */
#define LCD_30_HEIGHT	240		/* 3.0寸宽屏 高度，单位：像素 */
#define LCD_30_WIDTH	400		/* 3.0寸宽屏 宽度，单位：像素 */

#define LCD_43_HEIGHT	272		/* 4.3寸宽屏 高度，单位：像素 */
#define LCD_43_WIDTH	480		/* 4.3寸宽屏 宽度，单位：像素 */

#define LCD_70_HEIGHT	480		/* 7.0寸宽屏 高度，单位：像素 */
#define LCD_70_WIDTH	800		/* 7.0寸宽屏 宽度，单位：像素 */

/* 支持的驱动芯片ID */
enum
{
	IC_5420		= 0x5420,
	IC_4001		= 0x4001,
	IC_61509 	= 0xB509,
	IC_8875 	= 0x0075
};

#define CHIP_STR_5420	"SPFD5420A"
#define CHIP_STR_4001	"OTM4001A"
#define CHIP_STR_61509	"R61509V"
#define CHIP_STR_8875	"RA8875"

/* LCD 颜色代码，CL_是Color的简写 */
enum
{
	CL_WHITE        = 0xFFFF,	/* 白色 */
	CL_BLACK        = 0x0000,	/* 黑色 */
	CL_GREY         = 0xF7DE,	/* 灰色 */
	CL_BLUE         = 0x001F,	/* 蓝色 */
	CL_BLUE2        = 0x051F,	/* 浅蓝色 */
	CL_RED          = 0xF800,	/* 红色 */
	CL_MAGENTA      = 0xF81F,	/* 红紫色，洋红色 */
	CL_GREEN        = 0x07E0,	/* 绿色 */
	CL_CYAN         = 0x7FFF,	/* 蓝绿色，青色 */
	CL_YELLOW       = 0xFFE0,	/* 黄色 */
	CL_MASK			= 0x9999	/* 颜色掩码，用于文字背景透明 */
};

/* 字体代码 */

/* 字体代码 */
enum
{
	FC_ST_16 = 0,		/* 宋体15x16点阵 （宽x高） */
	FC_ST_12 = 1		/* 宋体12x12点阵 （宽x高） */
};

/* 字体属性结构, 用于LCD_DispStr() */
typedef struct
{
	uint16_t usFontCode;	/* 字体代码 0 表示16点阵 */
	uint16_t usTextColor;	/* 字体颜色 */
	uint16_t usBackColor;	/* 文字背景颜色，透明 */
	uint16_t usSpace;		/* 文字间距，单位 = 像素 */
}FONT_T;

/* 背景光控制 */
#define BRIGHT_MAX		255
#define BRIGHT_MIN		0
#define BRIGHT_DEFAULT	200
#define BRIGHT_STEP		5

/* 可供外部模块调用的函数 */
void LCD_InitHard(void);
void LCD_GetChipDescribe(char *_str);
uint16_t LCD_GetHeight(void);
uint16_t LCD_GetWidth(void);
void LCD_DispOn(void);
void LCD_DispOff(void);
void LCD_ClrScr(uint16_t _usColor);
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);
void LCD_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t LCD_GetPixel(uint16_t _usX, uint16_t _usY);
void LCD_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);
void LCD_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint16_t _usColor);
void LCD_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void LCD_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void LCD_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void LCD_DrawIcon(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void LCD_DrawIconActive(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void LCD_SetBackLight(uint8_t _bright);
uint8_t LCD_GetBackLight(void);

/* 下面3个变量，主要用于使程序同时支持不同的屏 */
extern uint16_t g_ChipID;			/* 驱动芯片ID */
extern uint16_t g_LcdHeight;		/* 显示屏分辨率-高度 */
extern uint16_t g_LcdWidth;			/* 显示屏分辨率-宽度 */

#endif
/****************************************************************************************************************************************************************************

	                                                                                      	模块名称 : 电阻式触摸板驱动模块
	                                                                                        文件名称 : bsp_touch.h
*****************************************************************************************************************************************************************************

/*
*********************************************************************************************************
*
*	模块名称 : 电阻式触摸板驱动模块
*	文件名称 : bsp_touch.c
*	版    本 : V1.0
*	说    明 : 驱动TS2046芯片 和 RA8875内置触摸
*	修改记录 :
*		版本号  日期        作者    说明
*       v1.0    2012-07-06 armfly  ST固件库V3.5.0版本。
*
*	Copyright (C), 2010-2012, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "stm32f10x.h"
#include <stdio.h>
#include <math.h>

#include "bsp_touch.h"
#include "bsp_tft_lcd.h"
#include "LCD_RA8875.h"
#include "bsp_timer.h"

/*
【1】安富莱STM32开发板 + 3.0寸显示模块， 显示模块上的触摸芯片为 TSC2046或其兼容芯片
	PG11  --> TP_CS
	PB5   --> TP_BUSY
	PA5   --> TP_SCK
	PA6   --> TP_MISO
	PA7   --> TP_MOSI
	PC5   --> TP_PEN_INT

【2】安富莱STM32开发板 + 4.3寸或7寸显示模块（内置RA8875芯片)
	RA8875内置触摸屏接口，因此直接通过FSMC访问RA8875相关寄存器即可。


	本程序未使用触笔中断功能。在1ms周期的 Systick定时中断服务程序中对触摸ADC值进行采样和
	滤波处理。当触笔按下超过40ms后，开始采集ADC值（每1ms采集1次，连续采集10次），然后对
	这10个样本进行排序，丢弃2头的样本，对中间一段样本进行求和并计算平均值。

	采样2点校准方法，校准后保存2个校准点的ADC值，实际工作时，根据2点直线方程计算屏幕坐标。
	校准参数有保存接口，本程序主要用于演示，未做保存功能。
	大家可以自己修改  TOUCH_SaveParam() 和 TOUCH_LoadParam() 两个函数实现保存功能。

*/

/* TSC2046 片选 */
#define TSC2046_CS_1()	GPIOG->BSRR = 	GPIO_Pin_11
#define TSC2046_CS_0()	GPIOG->BRR  = 	GPIO_Pin_11

/* TSC2046 内部ADC通道号 */
#define ADC_CH_X	1		/* X通道，测量X位置 */
#define ADC_CH_Y	5		/* Y通道，测量Y位置 */

#define SAMPLE_COUNT	10	/* 每次连续采集多少个样本 */

/*
触摸屏校准点相对屏幕像素四角的偏移像素
第1个点 ： x1 = CALIB_OFFSET, y1 = CALIB_OFFSET
第2个点 ： x2 = LCD_GetWidth() - CALIB_OFFSET, y2 = LCD_GetHeight() - CALIB_OFFSET
*/
#define CALIB_OFFSET	20
#define TP_X1	CALIB_OFFSET
#define TP_Y1	CALIB_OFFSET
#define TP_X2	(LCD_GetWidth() - CALIB_OFFSET)
#define TP_Y2	(LCD_GetHeight() - CALIB_OFFSET)

/* 有效ADC值的判断门限. 太接近ADC临界值的坐标认为无效 */
#define ADC_VALID_OFFSET	2

//#define WaitTPReady() while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5))
#define WaitTPReady() {}

/* 触屏模块用到的全局变量 */
TOUCH_T g_tTP;

static void TSC2046_InitHard(void);
static uint16_t TSC2046_ReadAdc(uint8_t _ucCh);
static uint8_t	TOUCH_PressValid(uint16_t _usX, uint16_t _usY);
static uint16_t TOUCH_DataFilter(uint16_t *_pBuf, uint8_t _ucCount);
static void TOUCH_LoadParam(void);
static void TOUCH_SaveParam(void);
static int32_t CalTwoPoint(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x);
static int16_t TOUCH_TransX(uint16_t _usAdcX, uint16_t _usAdcY);
static int16_t TOUCH_TransY(uint16_t _usAdcX, uint16_t _usAdcY);

/*
*********************************************************************************************************
*	函 数 名: bsp_InitTouch
*	功能说明: 配置STM32和触摸相关的口线，使能硬件SPI1, 片选由软件控制
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TOUCH_InitHard(void)
{
	if (g_ChipID == IC_8875)
	{
		RA8875_TouchInit();

		g_tTP.usMaxAdc = 1023;	/* 10位ADC */
	}
	else
	{
		TSC2046_InitHard();

		g_tTP.usMaxAdc = 4095;	/* 12位ADC */
	}

	TOUCH_LoadParam();	/* 读取校准参数 */

	g_tTP.Enable = 1;
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_ReadAdcX
*	功能说明: 获得触摸板X方向ADC采样值， 已进行滤波处理
*	形    参：无
*	返 回 值: X 方向ADC值
*********************************************************************************************************
*/
uint16_t TOUCH_ReadAdcX(void)
{
	uint16_t usAdc;

	__set_PRIMASK(1);  		/* 关中断 */
	usAdc = g_tTP.usAdcNowX;
	__set_PRIMASK(0);  		/* 开中断 */

	return usAdc;
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_ReadAdcY
*	功能说明: 获得触摸板Y方向ADC采样值， 已进行滤波处理
*	形    参：无
*	返 回 值: Y 坐标值，允许负值
*********************************************************************************************************
*/
uint16_t TOUCH_ReadAdcY(void)
{
	uint16_t usAdc;

	__set_PRIMASK(1);  		/* 关中断 */
	usAdc = g_tTP.usAdcNowY;
	__set_PRIMASK(0);  		/* 开中断 */

	return usAdc;
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_PutKey
*	功能说明: 将1个触摸点坐标值压入触摸FIFO缓冲区。可用于模拟一个按键。
*	形    参: _usX, _usY 坐标值
*	返 回 值: 无
*********************************************************************************************************
*/
void TOUCH_PutKey(uint8_t _ucEvent, uint16_t _usX, uint16_t _usY)
{
	g_tTP.Event[g_tTP.Write] = _ucEvent;
	g_tTP.XBuf[g_tTP.Write] = TOUCH_TransX(_usX, _usY);
	g_tTP.YBuf[g_tTP.Write] = TOUCH_TransY(_usX, _usY);

	if (++g_tTP.Write  >= TOUCH_FIFO_SIZE)
	{
		g_tTP.Write = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_GetKey
*	功能说明: 从触摸FIFO缓冲区读取一个坐标值。
*	形    参：无
*	返 回 值: 1表示有未处理的数据
*********************************************************************************************************
*/
uint8_t TOUCH_GetKey(int16_t *_pX, int16_t *_pY)
{
	uint8_t ret;

	if (g_tTP.Read == g_tTP.Write)
	{
		return TOUCH_NONE;
	}
	else
	{
		ret = g_tTP.Event[g_tTP.Read];
		*_pX = g_tTP.XBuf[g_tTP.Read];
		*_pY = g_tTP.YBuf[g_tTP.Read];

		if (++g_tTP.Read >= TOUCH_FIFO_SIZE)
		{
			g_tTP.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_InRect
*	功能说明: 判断当前坐标是否位于矩形框内
*	形    参：_usX, _usY: 输入坐标
*			_usRectX,_usRectY: 矩形起点
*			_usRectH、_usRectW : 矩形高度和宽度
*	返 回 值: 1 表示在范围内
*********************************************************************************************************
*/
uint8_t TOUCH_InRect(uint16_t _usX, uint16_t _usY,
	uint16_t _usRectX, uint16_t _usRectY, uint16_t _usRectH, uint16_t _usRectW)
{
	if ((_usX > _usRectX) && (_usX < _usRectX + _usRectW)
		&& (_usY > _usRectY) && (_usY < _usRectY + _usRectH))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_Scan
*	功能说明: 触摸板事件检测程序。该函数被周期性调用，每ms调用1次. 见 bsp_Timer.c
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TOUCH_Scan(void)
{
	uint16_t usAdcX;
	uint16_t usAdcY;
	static uint16_t s_usXBuf[SAMPLE_COUNT];
	static uint16_t s_usYBuf[SAMPLE_COUNT];
	static uint8_t s_ucPos = 0;
	static uint8_t s_count = 0;
	static uint8_t s_down = 0;

	if (g_tTP.Enable == 0)
	{
		return;
	}

	/* 获得原始的ADC值，未滤波 */
	if (g_ChipID == IC_8875)
	{
		/* 如果主程序正在访问RA8875,则丢弃本次触摸采集，避免影响显示 */
		if (RA8875_IsBusy())
		{
			return;
		}

		usAdcX = RA8875_TouchReadX();
		usAdcY = RA8875_TouchReadY();
	}
	else
	{
		usAdcX = TSC2046_ReadAdc(ADC_CH_X);
		usAdcY = TSC2046_ReadAdc(ADC_CH_Y);
	}

	if (TOUCH_PressValid(usAdcX, usAdcY))
	{
		/* 按压40ms之后才开始采集数据 */
		if (s_count >= 40)
		{
			s_usXBuf[s_ucPos] = usAdcX;
			s_usYBuf[s_ucPos] = usAdcY;

			/* 采集10ms数据进行滤波 */
			if (++s_ucPos >= SAMPLE_COUNT)
			{
				s_ucPos = 0;

			   	/* 对ADC采样值进行软件滤波 */
				g_tTP.usAdcNowX = TOUCH_DataFilter(s_usXBuf, SAMPLE_COUNT);
				g_tTP.usAdcNowY = TOUCH_DataFilter(s_usYBuf, SAMPLE_COUNT);

				if (s_down == 0)
				{
					s_down = 1;
					/* 触摸按下事件 */
					TOUCH_PutKey(TOUCH_DOWN, g_tTP.usAdcNowX, g_tTP.usAdcNowY);
				}
				else
				{
					/* 触摸移动事件 */
					TOUCH_PutKey(TOUCH_MOVE, g_tTP.usAdcNowX, g_tTP.usAdcNowY);
				}
			}
		}
		else
		{
			s_count++;
		}
	}
	else
	{
		if (s_count > 0)
		{
			if (--s_count == 0)
			{
				/* 触摸释放事件 */
				TOUCH_PutKey(TOUCH_RELEASE, g_tTP.usAdcNowX, g_tTP.usAdcNowY);

				g_tTP.usAdcNowX = 0;
				g_tTP.usAdcNowY = 0;

				s_count = 0;
				s_down = 0;
			}
		}
		s_ucPos = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: CalTwoPoint
*	功能说明: 根据2点直线方程，计算Y值
*	形    参：2个点的坐标和x输入量
*	返 回 值: x对应的y值
*********************************************************************************************************
*/
static int32_t CalTwoPoint(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x)
{
	return y1 + ((int32_t)(y2 - y1) * (x - x1)) / (x2 - x1);
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_TransX
*	功能说明: 将触摸ADC值转换为像素坐标
*	形    参：无
*	返 回 值: X 坐标值，允许负值
*********************************************************************************************************
*/
static int16_t TOUCH_TransX(uint16_t _usAdcX, uint16_t _usAdcY)
{
	uint16_t x;
	int32_t y;

	if (g_tTP.XYChange == 0)
	{
		x = _usAdcX;
		if (x == 0)
		{
			y = 0;
		}
		else
		{
			y = CalTwoPoint(g_tTP.usAdcX1, TP_X1, g_tTP.usAdcX2, TP_X2, x);
		}
	}
	else
	{
		x = _usAdcY;
		if (x == 0)
		{
			y = 0;
		}
		else
		{
			y = CalTwoPoint(g_tTP.usAdcY1, TP_X1, g_tTP.usAdcY2, TP_X2, x);
		}
	}
	return y;
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_TransY
*	功能说明: 将触摸ADC值转换为像素坐标
*	形    参：无
*	返 回 值: Y 坐标值，允许负值
*********************************************************************************************************
*/
static int16_t TOUCH_TransY(uint16_t _usAdcX, uint16_t _usAdcY)
{
	int32_t x;
	int32_t y;

	if (g_tTP.XYChange == 0)
	{
		x = _usAdcY;
		if (x == 0)
		{
			y = 0;
		}
		else
		{
			y = CalTwoPoint(g_tTP.usAdcY1, TP_Y1, g_tTP.usAdcY2, TP_Y2, x);
		}
	}
	else
	{
		x = _usAdcX;
		if (x == 0)
		{
			y = 0;
		}
		else
		{
			y = CalTwoPoint(g_tTP.usAdcX1, TP_Y1, g_tTP.usAdcX2, TP_Y2, x);
		}
	}
	return y;
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_GetX
*	功能说明: 获得当前的的触摸坐标X
*	形    参：无
*	返 回 值: X 坐标值，允许负值
*********************************************************************************************************
*/
int16_t TOUCH_GetX(void)
{
	return TOUCH_TransX(TOUCH_ReadAdcX(), TOUCH_ReadAdcY());
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_GetY
*	功能说明: 获得当前的的触摸坐标Y
*	形    参：无
*	返 回 值: Y 坐标值，允许负值
*********************************************************************************************************
*/
int16_t TOUCH_GetY(void)
{
	return TOUCH_TransY(TOUCH_ReadAdcX(), TOUCH_ReadAdcY());
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitTouch
*	功能说明: 配置STM32和触摸相关的口线，使能硬件SPI1, 片选由软件控制
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TSC2046_InitHard(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;

	/* 开启GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
		RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG, ENABLE);

	/* 开启 SPI1 外设时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	/* 配置 PA5、PA6、PA7 为复用推挽输出，用于 SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/* 配置 PG11 脚为推挽输出，用于 TP_CS  */
	TSC2046_CS_1();
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	/* 配置 PB5 引脚为上拉输入，用于 TP_BUSY */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 配置 PC5 为浮空输入模式，用于触笔中断 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* 安富莱开发板，SPI1和串行Flash复用(PB2是其片选)，需要将PB2设置为高 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_2);

	/* 配置 SPI1工作模式 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 		/* 软件控制片选 */
	/*
		SPI_BaudRatePrescaler_64 对应SCK时钟频率约1M
		TSC2046 对SCK时钟的要求，高电平和低电平最小200ns，周期400ns，也就是2.5M

		示波器实测频率
		SPI_BaudRatePrescaler_64 时，SCK时钟频率约 1.116M
		SPI_BaudRatePrescaler_32 时，SCK时钟频率月 2.232M
	*/
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,&SPI_InitStructure);

	/* 使能 SPI1 */
	SPI_Cmd(SPI1,ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: SPI_ShiftByte
*	功能说明: 向SPI总线发送一个字节，同时返回接收到的字节
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static uint8_t SPI_ShiftByte(uint8_t _ucByte)
{
	uint8_t ucRxByte;

	/* 等待发送缓冲区空 */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	/* 发送一个字节 */
	SPI_I2S_SendData(SPI1, _ucByte);

	/* 等待数据接收完毕 */
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	/* 读取接收到的数据 */
	ucRxByte = SPI_I2S_ReceiveData(SPI1);

	/* 返回读到的数据 */
	return ucRxByte;
}

/*
*********************************************************************************************************
*	函 数 名: SpiDelay
*	功能说明: 向SPI总线发送一个字节，同时返回接收到的字节
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SpiDelay(uint32_t DelayCnt)
{
	uint32_t i;

	for(i=0;i<DelayCnt;i++);
}

/*
*********************************************************************************************************
*	函 数 名: TSC2046_ReadAdc
*	功能说明: 选择一个模拟通道，启动ADC，并返回ADC采样结果
*	形    参：_ucCh = 0 表示X通道； 1表示Y通道
*	返 回 值: 12位ADC值
*********************************************************************************************************
*/
static uint16_t TSC2046_ReadAdc(uint8_t _ucCh)
{
	uint16_t usAdc;

	TSC2046_CS_0();		/* 使能TS2046的片选 */

	/*
		TSC2046 控制字（8Bit）
		Bit7   = S     起始位，必须是1
		Bit6:4 = A2-A0 模拟输入通道选择A2-A0; 共有6个通道。
		Bit3   = MODE  ADC位数选择，0 表示12Bit;1表示8Bit
		Bit2   = SER/DFR 模拟输入形式，  1表示单端输入；0表示差分输入
		Bit1:0 = PD1-PD0 掉电模式选择位
	*/
	SPI_ShiftByte((1 << 7) | (_ucCh << 4));			/* 选择通道1, 测量X位置 */

	/* 读ADC结果, 12位ADC值的高位先传，前12bit有效，最后4bit填0 */
	usAdc = SPI_ShiftByte(0x00);		/* 发送的0x00可以为任意值，无意义 */
	usAdc <<= 8;
	usAdc += SPI_ShiftByte(0x00);		/* 获得12位的ADC采样值 */

	usAdc >>= 3;						/* 右移3位，保留12位有效数字 */

	TSC2046_CS_1();					/* 禁能片选 */

	return (usAdc);
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_DataFilter
*	功能说明: 对采样数据进行滤波
*	形    参：无
*	返 回 值: X 坐标值，允许负值
*********************************************************************************************************
*/
static uint16_t TOUCH_DataFilter(uint16_t *_pBuf, uint8_t _ucCount)
{
	uint8_t flag;
	uint8_t i;
	uint16_t usTemp;
	uint32_t uiSum;

	/* 升序排列 */
    do
	{
		flag = 0;
		for (i = 0; i < _ucCount - 1; i++)
		{
			if (_pBuf[i] > _pBuf[i+1])
			{
				usTemp = _pBuf[i + 1];
				_pBuf[i+1] = _pBuf[i];
				_pBuf[i] = usTemp;
				flag = 1;
			}
		}
	}while(flag);

	uiSum = 0;
	for (i = 0; i < _ucCount / 3; i++)
	{
		uiSum += _pBuf[_ucCount / 3 + i];
	}
	usTemp = uiSum / (_ucCount / 3);
	return usTemp;
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_DispPoint1
*	功能说明: 显示第1个校准点
*	形    参：_ucIndex = 0 : 表示第1个点； _ucIndex = 1 表示第2个点
*	返 回 值: 无
*********************************************************************************************************
*/
static void TOUCH_DispPoint(uint8_t _ucIndex)
{
	FONT_T tFont16;			/* 定义一个字体结构体变量，用于设置字体参数 */

	/* 设置字体参数 */
	{
		tFont16.usFontCode = FC_ST_16;	/* 字体代码 16点阵 */
		tFont16.usTextColor = CL_WHITE;		/* 字体颜色 0 或 1 */
		tFont16.usBackColor = CL_BLUE;	/* 文字背景颜色 */
		tFont16.usSpace = 0;			/* 文字间距，单位 = 像素 */
	}

/*
	第1个点 ： x1 = CALIB_OFFSET, y1 = CALIB_OFFSET
	第2个点 ： x2 = LCD_GetHeight() - CALIB_OFFSET, y2 = LCD_GetWidth - CALIB_OFFSET
*/
	if (_ucIndex == 0)
	{
		LCD_ClrScr(CL_BLUE);  		/* 清屏，背景蓝色 */

		LCD_DispStr(50, 10, "校准触摸屏", &tFont16);		/* 在(8,3)坐标处显示一串汉字 */

		LCD_DrawCircle(TP_X1, TP_Y1, 6, CL_WHITE);
	}
	else
	{
		LCD_DrawCircle(TP_X1, TP_Y1, 6, CL_BLUE);		/* 擦除第1个点 */

		LCD_DrawCircle(TP_X2, TP_Y2, 6, CL_WHITE);
	}
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_PressValid
*	功能说明: 判断按压是否有效，根据X, Y的ADC值进行大致判断
*	形    参：无
*	返 回 值: 1 表示有效； 0 表示无效
*********************************************************************************************************
*/
static uint8_t	TOUCH_PressValid(uint16_t _usX, uint16_t _usY)
{
	if ((_usX <= ADC_VALID_OFFSET) || (_usY <= ADC_VALID_OFFSET)
		|| (_usX >= g_tTP.usMaxAdc - ADC_VALID_OFFSET)
		|| (_usY >= g_tTP.usMaxAdc - ADC_VALID_OFFSET))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_WaitRelease
*	功能说明: 等待触笔释放
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TOUCH_WaitRelease(void)
{
	uint8_t usCount;

	for (;;)
	{
		if (TOUCH_PressValid(TOUCH_ReadAdcX(), TOUCH_ReadAdcY()) == 0)
		{
			if (++usCount > 5)
			{
				break;
			}
		}
		else
		{
			usCount = 0;
		}
		bsp_DelayMS(10);
	}
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_Calibration
*	功能说明: 触摸屏校准
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TOUCH_Calibration(void)
{
	uint16_t usAdcX, usAdcX1, usAdcX2;
	uint16_t usAdcY, usAdcY1, usAdcY2;
	uint8_t usCount;

	TOUCH_DispPoint(0);	/* 显示第1个校准点 */

	TOUCH_WaitRelease(); 	/* 等待触笔释放 */

	usCount = 0;
	for (;;)
	{
		usAdcX = TOUCH_ReadAdcX();
		usAdcY = TOUCH_ReadAdcY();

		if (TOUCH_PressValid(usAdcX, usAdcY))
		{
			if (++usCount > 5)
			{
				/* 按压有效, 保存校准点ADC采样值 */
				usAdcX1 = usAdcX;
				usAdcY1 = usAdcY;
				break;
			}
		}
		else
		{
			usCount = 0;
		}
		bsp_DelayMS(10);
	}

	TOUCH_DispPoint(1);		/* 显示第1个校准点 */

	TOUCH_WaitRelease(); 	/* 等待触笔释放 */

	usCount = 0;
	for (;;)
	{
		usAdcX = TOUCH_ReadAdcX();
		usAdcY = TOUCH_ReadAdcY();

		if (TOUCH_PressValid(usAdcX, usAdcY))
		{
			if (++usCount > 5)
			{
				/* 按压有效, 保存校准点ADC采样值 */
				usAdcX2 = usAdcX;
				usAdcY2 = usAdcY;
				break;
			}
		}
		else
		{
			usCount = 0;
		}
		bsp_DelayMS(10);
	}

	/* 识别触摸的 X, Y 和 显示面板的 X，Y 是否需要交换 */
	g_tTP.XYChange = 0;		/* 1表示X Y需要交换 */
	if (LCD_GetHeight() < LCD_GetWidth())
	{
		if (abs(usAdcX1 - usAdcX2) < abs(usAdcY1 - usAdcY2))
		{
			g_tTP.XYChange = 1;
		}
	}
	else
	{
		if (abs(usAdcX1 - usAdcX2) > abs(usAdcY1 - usAdcY2))
		{
			g_tTP.XYChange = 1;
		}
	}

	g_tTP.usAdcX1 = usAdcX1;
	g_tTP.usAdcY1 = usAdcY1;

	g_tTP.usAdcX2 = usAdcX2;
	g_tTP.usAdcY2 = usAdcY2;

	/* 在最后一步，可以将校准参数保存入Flash 或者EEPROM */
	TOUCH_SaveParam();
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_SaveParam
*	功能说明: 保存校准参数	s_usAdcX1 s_usAdcX2 s_usAdcY1 s_usAdcX2
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TOUCH_SaveParam(void)
{
#if 0
	/* 保存下面的4个变量即可 */
	uint16_t usBuf[5];

	usBuf[0] = g_tTP.usAdcX1;
	usBuf[1] = g_tTP.usAdcY1;

	usBuf[2] = g_tTP.usAdcX2;
	usBuf[3] = g_tTP.usAdcY2;

	usBuf[4] = g_tTP.XYChange;

#endif
}

/*
*********************************************************************************************************
*	函 数 名: TOUCH_LoadParam
*	功能说明: 读取校准参数
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TOUCH_LoadParam(void)
{
#if 0
	/* 保存下面的5个变量即可 */
	uint16_t usBuf[5];

	/* ReadParamToBuf() */

	g_tTP.usAdcX1 = usBuf[0];
	g_tTP.usAdcY1 = usBuf[1];

	g_tTP.usAdcX2 = usBuf[2];
	g_tTP.usAdcY2 = usBuf[3];


	g_tTP.XYChange = usBuf[4];
#endif
}
/****************************************************************************************************************************************************************************

	                                                                                  模块名称 : RA8875芯片驱动模块
	                                                                                文件名称 : LCD_RA8875.c
*****************************************************************************************************************************************************************************
/*
*********************************************************************************************************
*
*	模块名称 : RA8875芯片驱动模块
*	文件名称 : LCD_RA8875.c
*	版    本 : V1.0
*	说    明 : RA8875底层驱动函数集。
*	修改记录 :
*		版本号  日期       作者    说明
*		V1.0    2012-08-07 armfly  发布首版
*
*	Copyright (C), 2011-2012, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "stm32f10x.h"
#include <stdio.h>
#include "LCD_RA8875.h"
#include "bsp_tft_lcd.h"

/*
本程序在 STM32F103VET6 上验证通过。
	
	接线方法：
	
	LCD模块的 RS 引脚     接 PD13/FSMC_A18
	LCD模块的 CS 片选引脚 接 PD7/FSMC_NE1
	其他的 NWE, NOE, D15~D0 接对应的FSMC口线即可。
*/

/* 定义LCD驱动器的访问地址 */
#define RA8875_BASE		((uint32_t)(0x60000000 | 0x00000000))

#define RA8875_REG		*(__IO uint16_t *)(RA8875_BASE + (1 << 19))	  //a18作为RS
#define RA8875_RAM		*(__IO uint16_t *)(RA8875_BASE )

static __IO uint16_t s_usTouchX, s_usTouchY;	/* 触摸屏ADC值 */
static __IO uint8_t s_ucRA8875Busy = 0;

/* 保存当前显示窗口的位置和大小，这几个变量由 RA8875_SetDispWin() 进行设置 */
static __IO uint16_t s_WinX, s_WinY, s_WinHeight, s_WinWidth;

static void RA8875_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue);
static uint8_t RA8875_ReadReg(uint8_t _ucRegAddr);

static void RA8875_SetCursor(uint16_t _usX, uint16_t _usY);
static void RA8875_SetReadCursor(uint16_t _usX, uint16_t _usY);
static void RA8875_SetTextCursor(uint16_t _usX, uint16_t _usY);

static void BTE_SetTarBlock(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t _ucLayer);
static void BTE_SetOperateCode(uint8_t _ucOperate);
static void BTE_Start(void);


static void RA8875_Delaly1us(void)
{
	uint8_t i;

	for (i = 0; i < 10; i++);	/* 延迟, 不准 */
}

static void RA8875_Delaly1ms(void)
{
	uint16_t i;

	for (i = 0; i < 5000; i++);	/* 延迟1us, 不准 */
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_ReadID
*	功能说明: 读取LCD驱动芯片ID
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t RA8875_ReadID(void)
{
	return RA8875_ReadReg(0x00);
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_WriteReg
*	功能说明: 写RA8875寄存器. RA8875的寄存器地址和数据都是8bit的
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void RA8875_WriteReg(uint8_t _ucRegAddr, uint8_t _ucRegValue)
{
	s_ucRA8875Busy = 1;
	RA8875_REG = _ucRegAddr;	/* 设置寄存器地址 */
	RA8875_RAM = _ucRegValue;	/* 写入寄存器值 */
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_WriteReg
*	功能说明: 写RA8875寄存器. RA8875的寄存器地址和数据都是8bit的
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static uint8_t RA8875_ReadReg(uint8_t _ucRegAddr)
{
	uint8_t value;

	RA8875_REG = _ucRegAddr;/* 设置寄存器地址 */
	value = RA8875_RAM;		/* 读取寄存器值 */

	return value;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_InitHard
*	功能说明: 初始化RA8875驱动芯片
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_InitHard(void)
{
	uint8_t ucGPIX;

	/* 读取 RA8875 芯片额GPIX引脚的电平状态；1表示4.3寸屏；0表示7寸屏
	【备注】这是安富莱电子为了便于统一测试例程，在LCD模块上做的硬件标识。在做产品时，不必自动识别。
	*/
	ucGPIX = RA8875_ReadReg(0xC7);

	if (ucGPIX == 1)	/* 	GPIX = 1 表示 4.3 寸屏 480x272 */
	{
		/* 初始化PLL.  晶振频率为25M */
		RA8875_REG = 0x88;
		RA8875_Delaly1us();		/* 延迟1us */
		RA8875_RAM = 10;	  	/* PLLDIVM [7] = 0 ;  PLLDIVN [4:0] = 10 */

	    RA8875_Delaly1ms();

		RA8875_REG = 0x89;
		RA8875_Delaly1us();		/* 延迟1us */
		RA8875_RAM = 2;		/* PLLDIVK[2:0] = 2, 除以4 */

		/* RA8875 的内部系统频率 (SYS_CLK) 是结合振荡电路及PLL 电路所产生，频率计算公式如下 :
		  SYS_CLK = FIN * ( PLLDIVN [4:0] +1 ) / ((PLLDIVM+1 ) * ( 2^PLLDIVK [2:0] ))
		          = 25M * (10 + 1) / ((0 + 1) * (2 ^ 2))
				  = 68.75MHz
		*/

		/* REG[88h]或REG[89h]被设定后，为保证PLL 输出稳定，须等待一段「锁频时间」(< 100us)。*/
	    RA8875_Delaly1ms();

		/*
			配置系统控制寄存器。 中文pdf 第18页:

			bit3:2 色彩深度设定 (Color Depth Setting)
				00b : 8-bpp 的通用TFT 接口， i.e. 256 色。
				1xb : 16-bpp 的通用TFT 接口， i.e. 65K 色。	 【选这个】

			bit1:0 MCUIF 选择
				00b : 8-位MCU 接口。
				1xb : 16-位MCU 接口。 【选这个】
		*/
		RA8875_WriteReg(0x10, (1 <<3 ) | (1 << 1));	/* 配置16位MCU并口，65K色 */

		/* REG[04h] Pixel Clock Setting Register   PCSR
			bit7  PCLK Inversion
				0 : PDAT 是在PCLK 正缘上升 (Rising Edge) 时被取样。
				1 : PDAT 是在PCLK 负缘下降 (Falling Edge) 时被取样。
			bit1:0 PCLK 频率周期设定
				Pixel Clock  PCLK 频率周期设定。
				00b: PCLK 频率周期= 系统频率周期。
				01b: PCLK 频率周期= 2 倍的系统频率周期。
				10b: PCLK 频率周期= 4 倍的系统频率周期。
				11b: PCLK 频率周期= 8 倍的系统频率周期。
		*/
	    RA8875_WriteReg(0x04, 0x82);    /* 设置PCLK反相 */
	    RA8875_Delaly1ms();

	    //Horizontal set
	    //HDWR//Horizontal Display Width Setting Bit[6:0]
	 	//Horizontal display width(pixels) = (HDWR + 1)*8
	    RA8875_WriteReg(0x14, 0x3B);
	    RA8875_WriteReg(0x15, 0x00);

	    //HNDR//Horizontal Non-Display Period Bit[4:0]
	    //Horizontal Non-Display Period (pixels) = (HNDR + 1)*8
		RA8875_WriteReg(0x16, 0x01);

	    //HSTR//HSYNC Start Position[4:0]
	    //HSYNC Start Position(PCLK) = (HSTR + 1)*8
		RA8875_WriteReg(0x17, 0x00);

	    //HPWR//HSYNC Polarity ,The period width of HSYNC.
	    //HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8
		RA8875_WriteReg(0x18, 0x05);

	    //Vertical set
	    //VDHR0 //Vertical Display Height Bit [7:0]
	    //Vertical pixels = VDHR + 1
		RA8875_WriteReg(0x19, 0x0F);

	    //VDHR1 //Vertical Display Height Bit [8]
	    //Vertical pixels = VDHR + 1
		RA8875_WriteReg(0x1A, 0x01);

	    //VNDR0 //Vertical Non-Display Period Bit [7:0]
	    //Vertical Non-Display area = (VNDR + 1)
		RA8875_WriteReg(0x1B, 0x02);

	    //VNDR1 //Vertical Non-Display Period Bit [8]
	    //Vertical Non-Display area = (VNDR + 1)
		RA8875_WriteReg(0x1C, 0x00);

	    //VSTR0 //VSYNC Start Position[7:0]
	    //VSYNC Start Position(PCLK) = (VSTR + 1)
		RA8875_WriteReg(0x1D, 0x07);

	    //VSTR1 //VSYNC Start Position[8]
	    //VSYNC Start Position(PCLK) = (VSTR + 1)
		RA8875_WriteReg(0x1E, 0x00);

	    //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]
	    //VSYNC Pulse Width(PCLK) = (VPWR + 1)
	    RA8875_WriteReg(0x1F, 0x09);


	    g_LcdHeight = LCD_43_HEIGHT;
		g_LcdWidth = LCD_43_WIDTH;
	}
	else	/* GPIX = 0 表示7寸屏(800x480) */
	{
	    g_LcdHeight = LCD_70_HEIGHT;
		g_LcdWidth = LCD_70_WIDTH;

		/* 初始化PLL.  晶振频率为25M */
		RA8875_REG = 0x88;
		RA8875_Delaly1us();		/* 延迟1us */
		RA8875_RAM = 12;	  	/* PLLDIVM [7] = 0 ;  PLLDIVN [4:0] = 10 */

	    RA8875_Delaly1ms();

		RA8875_REG = 0x89;
		RA8875_Delaly1us();		/* 延迟1us */
		RA8875_RAM = 2;		/* PLLDIVK[2:0] = 2, 除以4 */

		/* RA8875 的内部系统频率 (SYS_CLK) 是结合振荡电路及PLL 电路所产生，频率计算公式如下 :
		  SYS_CLK = FIN * ( PLLDIVN [4:0] +1 ) / ((PLLDIVM+1 ) * ( 2^PLLDIVK [2:0] ))
		          = 25M * (12 + 1) / ((0 + 1) * (2 ^ 2))
				  = 81.25MHz
		*/

		/* REG[88h]或REG[89h]被设定后，为保证PLL 输出稳定，须等待一段「锁频时间」(< 100us)。*/
	    RA8875_Delaly1ms();

		/*
			配置系统控制寄存器。 中文pdf 第18页:

			bit3:2 色彩深度设定 (Color Depth Setting)
				00b : 8-bpp 的通用TFT 接口， i.e. 256 色。
				1xb : 16-bpp 的通用TFT 接口， i.e. 65K 色。	 【选这个】

			bit1:0 MCUIF 选择
				00b : 8-位MCU 接口。
				1xb : 16-位MCU 接口。 【选这个】
		*/
		RA8875_WriteReg(0x10, (1 <<3 ) | (1 << 1));	/* 配置16位MCU并口，65K色 */

		/* REG[04h] Pixel Clock Setting Register (PCSR)
			bit7  PCLK Inversion
				0 : PDAT 是在PCLK 正缘上升 (Rising Edge) 时被取样。
				1 : PDAT 是在PCLK 负缘下降 (Falling Edge) 时被取样。
			bit1:0 PCLK 频率周期设定
				Pixel Clock ,PCLK 频率周期设定。
				00b: PCLK 频率周期= 系统频率周期。
				01b: PCLK 频率周期= 2 倍的系统频率周期。
				10b: PCLK 频率周期= 4 倍的系统频率周期。
				11b: PCLK 频率周期= 8 倍的系统频率周期。
		*/
	    RA8875_WriteReg(0x04, 0x81);
	    RA8875_Delaly1ms();

	#if 1
		/* OTD9960 & OTA7001 设置 */
		RA8875_WriteReg(0x14, 0x63); // 水平像素800
		RA8875_WriteReg(0x15, 0x00);
		RA8875_WriteReg(0x16, 0x03);
		RA8875_WriteReg(0x17, 0x03);
		RA8875_WriteReg(0x18, 0x0B);
		RA8875_WriteReg(0x19, 0xDF);
		RA8875_WriteReg(0x1A, 0x01);
		RA8875_WriteReg(0x1B, 0x1F);
		RA8875_WriteReg(0x1C, 0x00);
		RA8875_WriteReg(0x1D, 0x16);
		RA8875_WriteReg(0x1E, 0x00);
		RA8875_WriteReg(0x1F, 0x01);

	#else	/* AT070TN92  setting */	   //下面的不编译，用#if1 #else 便于注释
	    //Horizontal set
	    //HDWR//Horizontal Display Width Setting Bit[6:0]
	 	//Horizontal display width(pixels) = (HDWR + 1)*8
	    RA8875_WriteReg(0x14, 0x4F);
	    RA8875_WriteReg(0x15, 0x05);

	    //HNDR//Horizontal Non-Display Period Bit[4:0]
	    //Horizontal Non-Display Period (pixels) = (HNDR + 1)*8
		RA8875_WriteReg(0x16, 0x0F);

	    //HSTR//HSYNC Start Position[4:0]
	    //HSYNC Start Position(PCLK) = (HSTR + 1)*8
		RA8875_WriteReg(0x17, 0x01);

	    //HPWR//HSYNC Polarity ,The period width of HSYNC.
	    //HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8
		RA8875_WriteReg(0x18, 0x00);

	    //Vertical set
	    //VDHR0 //Vertical Display Height Bit [7:0]
	    //Vertical pixels = VDHR + 1
		RA8875_WriteReg(0x19, 0xDF);

	    //VDHR1 //Vertical Display Height Bit [8]
	    //Vertical pixels = VDHR + 1
		RA8875_WriteReg(0x1A, 0x01);

	    //VNDR0 //Vertical Non-Display Period Bit [7:0]
	    //Vertical Non-Display area = (VNDR + 1)
		RA8875_WriteReg(0x1B, 0x0A);

	    //VNDR1 //Vertical Non-Display Period Bit [8]
	    //Vertical Non-Display area = (VNDR + 1)
		RA8875_WriteReg(0x1C, 0x00);

	    //VSTR0 //VSYNC Start Position[7:0]
	    //VSYNC Start Position(PCLK) = (VSTR + 1)
		RA8875_WriteReg(0x1D, 0x0E);

	    //VSTR1 //VSYNC Start Position[8]
	    //VSYNC Start Position(PCLK) = (VSTR + 1)
		RA8875_WriteReg(0x1E, 0x00);

	    //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]
	    //VSYNC Pulse Width(PCLK) = (VPWR + 1)
	    RA8875_WriteReg(0x1F, 0x01);
	#endif
	}

	/* 设置TFT面板的 DISP  引脚为高，使能面板. 安富莱TFT模块的DISP引脚连接到RA8875芯片的GP0X脚 */
	RA8875_WriteReg(0xC7, 0x01);	/* DISP = 1 */

	/* LCD 显示/关闭讯号 (LCD Display on) */
	RA8875_WriteReg(0x01, 0x80);

	/* 	REG[40h] Memory Write Control Register 0 (MWCR0)

		Bit 7	显示模式设定
			0 : 绘图模式。
			1 : 文字模式。

		Bit 6	文字写入光标/内存写入光标设定
			0 : 设定文字/内存写入光标为不显示。
			1 : 设定文字/内存写入光标为显示。

		Bit 5	文字写入光标/内存写入光标闪烁设定
			0 : 游标不闪烁。
			1 : 游标闪烁。

		Bit 4   NA

		Bit 3-2  绘图模式时的内存写入方向
			00b : 左 -> 右，然后上 -> 下。
			01b : 右 -> 左，然后上 -> 下。
			10b : 上 -> 下，然后左 -> 右。
			11b : 下 -> 上，然后左 -> 右。

		Bit 1 	内存写入光标自动增加功能设定
			0 : 当内存写入时光标位置自动加一。
			1 : 当内存写入时光标位置不会自动加一。

		Bit 0 内存读取光标自动增加功能设定
			0 : 当内存读取时光标位置自动加一。
			1 : 当内存读取时光标位置不会自动加一。
	*/
	RA8875_WriteReg(0x40, 0x00);	/* 选择绘图模式 */


	/* 	REG[41h] Memory Write Control Register1 (MWCR1)
		写入目的位置，选择图层1
	*/
	RA8875_WriteReg(0x41, 0x00);	/* 选择绘图模式, 目的为CGRAM */

	RA8875_SetDispWin(0, 0, g_LcdHeight, g_LcdWidth);

	/* 设置单图层，水平扫描方向，垂直扫描方向 */
	RA8875_REG = 0x20;
	RA8875_RAM = 0x00;

	RA8875_REG = 0x52;
	RA8875_REG = 0x00;
}

/*
*********************************************************************************************************
*	函 数 名: BTE_SetTarBlock
*	功能说明: 设置RA8875 BTE目标区块以及目标图层
*	形    参:
*			uint16_t _usX : 水平起点坐标
*			uint16_t _usY : 垂直起点坐标
*			uint16_t _usHeight : 区块高度
*			uint16_t _usWidth : 区块宽度
*			uint8_t _ucLayer ： 0 图层1； 1 图层2
*	返 回 值: 无
*********************************************************************************************************
*/
static void BTE_SetTarBlock(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t _ucLayer)
{
	/* 设置起点坐标 */
	RA8875_WriteReg(0x58, _usX);
	RA8875_WriteReg(0x59, _usX >> 8);

	RA8875_WriteReg(0x5A, _usY);
	if (_ucLayer == 0)	/* 图层2 */
	{
		RA8875_WriteReg(0x5B, _usY >> 8);
	}
	else
	{
		RA8875_WriteReg(0x5B, (1 << 7) | (_usY >> 8));	/* Bit7 表示图层， 0 图层1； 1 图层2*/
	}

	/* 设置区块宽度 */
	RA8875_WriteReg(0x5C, _usWidth);
	RA8875_WriteReg(0x5D, _usWidth >> 8);

	/* 设置区块高度 */
	RA8875_WriteReg(0x5E, _usHeight);
	RA8875_WriteReg(0x5F, _usHeight >> 8);
}

/*
*********************************************************************************************************
*	函 数 名: BTE_SetOperateCode
*	功能说明: 设定BTE 操作码和光栅运算码
*	形    参: _ucOperate : 操作码
*	返 回 值: 无
*********************************************************************************************************
*/
static void BTE_SetOperateCode(uint8_t _ucOperate)
{
	/*  设定BTE 操作码和光栅运算码  */
	RA8875_WriteReg(0x51, _ucOperate);
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_SetFrontColor
*	功能说明: 设定前景色
*	形    参: 颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_SetFrontColor(uint16_t _usColor)
{
	s_ucRA8875Busy = 1;
	RA8875_WriteReg(0x63, (_usColor & 0xF800) >> 11);	/* R5  */
	RA8875_WriteReg(0x64, (_usColor & 0x07E0) >> 5);	/* G6 */
	RA8875_WriteReg(0x65, (_usColor & 0x001F));			/* B5 */
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_SetBackColor
*	功能说明: 设定背景色
*	形    参: 颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_SetBackColor(uint16_t _usColor)
{
	s_ucRA8875Busy = 1;
	RA8875_WriteReg(0x60, (_usColor & 0xF800) >> 11);	/* R5  */
	RA8875_WriteReg(0x61, (_usColor & 0x07E0) >> 5);	/* G6 */
	RA8875_WriteReg(0x62, (_usColor & 0x001F));			/* B5 */
	s_ucRA8875Busy = 0;
}


/*
*********************************************************************************************************
*	函 数 名: BTE_Start
*	功能说明: 启动BTE操作
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void BTE_Start(void)
{
	s_ucRA8875Busy = 1;
	/* RA8875_WriteReg(0x50, 0x80);  不能使用这个函数，因为内部已经操作了 s_ucRA8875Busy 标志 */
	RA8875_REG = 0x50;	/* 设置寄存器地址 */
	RA8875_RAM = 0x80;	/* 写入寄存器值 */
}

/*
*********************************************************************************************************
*	函 数 名: BTE_Wait
*	功能说明: 等待BTE操作结束
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void BTE_Wait(void)
{
	while ((RA8875_REG & 0x40) == 0x40);
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_IsBusy
*	功能说明: RA8875是否忙
*	形    参: 无
*	返 回 值: 1 表示忙； 0 表示空闲
*********************************************************************************************************
*/
uint8_t RA8875_IsBusy(void)
{
	if (s_ucRA8875Busy == 0)
	{
		return 0;
	}
	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_Layer1Visable
*	功能说明: RA8875 图层1显示打开
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_Layer1Visable(void)
{
	/* 0x52 寄存器的 Bit2:0
		图层显示模式
		000b : 只有图层1 显示。
		001b : 只有图层2 显示。
		010b : 显示图层1 与图层2 的渐进/渐出模式。
		011b : 显示图层1 与图层2 的通透模式。
		100b : Boolean OR。
		101b : Boolean AND。
		110b : 浮动窗口模式 (Floating window mode)。
		111b :保留。
	*/
	RA8875_WriteReg(0x52, RA8875_ReadReg(0x52) & 0xF8);	/* 只有图层1 显示 */
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_Layer2Visable
*	功能说明: 只显示图层2
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_Layer2Visable(void)
{
	/* 0x52 寄存器的 Bit2:0
		图层显示模式
		000b : 只有图层1 显示。
		001b : 只有图层2 显示。
		010b : 显示图层1 与图层2 的渐进/渐出模式。
		011b : 显示图层1 与图层2 的通透模式。
		100b : Boolean OR。
		101b : Boolean AND。
		110b : 浮动窗口模式 (Floating window mode)。
		111b :保留。
	*/
	RA8875_WriteReg(0x52, (RA8875_ReadReg(0x52) & 0xF8) | 0x01);	/* 只有图层2 显示 */
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_TouchInit
*	功能说明: 初始化触摸板
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_TouchInit(void)
{
	RA8875_WriteReg(0xF0, (1 << 2));	/* 开启触控面板中断位 */

	/*
		REG[71h] Touch Panel Control Register 1 (TPCR1)

		Bit7 N/A
		Bit6 触控面板模式设定
			0 : 自动模式。
			1 : 手动模式。
		Bit5 触控面板控制器ADC 参考电压(Vref)来源设定
			0 : 内部产生参考电压。
			1 : 外部输入参考电压，ADC 参考电压准位= 1/2 VDD。
		Bit4-3 N/A
		Bit2 触控中断讯号的消除弹跳电路选择
			0: 关闭消除弹跳电路。
			1: 开启消除弹跳电路。
		Bit1-0 触控面板手动模式之选择位
			00b : 闲置模式。触控控制单元进入闲置模式。
			01b : 侦测触摸事件发生。在此模式控制器会侦测触摸事件的发
				生，事件发生可以引发中断或是由缓存器得知(REG[F1h]
				Bit2)。
			10b : X 轴数据撷取模式。在此模式触摸位置的X 轴数据会被储
				存至 REG[72h] 和REG[74h]。
			11b : Y 轴数据撷取模式。在此模式触摸位置的Y 轴数据会被储
				存至REG[73h] and REG[74h]。
	*/
	RA8875_WriteReg(0x71, (0 << 6) | (0 << 5) | (1 << 2));	/* 选择自动模式 */

	/*
		REG[70h] Touch Panel Control Register 0 (TPCR0)

		Bit7 触控面板功能设定
			0 : 关闭。
			1 : 开启。
		Bit6-4 触控面板控制器取样时间设定
			000b : ADC 取样时间为512 个系统频率周期。
			001b : ADC 取样时间为 1024 个系统频率周期。
			010b : ADC 取样时间为 2048 个系统频率周期。
			011b : ADC 取样时间为 4096 个系统频率周期。
			100b : ADC 取样时间为 8192 个系统频率周期。
			101b : ADC 取样时间为 16384 个系统频率周期。
			110b : ADC 取样时间为 32768 个系统频率周期。
			111b : ADC 取样时间为65536 个系统频率周期。
		Bit3 触控面板唤醒模式
			0 : 关闭触控事件唤醒模式。
			1 : 触控事件可唤醒睡眠模式。
		Bit2-0 触控面板控制器ADC 频率设定
			000b : 系统频率。
			001b : 系统频率 / 2。
			010b : 系统频率 / 4。
			011b : 系统频率 / 8。
			100b : 系统频率 / 16。
			101b : 系统频率 / 32。
			110b : 系统频率 / 64。
			111b : 系统频率 / 128。

		注 : ADC 的输入频率设定不能超过10MHz。
	*/
	RA8875_WriteReg(0x70, (1 << 7) | (3 << 4) | (0 << 3) | (2 << 0));	/* 开启触摸功能； */
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_TouchReadX
*	功能说明: 读取X位置ADC   （这个函数被定时中断服务程序调用，需要避免寄存器访问冲突 )
*	形    参: 无
*	返 回 值: ADC值，10Bit
*********************************************************************************************************
*/
uint16_t RA8875_TouchReadX(void)
{
	uint16_t usAdc;
	uint8_t ucRegValue;

	/* 软件读取中断事件标志 */
	ucRegValue = RA8875_ReadReg(0xF1);
	if (ucRegValue & (1 << 2))
	{
		usAdc = RA8875_ReadReg(0x72);	/* Bit9-2 */
		usAdc <<= 2;
		usAdc += (RA8875_ReadReg(0x74) & 0x03);

		s_usTouchX = usAdc;

		usAdc = RA8875_ReadReg(0x73);	/* Bit9-2 */
		usAdc <<= 2;
		usAdc += ((RA8875_ReadReg(0x74) & 0x0C) >> 2);

		s_usTouchY = usAdc;
	}
	else
	{
		s_usTouchX = 0;
		s_usTouchY = 0;
	}

	/*
		bit2 写入功能?? 触控面板中断清除位
		0 : 未操作。
		1 : 清除触控面板中断。
		读取功能?? 触控面板中断状态
		0 : 未发生触控面板中断。
		1 : 发生触控面板中断。
	*/
	RA8875_WriteReg(0xF1, (1 << 2));	/* 必须清除， 才会下次采样 */

	return s_usTouchX;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_TouchReadY
*	功能说明: 读取Y位置ADC ； 必须先调用 RA8875_TouchReadX()
*	形    参: 无
*	返 回 值: ADC值，10Bit
*********************************************************************************************************
*/
uint16_t RA8875_TouchReadY(void)
{
	return s_usTouchY;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_DispOn
*	功能说明: 打开显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_DispOn(void)
{
	RA8875_WriteReg(0x01, 0x80);
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_DispOff
*	功能说明: 关闭显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_DispOff(void)
{
	RA8875_WriteReg(0x01, 0x00);
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_PutPixel
*	功能说明: 画1个像素
*	形    参：
*			_usX,_usY : 像素坐标
*			_usColor  ：像素颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	RA8875_SetCursor(_usX, _usY);	/* 设置光标位置 */

	s_ucRA8875Busy = 1;
	RA8875_REG = 0x02; 			/* 用于设定RA8875 进入内存(DDRAM或CGRAM)读取/写入模式 */
	RA8875_RAM = _usColor;
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_GetPixel
*	功能说明: 读取1个像素
*	形    参：
*			_usX,_usY : 像素坐标
*			_usColor  ：像素颜色
*	返 回 值: RGB颜色值
*********************************************************************************************************
*/
uint16_t RA8875_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint16_t usRGB;

	RA8875_SetReadCursor(_usX, _usY);	/* 设置读取光标位置 */

	s_ucRA8875Busy = 1;
	RA8875_REG = 0x02;
	usRGB = RA8875_RAM;
	s_ucRA8875Busy = 0;

	return usRGB;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_ClrScr
*	功能说明: 根据输入的颜色值清屏.RA8875支持硬件单色填充。该函数仅对当前激活的显示窗口进行清屏. 显示
*			 窗口的位置和大小由 RA8875_SetDispWin() 函数进行设置
*	形    参：_usColor : 背景色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_ClrScr(uint16_t _usColor)
{
	/* 也可以通过0x30-0x37寄存器获得获得当前激活的显示窗口 */

	/* 单色填满功能, 中文pdf 第162页
	此功能使用于将选定特定区域画面清除或是塡入给定某种前景色，R8875 填入的单色设定为BTE 前景色。

	操作步骤：
		1. 设定目的图层和位置 REG[58h], [59h], [5Ah], [5Bh]
		2. 设定BTE 宽度和高度 REG[5Ch], [5Dh], [5Eh], [5Fh]
		3. 设定BTE 操作码和光栅运算码  REG[51h] Bit[3:0] = 0Ch
		4. 设定前景色  REG[63h], [64h], [65h]
		5. 开启BTE 功能  REG[50h] Bit7 = 1
		6. 检查状态缓存器 STSR Bit6，确认BTE 是否完成
	*/
	BTE_SetTarBlock(s_WinX, s_WinY, s_WinHeight, s_WinWidth, 0);	/* 设置BTE位置和宽度高度以及目标图层（0或1） */
	BTE_SetOperateCode(0x0C);		/* 设定BTE 操作码和光栅运算码  REG[51h] Bit[3:0] = 0Ch */
	RA8875_SetFrontColor(_usColor);	/* 设置BTE前景色 */
	BTE_Start();					/* 开启BTE 功能 */
	BTE_Wait();						/* 等待操作结束 */
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_DrawBMP
*	功能说明: 在LCD上显示一个BMP位图，位图点阵扫描次序：从左到右，从上到下
*	形    参：
*			_usX, _usY : 图片的坐标
*			_usHeight  ：图片高度
*			_usWidth   ：图片宽度
*			_ptr       ：图片点阵指针
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	uint32_t index = 0;
	const uint16_t *p;

	/* 设置图片的位置和大小， 即设置显示窗口 */
	RA8875_SetDispWin(_usX, _usY, _usHeight, _usWidth);

	s_ucRA8875Busy = 1;
	RA8875_REG = 0x02; 			/* 准备读写显存 */

	p = _ptr;
	for (index = 0; index < _usHeight * _usWidth; index++)
	{
		/*
			armfly : 进行优化, 函数就地展开
			RA8875_WriteRAM(_ptr[index]);

			此处可考虑用DMA操作
		*/
		RA8875_RAM = *p++;
	}
	s_ucRA8875Busy = 0;

	/* 退出窗口绘图模式 */
	RA8875_QuitWinMode();
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_DrawLine
*	功能说明: 采用RA8875的硬件功能，在2点间画一条直线。
*	形    参：
*			_usX1, _usY1 ：起始点坐标
*			_usX2, _usY2 ：终止点Y坐标
*			_usColor     ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor)
{
	/* pdf 第131页
		RA8875 支持直线绘图功能，让使用者以简易或低速的MCU 就可以在TFT 模块上画直线。先设
		定直线的起始点REG[91h~94h] 与结束点REG[95h~98h]，直线的颜色REG[63h~65h]，然后启
		动绘图设定REG[90h] Bit4 = 0, Bit0=0 且REG[90h] Bit7 = 1，RA8875 就会将直线的图形写入
		DDRAM，相对的在TFT 模块上就可以显示所画的直线。
	*/

	/* 设置起点坐标 */
	RA8875_WriteReg(0x91, _usX1);
	RA8875_WriteReg(0x92, _usX1 >> 8);
	RA8875_WriteReg(0x93, _usY1);
	RA8875_WriteReg(0x94, _usY1 >> 8);

	/* 设置终点坐标 */
	RA8875_WriteReg(0x95, _usX2);
	RA8875_WriteReg(0x96, _usX2 >> 8);
	RA8875_WriteReg(0x97, _usY2);
	RA8875_WriteReg(0x98, _usY2 >> 8);

	RA8875_SetFrontColor(_usColor);	/* 设置直线的颜色 */

	s_ucRA8875Busy = 1;
	RA8875_WriteReg(0x90, (1 << 7) | (0 << 4) | (0 << 0));	/* 开始画直线 */
	while (RA8875_ReadReg(0x90) & (1 << 7));				/* 等待结束 */
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_DrawRect
*	功能说明: 采用RA8875硬件功能绘制矩形
*	形    参：
*			_usX,_usY：矩形左上角的坐标
*			_usHeight ：矩形的高度
*			_usWidth  ：矩形的宽度
*			_usColor  ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	/*
		RA8875 支持方形绘图功能，让使用者以简易或低速的MCU 就可以在TFT 模块上画方形。先设
	定方形的起始点REG[91h~94h]与结束点REG[95h~98h]，方形的颜色REG[63h~65h]，然后启
	动绘图设定REG[90h] Bit4=1, Bit0=0 且REG[90h] Bit7 = 1，RA8875 就会将方形的图形写入
	DDRAM，相对的在TFT 模块上就可以显示所画的方形。若设定REG[90h] Bit5 = 1，则可画出一
	实心方形 (Fill)

	 ---------------->---
	|(_usX，_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	/* 设置起点坐标 */
	RA8875_WriteReg(0x91, _usX);
	RA8875_WriteReg(0x92, _usX >> 8);
	RA8875_WriteReg(0x93, _usY);
	RA8875_WriteReg(0x94, _usY >> 8);

	/* 设置终点坐标 */
	RA8875_WriteReg(0x95, _usX + _usWidth);
	RA8875_WriteReg(0x96, (_usX + _usWidth) >> 8);
	RA8875_WriteReg(0x97, _usY + _usHeight);
	RA8875_WriteReg(0x98, (_usY + _usHeight) >> 8);

	RA8875_SetFrontColor(_usColor);	/* 设置颜色 */

	s_ucRA8875Busy = 1;
	RA8875_WriteReg(0x90, (1 << 7) | (0 << 5) | (1 << 4) | (0 << 0));	/* 开始画矩形  */
	while (RA8875_ReadReg(0x90) & (1 << 7));							/* 等待结束 */
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_FillRect
*	功能说明: 采用RA8875硬件功能填充一个矩形为单色
*	形    参：
*			_usX,_usY：矩形左上角的坐标
*			_usHeight ：矩形的高度
*			_usWidth  ：矩形的宽度
*			_usColor  ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_FillRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	/*
		RA8875 支持方形绘图功能，让使用者以简易或低速的MCU 就可以在TFT 模块上画方形。先设
	定方形的起始点REG[91h~94h]与结束点REG[95h~98h]，方形的颜色REG[63h~65h]，然后启
	动绘图设定REG[90h] Bit4=1, Bit0=0 且REG[90h] Bit7 = 1，RA8875 就会将方形的图形写入
	DDRAM，相对的在TFT 模块上就可以显示所画的方形。若设定REG[90h] Bit5 = 1，则可画出一
	实心方形 (Fill)

	 ---------------->---
	|(_usX，_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	/* 设置起点坐标 */
	RA8875_WriteReg(0x91, _usX);
	RA8875_WriteReg(0x92, _usX >> 8);
	RA8875_WriteReg(0x93, _usY);
	RA8875_WriteReg(0x94, _usY >> 8);

	/* 设置终点坐标 */
	RA8875_WriteReg(0x95, _usX + _usWidth);
	RA8875_WriteReg(0x96, (_usX + _usWidth) >> 8);
	RA8875_WriteReg(0x97, _usY + _usHeight);
	RA8875_WriteReg(0x98, (_usY + _usHeight) >> 8);

	RA8875_SetFrontColor(_usColor);	/* 设置颜色 */

	s_ucRA8875Busy = 1;
	RA8875_WriteReg(0x90, (1 << 7) | (1 << 5) | (1 << 4) | (0 << 0));	/* 开始填充矩形  */
	while (RA8875_ReadReg(0x90) & (1 << 7));							/* 等待结束 */
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_DrawCircle
*	功能说明: 绘制一个圆，笔宽为1个像素
*	形    参：
*			_usX,_usY  ：圆心的坐标
*			_usRadius  ：圆的半径
*			_usColor  ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	/*
		RA8875 支持圆形绘图功能，让使用者以简易或低速的MCU 就可以在TFT 模块上画圆。先设定
		圆的中心点REG[99h~9Ch]，圆的半径REG[9Dh]，圆的颜色REG[63h~65h]，然后启动绘图
		REG[90h] Bit6 = 1，RA8875 就会将圆的图形写入DDRAM，相对的在TFT 模块上就可以显示所
		画的圆。若设定REG[90h] Bit5 = 1，则可画出一实心圆 (Fill)；若设定REG[90h] Bit5 = 0，则可
		画出空心圆 (Not Fill
	*/
	/* 设置圆的半径 */
	if (_usRadius > 255)
	{
		return;
	}

	/* 设置圆心坐标 */
	RA8875_WriteReg(0x99, _usX);
	RA8875_WriteReg(0x9A, _usX >> 8);
	RA8875_WriteReg(0x9B, _usY);
	RA8875_WriteReg(0x9C, _usY >> 8);

	RA8875_WriteReg(0x9D, _usRadius);	/* 设置圆的半径 */

	RA8875_SetFrontColor(_usColor);	/* 设置颜色 */

	s_ucRA8875Busy = 1;
	RA8875_WriteReg(0x90, (1 << 6) | (0 << 5));				/* 开始画圆, 不填充  */
	while (RA8875_ReadReg(0x90) & (1 << 6));				/* 等待结束 */
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_FillCircle
*	功能说明: 填充一个圆
*	形    参：
*			_usX,_usY  ：圆心的坐标
*			_usRadius  ：圆的半径
*			_usColor   ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_FillCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	/*
		RA8875 支持圆形绘图功能，让使用者以简易或低速的MCU 就可以在TFT 模块上画圆。先设定
		圆的中心点REG[99h~9Ch]，圆的半径REG[9Dh]，圆的颜色REG[63h~65h]，然后启动绘图
		REG[90h] Bit6 = 1，RA8875 就会将圆的图形写入DDRAM，相对的在TFT 模块上就可以显示所
		画的圆。若设定REG[90h] Bit5 = 1，则可画出一实心圆 (Fill)；若设定REG[90h] Bit5 = 0，则可
		画出空心圆 (Not Fill
	*/
	/* 设置圆的半径 */
	if (_usRadius > 255)
	{
		return;
	}

	/* 设置圆心坐标 */
	RA8875_WriteReg(0x99, _usX);
	RA8875_WriteReg(0x9A, _usX >> 8);
	RA8875_WriteReg(0x9B, _usY);
	RA8875_WriteReg(0x9C, _usY >> 8);

	RA8875_WriteReg(0x9D, _usRadius);	/* 设置圆的半径 */

	RA8875_SetFrontColor(_usColor);	/* 设置颜色 */

	s_ucRA8875Busy = 1;
	RA8875_WriteReg(0x90, (1 << 6) | (1 << 5));				/* 开始画圆, 填充  */
	while (RA8875_ReadReg(0x90) & (1 << 6));				/* 等待结束 */
	s_ucRA8875Busy = 0;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_SetFont
*	功能说明: 文本模式，设置文字字体、行距和字距
*	形    参：
*			_ucFontType : 字体类型: RA_FONT_16, RA_FONT_24, RA_FONT_32
*			_ucLineSpace : 行距，像素单位 (0-31)
*			_ucCharSpace : 字距，像素单位 (0-63)
*
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_SetFont(uint8_t _ucFontType, uint8_t _ucLineSpace, uint8_t _ucCharSpace)
{
	/*
		[29H]在文字模式下，用来设定文字间的行距 (单位: 像素) 。
		只有低5个bit有效，0-31
	*/
	if (_ucLineSpace >31)
	{
		_ucLineSpace = 31;
	}
	RA8875_WriteReg(0x29, _ucLineSpace);

	/*
		[2EH] 设置字符间距（像素单位，0-63），和字体（16*16，24*24，32*32）
	*/
	if (_ucCharSpace > 63)
	{
		_ucCharSpace = 63;
	}
	if (_ucFontType > RA_FONT_32)
	{
		_ucFontType = RA_FONT_16;
	}
	RA8875_WriteReg(0x2E, (_ucCharSpace & 0x3F) | (_ucFontType << 6));
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_SetFontZoom
*	功能说明: 文本模式，设置文字的放大模式，1X,2X,3X, 4X
*	形    参：
*			_ucHSize : 文字水平放大倍数，0-3 分别对应 X1、X2、X3、X4
*			_ucVSize : 文字处置放大倍数，0-3 分别对应 X1、X2、X3、X4
*		建议输入参数用枚举：RA_SIZE_X1、RA_SIZE_X2、RA_SIZE_X3、RA_SIZE_X4
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_SetTextZoom(uint8_t _ucHSize, uint8_t _ucVSize)
{
	/*
		pdf 第22页		[22H]
		bit7 用于对齐，一般不用，缺省设0
		bit6 用于通透，一般不用，缺省设0
		bit4 用于旋转90读，一般不用，缺省设0
		bit3-2 水平放大倍数
		bit1-0 垂直放大倍数
	*/
	RA8875_WriteReg(0x22, ((_ucHSize & 0x03) << 2) | ( _ucVSize & 0x03));
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_DispAscii
*	功能说明: 显示ASCII字符串，字符点阵来自于RA8875内部ROM中的ASCII字符点阵（8*16），不支持汉字.
*			文字颜色、背景颜色、是否通透由其他的函数进行设置
*			void RA8875_SetFrontColor(uint16_t _usColor);
*			void RA8875_SetBackColor(uint16_t _usColor);
*	形    参：_usX, _usY : 显示位置（坐标）
*			 _ptr : AascII字符串，已0结束
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_DispAscii(uint16_t _usX, uint16_t _usY, char *_ptr)
{
	/*
		RA8875 内建8x16 点的ASCII 字型ROM，提供使用者更方便的方式用特定编码 (Code) 输入文
		字。内建的字集支持ISO/IEC 8859-1~4 编码标准，此外，使用者可以透过REG[60h~62h] 选择
		文字前景颜色，以及透过REG[63h~65h] 选择背景颜色.

		ISO/IEC 8859-1，又称Latin-1或“西欧语言”，是国际标准化组织内ISO/IEC 8859的第一个8位字符集。
		它以ASCII为基础，在空置的0xA0-0xFF的范围内，加入192个字母及符号，藉以供使用变音符号的拉丁字母语言使用。

		ISO/IEC 8859-2 Latin-2或“中欧语言”，是国际标准化组织内ISO/IEC 8859的其中一个8位字符集 .
		ISO/IEC 8859-3 南欧语言字符集
		ISO/IEC 8859-4 北欧语言字符集
	*/

	/*
	(1) Text mode  REG[40h] bit7=1
	(2) Internal Font ROM Select   REG[21h] bit7=0, bit5=0
	(3) Font foreground and background color Select  REG[63h~65h], REG[60h~62h]
	(4) Write the font Code  CMD_WR[02h]    DATA_WR[font_code]
	*/

	RA8875_SetTextCursor(_usX, _usY);

	RA8875_WriteReg(0x40, (1 << 7));	/* 设置为文本模式 */

	/* 选择CGROM font; 选择内部CGROM; 内部CGROM 编码选择ISO/IEC 8859-1. */
	RA8875_WriteReg(0x2F, 0x00);
	RA8875_WriteReg(0x21, (0 << 7) | (0 << 5) | (0 << 1) | (0 << 0));

	s_ucRA8875Busy = 1;
	RA8875_REG = 0x02; 			/* 用于设定RA8875 进入内存(DDRAM或CGRAM)读取/写入模式 */

	/* 开始循环处理字符 */
	while (*_ptr != 0)
	{
		RA8875_RAM = *_ptr;
		while ((RA8875_REG & 0x80) == 0x80);	/* 必须等待内部写屏操作完成 */
		_ptr++;
	}
	s_ucRA8875Busy = 0;

	RA8875_WriteReg(0x40, (0 << 7));	/* 还原为图形模式 */
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_DispStr
*	功能说明: 显示字符串，字符点阵来自于RA8875外接的字库芯片，支持汉字.
*			文字颜色、背景颜色、是否通透由其他的函数进行设置
*			void RA8875_SetFrontColor(uint16_t _usColor);
*			void RA8875_SetBackColor(uint16_t _usColor);
*	形    参：_usX, _usY : 显示位置（坐标）
*			 _ptr : AascII字符串，已0结束
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr)
{
	/*
		RA8875 透过使用 — 上海集通公司 (Genitop Inc) 外部串行式字体内存 (Font ROM)，可支持各样
		的文字写入到DDRAM 中。RA8875 与上海集通公司 (Genitop Inc) 兼容的产品包含 :
		GT21L16TW/GT21H16T1W 、GT23L16U2W 、GT23L24T3Y/GT23H24T3Y 、GT23L24M1Z 、
		及 GT23L32S4W/GT23H32S4W。这些字体包含16x16, 24x24, 32x32 点 (Dot) 与不同的字

		安富莱RA8875驱动板集成的字库芯片为 集通字库芯片_GT23l32S4W

		GT23L32S4W是一款内含11X12点阵、15X16点、24X24点阵、32X32点阵的汉字库芯片，支持GB2312
		国标汉字（含有国家信标委合法授权）及SCII字符。排列格式为横置横排。用户通过字符内码，利用本手
		册提供的方法计算出该字符点阵在芯片中的地址，可从该地址连续读出字符点阵信息。
	*/

	/* 设置文本显示位置，注意文本模式的写入光标和图形模式的写入光标是不同的寄存器 */
	RA8875_SetTextCursor(_usX, _usY);

	RA8875_WriteReg(0x40, (1 << 7));	/* 设置为文本模式 */

	/*
		Serial Flash/ROM 频率频率设定
			0xb: SFCL 频率 = 系统频率频率(当DMA 为致能状态，并且色彩深度为256 色，则SFCL 频率
				固定为=系统频率频率/ 2)
			10b: SFCL 频率 =系统频率频率/ 2
			11b: SFCL 频率 =系统频率频率/ 4

		安富莱驱动板系统频率为 68MHz

		GT23L32S4W的访问速度：SPI 时钟频率：20MHz(max.)

		因此需要设置 4 分频, 17MHz
	*/
	RA8875_WriteReg(0x06, (3 << 0));	/* 设置为文本模式 */

	/* 选择外部字体ROM */
	RA8875_WriteReg(0x21, (0 << 7) | (1 << 5));

	/* 05H  REG[05h] Serial Flash/ROM Configuration Register (SROC)
		7	Serial Flash/ROM I/F # 选择
				0:选择Serial Flash/ROM 0 接口。[安富莱RA8875驱动板字库芯片接在 0 接口]
				1:选择Serial Flash/ROM 1 接口。
		6	Serial Flash/ROM 寻址模式
				0: 24 位寻址模式。
				此位必须设为0。
		5	Serial Flash/ROM 波形模式
				0: 波形模式 0。
				1: 波形模式 3。
		4-3	Serial Flash /ROM 读取周期 (Read Cycle)
			00b: 4 bus ?? 无空周期 (No Dummy Cycle)。
			01b: 5 bus ??1 byte 空周期。
			1Xb: 6 bus ??2 byte 空周期。
		2	Serial Flash /ROM 存取模式 (Access Mode)
			0: 字型模式 。
			1: DMA 模式。
		1-0	Serial Flash /ROM I/F Data Latch 选择模式
			0Xb: 单一模式。
			10b: 双倍模式0。
			11b: 双倍模式1。
	*/
	RA8875_WriteReg(0x05, (0 << 7) | (0 << 6) | (1 << 5) | (1 << 3) | (0 << 2) | (0 << 1));

	/*
		设置外部字体芯片型号为 GT23L32S4W, 编码为GB2312,

		Bit1:0 决定ASCII字符的格式：
			0 = NORMAL		 [笔画细, 和汉字顶部对齐]
			1 = Arial		 [笔画粗，和汉字底部对齐]
			2 = Roman		 [笔画细, 和汉字底部对齐]
			3 = Bold		 [乱码,不可用]
	 */
	RA8875_WriteReg(0x2F, (4 << 5) | (0 << 2) | (1 << 0));

	s_ucRA8875Busy = 1;
	RA8875_REG = 0x02; 			/* 用于设定RA8875 进入内存(DDRAM或CGRAM)读取/写入模式 */

	/* 开始循环处理字符 */
	while (*_ptr != 0)
	{
		RA8875_RAM = *_ptr;
		while ((RA8875_REG & 0x80) == 0x80);
		_ptr++;
	}
	s_ucRA8875Busy = 0;

	RA8875_WriteReg(0x40, (0 << 7));	/* 还原为图形模式 */
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_SetDispWin
*	功能说明: 设置显示窗口，进入窗口显示模式。在窗口显示模式，连续写显存，光标会自动在设定窗口内进行递增
*	形    参：
*		_usX : 水平坐标
*		_usY : 垂直坐标
*		_usHeight: 窗口高度
*		_usWidth : 窗口宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth)
{

	uint16_t usTemp;

	/* 坐标系统示意图： （横屏）
			 -----------------------------
			|(0,0)                        |
			|     --------->              |
			|         |                   |
			|         |                   |
			|         |                   |
			|         V                   |
			|     --------->              |
			|                    (479,271)|
			 -----------------------------

		左上角是坐标零点, 扫描方向，先从左到右，再从上到下。

		如果需要做竖屏方式，你需要进行物理坐标和逻辑坐标的转换
	*/

	RA8875_WriteReg(0x30, _usX);
    RA8875_WriteReg(0x31, _usX >> 8);

	RA8875_WriteReg(0x32, _usY);
    RA8875_WriteReg(0x33, _usY >> 8);

	usTemp = _usWidth + _usX - 1;
	RA8875_WriteReg(0x34, usTemp);
    RA8875_WriteReg(0x35, usTemp >> 8);

	usTemp = _usHeight + _usY - 1;
	RA8875_WriteReg(0x36, usTemp);
    RA8875_WriteReg(0x37, usTemp >> 8);

	RA8875_SetCursor(_usX, _usY);

	/* 保存当前窗口信息，提高以后单色填充操作的效率.
	另外一种做法是通过读取0x30-0x37寄存器获得当前窗口，不过效率较低 */
	s_WinX = _usX;
	s_WinY = _usY;
	s_WinHeight = _usHeight;
	s_WinWidth = _usWidth;
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_SetCursor
*	功能说明: 设置写显存的光标位置（图形模式）
*	形    参：_usX : X坐标; _usY: Y坐标
*	返 回 值: 无
*********************************************************************************************************
*/
static void RA8875_SetCursor(uint16_t _usX, uint16_t _usY)
{
	/* 设置内存写光标的坐标 【注意0x80-83 是光标图形的坐标】 */
	RA8875_WriteReg(0x46, _usX);
	RA8875_WriteReg(0x47, _usX >> 8);
	RA8875_WriteReg(0x48, _usY);
	RA8875_WriteReg(0x49, _usY >> 8);
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_SetCursor
*	功能说明: 设置读显存的光标位置。 很多其他的控制器写光标和读光标是相同的寄存器，但是RA8875是不同的。
*	形    参：_usX : X坐标; _usY: Y坐标
*	返 回 值: 无
*********************************************************************************************************
*/
static void RA8875_SetReadCursor(uint16_t _usX, uint16_t _usY)
{
	/* 设置内存读光标的坐标 */
	RA8875_WriteReg(0x4A, _usX);
	RA8875_WriteReg(0x4B, _usX >> 8);
	RA8875_WriteReg(0x4C, _usY);
	RA8875_WriteReg(0x4D, _usY >> 8);
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_SetTextCursor
*	功能说明: 设置文本写入光标位置
*	形    参：_usX : X坐标; _usY: Y坐标
*	返 回 值: 无
*********************************************************************************************************
*/
static void RA8875_SetTextCursor(uint16_t _usX, uint16_t _usY)
{
	/* 设置内存读光标的坐标 */
	RA8875_WriteReg(0x2A, _usX);
	RA8875_WriteReg(0x2B, _usX >> 8);
	RA8875_WriteReg(0x2C, _usY);
	RA8875_WriteReg(0x2D, _usY >> 8);
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_QuitWinMode
*	功能说明: 退出窗口显示模式，变为全屏显示模式
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_QuitWinMode(void)
{
	RA8875_SetDispWin(0, 0, g_LcdHeight, g_LcdWidth);
}

/*
*********************************************************************************************************
*	函 数 名: RA8875_SetBackLight
*	功能说明: 配置RA8875芯片的PWM1相关寄存器，控制LCD背光
*	形    参：_bright 亮度，0是灭，255是最亮
*	返 回 值: 无
*********************************************************************************************************
*/
void RA8875_SetBackLight(uint8_t _bright)
{
	if (_bright == 0)
	{
		/* 关闭PWM, PWM1引脚缺省输出LOW  */
		RA8875_WriteReg(0x8A, 0 << 6);
	}
	else if (_bright == BRIGHT_MAX)	/* 最大亮度 */
	{
		/* 关闭PWM, PWM1引脚缺省输出HIGH */
		RA8875_WriteReg(0x8A, 1 << 6);
	}
	else
	{
		/* 使能PWM1, 进行占空比调节 */

		/* 	REG[8Ah] PWM1 Control Register (P1CR)

			Bit7 脉波宽度调变 (PWM1) 设定
				0 : 关闭，此状态下，PWM1 输出准位依照此缓存器Bit6 决定。
				1 : 开启。

			Bit6 PWM1 关闭时的准位
				0 : 当PWM 关闭或于睡眠模式时，PWM1 输出为”Low” 状态。
				1 : 当PWM 关闭或于睡眠模式时，PWM1 输出为”High” 状态。

			Bit5 保留

			Bit4 PWM1 功能选择
				0 : PWM1 功能。
				1 : PWM1 固定输出一频率为外部晶体振荡器Clock (Fin) 频率 1 /16 的Clock

			Bit3-0  PWM1 电路的频率来源选择PWM_CLK	【不是PWM输出频率】
				0000b : SYS_CLK / 1 1000b : SYS_CLK / 256
				0001b : SYS_CLK / 2 1001b : SYS_CLK / 512
				0010b : SYS_CLK / 4 1010b : SYS_CLK / 1024
				0011b : SYS_CLK / 8 1011b : SYS_CLK / 2048
				0100b : SYS_CLK / 16 1100b : SYS_CLK / 4096
				0101b : SYS_CLK / 32 1101b : SYS_CLK / 8192
				0110b : SYS_CLK / 64 1110b : SYS_CLK / 16384
				0111b : SYS_CLK / 128 1111b : SYS_CLK / 32768

				“SYS_CLK” 代表系统频率， 例如SYS_CLK 为20MHz， 当Bit[3:0] =0001b 时，PWM1 频率来源为10MHz。

				对于安富莱的4.3寸模块，SYS_CLK =  68.75MHz
				Bit3-0 选择8时，PWM_CLK = 68.75MHz / 256 = 0.2685546875 MHz = 268.55KHz；
				实际输出的PWM波形频率 = PWM_CLK / 256 = 1.049kHz。【用示波器已验证，PWM频率很准】

				选择10时，频率256Hz左右，避免1KHz的啸叫音
		*/

		RA8875_WriteReg(0x8A, (1 << 7) | 10);

		/* REG[8Bh] PWM1 Duty Cycle Register (P1DCR) */
		RA8875_WriteReg(0x8B, _bright);
	}
}

/****************************************************************************************************************************************************************************

	                                                                                  	模块名称 : TFT液晶显示器驱动模块
                                                                                           	文件名称 : SPFD5420_SPFD5420.c
*****************************************************************************************************************************************************************************
/*
*********************************************************************************************************
*
*	模块名称 : TFT液晶显示器驱动模块
*	文件名称 : SPFD5420_SPFD5420.c
*	版    本 : V2.2
*	说    明 : 安富莱开发板标配的TFT显示器分辨率为240x400，3.0寸宽屏，带PWM背光调节功能。
*				支持的LCD内部驱动芯片型号有：SPFD5420A、OTM4001A、R61509V
*				驱动芯片的访问地址为:  0x60000000
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-08-21 armfly  ST固件库版本 V3.5.0版本。
*					a) 取消访问寄存器的结构体，直接定义
*		V2.0    2011-10-16 armfly  增加R61509V驱动，实现图标显示函数
*		V2.1    2012-07-06 armfly  增加RA8875驱动，支持4.3寸屏
*		V2.2    2012-07-13 armfly  改进SPFD5420_DispStr函数，支持12点阵字符;修改SPFD5420_DrawRect,解决差
*								一个像素问题
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

/*
	友情提示：
	TFT控制器和一般的12864点阵显示器的控制器最大的区别在于引入了窗口绘图的机制，这个机制对于绘制局部图形
	是非常有效的。TFT可以先指定一个绘图窗口，然后所有的读写显存的操作均在这个窗口之内，因此它不需要CPU在
	内存中保存整个屏幕的像素数据。
*/

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "bsp_tft_lcd.h"
#include "fonts.h"


/* 定义LCD驱动器的访问地址
	TFT接口中的RS引脚连接FSMC_A0引脚，由于是16bit模式，RS对应A1地址线，因此
	SPFD5420_RAM的地址是+2
*/
#define SPFD5420_BASE       ((uint32_t)(0x60000000 | 0x00000000))

#define SPFD5420_REG		*(__IO uint16_t *)(SPFD5420_BASE)
#define SPFD5420_RAM		*(__IO uint16_t *)(SPFD5420_BASE + (1 << 17))

static __IO uint8_t s_RGBChgEn = 0;		/* RGB转换使能, 4001屏写显存后读会的RGB格式和写入的不同 */

static void SPFD5420_Delaly10ms(void);
static void SPFD5420_WriteReg(__IO uint16_t _usAddr, uint16_t _usValue);
static uint16_t SPFD5420_ReadReg(__IO uint16_t _usAddr);
static void Init_5420_4001(void);
static void Init_61509(void);
static void SPFD5420_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);
static void SPFD5420_QuitWinMode(void);
static void SPFD5420_SetCursor(uint16_t _usX, uint16_t _usY);
static uint16_t SPFD5420_BGR2RGB(uint16_t _usRGB);

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_Delaly10ms
*	功能说明: 延迟函数，不准
*	形    参:
*	返 回 值: 无
*********************************************************************************************************
*/
static void SPFD5420_Delaly10ms(void)
{
	uint16_t i;

	for (i = 0; i < 50000; i++);
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_WriteReg
*	功能说明: 修改LCD控制器的寄存器的值。
*	形    参：
*			SPFD5420_Reg ：寄存器地址;
*			SPFD5420_RegValue : 寄存器值
*	返 回 值: 无
*********************************************************************************************************
*/
static void SPFD5420_WriteReg(__IO uint16_t _usAddr, uint16_t _usValue)
{
	/* Write 16-bit Index, then Write Reg */
	SPFD5420_REG = _usAddr;
	/* Write 16-bit Reg */
	SPFD5420_RAM = _usValue;
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_ReadReg
*	功能说明: 读取LCD控制器的寄存器的值。
*	形    参：
*			SPFD5420_Reg ：寄存器地址;
*			SPFD5420_RegValue : 寄存器值
*	返 回 值: 无
*********************************************************************************************************
*/
static uint16_t SPFD5420_ReadReg(__IO uint16_t _usAddr)
{
	/* Write 16-bit Index (then Read Reg) */
	SPFD5420_REG = _usAddr;
	/* Read 16-bit Reg */
	return (SPFD5420_RAM);
}


/*
*********************************************************************************************************
*	函 数 名: SPFD5420_SetDispWin
*	功能说明: 设置显示窗口，进入窗口显示模式。TFT驱动芯片支持窗口显示模式，这是和一般的12864点阵LCD的最大区别。
*	形    参：
*		_usX : 水平坐标
*		_usY : 垂直坐标
*		_usHeight: 窗口高度
*		_usWidth : 窗口宽度
*	返 回 值: 无
*********************************************************************************************************
*/
static void SPFD5420_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth)
{
	uint16_t px, py;
	/*
		240x400屏幕物理坐标(px,py)如下:
		    R003 = 0x1018                  R003 = 0x1008
		  -------------------          -------------------
		 |(0,0)              |        |(0,0)              |
		 |                   |        |					  |
		 |  ^           ^    |        |   ^           ^   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |  ------>  |    |        |   | <------   |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |                   |        |					  |
		 |                   |        |                   |
		 |      (x=239,y=399)|        |      (x=239,y=399)|
		 |-------------------|        |-------------------|
		 |                   |        |                   |
		  -------------------          -------------------

		按照安富莱开发板LCD的方向，我们期望的虚拟坐标和扫描方向如下：(和上图第1个吻合)
		 --------------------------------
		|  |(0,0)                        |
		|  |     --------->              |
		|  |         |                   |
		|  |         |                   |
		|  |         |                   |
		|  |         V                   |
		|  |     --------->              |
		|  |                    (399,239)|
		 --------------------------------
	虚拟坐标和物理坐标转换关系：
		x = 399 - py;
		y = px;

		py = 399 - x;
		px = y;
	*/

	py = 399 - _usX;
	px = _usY;

	/* 设置显示窗口 WINDOWS */
	SPFD5420_WriteReg(0x0210, px);						/* 水平起始地址 */
	SPFD5420_WriteReg(0x0211, px + (_usHeight - 1));		/* 水平结束坐标 */
	SPFD5420_WriteReg(0x0212, py + 1 - _usWidth);		/* 垂直起始地址 */
	SPFD5420_WriteReg(0x0213, py);						/* 垂直结束地址 */

	SPFD5420_SetCursor(_usX, _usY);
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_SetCursor
*	功能说明: 设置光标位置
*	形    参：_usX : X坐标; _usY: Y坐标
*	返 回 值: 无
*********************************************************************************************************
*/
static void SPFD5420_SetCursor(uint16_t _usX, uint16_t _usY)
{
	/*
		px，py 是物理坐标， x，y是虚拟坐标
		转换公式:
		py = 399 - x;
		px = y;
	*/

	SPFD5420_WriteReg(0x0200, _usY);  		/* px */
	SPFD5420_WriteReg(0x0201, 399 - _usX);	/* py */
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_BGR2RGB
*	功能说明: RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
*	形    参：RGB颜色代码
*	返 回 值: 转化后的颜色代码
*********************************************************************************************************
*/
static uint16_t SPFD5420_BGR2RGB(uint16_t _usRGB)
{
	uint16_t  r, g, b, rgb;

	b = (_usRGB >> 0)  & 0x1F;
	g = (_usRGB >> 5)  & 0x3F;
	r = (_usRGB >> 11) & 0x1F;

	rgb = (b<<11) + (g<<5) + (r<<0);

	return( rgb );
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_QuitWinMode
*	功能说明: 退出窗口显示模式，变为全屏显示模式
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void SPFD5420_QuitWinMode(void)
{
	SPFD5420_SetDispWin(0, 0, g_LcdHeight, g_LcdWidth);
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_ReadID
*	功能说明: 读取LCD驱动芯片ID
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t SPFD5420_ReadID(void)
{
	return SPFD5420_ReadReg(0x0000);
}

/*
*********************************************************************************************************
*	函 数 名: Init_5420_4001
*	功能说明: 初始化5420和4001屏
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Init_5420_4001(void)
{
	/* 初始化LCD，写LCD寄存器进行配置 */
	SPFD5420_WriteReg(0x0000, 0x0000);
	SPFD5420_WriteReg(0x0001, 0x0100);
	SPFD5420_WriteReg(0x0002, 0x0100);

	/*
		R003H 寄存器很关键， Entry Mode ，决定了扫描方向
		参见：SPFD5420A.pdf 第15页

		240x400屏幕物理坐标(px,py)如下:
		    R003 = 0x1018                  R003 = 0x1008
		  -------------------          -------------------
		 |(0,0)              |        |(0,0)              |
		 |                   |        |					  |
		 |  ^           ^    |        |   ^           ^   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |  ------>  |    |        |   | <------   |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |  |           |    |        |   |           |   |
		 |                   |        |					  |
		 |                   |        |                   |
		 |      (x=239,y=399)|        |      (x=239,y=399)|
		 |-------------------|        |-------------------|
		 |                   |        |                   |
		  -------------------          -------------------

		按照安富莱开发板LCD的方向，我们期望的虚拟坐标和扫描方向如下：(和上图第1个吻合)
		 --------------------------------
		|  |(0,0)                        |
		|  |     --------->              |
		|  |         |                   |
		|  |         |                   |
		|  |         |                   |
		|  |         V                   |
		|  |     --------->              |
		|  |                    (399,239)|
		 --------------------------------

		虚拟坐标(x,y) 和物理坐标的转换关系
		x = 399 - py;
		y = px;

		py = 399 - x;
		px = y;

	*/
	SPFD5420_WriteReg(0x0003, 0x1018); /* 0x1018 1030 */

	SPFD5420_WriteReg(0x0008, 0x0808);
	SPFD5420_WriteReg(0x0009, 0x0001);
	SPFD5420_WriteReg(0x000B, 0x0010);
	SPFD5420_WriteReg(0x000C, 0x0000);
	SPFD5420_WriteReg(0x000F, 0x0000);
	SPFD5420_WriteReg(0x0007, 0x0001);
	SPFD5420_WriteReg(0x0010, 0x0013);
	SPFD5420_WriteReg(0x0011, 0x0501);
	SPFD5420_WriteReg(0x0012, 0x0300);
	SPFD5420_WriteReg(0x0020, 0x021E);
	SPFD5420_WriteReg(0x0021, 0x0202);
	SPFD5420_WriteReg(0x0090, 0x8000);
	SPFD5420_WriteReg(0x0100, 0x17B0);
	SPFD5420_WriteReg(0x0101, 0x0147);
	SPFD5420_WriteReg(0x0102, 0x0135);
	SPFD5420_WriteReg(0x0103, 0x0700);
	SPFD5420_WriteReg(0x0107, 0x0000);
	SPFD5420_WriteReg(0x0110, 0x0001);
	SPFD5420_WriteReg(0x0210, 0x0000);
	SPFD5420_WriteReg(0x0211, 0x00EF);
	SPFD5420_WriteReg(0x0212, 0x0000);
	SPFD5420_WriteReg(0x0213, 0x018F);
	SPFD5420_WriteReg(0x0280, 0x0000);
	SPFD5420_WriteReg(0x0281, 0x0004);
	SPFD5420_WriteReg(0x0282, 0x0000);
	SPFD5420_WriteReg(0x0300, 0x0101);
	SPFD5420_WriteReg(0x0301, 0x0B2C);
	SPFD5420_WriteReg(0x0302, 0x1030);
	SPFD5420_WriteReg(0x0303, 0x3010);
	SPFD5420_WriteReg(0x0304, 0x2C0B);
	SPFD5420_WriteReg(0x0305, 0x0101);
	SPFD5420_WriteReg(0x0306, 0x0807);
	SPFD5420_WriteReg(0x0307, 0x0708);
	SPFD5420_WriteReg(0x0308, 0x0107);
	SPFD5420_WriteReg(0x0309, 0x0105);
	SPFD5420_WriteReg(0x030A, 0x0F04);
	SPFD5420_WriteReg(0x030B, 0x0F00);
	SPFD5420_WriteReg(0x030C, 0x000F);
	SPFD5420_WriteReg(0x030D, 0x040F);
	SPFD5420_WriteReg(0x030E, 0x0300);
	SPFD5420_WriteReg(0x030F, 0x0701);
	SPFD5420_WriteReg(0x0400, 0x3500);
	SPFD5420_WriteReg(0x0401, 0x0001);
	SPFD5420_WriteReg(0x0404, 0x0000);
	SPFD5420_WriteReg(0x0500, 0x0000);
	SPFD5420_WriteReg(0x0501, 0x0000);
	SPFD5420_WriteReg(0x0502, 0x0000);
	SPFD5420_WriteReg(0x0503, 0x0000);
	SPFD5420_WriteReg(0x0504, 0x0000);
	SPFD5420_WriteReg(0x0505, 0x0000);
	SPFD5420_WriteReg(0x0600, 0x0000);
	SPFD5420_WriteReg(0x0606, 0x0000);
	SPFD5420_WriteReg(0x06F0, 0x0000);
	SPFD5420_WriteReg(0x07F0, 0x5420);
	SPFD5420_WriteReg(0x07DE, 0x0000);
	SPFD5420_WriteReg(0x07F2, 0x00DF);
	SPFD5420_WriteReg(0x07F3, 0x0810);
	SPFD5420_WriteReg(0x07F4, 0x0077);
	SPFD5420_WriteReg(0x07F5, 0x0021);
	SPFD5420_WriteReg(0x07F0, 0x0000);
	SPFD5420_WriteReg(0x0007, 0x0173);

	/* 设置显示窗口 WINDOWS */
	SPFD5420_WriteReg(0x0210, 0);	/* 水平起始地址 */
	SPFD5420_WriteReg(0x0211, 239);	/* 水平结束坐标 */
	SPFD5420_WriteReg(0x0212, 0);	/* 垂直起始地址 */
	SPFD5420_WriteReg(0x0213, 399);	/* 垂直结束地址 */
}

/*
*********************************************************************************************************
*	函 数 名: Init_61509
*	功能说明: 初始化61509屏
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Init_61509(void)
{
	SPFD5420_WriteReg(0x000,0x0000);
	SPFD5420_WriteReg(0x000,0x0000);
	SPFD5420_WriteReg(0x000,0x0000);
	SPFD5420_WriteReg(0x000,0x0000);
	SPFD5420_Delaly10ms();

	SPFD5420_WriteReg(0x008,0x0808);
	SPFD5420_WriteReg(0x010,0x0010);
	SPFD5420_WriteReg(0x400,0x6200);

	SPFD5420_WriteReg(0x300,0x0c06);	/* GAMMA */
	SPFD5420_WriteReg(0x301,0x9d0f);
	SPFD5420_WriteReg(0x302,0x0b05);
	SPFD5420_WriteReg(0x303,0x1217);
	SPFD5420_WriteReg(0x304,0x3333);
	SPFD5420_WriteReg(0x305,0x1712);
	SPFD5420_WriteReg(0x306,0x950b);
	SPFD5420_WriteReg(0x307,0x0f0d);
	SPFD5420_WriteReg(0x308,0x060c);
	SPFD5420_WriteReg(0x309,0x0000);

	SPFD5420_WriteReg(0x011,0x0202);
	SPFD5420_WriteReg(0x012,0x0101);
	SPFD5420_WriteReg(0x013,0x0001);

	SPFD5420_WriteReg(0x007,0x0001);
	SPFD5420_WriteReg(0x100,0x0730);	/* BT,AP 0x0330　*/
	SPFD5420_WriteReg(0x101,0x0237);	/* DC0,DC1,VC */
	SPFD5420_WriteReg(0x103,0x2b00);	/* VDV	//0x0f00 */
	SPFD5420_WriteReg(0x280,0x4000);	/* VCM */
	SPFD5420_WriteReg(0x102,0x81b0);	/* VRH,VCMR,PSON,PON */
	SPFD5420_Delaly10ms();

	SPFD5420_WriteReg(0x001,0x0100);
	SPFD5420_WriteReg(0x002,0x0100);
	/* SPFD5420_WriteReg(0x003,0x1030); */
	SPFD5420_WriteReg(0x003,0x1018);
	SPFD5420_WriteReg(0x009,0x0001);

	SPFD5420_WriteReg(0x0C,0x0000);	/* MCU interface  */
	/*
		SPFD5420_WriteReg(0x0C,0x0110);	//RGB interface 18bit
		SPFD5420_WriteReg(0x0C,0x0111);	//RGB interface 16bit
		SPFD5420_WriteReg(0x0C,0x0020);	//VSYNC interface
	*/

	SPFD5420_WriteReg(0x090,0x8000);
	SPFD5420_WriteReg(0x00f,0x0000);

	SPFD5420_WriteReg(0x210,0x0000);
	SPFD5420_WriteReg(0x211,0x00ef);
	SPFD5420_WriteReg(0x212,0x0000);
	SPFD5420_WriteReg(0x213,0x018f);

	SPFD5420_WriteReg(0x500,0x0000);
	SPFD5420_WriteReg(0x501,0x0000);
	SPFD5420_WriteReg(0x502,0x005f);
	SPFD5420_WriteReg(0x401,0x0001);
	SPFD5420_WriteReg(0x404,0x0000);
	SPFD5420_Delaly10ms();
	SPFD5420_WriteReg(0x007,0x0100);
	SPFD5420_Delaly10ms();
	SPFD5420_WriteReg(0x200,0x0000);
	SPFD5420_WriteReg(0x201,0x0000);
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_InitHard
*	功能说明: 初始化LCD
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SPFD5420_InitHard(void)
{
	uint16_t id;

	id = SPFD5420_ReadReg(0x0000);  	/* 读取LCD驱动芯片ID */

	if (id == 0x5420)	/* 4001屏和5420相同，4001屏读回显存RGB时，需要进行转换，5420无需 */
	{
		Init_5420_4001();	/* 初始化5420和4001屏硬件 */

		/* 下面这段代码用于识别是4001屏还是5420屏 */
		{
			uint16_t dummy;

			SPFD5420_WriteReg(0x0200, 0);
			SPFD5420_WriteReg(0x0201, 0);

			SPFD5420_REG = 0x0202;
			SPFD5420_RAM = 0x1234;		/* 写一个像素 */

			SPFD5420_WriteReg(0x0200, 0);
			SPFD5420_WriteReg(0x0201, 0);
			SPFD5420_REG = 0x0202;
			dummy = SPFD5420_RAM; 		/* 读回颜色值 */
			if (dummy == 0x1234)
			{
				s_RGBChgEn = 0;

				g_ChipID = IC_5420;
			}
			else
			{
				s_RGBChgEn = 1;		/* 如果读回的和写入的不同，则需要RGB转换, 只影响读取像素的函数 */

				g_ChipID = IC_4001;
			}
			g_LcdHeight = LCD_30_HEIGHT;
			g_LcdWidth = LCD_30_WIDTH;
		}
	}
	else if (id == 0xB509)
	{
		Init_61509();		/* 初始化61509屏硬件 */

		s_RGBChgEn = 1;			/* 如果读回的和写入的不同，则需要RGB转换, 只影响读取像素的函数 */

		g_ChipID = IC_61509;
		g_LcdHeight = LCD_30_HEIGHT;
		g_LcdWidth = LCD_30_WIDTH;
	}
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_DispOn
*	功能说明: 打开显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SPFD5420_DispOn(void)
{
	if (g_ChipID == IC_61509)
	{
		SPFD5420_WriteReg(0x007,0x0100);
	}
	else	/* IC_4001 */
	{
		SPFD5420_WriteReg(7, 0x0173); /* 设置262K颜色并且打开显示 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_DispOff
*	功能说明: 关闭显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void SPFD5420_DispOff(void)
{
	SPFD5420_WriteReg(7, 0x0000);
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_ClrScr
*	功能说明: 根据输入的颜色值清屏
*	形    参：_usColor : 背景色
*	返 回 值: 无
*********************************************************************************************************
*/
void SPFD5420_ClrScr(uint16_t _usColor)
{
	uint32_t i;

	SPFD5420_SetCursor(0, 0);		/* 设置光标位置 */

	SPFD5420_REG = 0x202; 			/* 准备读写显存 */

	for (i = 0; i < g_LcdHeight * g_LcdWidth; i++)
	{
		SPFD5420_RAM = _usColor;
	}
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_PutPixel
*	功能说明: 画1个像素
*	形    参：
*			_usX,_usY : 像素坐标
*			_usColor  ：像素颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void SPFD5420_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	SPFD5420_SetCursor(_usX, _usY);	/* 设置光标位置 */

	/* 写显存 */
	SPFD5420_REG = 0x202;
	/* Write 16-bit GRAM Reg */
	SPFD5420_RAM = _usColor;
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_GetPixel
*	功能说明: 读取1个像素
*	形    参：
*			_usX,_usY : 像素坐标
*			_usColor  ：像素颜色
*	返 回 值: RGB颜色值
*********************************************************************************************************
*/
uint16_t SPFD5420_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint16_t usRGB;

	SPFD5420_SetCursor(_usX, _usY);	/* 设置光标位置 */

	{
		/* 准备写显存 */
		SPFD5420_REG = 0x202;

		usRGB = SPFD5420_RAM;

		/* 读 16-bit GRAM Reg */
		if (s_RGBChgEn == 1)
		{
			usRGB = SPFD5420_BGR2RGB(usRGB);
		}
	}

	return usRGB;
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_DrawLine
*	功能说明: 采用 Bresenham 算法，在2点间画一条直线。
*	形    参：
*			_usX1, _usY1 ：起始点坐标
*			_usX2, _usY2 ：终止点Y坐标
*			_usColor     ：颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void SPFD5420_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor)
{
	int32_t dx , dy ;
	int32_t tx , ty ;
	int32_t inc1 , inc2 ;
	int32_t d , iTag ;
	int32_t x , y ;

	/* 采用 Bresenham 算法，在2点间画一条直线 */

	SPFD5420_PutPixel(_usX1 , _usY1 , _usColor);

	/* 如果两点重合，结束后面的动作。*/
	if ( _usX1 == _usX2 && _usY1 == _usY2 )
	{
		return;
	}

	iTag = 0 ;
	/* dx = abs ( _usX2 - _usX1 ); */
	if (_usX2 >= _usX1)
	{
		dx = _usX2 - _usX1;
	}
	else
	{
		dx = _usX1 - _usX2;
	}

	/* dy = abs ( _usY2 - _usY1 ); */
	if (_usY2 >= _usY1)
	{
		dy = _usY2 - _usY1;
	}
	else
	{
		dy = _usY1 - _usY2;
	}

	if ( dx < dy )   /*如果dy为计长方向，则交换纵横坐标。*/
	{
		uint16_t temp;

		iTag = 1 ;
		temp = _usX1; _usX1 = _usY1; _usY1 = temp;
		temp = _usX2; _usX2 = _usY2; _usY2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = _usX2 > _usX1 ? 1 : -1 ;    /* 确定是增1还是减1 */
	ty = _usY2 > _usY1 ? 1 : -1 ;
	x = _usX1 ;
	y = _usY1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != _usX2 )     /* 循环画点 */
	{
		if ( d < 0 )
		{
			d += inc1 ;
		}
		else
		{
			y += ty ;
			d += inc2 ;
		}
		if ( iTag )
		{
			SPFD5420_PutPixel ( y , x , _usColor) ;
		}
		else
		{
			SPFD5420_PutPixel ( x , y , _usColor) ;
		}
		x += tx ;
	}
}


/*
*********************************************************************************************************
*	函 数 名: SPFD5420_DrawRect
*	功能说明: 绘制水平放置的矩形。
*	形    参：
*			_usX,_usY：矩形左上角的坐标
*			_usHeight ：矩形的高度
*			_usWidth  ：矩形的宽度
*	返 回 值: 无
*********************************************************************************************************
*/
void SPFD5420_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint16_t _usColor)
{
	/*
	 ---------------->---
	|(_usX，_usY)        |
	V                    V  _usHeight
	|                    |
	 ---------------->---
		  _usWidth
	*/

	SPFD5420_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _usColor);	/* 顶 */
	SPFD5420_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _usColor);	/* 底 */

	SPFD5420_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _usColor);	/* 左 */
	SPFD5420_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _usColor);	/* 右 */
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_DrawCircle
*	功能说明: 绘制一个圆，笔宽为1个像素
*	形    参：
*			_usX,_usY  ：圆心的坐标
*			_usRadius  ：圆的半径
*	返 回 值: 无
*********************************************************************************************************
*/
void SPFD5420_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* 当前 X 值 */
	uint32_t  CurY;		/* 当前 Y 值 */

	D = 3 - (_usRadius << 1);
	CurX = 0;
	CurY = _usRadius;

	while (CurX <= CurY)
	{
		SPFD5420_PutPixel(_usX + CurX, _usY + CurY, _usColor);
		SPFD5420_PutPixel(_usX + CurX, _usY - CurY, _usColor);
		SPFD5420_PutPixel(_usX - CurX, _usY + CurY, _usColor);
		SPFD5420_PutPixel(_usX - CurX, _usY - CurY, _usColor);
		SPFD5420_PutPixel(_usX + CurY, _usY + CurX, _usColor);
		SPFD5420_PutPixel(_usX + CurY, _usY - CurX, _usColor);
		SPFD5420_PutPixel(_usX - CurY, _usY + CurX, _usColor);
		SPFD5420_PutPixel(_usX - CurY, _usY - CurX, _usColor);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}

/*
*********************************************************************************************************
*	函 数 名: SPFD5420_DrawBMP
*	功能说明: 在LCD上显示一个BMP位图，位图点阵扫描次序：从左到右，从上到下
*	形    参：
*			_usX, _usY : 图片的坐标
*			_usHeight  ：图片高度
*			_usWidth   ：图片宽度
*			_ptr       ：图片点阵指针
*	返 回 值: 无
*********************************************************************************************************
*/
void SPFD5420_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr)
{
	uint32_t index = 0;
	const uint16_t *p;

	/* 设置图片的位置和大小， 即设置显示窗口 */
	SPFD5420_SetDispWin(_usX, _usY, _usHeight, _usWidth);

	p = _ptr;
	for (index = 0; index < _usHeight * _usWidth; index++)
	{
		SPFD5420_PutPixel(_usX, _usY, *p++);
	}

	/* 退出窗口绘图模式 */
	SPFD5420_QuitWinMode();
}

/*
*********************************************************************************************************
*	函 数 名: SetBackLight_byMCU
*	功能说明: 初始化控制LCD背景光的GPIO,配置为PWM模式。
*			当关闭背光时，将CPU IO设置为浮动输入模式（推荐设置为推挽输出，并驱动到低电平)；将TIM3关闭 省电
*	形    参：_bright 亮度，0是灭，255是最亮
*	返 回 值: 无
*********************************************************************************************************
*/
void SPFD5420_SetBackLight(uint8_t _bright)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* 第1步：打开GPIOB RCC_APB2Periph_AFIO 的时钟	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	if (_bright == 0)
	{
		/* 配置背光GPIO为输入模式 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* 关闭TIM3 */
		TIM_Cmd(TIM3, DISABLE);
		return;
	}
	else if (_bright == BRIGHT_MAX)	/* 最大亮度 */
	{
		/* 配置背光GPIO为推挽输出模式 */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_SetBits(GPIOB, GPIO_Pin_1);

		/* 关闭TIM3 */
		TIM_Cmd(TIM3, DISABLE);
		return;
	}

	/* 配置背光GPIO为复用推挽输出模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 使能TIM3的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/*
		TIM3 配置: 产生1路PWM信号;
		TIM3CLK = 72 MHz, Prescaler = 0(不分频), TIM3 counter clock = 72 MHz
		计算公式：
		PWM输出频率 = TIM3 counter clock /(ARR + 1)

		我们期望设置为100Hz

		如果不对TIM3CLK预分频，那么不可能得到100Hz低频。
		我们设置分频比 = 1000， 那么  TIM3 counter clock = 72KHz
		TIM_Period = 720 - 1;
		频率下不来。
	 */
	TIM_TimeBaseStructure.TIM_Period = 720 - 1;	/* TIM_Period = TIM3 ARR Register */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*
		_bright = 1 时, TIM_Pulse = 1
		_bright = 255 时, TIM_Pulse = TIM_Period
	*/
	TIM_OCInitStructure.TIM_Pulse = (TIM_TimeBaseStructure.TIM_Period * _bright) / BRIGHT_MAX;	/* 改变占空比 */

	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);

	/* 使能 TIM3 定时器 */
	TIM_Cmd(TIM3, ENABLE);
}
/********************************************************************************************************************************************************************************************
                                                                                                main.c中的文件

*********************************************************************************************************************************************************************************************/
/*
*********************************************************************************************************
*	函 数 名: MainMenu
*	功能说明: 主菜单状态
*	形    参：无
*	返 回 值: 状态字
*********************************************************************************************************
*/
static uint8_t MainMenu(void)
{
//	uint8_t ucKeyCode;		/* 按键代码 */
	uint8_t ucTouch;		/* 触摸事件 */
	uint8_t fRefresh;		/* 刷屏请求标志,1表示需要刷新 */
	FONT_T tFont;			/* 定义一个字体结构体变量，用于设置字体参数 */
	char buf[64];
	uint16_t usAdcX, usAdcY;
	int16_t tpX, tpY;

	LCD_ClrScr(CL_BLUE);  	/* 清屏，背景蓝色 */

	/* 设置字体参数 */
	{
		tFont.usFontCode = FC_ST_16;	/* 字体代码 16点阵 */
		tFont.usTextColor = CL_WHITE;	/* 字体颜色 */
		tFont.usBackColor = CL_BLUE;	/* 文字背景颜色 */
		tFont.usSpace = 0;				/* 文字间距，单位 = 像素 */
	}

	fRefresh = 1;	/* 1表示需要刷新LCD */
	while (1)
	{
		CPU_IDLE();

		if (fRefresh)
		{
			fRefresh = 0;

			/* 实时刷新触摸ADC采样值和转换后的坐标 */
			{
				LCD_DispStr(5, 3, "安富莱触摸屏例程", &tFont);			/* 在(8,3)坐标处显示一串汉字 */

				/* 显示TFT控制器型号和屏幕分辨率 */
				LCD_GetChipDescribe(buf);
				sprintf(&buf[strlen(buf)], "   %d x %d", LCD_GetWidth(), LCD_GetHeight());
				LCD_DispStr(5, 23, buf, &tFont);			/* 在(8,3)坐标处显示一串汉字 */

				/* 读取并显示当前X轴和Y轴的ADC采样值 */
				usAdcX = TOUCH_ReadAdcX();
				usAdcY = TOUCH_ReadAdcY();
				sprintf(buf, "触摸ADC值 X = %4d, Y = %4d   ", usAdcX, usAdcY);
				LCD_DispStr(5, 60, buf, &tFont);

				/* 读取并显示当前触摸坐标 */
				tpX = TOUCH_GetX();
				tpY = TOUCH_GetY();
				sprintf(buf, "触摸坐标  X = %4d, Y = %4d   ", tpX, tpY);
				LCD_DispStr(5, 80, buf, &tFont);

				/* 在触笔所在位置显示一个小圈 */
				if ((tpX > 0) && (tpY > 0))
				{
					LCD_DrawCircle(tpX, tpY, 2, CL_YELLOW);
				}
			}

			/* 在指定位置显示3个图标 */
			LCD_DrawIcon(ICON1_X, ICON1_Y, ICON1_H, ICON1_W, (uint16_t *)gImage_QQ);
			LCD_DispStr(ICON1_X + 1, ICON1_Y + ICON1_H + 2, " 校准", &tFont);

			LCD_DrawIcon(ICON2_X, ICON2_Y, ICON2_H, ICON2_W, (uint16_t *)gImage_Bat);
			LCD_DispStr(ICON2_X + 1, ICON2_Y + ICON2_H + 2, " 背光", &tFont);

			LCD_DrawIcon(ICON3_X, ICON3_Y, ICON3_H, ICON3_W, (uint16_t *)gImage_Spk);
			LCD_DispStr(ICON3_X + 1, ICON3_Y + ICON3_H + 2, " 清屏", &tFont);
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* 读取触摸事件 */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* 触笔按下事件 */
					if (TOUCH_InRect(tpX, tpY, ICON1_X, ICON1_Y, ICON1_H, ICON1_W))
					{
						LCD_DrawIconActive(ICON1_X, ICON1_Y, ICON1_H, ICON1_W, (uint16_t *)gImage_QQ);
					}
					else if (TOUCH_InRect(tpX, tpY, ICON2_X, ICON2_Y, ICON2_H, ICON2_W))
					{
						LCD_DrawIconActive(ICON2_X, ICON2_Y, ICON2_H, ICON2_W, (uint16_t *)gImage_Bat);
					}
					else if (TOUCH_InRect(tpX, tpY, ICON3_X, ICON3_Y, ICON3_H, ICON3_W))
					{
						LCD_DrawIconActive(ICON3_X, ICON3_Y, ICON3_H, ICON3_W, (uint16_t *)gImage_Spk);
					}
					break;

				case TOUCH_MOVE:		/* 触笔移动事件 */
					/* 实时刷新触摸ADC采样值和转换后的坐标 */
					{
						/* 读取并显示当前X轴和Y轴的ADC采样值 */
						usAdcX = TOUCH_ReadAdcX();
						usAdcY = TOUCH_ReadAdcY();
						sprintf(buf, "触摸ADC值 X = %4d, Y = %4d   ", usAdcX, usAdcY);
						LCD_DispStr(5, 60, buf, &tFont);

						/* 读取并显示当前触摸坐标 */
						tpX = TOUCH_GetX();
						tpY = TOUCH_GetY();
						sprintf(buf, "触摸坐标  X = %4d, Y = %4d   ", tpX, tpY);
						LCD_DispStr(5, 80, buf, &tFont);

						/* 在触笔所在位置显示一个小圈 */
						if ((tpX > 0) && (tpY > 0))
						{
							LCD_DrawCircle(tpX, tpY, 2, CL_YELLOW);
						}
					}
					break;

				case TOUCH_RELEASE:		/* 触笔释放事件 */
					if (TOUCH_InRect(tpX, tpY, ICON1_X, ICON1_Y, ICON1_H, ICON1_W))
					{
						return MS_CALIBRATION;		/* 进入触摸校准界面 */
					}
					else if (TOUCH_InRect(tpX, tpY, ICON2_X, ICON2_Y, ICON2_H, ICON2_W))
					{
						return MS_ADJUST_BRIGHT;	/* 进入背光调节界面 */
					}
					else if (TOUCH_InRect(tpX, tpY, ICON3_X, ICON3_Y, ICON3_H, ICON3_W))
					{
						LCD_ClrScr(CL_BLUE);  		/* 清屏，背景蓝色 */
						fRefresh = 1;
					}
					break;
			}
		}

#if 0
		ucKeyCode = bsp_GetKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/* 有键按下 */
			switch (ucKeyCode)
			{
				case  KEY_DOWN_JOY_OK:		/* 摇杆OK键 */
					return MS_CALIBRATION;	/* 进入触摸界面 */
					//break;

				default:
					break;
			}
		}
#endif
	}
}

/*
*********************************************************************************************************
*	函 数 名: AdjustBright
*	功能说明: 调节背景光
*	形    参：无
*	返 回 值: 状态字
*********************************************************************************************************
*/
static uint8_t AdjustBright(void)
{
	uint8_t ucBright;		/* 设置为缺省亮度 */
//	uint8_t ucKeyCode;		/* 按键代码 */
	uint8_t fRefresh;		/* 界面刷新请求标志 */
	char buf[64];
	FONT_T tFont;		/* 定义一个字体结构体变量，用于设置字体参数 */

	/* 设置字体参数 */
	{
		tFont.usFontCode = FC_ST_16;	/* 字体代码 16点阵 */
		tFont.usTextColor = CL_WHITE;	/* 字体颜色 */
		tFont.usBackColor = CL_BLUE;	/* 文字背景颜色 */
		tFont.usSpace = 0;				/* 文字间距，单位 = 像素 */
	}

	/* 读取当前的背光亮度参数 */
	ucBright = LCD_GetBackLight();

	LCD_ClrScr(CL_BLUE);	/* 清除屏幕 */

	LCD_DispStr(10, 10, "摇杆上下键调节LCD背景光", &tFont);
	LCD_DispStr(10, 30, "摇杆中键确认退出", &tFont);

	/* 显示1个彩色图标，便于观察背光效果 */
	LCD_DrawIcon(10, 70, ICON1_H, ICON1_W, (uint16_t *)gImage_UDisk);

	/* 进入主程序循环体 */
	fRefresh = 1;
	while (1)
	{
		CPU_IDLE();

		if (fRefresh == 1)
		{
			fRefresh = 0;

			sprintf(buf, "当前LCD背景光亮度 = %d  ", ucBright);
			LCD_DispStr(10, 50, buf, &tFont);
		}

#if 0
		ucKeyCode = bsp_GetKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/* 有键按下 */
			switch (ucKeyCode)
			{
				case KEY_DOWN_JOY_LEFT:		/* 摇杆LEFT键按下 */
				case KEY_DOWN_JOY_RIGHT:	/* 摇杆RIGHT键按下 */
					break;

				case KEY_DOWN_JOY_UP:		/* 摇杆UP键按下 */
					if (ucBright < 255)
					{
						ucBright++;
					}
					LCD_SetBackLight(ucBright);
					fRefresh = 1;
					break;

				case KEY_DOWN_JOY_DOWN:		/* 摇杆DOWN键按下 */
					if (ucBright > 0)
					{
						ucBright--;
					}
					LCD_SetBackLight(ucBright);
					fRefresh = 1;
					break;

				case KEY_DOWN_JOY_OK:		/* 摇杆OK键 */
					return MS_MAIN_MENU;	/* 返回到主菜单 */
					/* break;*/

				default:
					break;
			}
		}
#endif
	}
}

/*
*********************************************************************************************************
*	函 数 名: InitBoard
*	功能说明: 初始化硬件设备
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void InitBoard(void)
{
	/* 配置串口，用于printf输出 */
	bsp_InitUart();

	/* 配置LED指示灯GPIO */
//	bsp_InitLed();

	/* 配置按键GPIO, 必须在bsp_InitTimer之前调用 */
//	bsp_InitButton();

	/* 初始化systick定时器，并启动定时中断 */
	bsp_InitTimer();
}

/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	printf("*************************************************************\n\r");
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", DEMO_VER);		/* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	/* 打印ST固件库版本，这3个定义宏在stm32f10x.h文件中 */
	printf("* 固件库版本 : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN,
			__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("* \n\r");	/* 打印一行空格 */
	printf("* QQ    : 1295744630 \r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* Copyright www.armfly.com 安富莱电子\r\n");
	printf("*************************************************************\n\r");


