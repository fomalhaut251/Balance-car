/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				zf_spi.h
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.32.4 or MDK 5.28
* @Target core		MM32F3277
* @Taobao			https://seekfree.taobao.com/
* @date				2021-02-22
********************************************************************************************************************/

#ifndef _zf_spi_h
#define _zf_spi_h

#include "common.h"

#include "zf_gpio.h"

typedef enum																		// 枚举串口号 此枚举定义不允许用户修改
{
	SPI_1,
	SPI_2,
	SPI_3,
}SPIN_enum;

typedef enum																		// 枚举串口引脚 此枚举定义不允许用户修改
{
	SPI1_SCK_A05					= 0x505,										// 0x500[AF5] 0x000[A group] 0x005[pin  5]
	SPI1_SCK_B03					= 0x513,										// 0x500[AF5] 0x010[B group] 0x003[pin  3]
	SPI1_SCK_E12					= 0x54C,										// 0x500[AF5] 0x040[E group] 0x00C[pin 12]

	SPI2_SCK_B10					= 0x51A,										// 0x500[AF5] 0x010[B group] 0x00A[pin 10]
	SPI2_SCK_B13					= 0x51D,										// 0x500[AF5] 0x010[B group] 0x00D[pin 13]
	SPI2_SCK_D03					= 0x533,										// 0x500[AF5] 0x030[D group] 0x003[pin  3]
	SPI2_SCK_E02					= 0x542,										// 0x500[AF5] 0x040[E group] 0x002[pin  2]

	SPI3_SCK_B03					= 0x613,										// 0x600[AF6] 0x010[B group] 0x003[pin  3]
	SPI3_SCK_C10					= 0x62A,										// 0x600[AF6] 0x020[C group] 0x00A[pin 10]
	SPI3_SCK_D04					= 0x534,										// 0x500[AF5] 0x030[D group] 0x004[pin  4]
	SPI3_SCK_D12					= 0x63C,										// 0x600[AF6] 0x030[D group] 0x00C[pin 12]
}SPI_SCKPIN_enum;

typedef enum																		// 枚举串口引脚 此枚举定义不允许用户修改
{
	SPI1_MOSI_A07					= 0x507,										// 0x500[AF5] 0x000[A group] 0x007[pin  7]
	SPI1_MOSI_B05					= 0x515,										// 0x500[AF5] 0x010[B group] 0x005[pin  5]
	SPI1_MOSI_E14					= 0x54E,										// 0x500[AF5] 0x040[E group] 0x00E[pin 14]

	SPI2_MOSI_B15					= 0x51F,										// 0x500[AF5] 0x010[B group] 0x00F[pin 15]
	SPI2_MOSI_C03					= 0x523,										// 0x500[AF5] 0x020[C group] 0x003[pin  3]
	SPI2_MOSI_E06					= 0x546,										// 0x500[AF5] 0x040[E group] 0x006[pin  6]

	SPI3_MOSI_B05					= 0x615,										// 0x600[AF6] 0x010[B group] 0x005[pin  5]
	SPI3_MOSI_C12					= 0x62C,										// 0x600[AF6] 0x020[C group] 0x00C[pin 12]
	SPI3_MOSI_D06					= 0x536,										// 0x500[AF5] 0x030[D group] 0x006[pin  6]
	SPI3_MOSI_D14					= 0x63E,										// 0x600[AF6] 0x030[D group] 0x00E[pin 14]
}SPI_MOSIPIN_enum;

typedef enum																		// 枚举串口引脚 此枚举定义不允许用户修改
{
	SPI_MISO_NULL					= 0xFFF,										// 占位使用 填入此参数标识不使用 MISO

	SPI1_MISO_A06					= 0x506,										// 0x500[AF5] 0x000[A group] 0x006[pin  6]
	SPI1_MISO_B04					= 0x514,										// 0x500[AF5] 0x010[B group] 0x004[pin  4]
	SPI1_MISO_E13					= 0x54D,										// 0x500[AF5] 0x040[E group] 0x00D[pin 13]

	SPI2_MISO_B14					= 0x51E,										// 0x500[AF5] 0x010[B group] 0x00E[pin 14]
	SPI2_MISO_C02					= 0x522,										// 0x500[AF5] 0x020[C group] 0x002[pin  2]
	SPI2_MISO_C06					= 0x526,										// 0x500[AF5] 0x020[C group] 0x006[pin  6]
	SPI2_MISO_E05					= 0x545,										// 0x500[AF5] 0x040[E group] 0x005[pin  5]

	SPI3_MISO_B04					= 0x614,										// 0x600[AF6] 0x010[B group] 0x004[pin  4]
	SPI3_MISO_C07					= 0x527,										// 0x500[AF5] 0x020[C group] 0x007[pin  7]
	SPI3_MISO_C11					= 0x62B,										// 0x600[AF6] 0x020[C group] 0x00B[pin 11]
	SPI3_MISO_D05					= 0x535,										// 0x500[AF5] 0x030[D group] 0x005[pin  5]
	SPI3_MISO_D13					= 0x63D,										// 0x600[AF6] 0x030[D group] 0x00D[pin 13]
}SPI_MISOPIN_enum;

typedef enum																		// 枚举串口引脚 此枚举定义不允许用户修改
{
	SPI_NSS_NULL					= 0xFFF,										// 占位使用 填入此参数标识不使用 NSS

	SPI1_NSS_A04					= 0x504,										// 0x500[AF5] 0x000[A group] 0x004[pin  4]
	SPI1_NSS_A15					= 0x50F,										// 0x500[AF5] 0x000[A group] 0x00F[pin 15]
	SPI1_NSS_E11					= 0x54B,										// 0x500[AF5] 0x040[E group] 0x00B[pin 11]

	SPI2_NSS_B09					= 0x519,										// 0x500[AF5] 0x010[B group] 0x009[pin  9]
	SPI2_NSS_B12					= 0x51C,										// 0x500[AF5] 0x010[B group] 0x00C[pin 12]
	SPI2_NSS_E03					= 0x543,										// 0x500[AF5] 0x040[E group] 0x003[pin  3]
	SPI2_NSS_E04					= 0x544,										// 0x500[AF5] 0x040[E group] 0x004[pin  4]

	SPI3_NSS_A04					= 0x604,										// 0x600[AF6] 0x000[A group] 0x004[pin  4]
	SPI3_NSS_A15					= 0x60F,										// 0x600[AF6] 0x000[A group] 0x00F[pin 15]
	SPI3_NSS_D07					= 0x537,										// 0x500[AF5] 0x030[D group] 0x007[pin  7]
	SPI3_NSS_D15					= 0x63F,										// 0x600[AF6] 0x030[D group] 0x00F[pin 15]
}SPI_NSSPIN_enum;

extern SPI_TypeDef *spi_index[3];

void	spi_init		(SPIN_enum spi_n, SPI_SCKPIN_enum sck_pin, SPI_MOSIPIN_enum mosi_pin, SPI_MISOPIN_enum miso_pin, SPI_NSSPIN_enum cs_pin, uint8 mode, uint32 baud);
void	spi_mosi		(SPIN_enum spi_n, uint8 *modata, uint8 *midata, uint32 len);

#endif
