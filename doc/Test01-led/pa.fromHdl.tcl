
# PlanAhead Launch Script for Pre-Synthesis Floorplanning, created by Project Navigator

create_project -name led -dir "D:/FPGA/timer_chipscope/Test01-led/Test01-led/planAhead_run_1" -part xc6slx16ftg256-2
set_param project.pinAheadLayout yes
set srcset [get_property srcset [current_run -impl]]
set_property target_constrs_file "sp6_led.ucf" [current_fileset -constrset]
set hdlfile [add_files [list {led_top.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set_property top led_top $srcset
add_files [list {sp6_led.ucf}] -fileset [get_property constrset [current_run]]
open_rtl_design -part xc6slx16ftg256-2
