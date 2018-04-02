`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    13:40:55 02/28/2016 
// Design Name: 
// Module Name:    led_top 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module led_top(
    input sys_clk,
    input sys_rst_n,
	 input pwm_in,
    output led_1,
	 output led_2,
	 output timer_out
    );

parameter DLY_CNT = 32'd50000000;
parameter HALF_DLY_CNT = 32'd25000000;
(*keep = "ture"*)wire pwm_in;
reg r_led;
reg l_led;
reg [31:0]count;
(*keep = "ture"*) wire timer_out; //∑¿÷πpwm_in ±ª”≈ªØ
//counter control
always@(posedge sys_clk or negedge sys_rst_n)
begin
	if(!sys_rst_n)
		begin
			count <= 32'd0;
		end
	else if(count == DLY_CNT)
		begin
			count <= 32'd0;
		end
	else
		begin
			count <= count+32'd1;
		end
end

//led output register control
always@(posedge sys_clk or negedge sys_rst_n)
begin
	if(!sys_rst_n)
		begin
			r_led <= 1'b0;
			l_led <= 1'b0;
		end
	else if(count < HALF_DLY_CNT)
		begin
			r_led <= 1'b1;
			l_led <= 1'b0;
		end
	else
		begin
			r_led <= 1'b0;
			l_led <= 1'b1;
		end
end

assign led_1 = r_led;
assign led_2 = l_led;

reg pwm_reg1;
(*keep = "ture"*)  reg pwm_reg2;
 
always @(posedge sys_clk)

begin
	pwm_reg1 <= pwm_in;
	pwm_reg2 <= pwm_reg1; 

end

assign timer_out = led_1 & pwm_reg2;

endmodule
