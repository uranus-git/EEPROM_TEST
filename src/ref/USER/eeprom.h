#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "stm32f10x.h"
//addr  [10:3] used,  addr[2:0] no-used;    PE[15:8]
//din_e [15:0] used,                        PD[15:0]
//dout  [7:0]  used,                        PA[7:0]     //receive data
//clk            PD3    //cancel
//read           PC10                               
//write          PC11                              
//sync           PC12                          
//all_on[1:0]    PC[5:4]
//wt_end         PB8     //
#define write_low()    GPIO_ResetBits(GPIOC, GPIO_Pin_11)// write = 0
#define write_high()   GPIO_SetBits(GPIOC, GPIO_Pin_11) // write = 1;

#define read_low()     GPIO_ResetBits(GPIOC, GPIO_Pin_10)// read = 0
#define read_high()    GPIO_SetBits (GPIOC, GPIO_Pin_10)// read =1;
     
#define sync_low()     GPIO_ResetBits(GPIOC, GPIO_Pin_12)// read = 0
#define sync_high()    GPIO_SetBits (GPIOC, GPIO_Pin_12) // read =1;

#define wt_end    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)// return wt_end value

#define dout()    ((GPIO_ReadInputData(GPIOA) & 0x00ff)) // read data 8 bit

void wr_addr(unsigned char addr) ;  //write the addres to bus
void wr_data(unsigned short data);    //write data to bus
void delay_ms(u16 time);            // ms delay
void delay_us(u16 time);            //us delay please test
void delay_100ns(unsigned short time);  //100ns delay
void wr_word(unsigned char addr, unsigned short data);//write a word to parameter addr
unsigned char rd_byte(unsigned char addr);          // read a byte from parameter addr
void eep_read(unsigned char addr_start, unsigned short num, unsigned char *p_data);//Continuous read
void cmd_write(const unsigned short *cmd_tab, unsigned char start_addr, unsigned char length);//Continuous write
unsigned short data_shift(unsigned short temp);	//data exchange
unsigned short write_read_2kbit(const unsigned short *cmd_tab);// compare write data and  read data until the data not equal, break  
void debug(void);  //test current 
void gpio_initial(void);//initial write = 0 read = 0
unsigned short read_2kbit(const unsigned short *cmd_tab);	//read 2kbit and compare
void single_write(unsigned char addr, unsigned short data, unsigned short *cmd_tab);
#endif
