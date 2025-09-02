/*
 * mian.
 *
 * Copyright (C) 2019 XinLuYao, Inc. All Rights Reserved.
 * Written by quantum (quantum@whxinluyao.com)
 * Website: www.whxinluyao.com
 */


#include "mc_flag.h"
#include "mc_angle.h"
#include "lpf.h"

//------------------------------------

uint32_t g_ph_a;
uint32_t g_ph_b;
uint32_t g_ph_c;


uint16_t bsp_get_adc(void)
{
	return 0;
}
uint16_t bsp_get_adc_filter(uint8_t times)
{
	uint32_t temp_val = 0;
	uint8_t t;
	for (t = 0; t < times; t++) 
	{
		temp_val += bsp_get_adc();
		delay_ms(5);
	}
	return (temp_val/times);
}
void bsp_enc_read_angle(void)
{
	int16_t val_tmp = 0;
	
	mc_angle.value = val_tmp;
	mc_angle_process(mc_angle.value);
}
//------------------------------------



/**
  * @brief  main函数，C环境准备好后，最先执行的代码
  */
int thread_motor_main(void)
{	
	lpf_1rd_init(&lpf_speed_fb, 0.2, 0);
	lpf_3rd_init(&lpf_adc, 0.6, 0);
	
	//delay_init();	//定时器初始化，初始化后可以使用delay_ms和delay_us进行延时
	//bsp_led_init();	//led灯初始化
	//bsp_mos_init(); //MP6536使能脚和刹车引脚初始化
	//bsp_24cxx_init();
	
	mc_read_elec_offset();
	//bsp_enc_init();
	//mc_angle_init();	
	//bsp_pwm_init();
	
	//bsp_timer2_init();
	
	//bsp_adc_init();
	
	// 开启驱动芯片输出，若无这行代码，这个芯片一直无输出，电机不会工作
	//bsp_mos_on();
	
	//mc_flag.ctrl_loop_mode = MC_NO_CTRL_MODE;
	//mc_flag.ctrl_loop_mode = MC_SPD_CLOSE_LOOP_MODE;
	mc_flag.ctrl_loop_mode = MC_OPEN_MODE;
	
	while(1)
	{
		// ADC采样进行速度控制
		uint16_t adcx_tmp = bsp_get_adc_filter(2);	// 10ms
		uint16_t adcx = lpf_3rd_calc(&lpf_adc, adcx_tmp);
		
		if (mc_flag.ctrl_loop_mode == MC_SPD_CLOSE_LOOP_MODE)
		{
			int16_t spd = adcx - 2048;
			int16_t speed = spd * 2200 / 2048;
			if (speed < -1500) {
				speed_ctrl = -1500;
			} else if (speed > 1500) {
				speed_ctrl = 1500;
			} else {
				speed_ctrl = speed;
			}
		} 
		else if (mc_flag.ctrl_loop_mode == MC_POS_CLOSE_LOOP_MODE)
		{
			int32_t pos = adcx * 32768 / 4095;
			if (pos < 0) {
				pos_ctrl = 0;
			} else if (pos > 32768) {
				pos_ctrl = 32768;
			} else {
				pos_ctrl = pos;
			}
		}
		
		// LED闪烁指示程序正在运行
		static uint32_t t = 0;
		t++;
		if (t > 3) {
			t = 0;
			//LED1 = !LED1;
			ee_printf_a("main111-------\n" );
		} else {
			t++;
		}
		rt_thread_delay( 10 );
	}
}


