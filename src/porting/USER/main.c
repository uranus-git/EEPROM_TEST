 /*******************************************************************************
-- Filename ©seeprom_test.c
-- Author ©sskylark
-- Description ©s2kbit eeprom_test
-- Revision History ©s2015-04-16
-- Revision 1.0
-- Company ©skeybridge ltd.company
-- Copyright(c) 2015, keybridge ltd.company, All right reserved
*******************************************************************************/
//addr  [10:3] used,  addr[2:0] no-used;    PE[15:8]
//din_e [15:0] used,                        PD[15:0]
//dout  [7:0]  used,                        PA[7:0]     //receive data
//clk            PD3    //cancel
//read           PC10
//write          PC11
//sync           PC12
//all_on[1:0]    PC[5:4]
//wt_end         PB8     //

#include "stm32f4xx.h"
#include <stdio.h>
#include "eeprom.h"
#include "datarom.h"


void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE , ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12|
	                              GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9| GPIO_Pin_8 ;  //addr[10:3] PE15: 8 address
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD , ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12|
                              	GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9  | GPIO_Pin_8 |
	                              GPIO_Pin_7  | GPIO_Pin_6  | GPIO_Pin_5  | GPIO_Pin_4 |
	                              GPIO_Pin_3  | GPIO_Pin_2 | GPIO_Pin_1| GPIO_Pin_0 ;    //DATA[15:0]
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA , ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7  | GPIO_Pin_6  | GPIO_Pin_5  | GPIO_Pin_4 | //dout[7:0]
	                               GPIO_Pin_3  | GPIO_Pin_2  | GPIO_Pin_1  | GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  /* TODO unknown how to replace IPU on stm32f407 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;*/
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC , ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10  | GPIO_Pin_11 | GPIO_Pin_12  | GPIO_Pin_13;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB , ENABLE); // wt_end GPIOB8;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  /* TODO unknown how to replace IPU on stm32f407 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;   */
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
// unsigned short is 16bits(sizeof(unsigned short) = 16)
// the 16bits data: low8bit high8bit
unsigned int q = 0;
unsigned short i, j, m, n;
#if 0
int main(void)
{
   // unsigned int q;
    //unsigned short i, j, m, n;
			unsigned short l; // the number of special write sucess;
	  unsigned char databuf1, databuf2, databuf3;
	  unsigned short data_a;
    unsigned char rd_data[256];
	  unsigned char rd_data_a[256];
	  unsigned short buff_data[256];// special write buff_data
	  m = 0;
	  n = 0;
	  l = 0;
    j =0;

	Timer_Init();

	while(1);

  	SystemInit();
    GPIO_Configuration();
	  gpio_initial();
	  debug();
//write word, number 128
	  for (j = 0x00; j < 0x80; j++)
	      {
			  //single_write(j << 1, 0x00aa, buff_data);
			  single_write(j << 1, 0xa500 + j, buff_data);
		    }
    l =  read_2kbit(buff_data);	//if l = 0x80 block write success

//--------------------------comon read ---------------------
//	for(j=0x00; j<0x100; j++)       //always read 256
//  read byte, number 256
		for (i = 0x00; i < 0x100; i++){
        rd_data[i] = rd_byte(i);
	  }

    eep_read(0x00, 0x80, rd_data_a)	;//read 0x80 datas
	  eep_read(0x00, 0x08, rd_data_a)	;//read 0x08 datas
	//---------------------------write data-------------------------
    wr_word(0xa4, 0xffff);
	  wr_word(0x02, 0x0001);
		cmd_write(data_word1, 0x00, 0x80);
		cmd_write(data_word2, 0x00, 0x80);
		cmd_write(data_word3, 0x00, 0x80);
		cmd_write(data_word4, 0x00, 0x80);
		cmd_write(data_word5, 0x00, 0x80);
		cmd_write(data_word6, 0x00, 0x80);

    for ( i= 0x00; i < 0x100; i++){
         wr_word(i << 1,0x0000);
        }
// write two array, if m != 0x80 ||n !=0x80 read failed.
		m = write_read_2kbit(data_word5);   //0xaaaa
		n = write_read_2kbit(data_word4);   //0x5555
	  l = read_2kbit(data_word4);		// read compare
	//for(j=0x00; j<0x100; j++)       //always read 256
//read byte, number 256
		for(i = 0x00; i < 0x100; i++){
        rd_data[i] = rd_byte(i);
	  }
	//--------------------------read data start---------------------
	cmd_write(data_word2, 0x00, 0x80);	    //all write 0
  cmd_write(data_word3, 0x00, 0x80);		  //all write	1
//----chapter.1  chapter.2-1  chapter.4-2 Test Report----------
//--------------------------------------------------------------
//----data_word1, data_word2=0, data_word3=1--------------------

    cmd_write(data_word1, 0x00, 0x80);		  //
//----------------------------------------------------------------
    eep_read(0x00, 0x100, rd_data_a);	   //read continue, always read=1. 20150127    256bytes
    eep_read(0x00, 0x008, rd_data_a);    // 8btyes
//------------------------------------------------------------
//----data_word1, 1.8V     20times------------------
//------------chapter.3.1-------------------------------------
		//for(i = 0x00; i< 0x10; i++)        //write data 256times
		//  for (i = 0x00; i< 5; i++)
    //  for (i = 0x00; i< 10; i++)
    //  for (i = 0x00; i< 20; i++)
    for (i = 0x00; i < 0x1000; i++)//read data
	      {
	       cmd_write(data_word1, 0x00, 0x80);    //write 2Kbit data
	      }
				eep_read(0x00, 0x100, rd_data_a);  //read  2Kbit data
//------------------------------------------------------------
//---- data_word2=0, data_word3=1-----------------all 0, all 1
//------------chapter.3.2.1-------------------------------------

		cmd_write(data_word2, 0x00, 0x80);  // all 0
	  cmd_write(data_word3, 0x00, 0x80);  // all 1
			//--------------------------read data start---------------------
//	for(j=0x00; j<0x100; j++)       //always read 256
//read byte, number 256
		for(i = 0x00; i < 0x100; i++){
        rd_data[i] = rd_byte(i);
	  }
//------------------------------------------------------------
//----data_word1, data_word2=0, data_word3=1------------------
//------------chapter.3.2.2 -------------------------------------
//march test
/* fist step :   all 2Kbit write 0,
   second step:           A0      A1       A2        A3
               read    0x0000   0x0000    0x0000   0x0000
               write   0x5555   0x5555    0xaaaa   0xaaaa
	                        .       .          .       .
                          .       .          .       .
*/
		// if j = 0x80, pass
    cmd_write(data_word2, 0x00, 0x80);//block write 0
	  for ( i = 0x00, j = 0x00, data_a = 0xaaaa; i < 0x100; i = i+0x04)	 //  second step  addr +++++
	      {
		        data_a = data_shift(data_a);	   //0x5555, 0xaaaa, exchange
		        databuf1 = (unsigned int)rd_byte(i);
		        databuf2 = (unsigned int)rd_byte(i + 1);
		        databuf3 = databuf1 + (databuf2 << 8);

		        if (databuf3 == 0x0000)
		           {
			             j = j + 0x01;
		           }
		        wr_word(i, data_a);	  //write first word

		        databuf1 = (unsigned int)rd_byte(i + 2);
		        databuf2 = (unsigned int)rd_byte(i + 3);
		        databuf3 = databuf1 + (databuf2<<8);
		       if (databuf3 == 0x0000)
		          {
			            j = j + 0x01;
	            }
		          wr_word(i + 2, data_a);   //write second word
	      }

/*----------------------------------------------------------------------
    Descending order        A255     A254      A253       A252
	                read      0xaaaa   0xaaaa   0x5555      0x5555
	                write     0x5555   0x5555    0xaaaa     0xaaaa

---------------------------------------------------------------------*/
			// if j = 0x80, pass
    for (i = 0x00, j = 0x00, data_a = 0x5555; i < 0x100; i = i + 0x04)	//addr ------
	      {
		        data_a =  data_shift(data_a);
		        databuf1 = (unsigned int)rd_byte(~i - 1);  //FD   254
		        databuf2 = (unsigned int)rd_byte(~i);    //FF   255
		        databuf3 = databuf1 + (databuf2 << 8);

		    if (databuf3 == data_a)
		       {
			         j = j + 0x01;
		       }
		    wr_word(~i - 1, data_shift(data_a));

		    databuf1 = (unsigned int)rd_byte(~i-3);
		    databuf2 = (unsigned int)rd_byte(~i-2);
		    databuf3 = databuf1 + (databuf2 << 8);
		    if (databuf3 == data_a)
		       {
			         j = j + 0x01;
		       }
		    wr_word(~i - 3, data_shift(data_a));
	       }
//read and compare with dataword6
    l = read_2kbit(data_word6);					 //if l = 0x80, pass
//------------------------------------------------------------
//------------chapter.3.2.3-------------------------------------
/*------------------------------------------------------------
	step 1     addr    data1[31:16]  data0[15:0]
	            3-0    0x0000     0x5A5A
	            7-4    0x5A5A     0x0000
	            11-8   0x0000     0x5A5A

	step 2       3-0   0x5A5A     0x0000
	             7-4   0x0000     0x5A5A
	             11-8  0x5A5A     0x0000

	step 3       3-0   0x0000     0x5A5A
	             7-4   0x0000     0x5A5A
	             11-8  0x0000     0x5A5A
-------------------------------------------------*/

    cmd_write(data_word2, 0x00, 0x80);   //clear
//	step 1
	  for (i = 0x00; i < 0x80; i += 0x04)		     //addr +++++
	      {
		        wr_word((i << 1), 0x5a5a);     // adress 0 and 3 write common data
		        wr_word((i + 3) << 1, 0x5a5a);
	      }
//step 2
	  cmd_write(data_word2, 0x00, 0x80);   //clear
	  for (i = 0x00; i < 0x80; i += 0x04)	//addr +++++
				{
		         wr_word((i + 1) << 1, 0x5a5a);    // 1 and 2 write common data
		         wr_word((i + 2 ) << 1, 0x5a5a);
	      }
//step 3
	  cmd_write(data_word2, 0x00, 0x80);   //clear
	  for (i = 0x00; i < 0x80; i += 0x02)		     //addr +++++
	      {
		        wr_word(i << 1, 0x5a5a);
	      }
//======================chapter.3-1==========================
		for (i = 0x00; i < 0x14; i++)		        //test 20times
		    {
			      m = write_read_2kbit(data_word4);   //data:  data_word4,  data_word5 , change datarom.c
			      n = write_read_2kbit(data_word5);
			     if ((m != 0x80) || (n != 0x80))
			        {
			            break;
			        }
		    }
//===============EEPROM Reset and Data Loading=======================

//--------------------write data, and read------------------------
//---------------------chapter 3.7----------------------------------
		//for (q = 0x0000; q < 0x286A0; q++)	//165500
			//	for (q = 0x0000; q < 10; q++)

				for(q=0x0000; q<0x120000; q++)	//100 cycle, test time = 30s
		    {
					  cmd_write(data_word2, 0x00, 0x80);
			      m = write_read_2kbit(data_word4);
					 if (m != 0x80)
			         {
				           break;
			         }
			    /*  n = write_read_2kbit(data_word5);
			      if (n != 0x80)
			         {
				           break;
			         }	*/
		    }
	  //return  q;


}
#endif

#include "common.h"
#include "stdio.H"
#include "usart1.H"
#include "LED.H"
#include "S13EE_X64Y64.h"

extern void bsp_init(void);
extern void EEP_T_MAIN(S13EE *s13ee);
__IO int singleDelayFlag = 0;

int main(void)
{
    S13EE s13ee;

	//bsp_init();
    S13EE_INIT (&s13ee);
    EEP_T_MAIN(&s13ee);

    while(1)
    {
    }

}
