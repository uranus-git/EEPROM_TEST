#include "stm32f4xx.h"
#include "eeprom.h"

void wr_addr(unsigned char addr)   //write the addr to bus
{
    GPIO_Write(GPIOE, addr << 8);
}

void wr_data(unsigned short data)
{
    GPIO_Write(GPIOD, data);
}

void delay_ms(unsigned short time)       // ms delay
{
    unsigned short i = 0;

	  while (time--) {
        i = 12000;
        while(i--);
   }
}
void delay_100ns(unsigned short time)
{
	     while (time--)  {
            __nop();__nop();__nop();__nop();
	     }
}
void delay_us(unsigned short time)        //us delay please test
{
	   unsigned short i = 0;

	   while (time--)  {
	     i = 10;
		   while(i--);
  }
}
void wr_word(unsigned char addr, unsigned short data)  //all time according to test
{
    read_low();                                        // read =0
  	write_low();                                       // write = 0
	sync_low();                                        // sync =0
	wr_addr(addr);                                     // write adress
	wr_data(data);	                                   // write data
	write_high() ;                                     // write = 1
	delay_us(5);                                       // sync = 1 5us delay
	sync_high();                                       // sync = 1
	  //delay_us(1);                                     // sync = 1 lasting min 0.5us, we keep 1us
	delay_100ns(2);                                     // no delay IO flip  275ns+500ns 750ns
	//__nop();__nop();__nop();__nop();
  	sync_low();                                        // reset sync  0.7us
	while (!wt_end);	                                 // wait for the write procese finish
    delay_us(6);                                       // 5us delay
  	write_low();                                       // write =  0
	delay_us(5);                                       // delay 5u  whether it is not neccessary, need test.
}

unsigned char rd_byte(unsigned char addr)  // read a byte  reference in read timing
{
    unsigned char rd_data;              //  definite the varialbe for  read data

	  wr_data(0x0000);                    // set bus to low level
	  read_low();                         // read = 0
	  sync_low();                         // sync set =0
	  write_low();                        // write = 0
	  read_high();                        // read = 1
	  wr_addr(addr);                      // write the address to bus
    delay_100ns(1);
		sync_high();                        // sync = 1
		delay_us(4);		                    // Pulse width of sync high in read mode  type 6 us 6.5us
    delay_100ns(3);
		sync_low();                         // produce falling edge
		//delay(0x0003);                    // appropriate delay
	  delay_100ns(1);
		rd_data = dout();	                  // read a byte
		//delay(0x0003);                    // appropriate delay
		delay_100ns(1);
	  read_low();                         // read = 0;
		return rd_data;                     // return the read data
}

void eep_read(unsigned char addr_start, unsigned short num, unsigned char *p_data)//Continuous reading
{			                  //start address			   //data number         //save data
    unsigned short i;

	  wr_data(0x0000);                      // write bus clear
	  read_low();                           // read = 0
	  sync_low();                           // sync = 0
	  write_low();                          // write =0
	  //delay(0x0001);                      // appropriate delay
	  read_high();;                         // read = 1
	  for (i = 0; i < num; i++) {
		    wr_addr(addr_start + (unsigned char)i); //write address
		    sync_high();
		    delay_us(4);
		    sync_low();
		    //delay(0x0003);
		    *(p_data + i) = dout();
		   //delay(0x0003);
	  }
	  read_low();                           // read finishde
}

void cmd_write(const unsigned short *cmd_tab, unsigned char start_addr, unsigned char length)
{                          // array name            //start adress           // number of words
	unsigned char i;

	for (i=0; i < length; i++) {
		    wr_word(((start_addr | i) << 1), *(cmd_tab + i));		//write word addres *2
	}
}

unsigned short data_shift(unsigned short temp)	   //data exchange
{
    unsigned short temp1;

	  temp1 = ~temp;
	  return 	 temp1;
}

unsigned short write_read_2kbit( const unsigned short *cmd_tab)   // compare write data and  read data until the data not equal, break
{
    unsigned short k, m;
	  unsigned short temp1, temp2, temp3;

	  cmd_write(cmd_tab, 0x00, 0x80);			              //write data  0x5555
		for (k = 0x00, m = 0x00; k < 0x100; k += 0x02) {
		    temp1 = (unsigned short)rd_byte(k);
				temp2 = (unsigned short)rd_byte(k + 1);
				temp3 = (temp2 << 8) + temp1;
				if (temp3 == cmd_tab[k >> 1]) {
				    m = m + 0x01;
				}
				else
				{
					m = m;
					//break;
				}
			}
			return m;
  }

unsigned short read_2kbit( const unsigned short *cmd_tab)	//read and compare
{
    unsigned short k, m=0;
	  unsigned short temp1, temp2, temp3;

	  for (k = 0x00, m = 0x00; k < 0x100; k = k + 0x02)
			    {
				      temp1 = (unsigned short)rd_byte(k);
				      temp2 = (unsigned short)rd_byte(k + 1);
				      temp3 = temp1 + (temp2 << 8);

		          if (temp3 == cmd_tab[k >> 1])
				         {
					           m = m + 0x01;
				         }
				     else
				         {
					           m = m;
					          // break;
				         }
			   }
		return m;
  }
void single_write(unsigned char addr, unsigned short data,  unsigned short *cmd_tab)//write word and write to cmd_tab
{
    wr_word(addr,data);
	  cmd_tab[addr >> 1] = data;
}
void debug(void)  //test current
{
	  read_low();
	  sync_low();
	  wr_addr(0x00);
	  wr_data(0x0000);
	  write_low();
	  write_high();
	//delay(0xd000);
	  write_low();
}
void gpio_initial()
{
	 read_low();
	 sync_low();
	 write_low();
}

