/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				zf_tim.h
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.32.4 or MDK 5.28
* @Target core		MM32F3277
* @Taobao			https://seekfree.taobao.com/
* @date				2021-02-22
********************************************************************************************************************/

#ifndef _zf_tim_h
#define _zf_tim_h

#include "common.h"

#include "zf_gpio.h"

#define TIM_ENCODER_COUNTER_BASE			0xFFFF

typedef enum																		// 枚举定时器模块 此枚举定义不允许用户修改
{
	// Advanced Timer 16 bits
	TIM_1					= 0x1000,												// 0x1000[APB2]	0x0000[APB_EN BIT] 0x0000[TIM index]
	TIM_8					= 0x1011,												// 0x1000[APB2]	0x0010[APB_EN BIT] 0x0001[TIM index]

	// General Timer 32 bits
	TIM_2					= 0x0002,												// 0x0000[APB1]	0x0000[APB_EN BIT] 0x0002[TIM index]
	TIM_5					= 0x0033,												// 0x0000[APB1]	0x0030[APB_EN BIT] 0x0003[TIM index]

	// General Timer 16 bits
	TIM_3					= 0x0014,												// 0x0000[APB1]	0x0010[APB_EN BIT] 0x0004[TIM index]
	TIM_4					= 0x0025,												// 0x0000[APB1]	0x0020[APB_EN BIT] 0x0005[TIM index]

	// Basic Timer 16 bits
	TIM_6					= 0x0046,												// 0x0000[APB1]	0x0040[APB_EN BIT] 0x0006[TIM index]
	TIM_7					= 0x0057,												// 0x0000[APB1]	0x0050[APB_EN BIT] 0x0007[TIM index]
}TIM_enum;

typedef enum																		// 枚举编码器通道 此枚举定义不允许用户修改
{
	// Advanced Timer 16 bits TIM1
	TIM_1_ENC1_A08			= 0x1108,												// 0x1000[CH1] 0x0100[AF1] 0x0000[A group] 0x0008[pin  8]
	TIM_1_ENC1_E09			= 0x1149,												// 0x1000[CH1] 0x0100[AF1] 0x0040[E group] 0x0009[pin  9]

	TIM_1_ENC2_A09			= 0x2109,												// 0x2000[CH2] 0x0100[AF1] 0x0000[A group] 0x0009[pin  9]
	TIM_1_ENC2_E11			= 0x214B,												// 0x2000[CH2] 0x0100[AF1] 0x0040[E group] 0x000B[pin 11]

	// Advanced Timer 16 bits TIM8
	TIM_8_ENC1_C06			= 0x1326,												// 0x1000[CH1] 0x0300[AF3] 0x0020[C group] 0x0006[pin  6]

	TIM_8_ENC2_C07			= 0x2327,												// 0x2000[CH2] 0x0300[AF3] 0x0020[C group] 0x0007[pin  7]

	// General Timer 32 bits TIM2
	TIM_2_ENC1_A00			= 0x1100,												// 0x1000[CH1] 0x0100[AF1] 0x0000[A group] 0x0000[pin  0]
	TIM_2_ENC1_A05			= 0x1105,												// 0x1000[CH1] 0x0100[AF1] 0x0000[A group] 0x0005[pin  5]
	TIM_2_ENC1_A15			= 0x110F,												// 0x1000[CH1] 0x0100[AF1] 0x0000[A group] 0x000F[pin 15]

	TIM_2_ENC2_A01			= 0x2101,												// 0x2000[CH2] 0x0100[AF1] 0x0000[A group] 0x0001[pin  1]
	TIM_2_ENC2_B03			= 0x2113,												// 0x2000[CH2] 0x0100[AF1] 0x0010[B group] 0x0003[pin  3]

	// General Timer 32 bits TIM5
	TIM_5_ENC1_A00			= 0x1200,												// 0x1000[CH1] 0x0200[AF2] 0x0000[A group] 0x0000[pin  0]

	TIM_5_ENC2_A01			= 0x2201,												// 0x2000[CH2] 0x0200[AF2] 0x0000[A group] 0x0001[pin  1]

	// General Timer 16 bits TIM3
	TIM_3_ENC1_A06			= 0x1206,												// 0x1000[CH1] 0x0200[AF2] 0x0000[A group] 0x0006[pin  6]
	TIM_3_ENC1_B04			= 0x1214,												// 0x1000[CH1] 0x0200[AF2] 0x0010[B group] 0x0004[pin  4]
	TIM_3_ENC1_C06			= 0x1226,												// 0x1000[CH1] 0x0200[AF2] 0x0020[C group] 0x0006[pin  6]

	TIM_3_ENC2_A07			= 0x2207,												// 0x2000[CH2] 0x0200[AF2] 0x0000[A group] 0x0007[pin  7]
	TIM_3_ENC2_B05			= 0x2215,												// 0x2000[CH2] 0x0200[AF2] 0x0010[B group] 0x0005[pin  5]
	TIM_3_ENC2_C07			= 0x2227,												// 0x2000[CH2] 0x0200[AF2] 0x0020[C group] 0x0007[pin  7]

	// General Timer 16 bits TIM4
	TIM_4_ENC1_B06			= 0x1216,												// 0x1000[CH1] 0x0200[AF2] 0x0010[B group] 0x0006[pin  6]
	TIM_4_ENC1_D12			= 0x123C,												// 0x1000[CH1] 0x0200[AF2] 0x0030[D group] 0x000C[pin 12]

	TIM_4_ENC2_B07			= 0x2217,												// 0x2000[CH2] 0x0200[AF2] 0x0010[B group] 0x0007[pin  7]
	TIM_4_ENC2_D13			= 0x223D,												// 0x2000[CH2] 0x0200[AF2] 0x0030[D group] 0x000D[pin 13]

}TIM_ENC_PIN_enum;

extern TIM_TypeDef *tim_index[8];

void	tim_encoder_init			(TIM_enum tim, TIM_ENC_PIN_enum PULSEA_pin, TIM_ENC_PIN_enum PULSEB_pin);
void	tim_encoder_rst				(TIM_enum tim);
int16	tim_encoder_get_count		(TIM_enum tim);

void	tim_counter_init			(TIM_enum tim, TIM_ENC_PIN_enum input_pin);
void	tim_counter_rst				(TIM_enum tim);
int16	tim_counter_get_count		(TIM_enum tim);

#endif
