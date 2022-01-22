/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				zf_fsmc.c
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.32.4 or MDK 5.28
* @Target core		MM32F3277
* @Taobao			https://seekfree.taobao.com/
* @date				2021-02-22
********************************************************************************************************************/

#include "zf_fsmc.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		摄像头DMA初始化
// @return		void
// Sample usage:					fsmc_init();
//-------------------------------------------------------------------------------------------------------------------
void fsmc_init (void)
{
	FSMC_InitTypeDef  FSMC_InitStructure;
	FSMC_NORSRAM_Bank_InitTypeDef  FSMC_BankInitStructure;

	FSMC_NORSRAM_BankStructInit(&FSMC_BankInitStructure);
	FSMC_NORSRAMStructInit(&FSMC_InitStructure);

	RCC_AHB3PeriphClockCmd(RCC_AHB3ENR_FSMC, ENABLE);

    FSMC_BankInitStructure.FSMC_SMReadPipe    = 0;							// 锁存读数据周期
    FSMC_BankInitStructure.FSMC_ReadyMode     = 0;							// 应答信号来自 0-内部FSMC 1-外部Device
    FSMC_BankInitStructure.FSMC_WritePeriod   = 0x2;						// 写入周期
    FSMC_BankInitStructure.FSMC_WriteHoldTime = 1;							// 写入的保持时间
    FSMC_BankInitStructure.FSMC_AddrSetTime   = 3;							// 地址的设置时间
    FSMC_BankInitStructure.FSMC_ReadPeriod    = 0x1;						// 读取周期
    FSMC_BankInitStructure.FSMC_DataWidth     = FSMC_DataWidth_16bits;		// 数据位宽
    FSMC_NORSRAM_Bank_Init(&FSMC_BankInitStructure, FSMC_NORSRAM_BANK0);

    FSMC_InitStructure.FSMC_Mode = FSMC_Mode_NorFlash;						// 设置存储器工作协议模式
    FSMC_InitStructure.FSMC_TimingRegSelect = FSMC_TimingRegSelect_0;		// 时序参数配置寄存器组
    FSMC_InitStructure.FSMC_MemSize = FSMC_MemSize_64MB;					// 存储器大小
    FSMC_InitStructure.FSMC_MemType = FSMC_MemType_NorSRAM;					// 存储器类型
    FSMC_InitStructure.FSMC_AddrDataMode = FSMC_AddrDataMUX;				// 工作兼容模式
    FSMC_NORSRAMInit(&FSMC_InitStructure);
}
