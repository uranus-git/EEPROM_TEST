
`timescale 1ns/1ps
//`include "./memory/memory_define.v"

module eeprom4k  (
									sys_clk,	//50M时钟，该时钟并不是eeprom的管脚时�
									addr,
									sys_rst_n,
                           data_in,
                           dout, 
									read,
                           pgm,	//	
									erase,
									sync,		
									bufrst,				
									loaden,
                           clk, //eerpomоƬ clk_in
                           clk_out //eerpomоƬ clk_out									
									);

input							sys_clk;
input                   clk;
input                   sys_rst_n;
input    [13:4]         addr;
input		[15:0]		   data_in;// bit13--bit12,  //bit13 = 1; 上半区写入相同的数据�bit14 = 下半区写入相同数据， addr[13:12] = 11; 全片写入相同的数�
output	[15:0] 		   dout;
input							read;
input                   pgm;
input                   erase;
input							sync;
input                   bufrst;
input							loaden;
output                  clk_out;
reg 							clk_out;
reg			[15:0] 		dout_reg;	   //输出数据寄存�
reg			[ 15:0]		buf_reg[3:0]; //buffer寄存�, 
                                      //addr[5:4] = 2'b00, data_in 存入buf_reg[0];
                                      //addr[5:4] = 3'b01, data_in 存入buf_reg[1]
												  //addr[5:4] = 3'b10, data_in 存入buf_reg[2]
												  //addr[5:4] = 3'b11, data_in 存入buf_reg[3]

reg			[ 3:0]		buf_en_flag_reg; // buffer 标志
reg			[15:0]		mem32x16[0:256]; //ram 数据寄存�
wire							nbufrst_wire;

reg			[255:0]		word_decode_wire;
wire		   [255:0] 	   nerase_decode_wire;    
wire		   [255:0]		pgm_decode_wire;
wire		   [15:0]		mem32x16_dout_wire;

assign						nbufrst_wire = ~bufrst;

wire clk_o;


  clock instance_name
   (// Clock in ports
    .CLK_IN1(sys_clk),      // IN
    // Clock out ports
    .CLK_OUT1(clk_o));    // OUT



always @(posedge clk_o or negedge sys_rst_n)
	begin
		if (!sys_rst_n)
		  clk_out <= 1'b0;
		else 
		  clk_out <= clk;
	end

//always	@ (posedge clk or negedge loaden)	begin  //系统时钟1.92M，写信号为高电平时，cnt开始计�
//	if(!loaden)
//		cnt <= 0;
//	else
//		cnt <= cnt + 1;
//end
//该版本手动产�

//always	@ (posedge clk or negedge loaden)	begin  //产生一个buffer_rst信号，高电平有效
//	if(!loaden)
//		bufrst <= 0;
//	else
//		bufrst <= cnt == 0 ? 1'b1 : 0;
//end


//现在版本是手动产生，等会修改
// always	@ (posedge clk or negedge loaden)	begin
	// if(!loaden)
		// erase <= 0;
	// else 
		// erase <= cnt == 101 || cnt == 4101 ? ~erase : erase;
// end

//现在版本是手动产生，之后修改为现在的pgm信号
// always	@ (posedge clk or negedge loaden)	begin // pgm 是实际的写信�
	// if(!loaden)
		// pgm <= 0;
	// else
		// pgm <= cnt == 4121 || cnt == 8121 ? ~pgm : pgm;
// end

//该信号现在的版本不要
// always	@ (negedge pgm or negedge loaden)	begin
	// if(!loaden)
		// loadend <= 0;
	// else
		// loadend <= 1;
// end

// Timing check
/*
specify
  $setup(addr, posedge sync, 15�
  $setup(data_in, posedge sync, 15� 
  
  $width(posedge sync, 300�
  $width(negedge sync, 70�
  $hold(negedge sync, addr, 15�
  $hold(negedge sync, data_in, 15�
  //$hold(negedge sync, negedge loaden, 15�

  $setup(posedge read, posedge sync, 15�
  $hold(negedge sync, negedge read, 15� 
endspecify
*/
/*
integer i;

initial	begin
	#3;
	if(initialized == 1)	begin
		case(key_all_zero)
			2'b00:		begin
				$readmemh("./memory/eeprom_all_zero.dat", mem32x16); 
			end
			2'b01:		begin
				$readmemh("./memory/eeprom_key_zero.dat", mem32x16);
			end
			default:	begin
				$readmemh("./memory/eeprom.dat", mem32x16);
			end
		endcase
	end
	
	//else	begin
		for (i = 0; i < 128; i = i + 1)
			mem32x16[i] = 16'hffff;
	//end
end
*/

always @(posedge sync or negedge nbufrst_wire) begin  //复位buffer,每次写入是复位buffer
  if(~nbufrst_wire) begin
    buf_en_flag_reg	<= 4'b0000;
    buf_reg[0] <= 1'b0;  
    buf_reg[1] <= 1'b0;
	buf_reg[2] <= 1'b0;  
    buf_reg[3] <= 1'b0;
  end
  else	
		begin  
        
			{buf_en_flag_reg[0],buf_reg[0]}	<= ((addr[5:4] == 2'b00) & loaden) ? 
																				{1'b1,data_in} : {buf_en_flag_reg[0],buf_reg[0]};  //addr[5:4] == 2'b00，赋值{1'b1,data_in}  �保持不变           
			{buf_en_flag_reg[1],buf_reg[1]}	<= ((addr[5:4] == 2'b01) & loaden) ? 
																				{1'b1,data_in} : {buf_en_flag_reg[1],buf_reg[1]};  //addr[5:4] == 2'b01，赋值{1'b1,data_in}  �保持不变 
			{buf_en_flag_reg[2],buf_reg[2]}	<= ((addr[5:4] == 2'b10) & loaden) ? 
																				{1'b1,data_in} : {buf_en_flag_reg[2],buf_reg[2]};  //addr[5:4] == 2'b10，赋值{1'b1,data_in}  �保持不变	
			{buf_en_flag_reg[3],buf_reg[3]}	<= ((addr[5:4] == 2'b11) & loaden) ? 
																				{1'b1,data_in} : {buf_en_flag_reg[3],buf_reg[3]};  //addr[5:4] == 2'b11，赋值{1'b1,data_in}  �保持不变																		 
  end 
end

always	@ (negedge sync or negedge nbufrst_wire)	begin   //sync的下降沿开始译�word_decode_wire 地址译码寄存�位addr, 实际的地址只有
	if(!nbufrst_wire)	
		word_decode_wire <= 32'b0;
	else if(loaden) begin    
	
	   //bottom 2kbit block
	
		word_decode_wire[0] <= buf_en_flag_reg[0] && ((addr[11:4] == 8'h00 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[0];
		word_decode_wire[1] <= buf_en_flag_reg[1] && ((addr[11:4] == 8'h01 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[1];
		word_decode_wire[2] <= buf_en_flag_reg[2] && ((addr[11:4] == 8'h02 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[2];
		word_decode_wire[3] <= buf_en_flag_reg[3] && ((addr[11:4] == 8'h03 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[3];
		
		word_decode_wire[4] <= buf_en_flag_reg[0] && ((addr[11:4] == 8'h04 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[4];
		word_decode_wire[5] <= buf_en_flag_reg[1] && ((addr[11:4] == 8'h05 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[5];
		word_decode_wire[6] <= buf_en_flag_reg[2] && ((addr[11:4] == 8'h06 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[6];
		word_decode_wire[7] <= buf_en_flag_reg[3] && ((addr[11:4] == 8'h07 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[7];
		
		word_decode_wire[8] <= buf_en_flag_reg[0] && ((addr[11:4] == 8'h08 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[8];	
		word_decode_wire[9] <= buf_en_flag_reg[1] && ((addr[11:4] == 8'h09 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[9];
		word_decode_wire[10] <= buf_en_flag_reg[2] && ((addr[11:4] == 8'h0a && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[10];
		word_decode_wire[11] <= buf_en_flag_reg[3] && ((addr[11:4] == 8'h0b && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[11];
		
		word_decode_wire[12] <= buf_en_flag_reg[0] && ((addr[11:4] == 8'h0c && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[12];
		word_decode_wire[13] <= buf_en_flag_reg[1] && ((addr[11:4] == 8'h0d && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[13];
		word_decode_wire[14] <= buf_en_flag_reg[2] && ((addr[11:4] == 8'h0e && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[14];
		word_decode_wire[15] <= buf_en_flag_reg[3] && ((addr[11:4] == 8'h0f && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[15];
		
		word_decode_wire[16] <= buf_en_flag_reg[0] && ((addr[11:4] == 8'h10 && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[16];
		word_decode_wire[17] <= buf_en_flag_reg[1] && ((addr[11:4] == 8'h11 && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[17];
		word_decode_wire[18] <= buf_en_flag_reg[2] && ((addr[11:4] == 8'h12 && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[18];
		word_decode_wire[19] <= buf_en_flag_reg[3] && ((addr[11:4] == 8'h13 && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[19];
		
		word_decode_wire[20] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h14 && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[20];
		word_decode_wire[21] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h15 && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[21];
		word_decode_wire[22] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h16 && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[22];
		word_decode_wire[23] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h17 && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[23];
		
		word_decode_wire[24] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h18 && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[24];
		word_decode_wire[25] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h19 && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[25];
		word_decode_wire[26] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h1a && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[26];
		word_decode_wire[27] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h1b && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[27];
		
		word_decode_wire[28] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h1c && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[28];	
		word_decode_wire[29] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h1d && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[29];
		word_decode_wire[30] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h1e && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[30];
		word_decode_wire[31] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h1f && addr[13:12] == 2'b0)  || addr[12]) ? 1'b1 : word_decode_wire[31];	
		
		word_decode_wire[32] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h20 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[32];
		word_decode_wire[33] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h21 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[33];
		word_decode_wire[34] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h22 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[34];
		word_decode_wire[35] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h23 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[35];
		
		word_decode_wire[36] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h24 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[36];	
		word_decode_wire[37] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h25 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[37];
		word_decode_wire[38] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h26 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[38];
		word_decode_wire[39] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h27 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[39];
		
		word_decode_wire[40] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h28 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[40];
		word_decode_wire[41] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h29 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[41];
		word_decode_wire[42] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h2a && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[42];
		word_decode_wire[43] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h2b && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[43];
		
		word_decode_wire[44] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h2c && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[44];
		word_decode_wire[45] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h2d && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[45];
		word_decode_wire[46] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h2e && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[46];
		word_decode_wire[47] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h2f && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[47];
		
		word_decode_wire[48] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h30 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[48];
		word_decode_wire[49] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h31 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[49];
		word_decode_wire[50] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h32 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[50];
		word_decode_wire[51] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h33 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[51];
		
		word_decode_wire[52] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h34 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[52];
		word_decode_wire[53] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h35 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[53];
		word_decode_wire[54] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h36 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[54];
		word_decode_wire[55] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h37 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[55];
		
		word_decode_wire[56] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h38 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[56];
		word_decode_wire[57] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h39 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[57];
		word_decode_wire[58] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h3a && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[58];
		word_decode_wire[59] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h3b && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[59];
		
		word_decode_wire[60] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h3c && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[60];		
		word_decode_wire[61] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h3d && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[61];
		word_decode_wire[62] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h3e && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[62];
		word_decode_wire[63] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h3f && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[63];
		
		word_decode_wire[64] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h40 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[64];
		word_decode_wire[65] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h41 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[65];
		word_decode_wire[66] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h42 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[66];
		word_decode_wire[67] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h43 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[67];
		
		word_decode_wire[68] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h44 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[68];
		word_decode_wire[69] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h45 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[69];
		word_decode_wire[70] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h46 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[70];
		word_decode_wire[71] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h47 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[71];
		
		word_decode_wire[72] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h48 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[72];
		word_decode_wire[73] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h49 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[73];
		word_decode_wire[74] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h4a && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[74];
		word_decode_wire[75] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h4b && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[75];
		
		word_decode_wire[76] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h4c && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[76];
		word_decode_wire[77] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h4d && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[77];
		word_decode_wire[78] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h4e && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[78];
		word_decode_wire[79] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h4f && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[79];
		
		word_decode_wire[80] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h50 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[80];
		word_decode_wire[81] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h51 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[81];
		word_decode_wire[82] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h52 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[82];
		word_decode_wire[83] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h53 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[83];
		
		word_decode_wire[84] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h54 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[84];
		word_decode_wire[85] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h55 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[85];
		word_decode_wire[86] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h56 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[86];
		word_decode_wire[87] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h57 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[87];
		
		word_decode_wire[88] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h58 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[88];
		word_decode_wire[89] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h59 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[89];
		word_decode_wire[90] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h5a && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[90];
		word_decode_wire[91] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h5b && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[91];
		
		word_decode_wire[92] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h5c && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[92];
		word_decode_wire[93] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h5d && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[93];
		word_decode_wire[94] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h5e && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[94];
		word_decode_wire[95] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h5f && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[95];
		
		word_decode_wire[96	] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h60 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[96 ];
		word_decode_wire[97	] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h61 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[97 ];
		word_decode_wire[98	] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h62 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[98 ];
		word_decode_wire[99	] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h63 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[99 ];
		
		word_decode_wire[100] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h64 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[100];
		word_decode_wire[101] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h65 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[101];
		word_decode_wire[102] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h66 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[102];
		word_decode_wire[103] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h67 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[103];
		
		word_decode_wire[104] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h68 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[104];
		word_decode_wire[105] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h69 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[105];
		word_decode_wire[106] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h6a && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[106];
		word_decode_wire[107] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h6b && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[107];
		
		word_decode_wire[108] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h6c && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[108];
		word_decode_wire[109] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h6d && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[109];
		word_decode_wire[110] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h6e && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[110];
		word_decode_wire[111] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h6f && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[111];
		
		word_decode_wire[112] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h70 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[112];
		word_decode_wire[113] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h71 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[113];
		word_decode_wire[114] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h72 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[114];
		word_decode_wire[115] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h73 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[115];
		
		word_decode_wire[116] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h74 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[116];
		word_decode_wire[117] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h75 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[117];
		word_decode_wire[118] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h76 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[118];
		word_decode_wire[119] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h77 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[119];
		
		word_decode_wire[120] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h78 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[120];
		word_decode_wire[121] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h79 && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[121];
		word_decode_wire[122] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h7a && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[122];
		word_decode_wire[123] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h7b && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[123];
		
		word_decode_wire[124] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h7c && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[124];
		word_decode_wire[125] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h7d && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[125];
		word_decode_wire[126] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h7e && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[126];
		word_decode_wire[127] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h7f && addr[13:12] == 2'b0) || addr[12]) ? 1'b1 : word_decode_wire[127];
		
		//upper 2kbit block
		
		word_decode_wire[128] <= buf_en_flag_reg[0] && ((addr[11:4] == 8'h80 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[128];
		word_decode_wire[129] <= buf_en_flag_reg[1] && ((addr[11:4] == 8'h81 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[129];
		word_decode_wire[130] <= buf_en_flag_reg[2] && ((addr[11:4] == 8'h82 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[130];
		word_decode_wire[131] <= buf_en_flag_reg[3] && ((addr[11:4] == 8'h83 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[131];
		
		word_decode_wire[132] <= buf_en_flag_reg[0] && ((addr[11:4] == 8'h84 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[132];
		word_decode_wire[133] <= buf_en_flag_reg[1] && ((addr[11:4] == 8'h85 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[133];
		word_decode_wire[134] <= buf_en_flag_reg[2] && ((addr[11:4] == 8'h86 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[134];
		word_decode_wire[135] <= buf_en_flag_reg[3] && ((addr[11:4] == 8'h87 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[135];
		
		word_decode_wire[136] <= buf_en_flag_reg[0] && ((addr[11:4] == 8'h88 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[136];	
		word_decode_wire[137] <= buf_en_flag_reg[1] && ((addr[11:4] == 8'h89 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[137];
		word_decode_wire[138] <= buf_en_flag_reg[2] && ((addr[11:4] == 8'h8a && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[138];
		word_decode_wire[139] <= buf_en_flag_reg[3] && ((addr[11:4] == 8'h8b && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[139];
		
		word_decode_wire[140] <= buf_en_flag_reg[0] && ((addr[11:4] == 8'h8c && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[140];
		word_decode_wire[141] <= buf_en_flag_reg[1] && ((addr[11:4] == 8'h8d && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[141];
		word_decode_wire[142] <= buf_en_flag_reg[2] && ((addr[11:4] == 8'h8e && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[142];
		word_decode_wire[143] <= buf_en_flag_reg[3] && ((addr[11:4] == 8'h8f && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[143];
		
		word_decode_wire[144] <= buf_en_flag_reg[0] && ((addr[11:4] == 8'h90 && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[144];
		word_decode_wire[145] <= buf_en_flag_reg[1] && ((addr[11:4] == 8'h91 && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[145];
		word_decode_wire[146] <= buf_en_flag_reg[2] && ((addr[11:4] == 8'h92 && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[146];
		word_decode_wire[147] <= buf_en_flag_reg[3] && ((addr[11:4] == 8'h93 && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[147];
		
		word_decode_wire[148] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h94 && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[148];
		word_decode_wire[149] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h95 && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[149];
		word_decode_wire[150] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h96 && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[150];
		word_decode_wire[151] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h97 && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[151];
		
		word_decode_wire[152] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h98 && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[152];
		word_decode_wire[153] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h99 && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[153];
		word_decode_wire[154] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h9a && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[154];
		word_decode_wire[155] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h9b && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[155];
		
		word_decode_wire[156] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'h9c && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[156];	
		word_decode_wire[157] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'h9d && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[157];
		word_decode_wire[158] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'h9e && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[158];
		word_decode_wire[159] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'h9f && addr[13:12] == 2'b0)  || addr[13]) ? 1'b1 : word_decode_wire[159];	
		
		word_decode_wire[160] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'ha0 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[160];
		word_decode_wire[161] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'ha1 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[161];
		word_decode_wire[162] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'ha2 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[162];
		word_decode_wire[163] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'ha3 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[163];
		
		word_decode_wire[164] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'ha4 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[164];	
		word_decode_wire[165] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'ha5 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[165];
		word_decode_wire[166] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'ha6 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[166];
		word_decode_wire[167] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'ha7 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[167];
		
		word_decode_wire[168] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'ha8 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[168];
		word_decode_wire[169] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'ha9 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[169];
		word_decode_wire[170] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'haa && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[170];
		word_decode_wire[171] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hab && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[171];
		
		word_decode_wire[172] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hac && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[172];
		word_decode_wire[173] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'had && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[173];
		word_decode_wire[174] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hae && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[174];
		word_decode_wire[175] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'haf && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[175];
		
		word_decode_wire[176] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hb0 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[176];
		word_decode_wire[177] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hb1 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[177];
		word_decode_wire[178] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hb2 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[178];
		word_decode_wire[179] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hb3 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[179];
		
		word_decode_wire[180] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hb4 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[180];
		word_decode_wire[181] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hb5 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[181];
		word_decode_wire[182] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hb6 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[182];
		word_decode_wire[183] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hb7 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[183];
		
		word_decode_wire[184] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hb8 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[184];
		word_decode_wire[185] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hb9 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[185];
		word_decode_wire[186] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hba && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[186];
		word_decode_wire[187] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hbb && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[187];
		
		word_decode_wire[188] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hbc && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[188];		
		word_decode_wire[189] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hbd && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[189];
		word_decode_wire[190] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hbe && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[190];
		word_decode_wire[191] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hbf && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[191];
		
		word_decode_wire[192] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hc0 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[192];
		word_decode_wire[193] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hc1 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[193];
		word_decode_wire[194] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hc2 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[194];
		word_decode_wire[195] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hc3 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[195];
		
		word_decode_wire[196] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hc4 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[196];
		word_decode_wire[197] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hc5 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[197];
		word_decode_wire[198] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hc6 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[198];
		word_decode_wire[199] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hc7 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[199];
		
		word_decode_wire[200] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hc8 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[200];
		word_decode_wire[201] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hc9 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[201];
		word_decode_wire[202] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hca && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[202];
		word_decode_wire[203] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hcb && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[203];
		
		word_decode_wire[204] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hcc && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[204];
		word_decode_wire[205] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hcd && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[205];
		word_decode_wire[206] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hce && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[206];
		word_decode_wire[207] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hcf && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[207];
		
		word_decode_wire[208] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hd0 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[208];
		word_decode_wire[209] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hd1 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[209];
		word_decode_wire[210] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hd2 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[210];
		word_decode_wire[211] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hd3 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[211];
		
		word_decode_wire[212] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hd4 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[212];
		word_decode_wire[213] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hd5 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[213];
		word_decode_wire[214] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hd6 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[214];
		word_decode_wire[215] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hd7 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[215];
		
		word_decode_wire[216] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hd8 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[216];
		word_decode_wire[217] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hd9 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[217];
		word_decode_wire[218] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hda && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[218];
		word_decode_wire[219] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hdb && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[219];
		
		word_decode_wire[220] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hdc && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[220];
		word_decode_wire[221] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hdd && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[221];
		word_decode_wire[222] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hde && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[222];
		word_decode_wire[223] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hdf && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[223];
		
		word_decode_wire[224] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'he0 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[224];
		word_decode_wire[225] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'he1 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[225];
		word_decode_wire[226] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'he2 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[226];
		word_decode_wire[227] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'he3 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[227];
		
		word_decode_wire[228] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'he4 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[228];
		word_decode_wire[229] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'he5 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[229];
		word_decode_wire[230] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'he6 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[230];
		word_decode_wire[231] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'he7 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[231];
		
		word_decode_wire[232] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'he8 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[232];
		word_decode_wire[233] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'he9 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[233];
		word_decode_wire[234] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hea && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[234];
		word_decode_wire[235] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'heb && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[235];
		
		word_decode_wire[236] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hec && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[236];
		word_decode_wire[237] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hed && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[237];
		word_decode_wire[238] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hee && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[238];
		word_decode_wire[239] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hef && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[239];
		
		word_decode_wire[240] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hf0 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[240];
		word_decode_wire[241] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hf1 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[241];
		word_decode_wire[242] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hf2 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[242];
		word_decode_wire[243] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hf3 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[243];
		
		word_decode_wire[244] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hf4 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[244];
		word_decode_wire[245] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hf5 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[245];
		word_decode_wire[246] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hf6 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[246];
		word_decode_wire[247] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hf7 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[247];
		
		word_decode_wire[248] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hf8 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[248];
		word_decode_wire[249] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hf9 && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[249];
		word_decode_wire[250] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hfa && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[250];
		word_decode_wire[251] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hfb && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[251];
		
		word_decode_wire[252] <= buf_en_flag_reg[0] && ((addr[11:4] ==8'hfc && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[252];
		word_decode_wire[253] <= buf_en_flag_reg[1] && ((addr[11:4] ==8'hfd && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[253];
		word_decode_wire[254] <= buf_en_flag_reg[2] && ((addr[11:4] ==8'hfe && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[254];
		word_decode_wire[255] <= buf_en_flag_reg[3] && ((addr[11:4] ==8'hff && addr[13:12] == 2'b0) || addr[13]) ? 1'b1 : word_decode_wire[255];		
		
	end
end
//每次写数据之前要把将要写入这个地址的数据清�
 //每个地址里面有数，且erase有效再清�
assign	nerase_decode_wire[0 ]  = ~(word_decode_wire[0 ] & erase); 
assign	nerase_decode_wire[1 ]  = ~(word_decode_wire[1 ] & erase);
assign	nerase_decode_wire[2 ]  = ~(word_decode_wire[2 ] & erase);  
assign	nerase_decode_wire[3 ]  = ~(word_decode_wire[3 ] & erase);
assign	nerase_decode_wire[4 ]  = ~(word_decode_wire[4 ] & erase);  
assign	nerase_decode_wire[5 ]  = ~(word_decode_wire[5 ] & erase);
assign	nerase_decode_wire[6 ]  = ~(word_decode_wire[6 ] & erase);  
assign	nerase_decode_wire[7 ]  = ~(word_decode_wire[7 ] & erase);
assign	nerase_decode_wire[8 ]  = ~(word_decode_wire[8 ] & erase);  
assign	nerase_decode_wire[9 ]  = ~(word_decode_wire[9 ] & erase);
assign	nerase_decode_wire[10]  = ~(word_decode_wire[10] & erase);  
assign	nerase_decode_wire[11]  = ~(word_decode_wire[11] & erase);
assign	nerase_decode_wire[12]  = ~(word_decode_wire[12] & erase);  
assign	nerase_decode_wire[13]  = ~(word_decode_wire[13] & erase);
assign	nerase_decode_wire[14]  = ~(word_decode_wire[14] & erase);  
assign	nerase_decode_wire[15]  = ~(word_decode_wire[15] & erase);

assign	nerase_decode_wire[16]  = ~(word_decode_wire[16] & erase);  
assign	nerase_decode_wire[17]  = ~(word_decode_wire[17] & erase);
assign	nerase_decode_wire[18]  = ~(word_decode_wire[18] & erase);  
assign	nerase_decode_wire[19]  = ~(word_decode_wire[19] & erase);
assign	nerase_decode_wire[20]  = ~(word_decode_wire[20] & erase);  
assign	nerase_decode_wire[21]  = ~(word_decode_wire[21] & erase);
assign	nerase_decode_wire[22]  = ~(word_decode_wire[22] & erase);  
assign	nerase_decode_wire[23]  = ~(word_decode_wire[23] & erase);
assign	nerase_decode_wire[24]  = ~(word_decode_wire[24] & erase);  
assign	nerase_decode_wire[25]  = ~(word_decode_wire[25] & erase);
assign	nerase_decode_wire[26]  = ~(word_decode_wire[26] & erase);  
assign	nerase_decode_wire[27]  = ~(word_decode_wire[27] & erase);
assign	nerase_decode_wire[28]  = ~(word_decode_wire[28] & erase);  
assign	nerase_decode_wire[29]  = ~(word_decode_wire[29] & erase);
assign	nerase_decode_wire[30]  = ~(word_decode_wire[30] & erase);  
assign	nerase_decode_wire[31]  = ~(word_decode_wire[31] & erase);

assign	nerase_decode_wire[32]  = ~(word_decode_wire[32] & erase);  
assign	nerase_decode_wire[33]  = ~(word_decode_wire[33] & erase);
assign	nerase_decode_wire[34]  = ~(word_decode_wire[34] & erase);  
assign	nerase_decode_wire[35]  = ~(word_decode_wire[35] & erase);
assign	nerase_decode_wire[36]  = ~(word_decode_wire[36] & erase);  
assign	nerase_decode_wire[37]  = ~(word_decode_wire[37] & erase);
assign	nerase_decode_wire[38]  = ~(word_decode_wire[38] & erase);  
assign	nerase_decode_wire[39]  = ~(word_decode_wire[39] & erase);
assign	nerase_decode_wire[40]  = ~(word_decode_wire[40] & erase);  
assign	nerase_decode_wire[41]  = ~(word_decode_wire[41] & erase);
assign	nerase_decode_wire[42]  = ~(word_decode_wire[42] & erase);  
assign	nerase_decode_wire[43]  = ~(word_decode_wire[43] & erase);
assign	nerase_decode_wire[44]  = ~(word_decode_wire[44] & erase);  
assign	nerase_decode_wire[45]  = ~(word_decode_wire[45] & erase);
assign	nerase_decode_wire[46]  = ~(word_decode_wire[46] & erase);  
assign	nerase_decode_wire[47]  = ~(word_decode_wire[47] & erase);

assign	nerase_decode_wire[48]  = ~(word_decode_wire[48] & erase);  
assign	nerase_decode_wire[49]  = ~(word_decode_wire[49] & erase);
assign	nerase_decode_wire[50]  = ~(word_decode_wire[50] & erase);  
assign	nerase_decode_wire[51]  = ~(word_decode_wire[51] & erase);
assign	nerase_decode_wire[52]  = ~(word_decode_wire[52] & erase);  
assign	nerase_decode_wire[53]  = ~(word_decode_wire[53] & erase);
assign	nerase_decode_wire[54]  = ~(word_decode_wire[54] & erase);  
assign	nerase_decode_wire[55]  = ~(word_decode_wire[55] & erase);
assign	nerase_decode_wire[56]  = ~(word_decode_wire[56] & erase);  
assign	nerase_decode_wire[57]  = ~(word_decode_wire[57] & erase);
assign	nerase_decode_wire[58]  = ~(word_decode_wire[58] & erase);  
assign	nerase_decode_wire[59]  = ~(word_decode_wire[59] & erase);
assign	nerase_decode_wire[60]  = ~(word_decode_wire[60] & erase);  
assign	nerase_decode_wire[61]  = ~(word_decode_wire[61] & erase);
assign	nerase_decode_wire[62]  = ~(word_decode_wire[62] & erase);  
assign	nerase_decode_wire[63]  = ~(word_decode_wire[63] & erase);

assign	nerase_decode_wire[64]  = ~(word_decode_wire[64] & erase);  
assign	nerase_decode_wire[65]  = ~(word_decode_wire[65] & erase);
assign	nerase_decode_wire[66]  = ~(word_decode_wire[66] & erase);  
assign	nerase_decode_wire[67]  = ~(word_decode_wire[67] & erase);
assign	nerase_decode_wire[68]  = ~(word_decode_wire[68] & erase);  
assign	nerase_decode_wire[69]  = ~(word_decode_wire[69] & erase);
assign	nerase_decode_wire[70]  = ~(word_decode_wire[70] & erase);  
assign	nerase_decode_wire[71]  = ~(word_decode_wire[71] & erase);
assign	nerase_decode_wire[72]  = ~(word_decode_wire[72] & erase);  
assign	nerase_decode_wire[73]  = ~(word_decode_wire[73] & erase);
assign	nerase_decode_wire[74]  = ~(word_decode_wire[74] & erase);  
assign	nerase_decode_wire[75]  = ~(word_decode_wire[75] & erase);
assign	nerase_decode_wire[76]  = ~(word_decode_wire[76] & erase);  
assign	nerase_decode_wire[77]  = ~(word_decode_wire[77] & erase);
assign	nerase_decode_wire[78]  = ~(word_decode_wire[78] & erase);  
assign	nerase_decode_wire[79]  = ~(word_decode_wire[79] & erase);

assign	nerase_decode_wire[80]  = ~(word_decode_wire[80] & erase);  
assign	nerase_decode_wire[81]  = ~(word_decode_wire[81] & erase);
assign	nerase_decode_wire[82]  = ~(word_decode_wire[82] & erase);  
assign	nerase_decode_wire[83]  = ~(word_decode_wire[83] & erase);
assign	nerase_decode_wire[84]  = ~(word_decode_wire[84] & erase);  
assign	nerase_decode_wire[85]  = ~(word_decode_wire[85] & erase);
assign	nerase_decode_wire[86]  = ~(word_decode_wire[86] & erase);  
assign	nerase_decode_wire[87]  = ~(word_decode_wire[87] & erase);
assign	nerase_decode_wire[88]  = ~(word_decode_wire[88] & erase);  
assign	nerase_decode_wire[89]  = ~(word_decode_wire[89] & erase);
assign	nerase_decode_wire[90]  = ~(word_decode_wire[90] & erase);  
assign	nerase_decode_wire[91]  = ~(word_decode_wire[91] & erase);
assign	nerase_decode_wire[92]  = ~(word_decode_wire[92] & erase);  
assign	nerase_decode_wire[93]  = ~(word_decode_wire[93] & erase);
assign	nerase_decode_wire[94]  = ~(word_decode_wire[94] & erase);  
assign	nerase_decode_wire[95]  = ~(word_decode_wire[95] & erase);

assign	nerase_decode_wire[96 ]  = ~(word_decode_wire[96 ] & erase);  
assign	nerase_decode_wire[97 ]  = ~(word_decode_wire[97 ] & erase);
assign	nerase_decode_wire[98 ]  = ~(word_decode_wire[98 ] & erase);  
assign	nerase_decode_wire[99 ]  = ~(word_decode_wire[99 ] & erase);
assign	nerase_decode_wire[100]  = ~(word_decode_wire[100] & erase);  
assign	nerase_decode_wire[101]  = ~(word_decode_wire[101] & erase);
assign	nerase_decode_wire[102]  = ~(word_decode_wire[102] & erase);  
assign	nerase_decode_wire[103]  = ~(word_decode_wire[103] & erase);
assign	nerase_decode_wire[104]  = ~(word_decode_wire[104] & erase);  
assign	nerase_decode_wire[105]  = ~(word_decode_wire[105] & erase);
assign	nerase_decode_wire[106]  = ~(word_decode_wire[106] & erase);  
assign	nerase_decode_wire[107]  = ~(word_decode_wire[107] & erase);
assign	nerase_decode_wire[108]  = ~(word_decode_wire[108] & erase);  
assign	nerase_decode_wire[109]  = ~(word_decode_wire[109] & erase);
assign	nerase_decode_wire[110]  = ~(word_decode_wire[110] & erase);  
assign	nerase_decode_wire[111]  = ~(word_decode_wire[111] & erase);

assign	nerase_decode_wire[112]  = ~(word_decode_wire[112] & erase);  
assign	nerase_decode_wire[113]  = ~(word_decode_wire[113] & erase);
assign	nerase_decode_wire[114]  = ~(word_decode_wire[114] & erase);  
assign	nerase_decode_wire[115]  = ~(word_decode_wire[115] & erase);
assign	nerase_decode_wire[116]  = ~(word_decode_wire[116] & erase);  
assign	nerase_decode_wire[117]  = ~(word_decode_wire[117] & erase);
assign	nerase_decode_wire[118]  = ~(word_decode_wire[118] & erase);  
assign	nerase_decode_wire[119]  = ~(word_decode_wire[119] & erase);
assign	nerase_decode_wire[120]  = ~(word_decode_wire[120] & erase);  
assign	nerase_decode_wire[121]  = ~(word_decode_wire[121] & erase);
assign	nerase_decode_wire[122]  = ~(word_decode_wire[122] & erase);  
assign	nerase_decode_wire[123]  = ~(word_decode_wire[123] & erase);
assign	nerase_decode_wire[124]  = ~(word_decode_wire[124] & erase);  
assign	nerase_decode_wire[125]  = ~(word_decode_wire[125] & erase);
assign	nerase_decode_wire[126]  = ~(word_decode_wire[126] & erase);  
assign	nerase_decode_wire[127]  = ~(word_decode_wire[127] & erase);

assign	nerase_decode_wire[128]  = ~(word_decode_wire[128] & erase); 
assign	nerase_decode_wire[129]  = ~(word_decode_wire[129] & erase);
assign	nerase_decode_wire[130]  = ~(word_decode_wire[130] & erase);  
assign	nerase_decode_wire[131]  = ~(word_decode_wire[131] & erase);
assign	nerase_decode_wire[132]  = ~(word_decode_wire[132] & erase);  
assign	nerase_decode_wire[133]  = ~(word_decode_wire[133] & erase);
assign	nerase_decode_wire[134]  = ~(word_decode_wire[134] & erase);  
assign	nerase_decode_wire[135]  = ~(word_decode_wire[135] & erase);
assign	nerase_decode_wire[136]  = ~(word_decode_wire[136] & erase);  
assign	nerase_decode_wire[137]  = ~(word_decode_wire[137] & erase);
assign	nerase_decode_wire[138]  = ~(word_decode_wire[138] & erase);  
assign	nerase_decode_wire[139]  = ~(word_decode_wire[139] & erase);
assign	nerase_decode_wire[140]  = ~(word_decode_wire[140] & erase);  
assign	nerase_decode_wire[141]  = ~(word_decode_wire[141] & erase);
assign	nerase_decode_wire[142]  = ~(word_decode_wire[142] & erase);  
assign	nerase_decode_wire[143]  = ~(word_decode_wire[143] & erase);

assign	nerase_decode_wire[144]  = ~(word_decode_wire[144] & erase);  
assign	nerase_decode_wire[145]  = ~(word_decode_wire[145] & erase);
assign	nerase_decode_wire[146]  = ~(word_decode_wire[146] & erase);  
assign	nerase_decode_wire[147]  = ~(word_decode_wire[147] & erase);
assign	nerase_decode_wire[148]  = ~(word_decode_wire[148] & erase);  
assign	nerase_decode_wire[149]  = ~(word_decode_wire[149] & erase);
assign	nerase_decode_wire[150]  = ~(word_decode_wire[150] & erase);  
assign	nerase_decode_wire[151]  = ~(word_decode_wire[151] & erase);
assign	nerase_decode_wire[152]  = ~(word_decode_wire[152] & erase);  
assign	nerase_decode_wire[153]  = ~(word_decode_wire[153] & erase);
assign	nerase_decode_wire[154]  = ~(word_decode_wire[154] & erase);  
assign	nerase_decode_wire[155]  = ~(word_decode_wire[155] & erase);
assign	nerase_decode_wire[156]  = ~(word_decode_wire[156] & erase);  
assign	nerase_decode_wire[157]  = ~(word_decode_wire[157] & erase);
assign	nerase_decode_wire[158]  = ~(word_decode_wire[158] & erase);  
assign	nerase_decode_wire[159]  = ~(word_decode_wire[159] & erase);

assign	nerase_decode_wire[160]  = ~(word_decode_wire[160] & erase);  
assign	nerase_decode_wire[161]  = ~(word_decode_wire[161] & erase);
assign	nerase_decode_wire[162]  = ~(word_decode_wire[162] & erase);  
assign	nerase_decode_wire[163]  = ~(word_decode_wire[163] & erase);
assign	nerase_decode_wire[164]  = ~(word_decode_wire[164] & erase);  
assign	nerase_decode_wire[165]  = ~(word_decode_wire[165] & erase);
assign	nerase_decode_wire[166]  = ~(word_decode_wire[166] & erase);  
assign	nerase_decode_wire[167]  = ~(word_decode_wire[167] & erase);
assign	nerase_decode_wire[168]  = ~(word_decode_wire[168] & erase);  
assign	nerase_decode_wire[169]  = ~(word_decode_wire[169] & erase);
assign	nerase_decode_wire[170]  = ~(word_decode_wire[170] & erase);  
assign	nerase_decode_wire[171]  = ~(word_decode_wire[171] & erase);
assign	nerase_decode_wire[172]  = ~(word_decode_wire[172] & erase);  
assign	nerase_decode_wire[173]  = ~(word_decode_wire[173] & erase);
assign	nerase_decode_wire[174]  = ~(word_decode_wire[174] & erase);  
assign	nerase_decode_wire[175]  = ~(word_decode_wire[175] & erase);

assign	nerase_decode_wire[176]  = ~(word_decode_wire[176] & erase);  
assign	nerase_decode_wire[177]  = ~(word_decode_wire[177] & erase);
assign	nerase_decode_wire[178]  = ~(word_decode_wire[178] & erase);  
assign	nerase_decode_wire[179]  = ~(word_decode_wire[179] & erase);
assign	nerase_decode_wire[180]  = ~(word_decode_wire[180] & erase);  
assign	nerase_decode_wire[181]  = ~(word_decode_wire[181] & erase);
assign	nerase_decode_wire[182]  = ~(word_decode_wire[182] & erase);  
assign	nerase_decode_wire[183]  = ~(word_decode_wire[183] & erase);
assign	nerase_decode_wire[184]  = ~(word_decode_wire[184] & erase);  
assign	nerase_decode_wire[185]  = ~(word_decode_wire[185] & erase);
assign	nerase_decode_wire[186]  = ~(word_decode_wire[186] & erase);  
assign	nerase_decode_wire[187]  = ~(word_decode_wire[187] & erase);
assign	nerase_decode_wire[188]  = ~(word_decode_wire[188] & erase);  
assign	nerase_decode_wire[189]  = ~(word_decode_wire[189] & erase);
assign	nerase_decode_wire[190]  = ~(word_decode_wire[190] & erase);  
assign	nerase_decode_wire[191]  = ~(word_decode_wire[191] & erase);

assign	nerase_decode_wire[192]  = ~(word_decode_wire[192] & erase);  
assign	nerase_decode_wire[193]  = ~(word_decode_wire[193] & erase);
assign	nerase_decode_wire[194]  = ~(word_decode_wire[194] & erase);  
assign	nerase_decode_wire[195]  = ~(word_decode_wire[195] & erase);
assign	nerase_decode_wire[196]  = ~(word_decode_wire[196] & erase);  
assign	nerase_decode_wire[197]  = ~(word_decode_wire[197] & erase);
assign	nerase_decode_wire[198]  = ~(word_decode_wire[198] & erase);  
assign	nerase_decode_wire[199]  = ~(word_decode_wire[199] & erase);
assign	nerase_decode_wire[200]  = ~(word_decode_wire[200] & erase);  
assign	nerase_decode_wire[201]  = ~(word_decode_wire[201] & erase);
assign	nerase_decode_wire[202]  = ~(word_decode_wire[202] & erase);  
assign	nerase_decode_wire[203]  = ~(word_decode_wire[203] & erase);
assign	nerase_decode_wire[204]  = ~(word_decode_wire[204] & erase);  
assign	nerase_decode_wire[205]  = ~(word_decode_wire[205] & erase);
assign	nerase_decode_wire[206]  = ~(word_decode_wire[206] & erase);  
assign	nerase_decode_wire[207]  = ~(word_decode_wire[207] & erase);

assign	nerase_decode_wire[208]  = ~(word_decode_wire[208] & erase);  
assign	nerase_decode_wire[209]  = ~(word_decode_wire[209] & erase);
assign	nerase_decode_wire[210]  = ~(word_decode_wire[210] & erase);  
assign	nerase_decode_wire[211]  = ~(word_decode_wire[211] & erase);
assign	nerase_decode_wire[212]  = ~(word_decode_wire[212] & erase);  
assign	nerase_decode_wire[213]  = ~(word_decode_wire[213] & erase);
assign	nerase_decode_wire[214]  = ~(word_decode_wire[214] & erase);  
assign	nerase_decode_wire[215]  = ~(word_decode_wire[215] & erase);
assign	nerase_decode_wire[216]  = ~(word_decode_wire[216] & erase);  
assign	nerase_decode_wire[217]  = ~(word_decode_wire[217] & erase);
assign	nerase_decode_wire[218]  = ~(word_decode_wire[218] & erase);  
assign	nerase_decode_wire[219]  = ~(word_decode_wire[219] & erase);
assign	nerase_decode_wire[220]  = ~(word_decode_wire[220] & erase);  
assign	nerase_decode_wire[221]  = ~(word_decode_wire[221] & erase);
assign	nerase_decode_wire[222]  = ~(word_decode_wire[222] & erase);  
assign	nerase_decode_wire[223]  = ~(word_decode_wire[223] & erase);

assign	nerase_decode_wire[224]  = ~(word_decode_wire[224] & erase);  
assign	nerase_decode_wire[225]  = ~(word_decode_wire[225] & erase);
assign	nerase_decode_wire[226]  = ~(word_decode_wire[226] & erase);  
assign	nerase_decode_wire[227]  = ~(word_decode_wire[227] & erase);
assign	nerase_decode_wire[228]  = ~(word_decode_wire[228] & erase);  
assign	nerase_decode_wire[229]  = ~(word_decode_wire[229] & erase);
assign	nerase_decode_wire[230]  = ~(word_decode_wire[230] & erase);  
assign	nerase_decode_wire[231]  = ~(word_decode_wire[231] & erase);
assign	nerase_decode_wire[232]  = ~(word_decode_wire[232] & erase);  
assign	nerase_decode_wire[233]  = ~(word_decode_wire[233] & erase);
assign	nerase_decode_wire[234]  = ~(word_decode_wire[234] & erase);  
assign	nerase_decode_wire[235]  = ~(word_decode_wire[235] & erase);
assign	nerase_decode_wire[236]  = ~(word_decode_wire[236] & erase);  
assign	nerase_decode_wire[237]  = ~(word_decode_wire[237] & erase);
assign	nerase_decode_wire[238]  = ~(word_decode_wire[238] & erase);  
assign	nerase_decode_wire[239]  = ~(word_decode_wire[239] & erase);

assign	nerase_decode_wire[240]  = ~(word_decode_wire[240] & erase);  
assign	nerase_decode_wire[241]  = ~(word_decode_wire[241] & erase);
assign	nerase_decode_wire[242]  = ~(word_decode_wire[242] & erase);  
assign	nerase_decode_wire[243]  = ~(word_decode_wire[243] & erase);
assign	nerase_decode_wire[244]  = ~(word_decode_wire[244] & erase);  
assign	nerase_decode_wire[245]  = ~(word_decode_wire[245] & erase);
assign	nerase_decode_wire[246]  = ~(word_decode_wire[246] & erase);  
assign	nerase_decode_wire[247]  = ~(word_decode_wire[247] & erase);
assign	nerase_decode_wire[248]  = ~(word_decode_wire[248] & erase);  
assign	nerase_decode_wire[249]  = ~(word_decode_wire[249] & erase);
assign	nerase_decode_wire[250]  = ~(word_decode_wire[250] & erase);  
assign	nerase_decode_wire[251]  = ~(word_decode_wire[251] & erase);
assign	nerase_decode_wire[252]  = ~(word_decode_wire[252] & erase);  
assign	nerase_decode_wire[253]  = ~(word_decode_wire[253] & erase);
assign	nerase_decode_wire[254]  = ~(word_decode_wire[254] & erase);  
assign	nerase_decode_wire[255]  = ~(word_decode_wire[255] & erase);

assign	pgm_decode_wire[0 ]  = word_decode_wire[0 ];
assign	pgm_decode_wire[1 ]  = word_decode_wire[1 ];
assign	pgm_decode_wire[2 ]  = word_decode_wire[2 ];
assign	pgm_decode_wire[3 ]  = word_decode_wire[3 ];
assign	pgm_decode_wire[4 ]  = word_decode_wire[4 ];
assign	pgm_decode_wire[5 ]  = word_decode_wire[5 ];
assign	pgm_decode_wire[6 ]  = word_decode_wire[6 ];
assign	pgm_decode_wire[7 ]  = word_decode_wire[7 ];
assign	pgm_decode_wire[8 ]  = word_decode_wire[8 ];
assign	pgm_decode_wire[9 ]  = word_decode_wire[9 ];
assign	pgm_decode_wire[10]  = word_decode_wire[10];
assign	pgm_decode_wire[11]  = word_decode_wire[11];
assign	pgm_decode_wire[12]  = word_decode_wire[12];
assign	pgm_decode_wire[13]  = word_decode_wire[13];
assign	pgm_decode_wire[14]  = word_decode_wire[14];
assign	pgm_decode_wire[15]  = word_decode_wire[15];
                                                     
assign	pgm_decode_wire[16]  = word_decode_wire[16];
assign	pgm_decode_wire[17]  = word_decode_wire[17];
assign	pgm_decode_wire[18]  = word_decode_wire[18];
assign	pgm_decode_wire[19]  = word_decode_wire[19];
assign	pgm_decode_wire[20]  = word_decode_wire[20];
assign	pgm_decode_wire[21]  = word_decode_wire[21];
assign	pgm_decode_wire[22]  = word_decode_wire[22];
assign	pgm_decode_wire[23]  = word_decode_wire[23];
assign	pgm_decode_wire[24]  = word_decode_wire[24];
assign	pgm_decode_wire[25]  = word_decode_wire[25];
assign	pgm_decode_wire[26]  = word_decode_wire[26];
assign	pgm_decode_wire[27]  = word_decode_wire[27];
assign	pgm_decode_wire[28]  = word_decode_wire[28];
assign	pgm_decode_wire[29]  = word_decode_wire[29];
assign	pgm_decode_wire[30]  = word_decode_wire[30];
assign	pgm_decode_wire[31]  = word_decode_wire[31];

assign	pgm_decode_wire[32]  = word_decode_wire[32];
assign	pgm_decode_wire[33]  = word_decode_wire[33];
assign	pgm_decode_wire[34]  = word_decode_wire[34];
assign	pgm_decode_wire[35]  = word_decode_wire[35];
assign	pgm_decode_wire[36]  = word_decode_wire[36];
assign	pgm_decode_wire[37]  = word_decode_wire[37];
assign	pgm_decode_wire[38]  = word_decode_wire[38];
assign	pgm_decode_wire[39]  = word_decode_wire[39];
assign	pgm_decode_wire[40]  = word_decode_wire[40];
assign	pgm_decode_wire[41]  = word_decode_wire[41];
assign	pgm_decode_wire[42]  = word_decode_wire[42];
assign	pgm_decode_wire[43]  = word_decode_wire[43];
assign	pgm_decode_wire[44]  = word_decode_wire[44];
assign	pgm_decode_wire[45]  = word_decode_wire[45];
assign	pgm_decode_wire[46]  = word_decode_wire[46];
assign	pgm_decode_wire[47]  = word_decode_wire[47];

assign	pgm_decode_wire[48]  = word_decode_wire[48];
assign	pgm_decode_wire[49]  = word_decode_wire[49];
assign	pgm_decode_wire[50]  = word_decode_wire[50];
assign	pgm_decode_wire[51]  = word_decode_wire[51];
assign	pgm_decode_wire[52]  = word_decode_wire[52];
assign	pgm_decode_wire[53]  = word_decode_wire[53];
assign	pgm_decode_wire[54]  = word_decode_wire[54];
assign	pgm_decode_wire[55]  = word_decode_wire[55];
assign	pgm_decode_wire[56]  = word_decode_wire[56];
assign	pgm_decode_wire[57]  = word_decode_wire[57];
assign	pgm_decode_wire[58]  = word_decode_wire[58];
assign	pgm_decode_wire[59]  = word_decode_wire[59];
assign	pgm_decode_wire[60]  = word_decode_wire[60];
assign	pgm_decode_wire[61]  = word_decode_wire[61];
assign	pgm_decode_wire[62]  = word_decode_wire[62];
assign	pgm_decode_wire[63]  = word_decode_wire[63];

assign	pgm_decode_wire[64]  = word_decode_wire[64];
assign	pgm_decode_wire[65]  = word_decode_wire[65];
assign	pgm_decode_wire[66]  = word_decode_wire[66];
assign	pgm_decode_wire[67]  = word_decode_wire[67];
assign	pgm_decode_wire[68]  = word_decode_wire[68];
assign	pgm_decode_wire[69]  = word_decode_wire[69];
assign	pgm_decode_wire[70]  = word_decode_wire[70];
assign	pgm_decode_wire[71]  = word_decode_wire[71];
assign	pgm_decode_wire[72]  = word_decode_wire[72];
assign	pgm_decode_wire[73]  = word_decode_wire[73];
assign	pgm_decode_wire[74]  = word_decode_wire[74];
assign	pgm_decode_wire[75]  = word_decode_wire[75];
assign	pgm_decode_wire[76]  = word_decode_wire[76];
assign	pgm_decode_wire[77]  = word_decode_wire[77];
assign	pgm_decode_wire[78]  = word_decode_wire[78];
assign	pgm_decode_wire[79]  = word_decode_wire[79];

assign	pgm_decode_wire[80]  = word_decode_wire[80];
assign	pgm_decode_wire[81]  = word_decode_wire[81];
assign	pgm_decode_wire[82]  = word_decode_wire[82];
assign	pgm_decode_wire[83]  = word_decode_wire[83];
assign	pgm_decode_wire[84]  = word_decode_wire[84];
assign	pgm_decode_wire[85]  = word_decode_wire[85];
assign	pgm_decode_wire[86]  = word_decode_wire[86];
assign	pgm_decode_wire[87]  = word_decode_wire[87];
assign	pgm_decode_wire[88]  = word_decode_wire[88];
assign	pgm_decode_wire[89]  = word_decode_wire[89];
assign	pgm_decode_wire[90]  = word_decode_wire[90];
assign	pgm_decode_wire[91]  = word_decode_wire[91];
assign	pgm_decode_wire[92]  = word_decode_wire[92];
assign	pgm_decode_wire[93]  = word_decode_wire[93];
assign	pgm_decode_wire[94]  = word_decode_wire[94];
assign	pgm_decode_wire[95]  = word_decode_wire[95];

assign	pgm_decode_wire[96 ]  = word_decode_wire[96 ];
assign	pgm_decode_wire[97 ]  = word_decode_wire[97 ];
assign	pgm_decode_wire[98 ]  = word_decode_wire[98 ];
assign	pgm_decode_wire[99 ]  = word_decode_wire[99 ];
assign	pgm_decode_wire[100]  = word_decode_wire[100];
assign	pgm_decode_wire[101]  = word_decode_wire[101];
assign	pgm_decode_wire[102]  = word_decode_wire[102];
assign	pgm_decode_wire[103]  = word_decode_wire[103];
assign	pgm_decode_wire[104]  = word_decode_wire[104];
assign	pgm_decode_wire[105]  = word_decode_wire[105];
assign	pgm_decode_wire[106]  = word_decode_wire[106];
assign	pgm_decode_wire[107]  = word_decode_wire[107];
assign	pgm_decode_wire[108]  = word_decode_wire[108];
assign	pgm_decode_wire[109]  = word_decode_wire[109];
assign	pgm_decode_wire[110]  = word_decode_wire[110];
assign	pgm_decode_wire[111]  = word_decode_wire[111];

assign	pgm_decode_wire[112]  = word_decode_wire[112];
assign	pgm_decode_wire[113]  = word_decode_wire[113];
assign	pgm_decode_wire[114]  = word_decode_wire[114];
assign	pgm_decode_wire[115]  = word_decode_wire[115];
assign	pgm_decode_wire[116]  = word_decode_wire[116];
assign	pgm_decode_wire[117]  = word_decode_wire[117];
assign	pgm_decode_wire[118]  = word_decode_wire[118];
assign	pgm_decode_wire[119]  = word_decode_wire[119];
assign	pgm_decode_wire[120]  = word_decode_wire[120];
assign	pgm_decode_wire[121]  = word_decode_wire[121];
assign	pgm_decode_wire[122]  = word_decode_wire[122];
assign	pgm_decode_wire[123]  = word_decode_wire[123];
assign	pgm_decode_wire[124]  = word_decode_wire[124];
assign	pgm_decode_wire[125]  = word_decode_wire[125];
assign	pgm_decode_wire[126]  = word_decode_wire[126];
assign	pgm_decode_wire[127]  = word_decode_wire[127];

assign	pgm_decode_wire[128 ]  = word_decode_wire[128 ];
assign	pgm_decode_wire[129 ]  = word_decode_wire[129];
assign	pgm_decode_wire[130]  = word_decode_wire[130];
assign	pgm_decode_wire[131]  = word_decode_wire[131];
assign	pgm_decode_wire[132]  = word_decode_wire[132];
assign	pgm_decode_wire[133]  = word_decode_wire[133];
assign	pgm_decode_wire[134]  = word_decode_wire[134];
assign	pgm_decode_wire[135]  = word_decode_wire[135];
assign	pgm_decode_wire[136]  = word_decode_wire[136];
assign	pgm_decode_wire[137]  = word_decode_wire[137];
assign	pgm_decode_wire[138]  = word_decode_wire[138];
assign	pgm_decode_wire[139]  = word_decode_wire[139];
assign	pgm_decode_wire[140]  = word_decode_wire[140];
assign	pgm_decode_wire[141]  = word_decode_wire[141];
assign	pgm_decode_wire[142]  = word_decode_wire[142];
assign	pgm_decode_wire[143]  = word_decode_wire[143];
                                                     
assign	pgm_decode_wire[144]  = word_decode_wire[144];
assign	pgm_decode_wire[145]  = word_decode_wire[145];
assign	pgm_decode_wire[146]  = word_decode_wire[146];
assign	pgm_decode_wire[147]  = word_decode_wire[147];
assign	pgm_decode_wire[148]  = word_decode_wire[148];
assign	pgm_decode_wire[149]  = word_decode_wire[149];
assign	pgm_decode_wire[150]  = word_decode_wire[150];
assign	pgm_decode_wire[151]  = word_decode_wire[151];
assign	pgm_decode_wire[152]  = word_decode_wire[152];
assign	pgm_decode_wire[153]  = word_decode_wire[153];
assign	pgm_decode_wire[154]  = word_decode_wire[154];
assign	pgm_decode_wire[155]  = word_decode_wire[155];
assign	pgm_decode_wire[156]  = word_decode_wire[156];
assign	pgm_decode_wire[157]  = word_decode_wire[157];
assign	pgm_decode_wire[158]  = word_decode_wire[158];
assign	pgm_decode_wire[159]  = word_decode_wire[159];

assign	pgm_decode_wire[160]  = word_decode_wire[160];
assign	pgm_decode_wire[161]  = word_decode_wire[161];
assign	pgm_decode_wire[162]  = word_decode_wire[162];
assign	pgm_decode_wire[163]  = word_decode_wire[163];
assign	pgm_decode_wire[164]  = word_decode_wire[164];
assign	pgm_decode_wire[165]  = word_decode_wire[165];
assign	pgm_decode_wire[166]  = word_decode_wire[166];
assign	pgm_decode_wire[167]  = word_decode_wire[167];
assign	pgm_decode_wire[168]  = word_decode_wire[168];
assign	pgm_decode_wire[169]  = word_decode_wire[169];
assign	pgm_decode_wire[170]  = word_decode_wire[170];
assign	pgm_decode_wire[171]  = word_decode_wire[171];
assign	pgm_decode_wire[172]  = word_decode_wire[172];
assign	pgm_decode_wire[173]  = word_decode_wire[173];
assign	pgm_decode_wire[174]  = word_decode_wire[174];
assign	pgm_decode_wire[175]  = word_decode_wire[175];

assign	pgm_decode_wire[176]  = word_decode_wire[176];
assign	pgm_decode_wire[177]  = word_decode_wire[177];
assign	pgm_decode_wire[178]  = word_decode_wire[178];
assign	pgm_decode_wire[179]  = word_decode_wire[179];
assign	pgm_decode_wire[180]  = word_decode_wire[180];
assign	pgm_decode_wire[181]  = word_decode_wire[181];
assign	pgm_decode_wire[182]  = word_decode_wire[182];
assign	pgm_decode_wire[183]  = word_decode_wire[183];
assign	pgm_decode_wire[184]  = word_decode_wire[184];
assign	pgm_decode_wire[185]  = word_decode_wire[185];
assign	pgm_decode_wire[186]  = word_decode_wire[186];
assign	pgm_decode_wire[187]  = word_decode_wire[187];
assign	pgm_decode_wire[188]  = word_decode_wire[188];
assign	pgm_decode_wire[189]  = word_decode_wire[189];
assign	pgm_decode_wire[190]  = word_decode_wire[190];
assign	pgm_decode_wire[191]  = word_decode_wire[191];

assign	pgm_decode_wire[192]  = word_decode_wire[192];
assign	pgm_decode_wire[193]  = word_decode_wire[193];
assign	pgm_decode_wire[194]  = word_decode_wire[194];
assign	pgm_decode_wire[195]  = word_decode_wire[195];
assign	pgm_decode_wire[196]  = word_decode_wire[196];
assign	pgm_decode_wire[197]  = word_decode_wire[197];
assign	pgm_decode_wire[198]  = word_decode_wire[198];
assign	pgm_decode_wire[199]  = word_decode_wire[199];
assign	pgm_decode_wire[200]  = word_decode_wire[200];
assign	pgm_decode_wire[201]  = word_decode_wire[201];
assign	pgm_decode_wire[202]  = word_decode_wire[202];
assign	pgm_decode_wire[203]  = word_decode_wire[203];
assign	pgm_decode_wire[204]  = word_decode_wire[204];
assign	pgm_decode_wire[205]  = word_decode_wire[205];
assign	pgm_decode_wire[206]  = word_decode_wire[206];
assign	pgm_decode_wire[207]  = word_decode_wire[207];

assign	pgm_decode_wire[208]  = word_decode_wire[208];
assign	pgm_decode_wire[209]  = word_decode_wire[209];
assign	pgm_decode_wire[210]  = word_decode_wire[210];
assign	pgm_decode_wire[211]  = word_decode_wire[211];
assign	pgm_decode_wire[212]  = word_decode_wire[212];
assign	pgm_decode_wire[213]  = word_decode_wire[213];
assign	pgm_decode_wire[214]  = word_decode_wire[214];
assign	pgm_decode_wire[215]  = word_decode_wire[215];
assign	pgm_decode_wire[216]  = word_decode_wire[216];
assign	pgm_decode_wire[217]  = word_decode_wire[217];
assign	pgm_decode_wire[218]  = word_decode_wire[218];
assign	pgm_decode_wire[219]  = word_decode_wire[219];
assign	pgm_decode_wire[220]  = word_decode_wire[220];
assign	pgm_decode_wire[221]  = word_decode_wire[221];
assign	pgm_decode_wire[222]  = word_decode_wire[222];
assign	pgm_decode_wire[223]  = word_decode_wire[223];

assign	pgm_decode_wire[224]  = word_decode_wire[224];
assign	pgm_decode_wire[225]  = word_decode_wire[225];
assign	pgm_decode_wire[226]  = word_decode_wire[226];
assign	pgm_decode_wire[227]  = word_decode_wire[227];
assign	pgm_decode_wire[228]  = word_decode_wire[228];
assign	pgm_decode_wire[229]  = word_decode_wire[229];
assign	pgm_decode_wire[230]  = word_decode_wire[230];
assign	pgm_decode_wire[231]  = word_decode_wire[231];
assign	pgm_decode_wire[232]  = word_decode_wire[232];
assign	pgm_decode_wire[233]  = word_decode_wire[233];
assign	pgm_decode_wire[234]  = word_decode_wire[234];
assign	pgm_decode_wire[235]  = word_decode_wire[235];
assign	pgm_decode_wire[236]  = word_decode_wire[236];
assign	pgm_decode_wire[237]  = word_decode_wire[237];
assign	pgm_decode_wire[238]  = word_decode_wire[238];
assign	pgm_decode_wire[239]  = word_decode_wire[239];

assign	pgm_decode_wire[240]  = word_decode_wire[240];
assign	pgm_decode_wire[241]  = word_decode_wire[241];
assign	pgm_decode_wire[242]  = word_decode_wire[242];
assign	pgm_decode_wire[243]  = word_decode_wire[243];
assign	pgm_decode_wire[244]  = word_decode_wire[244];
assign	pgm_decode_wire[245]  = word_decode_wire[245];
assign	pgm_decode_wire[246]  = word_decode_wire[246];
assign	pgm_decode_wire[247]  = word_decode_wire[247];
assign	pgm_decode_wire[248]  = word_decode_wire[248];
assign	pgm_decode_wire[249]  = word_decode_wire[249];
assign	pgm_decode_wire[250]  = word_decode_wire[250];
assign	pgm_decode_wire[251]  = word_decode_wire[251];
assign	pgm_decode_wire[252]  = word_decode_wire[252];
assign	pgm_decode_wire[253]  = word_decode_wire[253];
assign	pgm_decode_wire[254]  = word_decode_wire[254];
assign	pgm_decode_wire[255]  = word_decode_wire[255];


always @(posedge pgm or negedge nerase_decode_wire[0]) begin
  if(~nerase_decode_wire[0])
    mem32x16[0] <= 16'h0000;
  else
    mem32x16[0] <= pgm_decode_wire[0] ? buf_reg[0] : mem32x16[0];
end

always @(posedge pgm or negedge nerase_decode_wire[1]) begin
  if(~nerase_decode_wire[1])
    mem32x16[1] <= 16'h0000;
  else
    mem32x16[1] <= pgm_decode_wire[1] ? buf_reg[1] : mem32x16[1];
end

always @(posedge pgm or negedge nerase_decode_wire[2]) begin
  if(~nerase_decode_wire[2])
    mem32x16[2] <= 16'h0000;
  else
    mem32x16[2] <= pgm_decode_wire[2] ? buf_reg[2] : mem32x16[2];
end

always @(posedge pgm or negedge nerase_decode_wire[3]) begin
  if(~nerase_decode_wire[3])
    mem32x16[3] <= 16'h0000;
  else
    mem32x16[3] <= pgm_decode_wire[3] ? buf_reg[3] : mem32x16[3];
end
//

always @(posedge pgm or negedge nerase_decode_wire[4]) begin
  if(~nerase_decode_wire[4])
    mem32x16[4] <= 16'h0000;
  else
    mem32x16[4] <= pgm_decode_wire[4] ? buf_reg[0] : mem32x16[4];
end

always @(posedge pgm or negedge nerase_decode_wire[5]) begin
  if(~nerase_decode_wire[5])
    mem32x16[5] <= 16'h0000;
  else
    mem32x16[5] <= pgm_decode_wire[5] ? buf_reg[1] : mem32x16[5];
end

always @(posedge pgm or negedge nerase_decode_wire[6]) begin
  if(~nerase_decode_wire[6])
    mem32x16[6] <= 16'h0000;
  else
    mem32x16[6] <= pgm_decode_wire[6] ? buf_reg[2] : mem32x16[6];
end

always @(posedge pgm or negedge nerase_decode_wire[7]) begin
  if(~nerase_decode_wire[7])
    mem32x16[7] <= 16'h0000;
  else
    mem32x16[7] <= pgm_decode_wire[7] ? buf_reg[3] : mem32x16[7];
end


//
always @(posedge pgm or negedge nerase_decode_wire[8]) begin
  if(~nerase_decode_wire[8])
    mem32x16[8] <= 16'h0000;
  else
    mem32x16[8] <= pgm_decode_wire[8] ? buf_reg[0] : mem32x16[8];
end

always @(posedge pgm or negedge nerase_decode_wire[9]) begin
  if(~nerase_decode_wire[9])
    mem32x16[9] <= 16'h0000;
  else
    mem32x16[9] <= pgm_decode_wire[9] ? buf_reg[1] : mem32x16[9];
end

always @(posedge pgm or negedge nerase_decode_wire[10]) begin
  if(~nerase_decode_wire[10])
    mem32x16[10] <= 16'h0000;
  else
    mem32x16[10] <= pgm_decode_wire[10] ? buf_reg[2] : mem32x16[10];
end

always @(posedge pgm or negedge nerase_decode_wire[11]) begin
  if(~nerase_decode_wire[11])
    mem32x16[11] <= 16'h0000;
  else
    mem32x16[11] <= pgm_decode_wire[11] ? buf_reg[3] : mem32x16[11];
end


//
always @(posedge pgm or negedge nerase_decode_wire[12]) begin
  if(~nerase_decode_wire[12])
    mem32x16[12] <= 16'h0000;
  else
    mem32x16[12] <= pgm_decode_wire[12] ? buf_reg[0] : mem32x16[12];
end

always @(posedge pgm or negedge nerase_decode_wire[13]) begin
  if(~nerase_decode_wire[13])
    mem32x16[13] <= 16'h0000;
  else
    mem32x16[13] <= pgm_decode_wire[13] ? buf_reg[1] : mem32x16[13];
end

always @(posedge pgm or negedge nerase_decode_wire[14]) begin
  if(~nerase_decode_wire[14])
    mem32x16[14] <= 16'h0000;
  else
    mem32x16[14] <= pgm_decode_wire[14] ? buf_reg[2] : mem32x16[14];
end

always @(posedge pgm or negedge nerase_decode_wire[15]) begin
  if(~nerase_decode_wire[15])
    mem32x16[15] <= 16'h0000;
  else
    mem32x16[15] <= pgm_decode_wire[15] ? buf_reg[3] : mem32x16[15];
end
//


always @(posedge pgm or negedge nerase_decode_wire[16]) begin
  if(~nerase_decode_wire[16])
    mem32x16[16] <= 16'h0000;
  else
    mem32x16[16] <= pgm_decode_wire[16] ? buf_reg[0] : mem32x16[16];
end

always @(posedge pgm or negedge nerase_decode_wire[17]) begin
  if(~nerase_decode_wire[17])
    mem32x16[17] <= 16'h0000;
  else
    mem32x16[17] <= pgm_decode_wire[17] ? buf_reg[1] : mem32x16[17];
end

always @(posedge pgm or negedge nerase_decode_wire[18]) begin
  if(~nerase_decode_wire[18])
    mem32x16[18] <= 16'h0000;
  else
    mem32x16[18] <= pgm_decode_wire[18] ? buf_reg[2] : mem32x16[18];
end

always @(posedge pgm or negedge nerase_decode_wire[19]) begin
  if(~nerase_decode_wire[19])
    mem32x16[19] <= 16'h0000;
  else
    mem32x16[19] <= pgm_decode_wire[19] ? buf_reg[3] : mem32x16[19];
end

//
always @(posedge pgm or negedge nerase_decode_wire[20]) begin
  if(~nerase_decode_wire[20])
    mem32x16[20] <= 16'h0000;
  else
    mem32x16[20] <= pgm_decode_wire[20] ? buf_reg[0] : mem32x16[20];
end

always @(posedge pgm or negedge nerase_decode_wire[21]) begin
  if(~nerase_decode_wire[21])
    mem32x16[21] <= 16'h0000;
  else
    mem32x16[21] <= pgm_decode_wire[21] ? buf_reg[1] : mem32x16[21];
end

always @(posedge pgm or negedge nerase_decode_wire[22]) begin
  if(~nerase_decode_wire[22])
    mem32x16[22] <= 16'h0000;
  else
    mem32x16[22] <= pgm_decode_wire[22] ? buf_reg[2] : mem32x16[22];
end

always @(posedge pgm or negedge nerase_decode_wire[23]) begin
  if(~nerase_decode_wire[23])
    mem32x16[23] <= 16'h0000;
  else
    mem32x16[23] <= pgm_decode_wire[23] ? buf_reg[3] : mem32x16[23];
end

//
always @(posedge pgm or negedge nerase_decode_wire[24]) begin
  if(~nerase_decode_wire[24])
    mem32x16[24] <= 16'h0000;
  else
    mem32x16[24] <= pgm_decode_wire[24] ? buf_reg[0] : mem32x16[24];
end

always @(posedge pgm or negedge nerase_decode_wire[25]) begin
  if(~nerase_decode_wire[25])
    mem32x16[25] <= 16'h0000;
  else
    mem32x16[25] <= pgm_decode_wire[25] ? buf_reg[1] : mem32x16[25];
end

always @(posedge pgm or negedge nerase_decode_wire[26]) begin
  if(~nerase_decode_wire[26])
    mem32x16[26] <= 16'h0000;
  else
    mem32x16[26] <= pgm_decode_wire[26] ? buf_reg[2] : mem32x16[26];
end

always @(posedge pgm or negedge nerase_decode_wire[27]) begin
  if(~nerase_decode_wire[27])
    mem32x16[27] <= 16'h0000;
  else
    mem32x16[27] <= pgm_decode_wire[27] ? buf_reg[3] : mem32x16[27];
end

//
always @(posedge pgm or negedge nerase_decode_wire[28]) begin
  if(~nerase_decode_wire[28])
    mem32x16[28] <= 16'h0000;
  else
    mem32x16[28] <= pgm_decode_wire[28] ? buf_reg[0] : mem32x16[28];
end

always @(posedge pgm or negedge nerase_decode_wire[29]) begin
  if(~nerase_decode_wire[29])
    mem32x16[29] <= 16'h0000;
  else
    mem32x16[29] <= pgm_decode_wire[29] ? buf_reg[1] : mem32x16[29];
end

always @(posedge pgm or negedge nerase_decode_wire[30]) begin
  if(~nerase_decode_wire[30])
    mem32x16[30] <= 16'h0000;
  else
    mem32x16[30] <= pgm_decode_wire[30] ? buf_reg[2] : mem32x16[30];
end

always @(posedge pgm or negedge nerase_decode_wire[31]) begin
  if(~nerase_decode_wire[31])
    mem32x16[31] <= 16'h0000;
  else
    mem32x16[31] <= pgm_decode_wire[31] ? buf_reg[3] : mem32x16[31];
end

//
always @(posedge pgm or negedge nerase_decode_wire[32]) begin
  if(~nerase_decode_wire[32])
    mem32x16[32] <= 16'h0000;
  else
    mem32x16[32] <= pgm_decode_wire[32] ? buf_reg[0] : mem32x16[32];
end

always @(posedge pgm or negedge nerase_decode_wire[33]) begin
  if(~nerase_decode_wire[33])
    mem32x16[33] <= 16'h0000;
  else
    mem32x16[33] <= pgm_decode_wire[33] ? buf_reg[1] : mem32x16[33];
end

always @(posedge pgm or negedge nerase_decode_wire[34]) begin
  if(~nerase_decode_wire[34])
    mem32x16[34] <= 16'h0000;
  else
    mem32x16[34] <= pgm_decode_wire[34] ? buf_reg[2] : mem32x16[34];
end

always @(posedge pgm or negedge nerase_decode_wire[35]) begin
  if(~nerase_decode_wire[35])
    mem32x16[35] <= 16'h0000;
  else
    mem32x16[35] <= pgm_decode_wire[35] ? buf_reg[3] : mem32x16[35];
end

//
always @(posedge pgm or negedge nerase_decode_wire[36]) begin
  if(~nerase_decode_wire[36])
    mem32x16[36] <= 16'h0000;
  else
    mem32x16[36] <= pgm_decode_wire[36] ? buf_reg[0] : mem32x16[36];
end

always @(posedge pgm or negedge nerase_decode_wire[37]) begin
  if(~nerase_decode_wire[37])
    mem32x16[37] <= 16'h0000;
  else
    mem32x16[37] <= pgm_decode_wire[37] ? buf_reg[1] : mem32x16[37];
end

always @(posedge pgm or negedge nerase_decode_wire[38]) begin
  if(~nerase_decode_wire[38])
    mem32x16[38] <= 16'h0000;
  else
    mem32x16[38] <= pgm_decode_wire[38] ? buf_reg[2] : mem32x16[38];
end

always @(posedge pgm or negedge nerase_decode_wire[39]) begin
  if(~nerase_decode_wire[39])
    mem32x16[39] <= 16'h0000;
  else
    mem32x16[39] <= pgm_decode_wire[39] ? buf_reg[3] : mem32x16[39];
end

//
always @(posedge pgm or negedge nerase_decode_wire[40]) begin
  if(~nerase_decode_wire[40])
    mem32x16[40] <= 16'h0000;
  else
    mem32x16[40] <= pgm_decode_wire[40] ? buf_reg[0] : mem32x16[40];
end

always @(posedge pgm or negedge nerase_decode_wire[41]) begin
  if(~nerase_decode_wire[41])
    mem32x16[41] <= 16'h0000;
  else
    mem32x16[41] <= pgm_decode_wire[41] ? buf_reg[1] : mem32x16[41];
end

always @(posedge pgm or negedge nerase_decode_wire[42]) begin
  if(~nerase_decode_wire[42])
    mem32x16[42] <= 16'h0000;
  else
    mem32x16[42] <= pgm_decode_wire[42] ? buf_reg[2] : mem32x16[42];
end

always @(posedge pgm or negedge nerase_decode_wire[43]) begin
  if(~nerase_decode_wire[43])
    mem32x16[43] <= 16'h0000;
  else
    mem32x16[43] <= pgm_decode_wire[43] ? buf_reg[3] : mem32x16[43];
end


//
always @(posedge pgm or negedge nerase_decode_wire[44]) begin
  if(~nerase_decode_wire[44])
    mem32x16[44] <= 16'h0000;
  else
    mem32x16[44] <= pgm_decode_wire[44] ? buf_reg[0] : mem32x16[44];
end

always @(posedge pgm or negedge nerase_decode_wire[45]) begin
  if(~nerase_decode_wire[45])
    mem32x16[45] <= 16'h0000;
  else
    mem32x16[45] <= pgm_decode_wire[45] ? buf_reg[1] : mem32x16[45];
end

always @(posedge pgm or negedge nerase_decode_wire[46]) begin
  if(~nerase_decode_wire[46])
    mem32x16[46] <= 16'h0000;
  else
    mem32x16[46] <= pgm_decode_wire[46] ? buf_reg[2] : mem32x16[46];
end

always @(posedge pgm or negedge nerase_decode_wire[47]) begin
  if(~nerase_decode_wire[47])
    mem32x16[47] <= 16'h0000;
  else
    mem32x16[47] <= pgm_decode_wire[47] ? buf_reg[3] : mem32x16[47];
end


//
always @(posedge pgm or negedge nerase_decode_wire[48]) begin
  if(~nerase_decode_wire[48])
    mem32x16[48] <= 16'h0000;
  else
    mem32x16[48] <= pgm_decode_wire[48] ? buf_reg[0] : mem32x16[48];
end

always @(posedge pgm or negedge nerase_decode_wire[49]) begin
  if(~nerase_decode_wire[49])
    mem32x16[49] <= 16'h0000;
  else
    mem32x16[49] <= pgm_decode_wire[49] ? buf_reg[1] : mem32x16[49];
end

always @(posedge pgm or negedge nerase_decode_wire[50]) begin
  if(~nerase_decode_wire[50])
    mem32x16[50] <= 16'h0000;
  else
    mem32x16[50] <= pgm_decode_wire[50] ? buf_reg[2] : mem32x16[50];
end

always @(posedge pgm or negedge nerase_decode_wire[51]) begin
  if(~nerase_decode_wire[51])
    mem32x16[51] <= 16'h0000;
  else
    mem32x16[51] <= pgm_decode_wire[51] ? buf_reg[3] : mem32x16[51];
end


//
always @(posedge pgm or negedge nerase_decode_wire[52]) begin
  if(~nerase_decode_wire[52])
    mem32x16[52] <= 16'h0000;
  else
    mem32x16[52] <= pgm_decode_wire[52] ? buf_reg[0] : mem32x16[52];
end

always @(posedge pgm or negedge nerase_decode_wire[53]) begin
  if(~nerase_decode_wire[53])
    mem32x16[53] <= 16'h0000;
  else
    mem32x16[53] <= pgm_decode_wire[53] ? buf_reg[1] : mem32x16[53];
end

always @(posedge pgm or negedge nerase_decode_wire[54]) begin
  if(~nerase_decode_wire[54])
    mem32x16[54] <= 16'h0000;
  else
    mem32x16[54] <= pgm_decode_wire[54] ? buf_reg[2] : mem32x16[54];
end

always @(posedge pgm or negedge nerase_decode_wire[55]) begin
  if(~nerase_decode_wire[55])
    mem32x16[55] <= 16'h0000;
  else
    mem32x16[55] <= pgm_decode_wire[55] ? buf_reg[3] : mem32x16[55];
end


//
always @(posedge pgm or negedge nerase_decode_wire[56]) begin
  if(~nerase_decode_wire[56])
    mem32x16[56] <= 16'h0000;
  else
    mem32x16[56] <= pgm_decode_wire[56] ? buf_reg[0] : mem32x16[56];
end

always @(posedge pgm or negedge nerase_decode_wire[57]) begin
  if(~nerase_decode_wire[57])
    mem32x16[57] <= 16'h0000;
  else
    mem32x16[57] <= pgm_decode_wire[57] ? buf_reg[1] : mem32x16[57];
end

always @(posedge pgm or negedge nerase_decode_wire[58]) begin
  if(~nerase_decode_wire[58])
    mem32x16[58] <= 16'h0000;
  else
    mem32x16[58] <= pgm_decode_wire[58] ? buf_reg[2] : mem32x16[58];
end

always @(posedge pgm or negedge nerase_decode_wire[59]) begin
  if(~nerase_decode_wire[59])
    mem32x16[59] <= 16'h0000;
  else
    mem32x16[59] <= pgm_decode_wire[59] ? buf_reg[3] : mem32x16[59];
end


//
always @(posedge pgm or negedge nerase_decode_wire[60]) begin
  if(~nerase_decode_wire[60])
    mem32x16[60] <= 16'h0000;
  else
    mem32x16[60] <= pgm_decode_wire[60] ? buf_reg[0] : mem32x16[60];
end

always @(posedge pgm or negedge nerase_decode_wire[61]) begin
  if(~nerase_decode_wire[61])
    mem32x16[61] <= 16'h0000;
  else
    mem32x16[61] <= pgm_decode_wire[61] ? buf_reg[1] : mem32x16[61];
end

always @(posedge pgm or negedge nerase_decode_wire[62]) begin
  if(~nerase_decode_wire[62])
    mem32x16[62] <= 16'h0000;
  else
    mem32x16[62] <= pgm_decode_wire[62] ? buf_reg[2] : mem32x16[62];
end

always @(posedge pgm or negedge nerase_decode_wire[63]) begin
  if(~nerase_decode_wire[63])
    mem32x16[63] <= 16'h0000;
  else
    mem32x16[63] <= pgm_decode_wire[63] ? buf_reg[3] : mem32x16[63];
end

//
always @(posedge pgm or negedge nerase_decode_wire[64]) begin
  if(~nerase_decode_wire[64])
    mem32x16[64] <= 16'h0000;
  else
    mem32x16[64] <= pgm_decode_wire[64] ? buf_reg[0] : mem32x16[64];
end

always @(posedge pgm or negedge nerase_decode_wire[65]) begin
  if(~nerase_decode_wire[65])
    mem32x16[65] <= 16'h0000;
  else
    mem32x16[65] <= pgm_decode_wire[65] ? buf_reg[1] : mem32x16[65];
end

always @(posedge pgm or negedge nerase_decode_wire[66]) begin
  if(~nerase_decode_wire[66])
    mem32x16[66] <= 16'h0000;
  else
    mem32x16[66] <= pgm_decode_wire[66] ? buf_reg[2] : mem32x16[66];
end

always @(posedge pgm or negedge nerase_decode_wire[67]) begin
  if(~nerase_decode_wire[67])
    mem32x16[67] <= 16'h0000;
  else
    mem32x16[67] <= pgm_decode_wire[67] ? buf_reg[3] : mem32x16[67];
end


//
always @(posedge pgm or negedge nerase_decode_wire[68]) begin
  if(~nerase_decode_wire[68])
    mem32x16[68] <= 16'h0000;
  else
    mem32x16[68] <= pgm_decode_wire[68] ? buf_reg[0] : mem32x16[68];
end

always @(posedge pgm or negedge nerase_decode_wire[69]) begin
  if(~nerase_decode_wire[69])
    mem32x16[69] <= 16'h0000;
  else
    mem32x16[69] <= pgm_decode_wire[69] ? buf_reg[1] : mem32x16[69];
end

always @(posedge pgm or negedge nerase_decode_wire[70]) begin
  if(~nerase_decode_wire[70])
    mem32x16[70] <= 16'h0000;
  else
    mem32x16[70] <= pgm_decode_wire[70] ? buf_reg[2] : mem32x16[70];
end

always @(posedge pgm or negedge nerase_decode_wire[71]) begin
  if(~nerase_decode_wire[71])
    mem32x16[71] <= 16'h0000;
  else
    mem32x16[71] <= pgm_decode_wire[71] ? buf_reg[3] : mem32x16[71];
end


//
always @(posedge pgm or negedge nerase_decode_wire[72]) begin
  if(~nerase_decode_wire[72])
    mem32x16[72] <= 16'h0000;
  else
    mem32x16[72] <= pgm_decode_wire[72] ? buf_reg[0] : mem32x16[72];
end

always @(posedge pgm or negedge nerase_decode_wire[73]) begin
  if(~nerase_decode_wire[73])
    mem32x16[73] <= 16'h0000;
  else
    mem32x16[73] <= pgm_decode_wire[73] ? buf_reg[1] : mem32x16[73];
end


always @(posedge pgm or negedge nerase_decode_wire[74]) begin
  if(~nerase_decode_wire[74])
    mem32x16[74] <= 16'h0000;
  else
    mem32x16[74] <= pgm_decode_wire[74] ? buf_reg[2] : mem32x16[74];
end

always @(posedge pgm or negedge nerase_decode_wire[75]) begin
  if(~nerase_decode_wire[75])
    mem32x16[75] <= 16'h0000;
  else
    mem32x16[75] <= pgm_decode_wire[75] ? buf_reg[3] : mem32x16[75];
end


//
always @(posedge pgm or negedge nerase_decode_wire[76]) begin
  if(~nerase_decode_wire[76])
    mem32x16[76] <= 16'h0000;
  else
    mem32x16[76] <= pgm_decode_wire[76] ? buf_reg[0] : mem32x16[76];
end

always @(posedge pgm or negedge nerase_decode_wire[77]) begin
  if(~nerase_decode_wire[77])
    mem32x16[77] <= 16'h0000;
  else
    mem32x16[77] <= pgm_decode_wire[77] ? buf_reg[1] : mem32x16[77];
end

always @(posedge pgm or negedge nerase_decode_wire[78]) begin
  if(~nerase_decode_wire[78])
    mem32x16[78] <= 16'h0000;
  else
    mem32x16[78] <= pgm_decode_wire[78] ? buf_reg[2] : mem32x16[78];
end

always @(posedge pgm or negedge nerase_decode_wire[79]) begin
  if(~nerase_decode_wire[79])
    mem32x16[79] <= 16'h0000;
  else
    mem32x16[79] <= pgm_decode_wire[79] ? buf_reg[3] : mem32x16[79];
end

//
always @(posedge pgm or negedge nerase_decode_wire[80]) begin
  if(~nerase_decode_wire[80])
    mem32x16[80] <= 16'h0000;
  else
    mem32x16[80] <= pgm_decode_wire[80] ? buf_reg[0] : mem32x16[80];
end

always @(posedge pgm or negedge nerase_decode_wire[81]) begin
  if(~nerase_decode_wire[81])
    mem32x16[81] <= 16'h0000;
  else
    mem32x16[81] <= pgm_decode_wire[81] ? buf_reg[1] : mem32x16[81];
end

always @(posedge pgm or negedge nerase_decode_wire[82]) begin
  if(~nerase_decode_wire[82])
    mem32x16[82] <= 16'h0000;
  else
    mem32x16[82] <= pgm_decode_wire[82] ? buf_reg[2] : mem32x16[82];
end

always @(posedge pgm or negedge nerase_decode_wire[83]) begin
  if(~nerase_decode_wire[83])
    mem32x16[83] <= 16'h0000;
  else
    mem32x16[83] <= pgm_decode_wire[83] ? buf_reg[3] : mem32x16[83];
end


//
always @(posedge pgm or negedge nerase_decode_wire[84]) begin
  if(~nerase_decode_wire[84])
    mem32x16[84] <= 16'h0000;
  else
    mem32x16[84] <= pgm_decode_wire[84] ? buf_reg[0] : mem32x16[84];
end

always @(posedge pgm or negedge nerase_decode_wire[85]) begin
  if(~nerase_decode_wire[85])
    mem32x16[85] <= 16'h0000;
  else
    mem32x16[85] <= pgm_decode_wire[85] ? buf_reg[1] : mem32x16[85];
end

always @(posedge pgm or negedge nerase_decode_wire[86]) begin
  if(~nerase_decode_wire[86])
    mem32x16[86] <= 16'h0000;
  else
    mem32x16[86] <= pgm_decode_wire[86] ? buf_reg[2] : mem32x16[86];
end

always @(posedge pgm or negedge nerase_decode_wire[87]) begin
  if(~nerase_decode_wire[87])
    mem32x16[87] <= 16'h0000;
  else
    mem32x16[87] <= pgm_decode_wire[87] ? buf_reg[3] : mem32x16[87];
end

//
always @(posedge pgm or negedge nerase_decode_wire[88]) begin
  if(~nerase_decode_wire[88])
    mem32x16[88] <= 16'h0000;
  else
    mem32x16[88] <= pgm_decode_wire[88] ? buf_reg[0] : mem32x16[88];
end

always @(posedge pgm or negedge nerase_decode_wire[89]) begin
  if(~nerase_decode_wire[89])
    mem32x16[89] <= 16'h0000;
  else
    mem32x16[89] <= pgm_decode_wire[89] ? buf_reg[1] : mem32x16[89];
end

always @(posedge pgm or negedge nerase_decode_wire[90]) begin
  if(~nerase_decode_wire[90])
    mem32x16[90] <= 16'h0000;
  else
    mem32x16[90] <= pgm_decode_wire[90] ? buf_reg[2] : mem32x16[90];
end

always @(posedge pgm or negedge nerase_decode_wire[91]) begin
  if(~nerase_decode_wire[91])
    mem32x16[91] <= 16'h0000;
  else
    mem32x16[91] <= pgm_decode_wire[91] ? buf_reg[3] : mem32x16[91];
end

//
always @(posedge pgm or negedge nerase_decode_wire[92]) begin
  if(~nerase_decode_wire[92])
    mem32x16[92] <= 16'h0000;
  else
    mem32x16[92] <= pgm_decode_wire[92] ? buf_reg[0] : mem32x16[92];
end

always @(posedge pgm or negedge nerase_decode_wire[93]) begin
  if(~nerase_decode_wire[93])
    mem32x16[93] <= 16'h0000;
  else
    mem32x16[93] <= pgm_decode_wire[93] ? buf_reg[1] : mem32x16[93];
end

always @(posedge pgm or negedge nerase_decode_wire[94]) begin
  if(~nerase_decode_wire[94])
    mem32x16[94] <= 16'h0000;
  else
    mem32x16[94] <= pgm_decode_wire[94] ? buf_reg[2] : mem32x16[94];
end

always @(posedge pgm or negedge nerase_decode_wire[95]) begin
  if(~nerase_decode_wire[95])
    mem32x16[95] <= 16'h0000;
  else
    mem32x16[95] <= pgm_decode_wire[95] ? buf_reg[3] : mem32x16[95];
end


//
always @(posedge pgm or negedge nerase_decode_wire[96]) begin
  if(~nerase_decode_wire[96])
    mem32x16[96] <= 16'h0000;
  else
    mem32x16[96] <= pgm_decode_wire[96] ? buf_reg[0] : mem32x16[96];
end

always @(posedge pgm or negedge nerase_decode_wire[97]) begin
  if(~nerase_decode_wire[97])
    mem32x16[97] <= 16'h0000;
  else
    mem32x16[97] <= pgm_decode_wire[97] ? buf_reg[1] : mem32x16[97];
end

always @(posedge pgm or negedge nerase_decode_wire[98]) begin
  if(~nerase_decode_wire[98])
    mem32x16[98] <= 16'h0000;
  else
    mem32x16[98] <= pgm_decode_wire[98] ? buf_reg[2] : mem32x16[98];
end

always @(posedge pgm or negedge nerase_decode_wire[99]) begin
  if(~nerase_decode_wire[99])
    mem32x16[99] <= 16'h0000;
  else
    mem32x16[99] <= pgm_decode_wire[99] ? buf_reg[3] : mem32x16[99];
end

//
always @(posedge pgm or negedge nerase_decode_wire[100]) begin
  if(~nerase_decode_wire[100])
    mem32x16[100] <= 16'h0000;
  else
    mem32x16[100] <= pgm_decode_wire[100] ? buf_reg[0] : mem32x16[100];
end

always @(posedge pgm or negedge nerase_decode_wire[101]) begin
  if(~nerase_decode_wire[101])
    mem32x16[101] <= 16'h0000;
  else
    mem32x16[101] <= pgm_decode_wire[101] ? buf_reg[1] : mem32x16[101];
end


always @(posedge pgm or negedge nerase_decode_wire[102]) begin
  if(~nerase_decode_wire[102])
    mem32x16[102] <= 16'h0000;
  else
    mem32x16[102] <= pgm_decode_wire[102] ? buf_reg[2] : mem32x16[102];
end

always @(posedge pgm or negedge nerase_decode_wire[103]) begin
  if(~nerase_decode_wire[103])
    mem32x16[103] <= 16'h0000;
  else
    mem32x16[103] <= pgm_decode_wire[103] ? buf_reg[3] : mem32x16[103];
end

//
always @(posedge pgm or negedge nerase_decode_wire[104]) begin
  if(~nerase_decode_wire[104])
    mem32x16[104] <= 16'h0000;
  else
    mem32x16[104] <= pgm_decode_wire[104] ? buf_reg[0] : mem32x16[104];
end

always @(posedge pgm or negedge nerase_decode_wire[105]) begin
  if(~nerase_decode_wire[105])
    mem32x16[105] <= 16'h0000;
  else
    mem32x16[105] <= pgm_decode_wire[105] ? buf_reg[1] : mem32x16[105];
end

always @(posedge pgm or negedge nerase_decode_wire[106]) begin
  if(~nerase_decode_wire[106])
    mem32x16[106] <= 16'h0000;
  else
    mem32x16[106] <= pgm_decode_wire[106] ? buf_reg[2] : mem32x16[106];
end

always @(posedge pgm or negedge nerase_decode_wire[107]) begin
  if(~nerase_decode_wire[107])
    mem32x16[107] <= 16'h0000;
  else
    mem32x16[107] <= pgm_decode_wire[107] ? buf_reg[3] : mem32x16[107];
end

//
always @(posedge pgm or negedge nerase_decode_wire[108]) begin
  if(~nerase_decode_wire[108])
    mem32x16[108] <= 16'h0000;
  else
    mem32x16[108] <= pgm_decode_wire[108] ? buf_reg[0] : mem32x16[108];
end

always @(posedge pgm or negedge nerase_decode_wire[109]) begin
  if(~nerase_decode_wire[109])
    mem32x16[109] <= 16'h0000;
  else
    mem32x16[109] <= pgm_decode_wire[109] ? buf_reg[1] : mem32x16[109];
end

always @(posedge pgm or negedge nerase_decode_wire[110]) begin
  if(~nerase_decode_wire[110])
    mem32x16[110] <= 16'h0000;
  else
    mem32x16[110] <= pgm_decode_wire[110] ? buf_reg[2] : mem32x16[110];
end

always @(posedge pgm or negedge nerase_decode_wire[111]) begin
  if(~nerase_decode_wire[111])
    mem32x16[111] <= 16'h0000;
  else
    mem32x16[111] <= pgm_decode_wire[111] ? buf_reg[3] : mem32x16[111];
end

//
always @(posedge pgm or negedge nerase_decode_wire[112]) begin
  if(~nerase_decode_wire[112])
    mem32x16[112] <= 16'h0000;
  else
    mem32x16[112] <= pgm_decode_wire[112] ? buf_reg[0] : mem32x16[112];
end

always @(posedge pgm or negedge nerase_decode_wire[113]) begin
  if(~nerase_decode_wire[113])
    mem32x16[113] <= 16'h0000;
  else
    mem32x16[113] <= pgm_decode_wire[113] ? buf_reg[1] : mem32x16[113];
end

always @(posedge pgm or negedge nerase_decode_wire[114]) begin
  if(~nerase_decode_wire[114])
    mem32x16[114] <= 16'h0000;
  else
    mem32x16[114] <= pgm_decode_wire[114] ? buf_reg[2] : mem32x16[114];
end

always @(posedge pgm or negedge nerase_decode_wire[115]) begin
  if(~nerase_decode_wire[115])
    mem32x16[115] <= 16'h0000;
  else
    mem32x16[115] <= pgm_decode_wire[115] ? buf_reg[3] : mem32x16[115];
end

//
always @(posedge pgm or negedge nerase_decode_wire[116]) begin
  if(~nerase_decode_wire[116])
    mem32x16[116] <= 16'h0000;
  else
    mem32x16[116] <= pgm_decode_wire[116] ? buf_reg[0] : mem32x16[116];
end

always @(posedge pgm or negedge nerase_decode_wire[117]) begin
  if(~nerase_decode_wire[117])
    mem32x16[117] <= 16'h0000;
  else
    mem32x16[117] <= pgm_decode_wire[117] ? buf_reg[1] : mem32x16[117];
end

always @(posedge pgm or negedge nerase_decode_wire[118]) begin
  if(~nerase_decode_wire[118])
    mem32x16[118] <= 16'h0000;
  else
    mem32x16[118] <= pgm_decode_wire[118] ? buf_reg[2] : mem32x16[118];
end

always @(posedge pgm or negedge nerase_decode_wire[119]) begin
  if(~nerase_decode_wire[119])
    mem32x16[119] <= 16'h0000;
  else
    mem32x16[119] <= pgm_decode_wire[119] ? buf_reg[3] : mem32x16[119];
end

//
always @(posedge pgm or negedge nerase_decode_wire[120]) begin
  if(~nerase_decode_wire[120])
    mem32x16[120] <= 16'h0000;
  else
    mem32x16[120] <= pgm_decode_wire[120] ? buf_reg[0] : mem32x16[120];
end

always @(posedge pgm or negedge nerase_decode_wire[121]) begin
  if(~nerase_decode_wire[121])
    mem32x16[121] <= 16'h0000;
  else
    mem32x16[121] <= pgm_decode_wire[121] ? buf_reg[1] : mem32x16[121];
end

always @(posedge pgm or negedge nerase_decode_wire[122]) begin
  if(~nerase_decode_wire[122])
    mem32x16[122] <= 16'h0000;
  else
    mem32x16[122] <= pgm_decode_wire[122] ? buf_reg[2] : mem32x16[122];
end

always @(posedge pgm or negedge nerase_decode_wire[123]) begin
  if(~nerase_decode_wire[123])
    mem32x16[123] <= 16'h0000;
  else
    mem32x16[123] <= pgm_decode_wire[123] ? buf_reg[3] : mem32x16[123];
end

//
always @(posedge pgm or negedge nerase_decode_wire[124]) begin
  if(~nerase_decode_wire[124])
    mem32x16[124] <= 16'h0000;
  else
    mem32x16[124] <= pgm_decode_wire[124] ? buf_reg[0] : mem32x16[124];
end

always @(posedge pgm or negedge nerase_decode_wire[125]) begin
  if(~nerase_decode_wire[125])
    mem32x16[125] <= 16'h0000;
  else
    mem32x16[125] <= pgm_decode_wire[125] ? buf_reg[1] : mem32x16[125];
end

always @(posedge pgm or negedge nerase_decode_wire[126]) begin
  if(~nerase_decode_wire[126])
    mem32x16[126] <= 16'h0000;
  else
    mem32x16[126] <= pgm_decode_wire[126] ? buf_reg[2] : mem32x16[126];
end

always @(posedge pgm or negedge nerase_decode_wire[127]) begin
  if(~nerase_decode_wire[127])
    mem32x16[127] <= 16'h0000;
  else
    mem32x16[127] <= pgm_decode_wire[127] ? buf_reg[3] : mem32x16[127];
end

//
always @(posedge pgm or negedge nerase_decode_wire[128]) begin
  if(~nerase_decode_wire[128])
    mem32x16[128] <= 16'h0000;
  else
    mem32x16[128] <= pgm_decode_wire[128] ? buf_reg[0] : mem32x16[128];
end

always @(posedge pgm or negedge nerase_decode_wire[129]) begin
  if(~nerase_decode_wire[129])
    mem32x16[129] <= 16'h0000;
  else
    mem32x16[129] <= pgm_decode_wire[129] ? buf_reg[1] : mem32x16[129];
end

always @(posedge pgm or negedge nerase_decode_wire[130]) begin
  if(~nerase_decode_wire[130])
    mem32x16[130] <= 16'h0000;
  else
    mem32x16[130] <= pgm_decode_wire[130] ? buf_reg[2] : mem32x16[130];
end

always @(posedge pgm or negedge nerase_decode_wire[131]) begin
  if(~nerase_decode_wire[131])
    mem32x16[131] <= 16'h0000;
  else
    mem32x16[131] <= pgm_decode_wire[131] ? buf_reg[3] : mem32x16[131];
end

//
always @(posedge pgm or negedge nerase_decode_wire[132]) begin
  if(~nerase_decode_wire[132])
    mem32x16[132] <= 16'h0000;
  else
    mem32x16[132] <= pgm_decode_wire[132] ? buf_reg[0] : mem32x16[132];
end

always @(posedge pgm or negedge nerase_decode_wire[133]) begin
  if(~nerase_decode_wire[133])
    mem32x16[133] <= 16'h0000;
  else
    mem32x16[133] <= pgm_decode_wire[133] ? buf_reg[1] : mem32x16[133];
end

always @(posedge pgm or negedge nerase_decode_wire[134]) begin
  if(~nerase_decode_wire[134])
    mem32x16[134] <= 16'h0000;
  else
    mem32x16[134] <= pgm_decode_wire[134] ? buf_reg[2] : mem32x16[134];
end

always @(posedge pgm or negedge nerase_decode_wire[135]) begin
  if(~nerase_decode_wire[135])
    mem32x16[135] <= 16'h0000;
  else
    mem32x16[135] <= pgm_decode_wire[135] ? buf_reg[3] : mem32x16[135];
end

//
always @(posedge pgm or negedge nerase_decode_wire[136]) begin
  if(~nerase_decode_wire[136])
    mem32x16[136] <= 16'h0000;
  else
    mem32x16[136] <= pgm_decode_wire[136] ? buf_reg[0] : mem32x16[136];
end

always @(posedge pgm or negedge nerase_decode_wire[137]) begin
  if(~nerase_decode_wire[137])
    mem32x16[137] <= 16'h0000;
  else
    mem32x16[137] <= pgm_decode_wire[137] ? buf_reg[1] : mem32x16[137];
end

always @(posedge pgm or negedge nerase_decode_wire[138]) begin
  if(~nerase_decode_wire[138])
    mem32x16[138] <= 16'h0000;
  else
    mem32x16[138] <= pgm_decode_wire[138] ? buf_reg[2] : mem32x16[138];
end

always @(posedge pgm or negedge nerase_decode_wire[139]) begin
  if(~nerase_decode_wire[139])
    mem32x16[139] <= 16'h0000;
  else
    mem32x16[139] <= pgm_decode_wire[139] ? buf_reg[3] : mem32x16[139];
end

//
always @(posedge pgm or negedge nerase_decode_wire[140]) begin
  if(~nerase_decode_wire[140])
    mem32x16[140] <= 16'h0000;
  else
    mem32x16[140] <= pgm_decode_wire[140] ? buf_reg[0] : mem32x16[140];
end

always @(posedge pgm or negedge nerase_decode_wire[141]) begin
  if(~nerase_decode_wire[141])
    mem32x16[141] <= 16'h0000;
  else
    mem32x16[141] <= pgm_decode_wire[141] ? buf_reg[1] : mem32x16[141];
end

always @(posedge pgm or negedge nerase_decode_wire[142]) begin
  if(~nerase_decode_wire[142])
    mem32x16[142] <= 16'h0000;
  else
    mem32x16[142] <= pgm_decode_wire[142] ? buf_reg[2] : mem32x16[142];
end

always @(posedge pgm or negedge nerase_decode_wire[143]) begin
  if(~nerase_decode_wire[143])
    mem32x16[143] <= 16'h0000;
  else
    mem32x16[143] <= pgm_decode_wire[143] ? buf_reg[3] : mem32x16[143];
end
//


always @(posedge pgm or negedge nerase_decode_wire[144]) begin
  if(~nerase_decode_wire[144])
    mem32x16[144] <= 16'h0000;
  else
    mem32x16[144] <= pgm_decode_wire[144] ? buf_reg[0] : mem32x16[144];
end

always @(posedge pgm or negedge nerase_decode_wire[145]) begin
  if(~nerase_decode_wire[145])
    mem32x16[145] <= 16'h0000;
  else
    mem32x16[145] <= pgm_decode_wire[145] ? buf_reg[1] : mem32x16[145];
end

always @(posedge pgm or negedge nerase_decode_wire[146]) begin
  if(~nerase_decode_wire[146])
    mem32x16[146] <= 16'h0000;
  else
    mem32x16[146] <= pgm_decode_wire[146] ? buf_reg[2] : mem32x16[146];
end

always @(posedge pgm or negedge nerase_decode_wire[147]) begin
  if(~nerase_decode_wire[147])
    mem32x16[147] <= 16'h0000;
  else
    mem32x16[147] <= pgm_decode_wire[147] ? buf_reg[3] : mem32x16[147];
end

//
always @(posedge pgm or negedge nerase_decode_wire[148]) begin
  if(~nerase_decode_wire[148])
    mem32x16[148] <= 16'h0000;
  else
    mem32x16[148] <= pgm_decode_wire[148] ? buf_reg[0] : mem32x16[148];
end

always @(posedge pgm or negedge nerase_decode_wire[149]) begin
  if(~nerase_decode_wire[149])
    mem32x16[149] <= 16'h0000;
  else
    mem32x16[149] <= pgm_decode_wire[149] ? buf_reg[1] : mem32x16[149];
end

always @(posedge pgm or negedge nerase_decode_wire[150]) begin
  if(~nerase_decode_wire[150])
    mem32x16[150] <= 16'h0000;
  else
    mem32x16[150] <= pgm_decode_wire[150] ? buf_reg[2] : mem32x16[150];
end

always @(posedge pgm or negedge nerase_decode_wire[151]) begin
  if(~nerase_decode_wire[151])
    mem32x16[151] <= 16'h0000;
  else
    mem32x16[151] <= pgm_decode_wire[151] ? buf_reg[3] : mem32x16[151];
end

//
always @(posedge pgm or negedge nerase_decode_wire[152]) begin
  if(~nerase_decode_wire[152])
    mem32x16[152] <= 16'h0000;
  else
    mem32x16[152] <= pgm_decode_wire[152] ? buf_reg[0] : mem32x16[152];
end

always @(posedge pgm or negedge nerase_decode_wire[153]) begin
  if(~nerase_decode_wire[153])
    mem32x16[153] <= 16'h0000;
  else
    mem32x16[153] <= pgm_decode_wire[153] ? buf_reg[1] : mem32x16[153];
end

always @(posedge pgm or negedge nerase_decode_wire[154]) begin
  if(~nerase_decode_wire[154])
    mem32x16[154] <= 16'h0000;
  else
    mem32x16[154] <= pgm_decode_wire[154] ? buf_reg[2] : mem32x16[154];
end

always @(posedge pgm or negedge nerase_decode_wire[155]) begin
  if(~nerase_decode_wire[155])
    mem32x16[155] <= 16'h0000;
  else
    mem32x16[155] <= pgm_decode_wire[155] ? buf_reg[3] : mem32x16[155];
end

//
always @(posedge pgm or negedge nerase_decode_wire[156]) begin
  if(~nerase_decode_wire[156])
    mem32x16[156] <= 16'h0000;
  else
    mem32x16[156] <= pgm_decode_wire[156] ? buf_reg[0] : mem32x16[156];
end

always @(posedge pgm or negedge nerase_decode_wire[157]) begin
  if(~nerase_decode_wire[157])
    mem32x16[157] <= 16'h0000;
  else
    mem32x16[157] <= pgm_decode_wire[157] ? buf_reg[1] : mem32x16[157];
end

always @(posedge pgm or negedge nerase_decode_wire[158]) begin
  if(~nerase_decode_wire[158])
    mem32x16[158] <= 16'h0000;
  else
    mem32x16[158] <= pgm_decode_wire[158] ? buf_reg[2] : mem32x16[158];
end

always @(posedge pgm or negedge nerase_decode_wire[159]) begin
  if(~nerase_decode_wire[159])
    mem32x16[159] <= 16'h0000;
  else
    mem32x16[159] <= pgm_decode_wire[159] ? buf_reg[3] : mem32x16[159];
end

//
always @(posedge pgm or negedge nerase_decode_wire[160]) begin
  if(~nerase_decode_wire[160])
    mem32x16[160] <= 16'h0000;
  else
    mem32x16[160] <= pgm_decode_wire[160] ? buf_reg[0] : mem32x16[160];
end

always @(posedge pgm or negedge nerase_decode_wire[161]) begin
  if(~nerase_decode_wire[161])
    mem32x16[161] <= 16'h0000;
  else
    mem32x16[161] <= pgm_decode_wire[161] ? buf_reg[1] : mem32x16[161];
end

always @(posedge pgm or negedge nerase_decode_wire[162]) begin
  if(~nerase_decode_wire[162])
    mem32x16[162] <= 16'h0000;
  else
    mem32x16[162] <= pgm_decode_wire[162] ? buf_reg[2] : mem32x16[162];
end

always @(posedge pgm or negedge nerase_decode_wire[163]) begin
  if(~nerase_decode_wire[163])
    mem32x16[163] <= 16'h0000;
  else
    mem32x16[163] <= pgm_decode_wire[163] ? buf_reg[3] : mem32x16[163];
end

//
always @(posedge pgm or negedge nerase_decode_wire[164]) begin
  if(~nerase_decode_wire[164])
    mem32x16[164] <= 16'h0000;
  else
    mem32x16[164] <= pgm_decode_wire[164] ? buf_reg[0] : mem32x16[164];
end

always @(posedge pgm or negedge nerase_decode_wire[165]) begin
  if(~nerase_decode_wire[165])
    mem32x16[165] <= 16'h0000;
  else
    mem32x16[165] <= pgm_decode_wire[165] ? buf_reg[1] : mem32x16[165];
end

always @(posedge pgm or negedge nerase_decode_wire[166]) begin
  if(~nerase_decode_wire[166])
    mem32x16[166] <= 16'h0000;
  else
    mem32x16[166] <= pgm_decode_wire[166] ? buf_reg[2] : mem32x16[166];
end

always @(posedge pgm or negedge nerase_decode_wire[167]) begin
  if(~nerase_decode_wire[167])
    mem32x16[167] <= 16'h0000;
  else
    mem32x16[167] <= pgm_decode_wire[167] ? buf_reg[3] : mem32x16[167];
end

//
always @(posedge pgm or negedge nerase_decode_wire[168]) begin
  if(~nerase_decode_wire[168])
    mem32x16[168] <= 16'h0000;
  else
    mem32x16[168] <= pgm_decode_wire[168] ? buf_reg[0] : mem32x16[168];
end

always @(posedge pgm or negedge nerase_decode_wire[169]) begin
  if(~nerase_decode_wire[169])
    mem32x16[169] <= 16'h0000;
  else
    mem32x16[169] <= pgm_decode_wire[169] ? buf_reg[1] : mem32x16[169];
end

always @(posedge pgm or negedge nerase_decode_wire[170]) begin
  if(~nerase_decode_wire[170])
    mem32x16[170] <= 16'h0000;
  else
    mem32x16[170] <= pgm_decode_wire[170] ? buf_reg[2] : mem32x16[170];
end

always @(posedge pgm or negedge nerase_decode_wire[171]) begin
  if(~nerase_decode_wire[171])
    mem32x16[171] <= 16'h0000;
  else
    mem32x16[171] <= pgm_decode_wire[171] ? buf_reg[3] : mem32x16[171];
end


//
always @(posedge pgm or negedge nerase_decode_wire[172]) begin
  if(~nerase_decode_wire[172])
    mem32x16[172] <= 16'h0000;
  else
    mem32x16[172] <= pgm_decode_wire[172] ? buf_reg[0] : mem32x16[172];
end

always @(posedge pgm or negedge nerase_decode_wire[173]) begin
  if(~nerase_decode_wire[173])
    mem32x16[173] <= 16'h0000;
  else
    mem32x16[173] <= pgm_decode_wire[173] ? buf_reg[1] : mem32x16[173];
end

always @(posedge pgm or negedge nerase_decode_wire[174]) begin
  if(~nerase_decode_wire[174])
    mem32x16[174] <= 16'h0000;
  else
    mem32x16[174] <= pgm_decode_wire[174] ? buf_reg[2] : mem32x16[174];
end

always @(posedge pgm or negedge nerase_decode_wire[175]) begin
  if(~nerase_decode_wire[175])
    mem32x16[175] <= 16'h0000;
  else
    mem32x16[175] <= pgm_decode_wire[175] ? buf_reg[3] : mem32x16[175];
end


//
always @(posedge pgm or negedge nerase_decode_wire[176]) begin
  if(~nerase_decode_wire[176])
    mem32x16[176] <= 16'h0000;
  else
    mem32x16[176] <= pgm_decode_wire[176] ? buf_reg[0] : mem32x16[176];
end

always @(posedge pgm or negedge nerase_decode_wire[177]) begin
  if(~nerase_decode_wire[177])
    mem32x16[177] <= 16'h0000;
  else
    mem32x16[177] <= pgm_decode_wire[177] ? buf_reg[1] : mem32x16[177];
end

always @(posedge pgm or negedge nerase_decode_wire[178]) begin
  if(~nerase_decode_wire[178])
    mem32x16[178] <= 16'h0000;
  else
    mem32x16[178] <= pgm_decode_wire[178] ? buf_reg[2] : mem32x16[178];
end

always @(posedge pgm or negedge nerase_decode_wire[179]) begin
  if(~nerase_decode_wire[179])
    mem32x16[179] <= 16'h0000;
  else
    mem32x16[179] <= pgm_decode_wire[179] ? buf_reg[3] : mem32x16[179];
end


//
always @(posedge pgm or negedge nerase_decode_wire[180]) begin
  if(~nerase_decode_wire[180])
    mem32x16[180] <= 16'h0000;
  else
    mem32x16[180] <= pgm_decode_wire[180] ? buf_reg[0] : mem32x16[180];
end

always @(posedge pgm or negedge nerase_decode_wire[181]) begin
  if(~nerase_decode_wire[181])
    mem32x16[181] <= 16'h0000;
  else
    mem32x16[181] <= pgm_decode_wire[181] ? buf_reg[1] : mem32x16[181];
end

always @(posedge pgm or negedge nerase_decode_wire[182]) begin
  if(~nerase_decode_wire[182])
    mem32x16[182] <= 16'h0000;
  else
    mem32x16[182] <= pgm_decode_wire[182] ? buf_reg[2] : mem32x16[182];
end

always @(posedge pgm or negedge nerase_decode_wire[183]) begin
  if(~nerase_decode_wire[183])
    mem32x16[183] <= 16'h0000;
  else
    mem32x16[183] <= pgm_decode_wire[183] ? buf_reg[3] : mem32x16[183];
end


//
always @(posedge pgm or negedge nerase_decode_wire[184]) begin
  if(~nerase_decode_wire[184])
    mem32x16[184] <= 16'h0000;
  else
    mem32x16[184] <= pgm_decode_wire[184] ? buf_reg[0] : mem32x16[184];
end

always @(posedge pgm or negedge nerase_decode_wire[185]) begin
  if(~nerase_decode_wire[185])
    mem32x16[185] <= 16'h0000;
  else
    mem32x16[185] <= pgm_decode_wire[185] ? buf_reg[1] : mem32x16[185];
end

always @(posedge pgm or negedge nerase_decode_wire[186]) begin
  if(~nerase_decode_wire[186])
    mem32x16[186] <= 16'h0000;
  else
    mem32x16[186] <= pgm_decode_wire[186] ? buf_reg[2] : mem32x16[186];
end

always @(posedge pgm or negedge nerase_decode_wire[187]) begin
  if(~nerase_decode_wire[187])
    mem32x16[187] <= 16'h0000;
  else
    mem32x16[187] <= pgm_decode_wire[187] ? buf_reg[3] : mem32x16[187];
end


//
always @(posedge pgm or negedge nerase_decode_wire[188]) begin
  if(~nerase_decode_wire[188])
    mem32x16[188] <= 16'h0000;
  else
    mem32x16[188] <= pgm_decode_wire[188] ? buf_reg[0] : mem32x16[188];
end

always @(posedge pgm or negedge nerase_decode_wire[189]) begin
  if(~nerase_decode_wire[189])
    mem32x16[189] <= 16'h0000;
  else
    mem32x16[189] <= pgm_decode_wire[189] ? buf_reg[1] : mem32x16[189];
end

always @(posedge pgm or negedge nerase_decode_wire[190]) begin
  if(~nerase_decode_wire[190])
    mem32x16[190] <= 16'h0000;
  else
    mem32x16[190] <= pgm_decode_wire[190] ? buf_reg[2] : mem32x16[190];
end

always @(posedge pgm or negedge nerase_decode_wire[191]) begin
  if(~nerase_decode_wire[191])
    mem32x16[191] <= 16'h0000;
  else
    mem32x16[191] <= pgm_decode_wire[191] ? buf_reg[3] : mem32x16[191];
end

//
always @(posedge pgm or negedge nerase_decode_wire[192]) begin
  if(~nerase_decode_wire[192])
    mem32x16[192] <= 16'h0000;
  else
    mem32x16[192] <= pgm_decode_wire[192] ? buf_reg[0] : mem32x16[192];
end

always @(posedge pgm or negedge nerase_decode_wire[193]) begin
  if(~nerase_decode_wire[193])
    mem32x16[193] <= 16'h0000;
  else
    mem32x16[193] <= pgm_decode_wire[193] ? buf_reg[1] : mem32x16[193];
end

always @(posedge pgm or negedge nerase_decode_wire[194]) begin
  if(~nerase_decode_wire[194])
    mem32x16[194] <= 16'h0000;
  else
    mem32x16[194] <= pgm_decode_wire[194] ? buf_reg[2] : mem32x16[194];
end

always @(posedge pgm or negedge nerase_decode_wire[195]) begin
  if(~nerase_decode_wire[195])
    mem32x16[195] <= 16'h0000;
  else
    mem32x16[195] <= pgm_decode_wire[195] ? buf_reg[3] : mem32x16[195];
end


//
always @(posedge pgm or negedge nerase_decode_wire[196]) begin
  if(~nerase_decode_wire[196])
    mem32x16[196] <= 16'h0000;
  else
    mem32x16[196] <= pgm_decode_wire[196] ? buf_reg[0] : mem32x16[196];
end

always @(posedge pgm or negedge nerase_decode_wire[197]) begin
  if(~nerase_decode_wire[197])
    mem32x16[197] <= 16'h0000;
  else
    mem32x16[197] <= pgm_decode_wire[197] ? buf_reg[1] : mem32x16[197];
end

always @(posedge pgm or negedge nerase_decode_wire[198]) begin
  if(~nerase_decode_wire[198])
    mem32x16[198] <= 16'h0000;
  else
    mem32x16[198] <= pgm_decode_wire[198] ? buf_reg[2] : mem32x16[198];
end

always @(posedge pgm or negedge nerase_decode_wire[199]) begin
  if(~nerase_decode_wire[199])
    mem32x16[199] <= 16'h0000;
  else
    mem32x16[199] <= pgm_decode_wire[71] ? buf_reg[3] : mem32x16[199];
end


//
always @(posedge pgm or negedge nerase_decode_wire[200]) begin
  if(~nerase_decode_wire[200])
    mem32x16[200] <= 16'h0000;
  else
    mem32x16[200] <= pgm_decode_wire[200] ? buf_reg[0] : mem32x16[200];
end

always @(posedge pgm or negedge nerase_decode_wire[201]) begin
  if(~nerase_decode_wire[201])
    mem32x16[201] <= 16'h0000;
  else
    mem32x16[201] <= pgm_decode_wire[201] ? buf_reg[1] : mem32x16[201];
end


always @(posedge pgm or negedge nerase_decode_wire[202]) begin
  if(~nerase_decode_wire[202])
    mem32x16[202] <= 16'h0000;
  else
    mem32x16[202] <= pgm_decode_wire[202] ? buf_reg[2] : mem32x16[202];
end

always @(posedge pgm or negedge nerase_decode_wire[203]) begin
  if(~nerase_decode_wire[203])
    mem32x16[203] <= 16'h0000;
  else
    mem32x16[203] <= pgm_decode_wire[203] ? buf_reg[3] : mem32x16[203];
end


//
always @(posedge pgm or negedge nerase_decode_wire[204]) begin
  if(~nerase_decode_wire[204])
    mem32x16[204] <= 16'h0000;
  else
    mem32x16[204] <= pgm_decode_wire[204] ? buf_reg[0] : mem32x16[204];
end

always @(posedge pgm or negedge nerase_decode_wire[205]) begin
  if(~nerase_decode_wire[205])
    mem32x16[205] <= 16'h0000;
  else
    mem32x16[205] <= pgm_decode_wire[205] ? buf_reg[1] : mem32x16[205];
end

always @(posedge pgm or negedge nerase_decode_wire[206]) begin
  if(~nerase_decode_wire[206])
    mem32x16[206] <= 16'h0000;
  else
    mem32x16[206] <= pgm_decode_wire[206] ? buf_reg[2] : mem32x16[206];
end

always @(posedge pgm or negedge nerase_decode_wire[207]) begin
  if(~nerase_decode_wire[207])
    mem32x16[207] <= 16'h0000;
  else
    mem32x16[207] <= pgm_decode_wire[207] ? buf_reg[3] : mem32x16[207];
end

//
always @(posedge pgm or negedge nerase_decode_wire[208]) begin
  if(~nerase_decode_wire[208])
    mem32x16[208] <= 16'h0000;
  else
    mem32x16[208] <= pgm_decode_wire[208] ? buf_reg[0] : mem32x16[208];
end

always @(posedge pgm or negedge nerase_decode_wire[209]) begin
  if(~nerase_decode_wire[209])
    mem32x16[209] <= 16'h0000;
  else
    mem32x16[209] <= pgm_decode_wire[209] ? buf_reg[1] : mem32x16[209];
end

always @(posedge pgm or negedge nerase_decode_wire[210]) begin
  if(~nerase_decode_wire[210])
    mem32x16[210] <= 16'h0000;
  else
    mem32x16[210] <= pgm_decode_wire[210] ? buf_reg[2] : mem32x16[210];
end

always @(posedge pgm or negedge nerase_decode_wire[211]) begin
  if(~nerase_decode_wire[211])
    mem32x16[211] <= 16'h0000;
  else
    mem32x16[211] <= pgm_decode_wire[211] ? buf_reg[3] : mem32x16[211];
end


//
always @(posedge pgm or negedge nerase_decode_wire[212]) begin
  if(~nerase_decode_wire[212])
    mem32x16[212] <= 16'h0000;
  else
    mem32x16[212] <= pgm_decode_wire[212] ? buf_reg[0] : mem32x16[212];
end

always @(posedge pgm or negedge nerase_decode_wire[213]) begin
  if(~nerase_decode_wire[213])
    mem32x16[213] <= 16'h0000;
  else
    mem32x16[213] <= pgm_decode_wire[213] ? buf_reg[1] : mem32x16[213];
end

always @(posedge pgm or negedge nerase_decode_wire[214]) begin
  if(~nerase_decode_wire[214])
    mem32x16[214] <= 16'h0000;
  else
    mem32x16[214] <= pgm_decode_wire[214] ? buf_reg[2] : mem32x16[214];
end

always @(posedge pgm or negedge nerase_decode_wire[215]) begin
  if(~nerase_decode_wire[215])
    mem32x16[215] <= 16'h0000;
  else
    mem32x16[215] <= pgm_decode_wire[215] ? buf_reg[3] : mem32x16[215];
end

//
always @(posedge pgm or negedge nerase_decode_wire[216]) begin
  if(~nerase_decode_wire[216])
    mem32x16[216] <= 16'h0000;
  else
    mem32x16[216] <= pgm_decode_wire[216] ? buf_reg[0] : mem32x16[216];
end

always @(posedge pgm or negedge nerase_decode_wire[217]) begin
  if(~nerase_decode_wire[217])
    mem32x16[217] <= 16'h0000;
  else
    mem32x16[217] <= pgm_decode_wire[217] ? buf_reg[1] : mem32x16[217];
end

always @(posedge pgm or negedge nerase_decode_wire[218]) begin
  if(~nerase_decode_wire[218])
    mem32x16[218] <= 16'h0000;
  else
    mem32x16[218] <= pgm_decode_wire[218] ? buf_reg[2] : mem32x16[218];
end

always @(posedge pgm or negedge nerase_decode_wire[219]) begin
  if(~nerase_decode_wire[219])
    mem32x16[219] <= 16'h0000;
  else
    mem32x16[219] <= pgm_decode_wire[219] ? buf_reg[3] : mem32x16[219];
end

//
always @(posedge pgm or negedge nerase_decode_wire[220]) begin
  if(~nerase_decode_wire[220])
    mem32x16[220] <= 16'h0000;
  else
    mem32x16[220] <= pgm_decode_wire[220] ? buf_reg[0] : mem32x16[220];
end

always @(posedge pgm or negedge nerase_decode_wire[221]) begin
  if(~nerase_decode_wire[221])
    mem32x16[221] <= 16'h0000;
  else
    mem32x16[221] <= pgm_decode_wire[221] ? buf_reg[1] : mem32x16[221];
end

always @(posedge pgm or negedge nerase_decode_wire[222]) begin
  if(~nerase_decode_wire[222])
    mem32x16[222] <= 16'h0000;
  else
    mem32x16[222] <= pgm_decode_wire[222] ? buf_reg[2] : mem32x16[222];
end

always @(posedge pgm or negedge nerase_decode_wire[223]) begin
  if(~nerase_decode_wire[223])
    mem32x16[223] <= 16'h0000;
  else
    mem32x16[223] <= pgm_decode_wire[223] ? buf_reg[3] : mem32x16[223];
end


//
always @(posedge pgm or negedge nerase_decode_wire[224]) begin
  if(~nerase_decode_wire[224])
    mem32x16[224] <= 16'h0000;
  else
    mem32x16[224] <= pgm_decode_wire[224] ? buf_reg[0] : mem32x16[224];
end

always @(posedge pgm or negedge nerase_decode_wire[225]) begin
  if(~nerase_decode_wire[225])
    mem32x16[225] <= 16'h0000;
  else
    mem32x16[225] <= pgm_decode_wire[255] ? buf_reg[1] : mem32x16[225];
end

always @(posedge pgm or negedge nerase_decode_wire[226]) begin
  if(~nerase_decode_wire[226])
    mem32x16[226] <= 16'h0000;
  else
    mem32x16[226] <= pgm_decode_wire[226] ? buf_reg[2] : mem32x16[226];
end

always @(posedge pgm or negedge nerase_decode_wire[227]) begin
  if(~nerase_decode_wire[227])
    mem32x16[227] <= 16'h0000;
  else
    mem32x16[227] <= pgm_decode_wire[227] ? buf_reg[3] : mem32x16[227];
end

//
always @(posedge pgm or negedge nerase_decode_wire[228]) begin
  if(~nerase_decode_wire[228])
    mem32x16[228] <= 16'h0000;
  else
    mem32x16[228] <= pgm_decode_wire[228] ? buf_reg[0] : mem32x16[228];
end

always @(posedge pgm or negedge nerase_decode_wire[229]) begin
  if(~nerase_decode_wire[229])
    mem32x16[229] <= 16'h0000;
  else
    mem32x16[229] <= pgm_decode_wire[229] ? buf_reg[1] : mem32x16[229];
end


always @(posedge pgm or negedge nerase_decode_wire[230]) begin
  if(~nerase_decode_wire[230])
    mem32x16[230] <= 16'h0000;
  else
    mem32x16[230] <= pgm_decode_wire[230] ? buf_reg[2] : mem32x16[230];
end

always @(posedge pgm or negedge nerase_decode_wire[231]) begin
  if(~nerase_decode_wire[231])
    mem32x16[231] <= 16'h0000;
  else
    mem32x16[231] <= pgm_decode_wire[231] ? buf_reg[3] : mem32x16[231];
end

//
always @(posedge pgm or negedge nerase_decode_wire[232]) begin
  if(~nerase_decode_wire[232])
    mem32x16[232] <= 16'h0000;
  else
    mem32x16[232] <= pgm_decode_wire[232] ? buf_reg[0] : mem32x16[232];
end

always @(posedge pgm or negedge nerase_decode_wire[233]) begin
  if(~nerase_decode_wire[233])
    mem32x16[233] <= 16'h0000;
  else
    mem32x16[233] <= pgm_decode_wire[233] ? buf_reg[1] : mem32x16[233];
end

always @(posedge pgm or negedge nerase_decode_wire[234]) begin
  if(~nerase_decode_wire[234])
    mem32x16[234] <= 16'h0000;
  else
    mem32x16[234] <= pgm_decode_wire[234] ? buf_reg[2] : mem32x16[234];
end

always @(posedge pgm or negedge nerase_decode_wire[235]) begin
  if(~nerase_decode_wire[235])
    mem32x16[235] <= 16'h0000;
  else
    mem32x16[235] <= pgm_decode_wire[235] ? buf_reg[3] : mem32x16[235];
end

//
always @(posedge pgm or negedge nerase_decode_wire[236]) begin
  if(~nerase_decode_wire[236])
    mem32x16[236] <= 16'h0000;
  else
    mem32x16[236] <= pgm_decode_wire[236] ? buf_reg[0] : mem32x16[236];
end

always @(posedge pgm or negedge nerase_decode_wire[237]) begin
  if(~nerase_decode_wire[237])
    mem32x16[237] <= 16'h0000;
  else
    mem32x16[237] <= pgm_decode_wire[237] ? buf_reg[1] : mem32x16[237];
end

always @(posedge pgm or negedge nerase_decode_wire[238]) begin
  if(~nerase_decode_wire[238])
    mem32x16[238] <= 16'h0000;
  else
    mem32x16[238] <= pgm_decode_wire[238] ? buf_reg[2] : mem32x16[238];
end

always @(posedge pgm or negedge nerase_decode_wire[239]) begin
  if(~nerase_decode_wire[239])
    mem32x16[239] <= 16'h0000;
  else
    mem32x16[239] <= pgm_decode_wire[239] ? buf_reg[3] : mem32x16[239];
end

//
always @(posedge pgm or negedge nerase_decode_wire[240]) begin
  if(~nerase_decode_wire[240])
    mem32x16[240] <= 16'h0000;
  else
    mem32x16[240] <= pgm_decode_wire[240] ? buf_reg[0] : mem32x16[240];
end

always @(posedge pgm or negedge nerase_decode_wire[241]) begin
  if(~nerase_decode_wire[241])
    mem32x16[241] <= 16'h0000;
  else
    mem32x16[241] <= pgm_decode_wire[241] ? buf_reg[1] : mem32x16[241];
end

always @(posedge pgm or negedge nerase_decode_wire[242]) begin
  if(~nerase_decode_wire[242])
    mem32x16[242] <= 16'h0000;
  else
    mem32x16[242] <= pgm_decode_wire[242] ? buf_reg[2] : mem32x16[242];
end

always @(posedge pgm or negedge nerase_decode_wire[243]) begin
  if(~nerase_decode_wire[243])
    mem32x16[243] <= 16'h0000;
  else
    mem32x16[243] <= pgm_decode_wire[243] ? buf_reg[3] : mem32x16[243];
end

//
always @(posedge pgm or negedge nerase_decode_wire[244]) begin
  if(~nerase_decode_wire[244])
    mem32x16[244] <= 16'h0000;
  else
    mem32x16[244] <= pgm_decode_wire[244] ? buf_reg[0] : mem32x16[244];
end

always @(posedge pgm or negedge nerase_decode_wire[245]) begin
  if(~nerase_decode_wire[245])
    mem32x16[245] <= 16'h0000;
  else
    mem32x16[245] <= pgm_decode_wire[245] ? buf_reg[1] : mem32x16[245];
end

always @(posedge pgm or negedge nerase_decode_wire[246]) begin
  if(~nerase_decode_wire[246])
    mem32x16[246] <= 16'h0000;
  else
    mem32x16[246] <= pgm_decode_wire[246] ? buf_reg[2] : mem32x16[246];
end

always @(posedge pgm or negedge nerase_decode_wire[247]) begin
  if(~nerase_decode_wire[247])
    mem32x16[247] <= 16'h0000;
  else
    mem32x16[247] <= pgm_decode_wire[247] ? buf_reg[3] : mem32x16[247];
end

//
always @(posedge pgm or negedge nerase_decode_wire[248]) begin
  if(~nerase_decode_wire[248])
    mem32x16[248] <= 16'h0000;
  else
    mem32x16[248] <= pgm_decode_wire[248] ? buf_reg[0] : mem32x16[248];
end

always @(posedge pgm or negedge nerase_decode_wire[249]) begin
  if(~nerase_decode_wire[249])
    mem32x16[249] <= 16'h0000;
  else
    mem32x16[249] <= pgm_decode_wire[249] ? buf_reg[1] : mem32x16[249];
end

always @(posedge pgm or negedge nerase_decode_wire[250]) begin
  if(~nerase_decode_wire[250])
    mem32x16[250] <= 16'h0000;
  else
    mem32x16[250] <= pgm_decode_wire[250] ? buf_reg[2] : mem32x16[250];
end

always @(posedge pgm or negedge nerase_decode_wire[251]) begin
  if(~nerase_decode_wire[251])
    mem32x16[251] <= 16'h0000;
  else
    mem32x16[251] <= pgm_decode_wire[251] ? buf_reg[3] : mem32x16[251];
end

//
always @(posedge pgm or negedge nerase_decode_wire[252]) begin
  if(~nerase_decode_wire[252])
    mem32x16[252] <= 16'h0000;
  else
    mem32x16[252] <= pgm_decode_wire[252] ? buf_reg[0] : mem32x16[252];
end

always @(posedge pgm or negedge nerase_decode_wire[253]) begin
  if(~nerase_decode_wire[253])
    mem32x16[253] <= 16'h0000;
  else
    mem32x16[253] <= pgm_decode_wire[253] ? buf_reg[1] : mem32x16[253];
end

always @(posedge pgm or negedge nerase_decode_wire[254]) begin
  if(~nerase_decode_wire[254])
    mem32x16[254] <= 16'h0000;
  else
    mem32x16[254] <= pgm_decode_wire[254] ? buf_reg[2] : mem32x16[254];
end

always @(posedge pgm or negedge nerase_decode_wire[255]) begin
  if(~nerase_decode_wire[255])
    mem32x16[255] <= 16'h0000;
  else
    mem32x16[255] <= pgm_decode_wire[255] ? buf_reg[3] : mem32x16[255];
end








assign mem32x16_dout_wire = mem32x16[addr[11:4]];

always @(negedge sync)	begin 
  if(read)
    dout_reg <=  mem32x16_dout_wire;
end 
             
/****/            
assign #150 dout = dout_reg;

endmodule

