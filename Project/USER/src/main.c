/********************************************************************************
 * 致谢:真诚感谢李子昊,刘希文,曾繁钦,高集殊,季炜,雷超等学长学姐对我们提供的支持与帮助.
 * 		感谢一起交流的外校同学,尤其是中国矿业大学徐海学院的同学,在交流中学习到了很多.
 * 		感谢往届的车友前辈,许多时候还是靠祖传方案才能快乐逮虾户.
 * 
 * 说明:作者水平有限,采用了上古时期多个PID结果直接相加的方案.
 * 		程序的变量名都比较长,但也便于理解,注释较全,希望能帮到学弟们.
 * 		仅限初学者把直立车立起来走两步,元素处理也就图一乐,后续可能更新串级PID方案.
 * 		欢迎技术交流讨论.
 * 
 * 版权信息:
 *	中国矿业大学 机电工程学院 不显山车神队
 *  马梦雅,刘宇鑫,张书晨,陈振尧.
 *  (C)Copyright CUTM, 2021. 
 *  ALL RIGHTS RESERVED.
********************************************************************************/

#include "headfile.h"

int main(void)
{
	//初始化开始
	board_init(true);												// 初始化 debug 输出串口
	gpio_init(KEY, GPI, GPIO_HIGH, GPI_PULL_UP);					// 初始化引脚为上拉输入 默认高电平
	gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);					// 初始化引脚为推挽输出 默认高电平
	gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);					// 初始化引脚为推挽输出 默认高电平
    gpio_init(BUZZER_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
	exti_interrupt_init(A15, EXTI_Trigger_Falling, 0x01);			//干簧管下降沿触发
	oled_init();
    oled_p6x8str(0, 0, "CUMT");
    icm20602_init_spi();                                            //陀螺仪初始化
    tim_encoder_init(TIM_3, TIM_3_ENC1_B04, TIM_3_ENC2_B05);		//编码器初始化
    tim_encoder_init(TIM_4, TIM_4_ENC1_B06, TIM_4_ENC2_B07);
	adc_init(ADC_1, ADC1_CH10_C00, ADC_12BIT);						//ADC初始化
	adc_init(ADC_1, ADC1_CH11_C01, ADC_12BIT);
	adc_init(ADC_1, ADC1_CH12_C02, ADC_12BIT);
	adc_init(ADC_1, ADC1_CH13_C03, ADC_12BIT);
	adc_init(ADC_1, ADC1_CH04_A04, ADC_12BIT);
    gpio_init(RTS_PIN, GPI, GPIO_HIGH, GPI_PULL_UP);						// 初始化流控引脚
	uart_init (WIRELESS_UART, 9600, WIRELESS_UART_TX, WIRELESS_UART_RX);	// 初始化串口
	uart_rx_irq(WIRELESS_UART, ENABLE);										//串口接收中断
	pwm_init(PWM_TIM, PWM_L_CH1, 16000, 0);		//PWM 通道1 初始化频率 占空比初始为0
	pwm_init(PWM_TIM, PWM_L_CH2, 16000, 0);									
	pwm_init(PWM_TIM, PWM_R_CH1, 16000, 0);										
	pwm_init(PWM_TIM, PWM_R_CH2, 16000, 0);							
	tim_interrupt_init_ms(TIM_2, 1000, 2);		//定时用		
	tim_interrupt_init_ms(TIM_8, 1, 0);			//直立环中断	
	tim_interrupt_init_ms(TIM_6, 10, 1);		//速度环中断	
	tim_interrupt_init_ms(TIM_7, 5, 1);			//方向环中断
    _BEE_OFF		
	//初始化结束
	systick_delay_ms(1000);
    rushB();									//Time bomb has planted in B side
    first2M();									//前两米慢点走,出三岔路不翻车
	while(1)
	{
//        oled_printf_int32(0,2,Vol_left2_actual ,5);
//        oled_printf_int32(20,3,Vol_left1_actual ,5); 
//        oled_printf_int32(50,4,Vol_mid_actual ,5);
//        oled_printf_int32(70,3,Vol_right1_actual ,5);  
//        oled_printf_int32(90,2,Vol_right2_actual ,5); 
//        oled_printf_float(0,7,angle_Filtering,2,3);
//        oled_printf_int32(6,2,speedControlOut, 5); 
//        oled_printf_int32(6,5,dirControlOut, 5);  
//        oled_printf_int32(0,6,actualSpeed, 5);
//        oled_printf_int32(6,7,Encoder_total/93000, 3);
        run();
        Branch();
		fixedCircle();
	}
}
//TIM1 摄像头
//TIM3 编码器
//TIM4 编码器
//TIM5 电机
	
//oled_printf_float(0,0,x,2,3);			显示浮点数例程
//oled_printf_int32(0,0,x,5);			显示整数例程
//uart_putstr(UART_4,"i lvoe you"); 	发送字符串
//uart_putbuff(UART_4,&a[0],5);			发送数组
//seekfree_sendimg_03x(WIRELESS_UART, mt9v03x_image[0], MT9V03X_W, MT9V03X_H);

