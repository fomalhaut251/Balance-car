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

#ifndef _SEEKFREE_FUN_h
#define _SEEKFREE_FUN_h

#include "common.h"

#define myabs(x)			((x>=0)?(x):(-x))
#define limit(x,y)			((x>y)?(y):((x<-y)?(-y):(x)))
#define limit_ab(x,a,b)		((x<a)?(a):((x>b)?(b):(x)))

void  my_delay(volatile long t);
int str_to_int (char *str);
void int_to_str (char *str, int number);
float str_to_float (char *str);
void float_to_str (char *str, float number, uint8 point_bit);

#endif
