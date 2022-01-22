/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				zf_pit.c
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.32.4 or MDK 5.28
* @Target core		MM32F3277
* @Taobao			https://seekfree.taobao.com/
* @date				2021-02-22
********************************************************************************************************************/

#include "zf_pit.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		TIM 中断初始化
// @param		tim				选择中断使用的 TIM
// @param		freq			设置频率 同个模块只有最后一次设置生效
// @param		priority		选择该中断优先级 范围 [0-7]
// @return		void
// Sample usage:				tim_interrupt_init(TIM_1, 1, 0x00);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_init (TIM_enum tim, uint32 freq, uint8 priority)
{
	IRQn_Type irq_index[8] = 
	{
		TIM1_UP_IRQn,
		TIM8_UP_IRQn,
		TIM2_IRQn,
		TIM5_IRQn,
		TIM3_IRQn,
		TIM4_IRQn,
		TIM6_IRQn,
		TIM7_IRQn
	};

	uint16 freq_div = ((SystemCoreClock / freq) >> 15);											// 计算预分频
	uint16 period_temp = (SystemCoreClock / freq / (freq_div+1));								// 计算自动重装载值

	if(tim & 0xf000)
		RCC->APB2ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// 使能时钟
	else
		RCC->APB1ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// 使能时钟

	tim_index[(tim&0x0f)]->ARR = period_temp;													// 装载自动重装载值
	tim_index[(tim&0x0f)]->PSC = freq_div;														// 装载预分频
	tim_index[(tim&0x0f)]->DIER |= TIM_DIER_UI;													// 使能中断更新

	nvic_init(irq_index[(tim&0x0f)], priority, ENABLE);

	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// 使能定时器
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TIM 中断初始化 us级别中断
// @param		tim				选择中断使用的 TIM
// @param		timer			设置时长 同个模块只有最后一次设置生效
// @param		priority		选择该中断优先级 范围 [0-7]
// @return		void
// Sample usage:				tim_interrupt_init_us(TIM_1, 1, 0x00);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_init_us (TIM_enum tim, uint32 timer, uint8 priority)
{
	if(timer <= 0 || timer > 1000)		return;
	IRQn_Type irq_index[8] = 
	{
		TIM1_UP_IRQn,
		TIM8_UP_IRQn,
		TIM2_IRQn,
		TIM5_IRQn,
		TIM3_IRQn,
		TIM4_IRQn,
		TIM6_IRQn,
		TIM7_IRQn
	};

	uint16 freq_div = 1;																		// 计算预分频
	uint16 period_temp = timer*60-1;															// 计算自动重装载值

	if(tim & 0xf000)
		RCC->APB2ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// 使能时钟
	else
		RCC->APB1ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// 使能时钟

	tim_index[(tim&0x0f)]->ARR = period_temp;													// 装载自动重装载值
	tim_index[(tim&0x0f)]->PSC = freq_div;														// 装载预分频
	tim_index[(tim&0x0f)]->DIER |= TIM_DIER_UI;													// 使能中断更新

	nvic_init(irq_index[(tim&0x0f)], priority, ENABLE);

	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// 使能定时器
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TIM 中断初始化 ms级别中断
// @param		tim				选择中断使用的 TIM
// @param		timer			设置时长 同个模块只有最后一次设置生效
// @param		priority		选择该中断优先级 范围 [0-7]
// @return		void
// Sample usage:				tim_interrupt_init_ms(TIM_1, 1, 0x00);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_init_ms (TIM_enum tim, uint32 timer, uint8 priority)
{
	if(timer <= 0 || timer > 5000)		return;
	IRQn_Type irq_index[8] = 
	{
		TIM1_UP_IRQn,
		TIM8_UP_IRQn,
		TIM2_IRQn,
		TIM5_IRQn,
		TIM3_IRQn,
		TIM4_IRQn,
		TIM6_IRQn,
		TIM7_IRQn
	};

	uint16 freq_div = timer*2-1;																// 计算预分频
	uint16 period_temp = SystemCoreClock / 2000 - 1;											// 计算自动重装载值

	if(tim & 0xf000)
		RCC->APB2ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// 使能时钟
	else
		RCC->APB1ENR |= ((uint32_t)0x00000001 << ((tim&0x0ff0) >> 4));							// 使能时钟

	tim_index[(tim&0x0f)]->ARR = period_temp;													// 装载自动重装载值
	tim_index[(tim&0x0f)]->PSC = freq_div;														// 装载预分频
	tim_index[(tim&0x0f)]->DIER |= TIM_DIER_UI;													// 使能中断更新

	nvic_init(irq_index[(tim&0x0f)], priority, ENABLE);

	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// 使能定时器
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TIM 中断使能
// @param		tim				选择 TIM
// @return		void
// Sample usage:				tim_interrupt_enable(TIM_1);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_enable (TIM_enum tim)
{
	tim_index[(tim&0x0f)]->CR1 |= TIM_CR1_CEN;													// 使能定时器
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		TIM 中断失能
// @param		tim				选择 TIM
// @return		void
// Sample usage:				tim_interrupt_disable(TIM_1);
//-------------------------------------------------------------------------------------------------------------------
void tim_interrupt_disable (TIM_enum tim)
{
	tim_index[(tim&0x0f)]->CR1 &= ~TIM_CR1_CEN;													// 失能定时器
}
