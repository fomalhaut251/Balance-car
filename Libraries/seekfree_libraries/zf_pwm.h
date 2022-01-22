/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				zf_pwm.h
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.32.4 or MDK 5.28
* @Target core		MM32F3277
* @Taobao			https://seekfree.taobao.com/
* @date				2021-02-22
********************************************************************************************************************/

#ifndef _zf_pwm_h
#define _zf_pwm_h

#include "common.h"

#include "zf_gpio.h"
#include "zf_tim.h"

#define PWM_DUTY_MAX		10000

typedef enum																		// 枚举 PWM 通道	此枚举定义不允许用户修改
{
	// Advanced Timer 16 bits TIM1
	TIM_1_CH1_A08			= 0x1108,												// 0x1000[CH1] 0x0100[AF1] 0x0000[A group] 0x0008[pin  8]
	TIM_1_CH1_E09			= 0x1149,												// 0x1000[CH1] 0x0100[AF1] 0x0040[E group] 0x0009[pin  9]

	TIM_1_CH2_A09			= 0x2109,												// 0x2000[CH2] 0x0100[AF1] 0x0000[A group] 0x0009[pin  9]
	TIM_1_CH2_E11			= 0x214B,												// 0x2000[CH2] 0x0100[AF1] 0x0040[E group] 0x000B[pin 11]

	TIM_1_CH3_A10			= 0x310A,												// 0x3000[CH3] 0x0100[AF1] 0x0000[A group] 0x000A[pin 10]
	TIM_1_CH3_E13			= 0x314D,												// 0x3000[CH2] 0x0100[AF1] 0x0040[E group] 0x000D[pin 13]

	TIM_1_CH4_A11			= 0x410B,												// 0x4000[CH4] 0x0100[AF1] 0x0000[A group] 0x000B[pin 11]
	TIM_1_CH4_E14			= 0x414E,												// 0x4000[CH4] 0x0100[AF1] 0x0040[E group] 0x000E[pin 14]

	// Advanced Timer 16 bits TIM8
	TIM_8_CH1_C06			= 0x1326,												// 0x1000[CH1] 0x0300[AF3] 0x0020[C group] 0x0006[pin  6]

	TIM_8_CH2_C07			= 0x2327,												// 0x2000[CH2] 0x0300[AF3] 0x0020[C group] 0x0007[pin  7]

	TIM_8_CH3_C08			= 0x3328,												// 0x3000[CH3] 0x0300[AF3] 0x0020[C group] 0x0008[pin  8]

	TIM_8_CH4_C09			= 0x4329,												// 0x4000[CH4] 0x0300[AF3] 0x0020[C group] 0x0009[pin  9]

	// General Timer 32 bits TIM2
	TIM_2_CH1_A00			= 0x1100,												// 0x1000[CH1] 0x0100[AF1] 0x0000[A group] 0x0000[pin  0]
	TIM_2_CH1_A05			= 0x1105,												// 0x1000[CH1] 0x0100[AF1] 0x0000[A group] 0x0005[pin  5]
	TIM_2_CH1_A15			= 0x110F,												// 0x1000[CH1] 0x0100[AF1] 0x0000[A group] 0x000F[pin 15]

	TIM_2_CH2_A01			= 0x2101,												// 0x2000[CH2] 0x0100[AF1] 0x0000[A group] 0x0001[pin  1]
	TIM_2_CH2_B03			= 0x2113,												// 0x2000[CH2] 0x0100[AF1] 0x0010[B group] 0x0003[pin  3]

	TIM_2_CH3_A02			= 0x3102,												// 0x3000[CH3] 0x0100[AF1] 0x0000[A group] 0x0002[pin  2]
	TIM_2_CH3_B10			= 0x311A,												// 0x3000[CH3] 0x0100[AF1] 0x0010[B group] 0x000A[pin 10]

	TIM_2_CH4_A03			= 0x4103,												// 0x4000[CH4] 0x0100[AF1] 0x0000[A group] 0x0003[pin  3]
	TIM_2_CH4_B11			= 0x411B,												// 0x4000[CH4] 0x0100[AF1] 0x0010[B group] 0x000B[pin 11]

	// General Timer 32 bits TIM5
	TIM_5_CH1_A00			= 0x1200,												// 0x1000[CH1] 0x0200[AF2] 0x0000[A group] 0x0000[pin  0]

	TIM_5_CH2_A01			= 0x2201,												// 0x2000[CH2] 0x0200[AF2] 0x0000[A group] 0x0001[pin  1]

	TIM_5_CH3_A02			= 0x3202,												// 0x3000[CH3] 0x0200[AF2] 0x0000[A group] 0x0002[pin  2]

	TIM_5_CH4_A03			= 0x4203,												// 0x4000[CH4] 0x0200[AF2] 0x0000[A group] 0x0003[pin  3]

	// General Timer 16 bits TIM3
	TIM_3_CH1_A06			= 0x1206,												// 0x1000[CH1] 0x0200[AF2] 0x0000[A group] 0x0006[pin  6]
	TIM_3_CH1_B04			= 0x1214,												// 0x1000[CH1] 0x0200[AF2] 0x0010[B group] 0x0004[pin  4]
	TIM_3_CH1_C06			= 0x1226,												// 0x1000[CH1] 0x0200[AF2] 0x0020[C group] 0x0006[pin  6]

	TIM_3_CH2_A07			= 0x2207,												// 0x2000[CH2] 0x0200[AF2] 0x0000[A group] 0x0007[pin  7]
	TIM_3_CH2_B05			= 0x2215,												// 0x2000[CH2] 0x0200[AF2] 0x0010[B group] 0x0005[pin  5]
	TIM_3_CH2_C07			= 0x2227,												// 0x2000[CH2] 0x0200[AF2] 0x0020[C group] 0x0007[pin  7]

	TIM_3_CH3_B00			= 0x3210,												// 0x3000[CH3] 0x0200[AF2] 0x0010[B group] 0x0000[pin  0]
	TIM_3_CH3_C08			= 0x3228,												// 0x3000[CH3] 0x0200[AF2] 0x0020[C group] 0x0008[pin  8]

	TIM_3_CH4_B01			= 0x4211,												// 0x4000[CH4] 0x0200[AF2] 0x0010[B group] 0x0001[pin  1]
	TIM_3_CH4_C09			= 0x4229,												// 0x4000[CH4] 0x0200[AF2] 0x0020[C group] 0x0009[pin  9]

	// General Timer 16 bits TIM4
	TIM_4_CH1_B06			= 0x1216,												// 0x1000[CH1] 0x0200[AF2] 0x0010[B group] 0x0006[pin  6]
	TIM_4_CH1_D12			= 0x123C,												// 0x1000[CH1] 0x0200[AF2] 0x0030[D group] 0x000C[pin 12]

	TIM_4_CH2_B07			= 0x2217,												// 0x2000[CH2] 0x0200[AF2] 0x0010[B group] 0x0007[pin  7]
	TIM_4_CH2_D13			= 0x223D,												// 0x2000[CH2] 0x0200[AF2] 0x0030[D group] 0x000D[pin 13]

	TIM_4_CH3_B08			= 0x3218,												// 0x3000[CH3] 0x0200[AF2] 0x0010[B group] 0x0008[pin  8]
	TIM_4_CH3_D14			= 0x323E,												// 0x3000[CH3] 0x0200[AF2] 0x0030[D group] 0x000E[pin 14]

	TIM_4_CH4_B09			= 0x4219,												// 0x4000[CH4] 0x0200[AF2] 0x0010[B group] 0x0009[pin  9]
	TIM_4_CH4_D15			= 0x423F,												// 0x4000[CH4] 0x0200[AF2] 0x0030[D group] 0x000F[pin 15]
}TIM_PWMPIN_enum;

void pwm_init (TIM_enum tim, TIM_PWMPIN_enum pin, uint32 freq, uint32 duty);
void pwm_enable (TIM_enum tim);
void pwm_disable (TIM_enum tim);
void pwm_duty_updata (TIM_enum tim, TIM_PWMPIN_enum pin, uint32 duty);

#endif
