/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file       		SEEKFREE_FUN.c
* @company	   		成都逐飞科技有限公司
* @author     		逐飞科技(QQ3184284598)
* @version    		查看doc内version文件 版本说明
* @Software			IAR 8.32.4 or MDK 5.28
* @Target core		MM32F3277
* @Taobao   		https://seekfree.taobao.com/
* @date       		2019-04-30
********************************************************************************************************************/

#include "SEEKFREE_FUN.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief		绝对值函数 数据范围是 [-32767,32767]
// @param		dat				需要求绝对值的数
// @return		int				返回绝对值
// Sample usage:				dat = myabs(dat);						// 将dat变成正数
//-------------------------------------------------------------------------------------------------------------------
//int  myabs (int dat)													// 定义查看 SEEKFREE_FUN.h 

//-------------------------------------------------------------------------------------------------------------------
// @brief		限幅 数据范围是 [-32768,32767]
// @param		x				被限幅的数据
// @param		y				限幅范围(数据会被限制在-y至+y之间)
// @return		int				限幅之后的数据			
// Sample usage:				int dat = limit(500,300);				//数据被限制在-300至+300之间  因此返回的结果是300
//-------------------------------------------------------------------------------------------------------------------
//int limit(int x, int32 y)												// 定义查看 SEEKFREE_FUN.h 

//-------------------------------------------------------------------------------------------------------------------
// @brief		双边限幅 数据范围是 [-32768,32767]
// @param		x				被限幅的数据
// @param		a				限幅范围左边界
// @param		b				限幅范围右边界
// @return		int				限幅之后的数据			
// Sample usage:				int dat = limit_ab(500,-300,400);		//数据被限制在-300至+400之间  因此返回的结果是400
//-------------------------------------------------------------------------------------------------------------------
//int limit_ab(int x, int a, int b)										// 定义查看 SEEKFREE_FUN.h 

//-------------------------------------------------------------------------------------------------------------------
// @brief		软件延时
// @param		t				延时时间
// @return		void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void my_delay(volatile long t)
{
    while(t--);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		字符串转整形数字 数据范围是 [-32768,32767]
// @param		str				传入字符串 可带符号
// @return		int				转换后的数据			
// Sample usage:				int dat = str_to_int("-100");			// 结果输出 dat = -100
//-------------------------------------------------------------------------------------------------------------------
int str_to_int (char *str)
{
	uint8 sign = 0;
	int temp;

	if(*str == '-')
	{
		sign = 1;
		str ++;
	}
	else if(*str == '+')
	{
		str ++;
	}

	while((*str >= '0')&&(*str <= '9'))
	{
		temp = temp*10+((uint8)(*str)-0x30);
		str ++;
	}

	if(sign)
		return -temp;
	return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		整形数字转字符串 数据范围是 [-32768,32767]
// @param		*str			字符串指针
// @param		number			传入的数据
// @return		NULL
// Sample usage:				int_to_str(data_buffer, -300);			// 结果输出 data_buffer = "-300"
//-------------------------------------------------------------------------------------------------------------------
void int_to_str (char *str, int number)
{
	int data_temp[5];
	uint8 bit = 0;

	if(number < 0)
	{
		*str++ = '-';
	}
	else if(number == 0)
	{
		*str = '0';
		return;
	}

	// int [-32768,32767]
	while(number != 0)
	{
		data_temp[bit++] = number%10;
		number /= 10;
	}
	while(bit != 0)
	{
		*str++ = (myabs(data_temp[bit-1])+0x30);
		bit--;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		字符串转浮点数
// @param		str				传入字符串 可带符号
// @return		float			转换后的数据			
// Sample usage:				float dat = str_to_int("-100.2");		// 结果输出 dat = -100.2
//-------------------------------------------------------------------------------------------------------------------
float str_to_float (char *str)
{
	uint8 sign = 0;
	float temp = 0.0;
	float temp_point = 0.0;
	uint8 point_bit = 0;

	if(*str == '-')
	{
		sign = 1;
		str ++;
	}
	else if(*str == '+')
	{
		str ++;
	}

	while((*str >= '0')&&(*str <= '9'))
	{
		temp = temp*10+((uint8)(*str)-0x30);
		str ++;
	}
	if(*str == '.')
	{
		str++;
		while((*str >= '0')&&(*str <= '9'))
		{
			temp_point = temp_point*10+((uint8)(*str)-0x30);
			point_bit++;
			str ++;
		}
		while(point_bit--)
		{
			temp_point /= 10;
		}
	}
	temp += temp_point;

	if(sign)
		return -temp;
	return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		浮点数字转字符串
// @param		*str			字符串指针
// @param		number			传入的数据
// @param		point_bit		小数点精度
// @return		NULL
// Sample usage:				float_to_str(data_buffer, 3.1415, 2);	// 结果输出 data_buffer = "3.14"
//-------------------------------------------------------------------------------------------------------------------
void float_to_str (char *str, float number, uint8 point_bit)
{
	int data_int = 0;
	int data_float = 0.0;
	int data_temp[5];
	int data_temp_point[5];
	uint8 bit = 0;

	data_int = (int)number;
	if(number < 0)
	{
		*str++ = '-';
	}
	else if(number == 0.0)
	{
		*str = '0';
		return;
	}

	number = number - data_int;
	while(point_bit--)
	{
		number = number*10;
	}
	data_float = (int)number;

	while(data_int != 0)
	{
		data_temp[bit++] = data_int%10;
		data_int /= 10;
	}
	while(bit != 0)
	{
		*str++ = (myabs(data_temp[bit-1])+0x30);
		bit--;
	}
	*str++ = '.';
	if(data_float == 0)
		*str = '0';
	else
	{
		while(data_float != 0)
		{
			data_temp_point[bit++] = data_float%10;
			data_float /= 10;
		}
		while(bit != 0)
		{
			*str++ = (myabs(data_temp_point[bit-1])+0x30);
			bit--;
		}
	}
}


