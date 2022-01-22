#include "headfile.h"
///////////////////////////////////////////////////////////////////////////////////////
//本文件放模块化小函数

void integralAngle()           //积分得出转向角度
{
    if(Flag_turn == true)
    {
        gyro_z = mpu_gyro_z / 16.4;
        angle_z += gyro_z * dtt;
    }
}

/*************************************************************************
* 功能说明: 转向一定角度
* 参数说明:angleTurn  : 转向角度    输入给正
*          dir        ：转向方向   1左转 -1右转 (禁止开着左转灯右转bushi)
*          turn_V     :转向速度,有没有必要还在实验中
* 函数返回：无
*************************************************************************/
void turnAngle(int angleTurn,int dir,int turn_V)
{   
    _BEE_ON
    Flag_turn = open;
    Flag_dirPID = close;
    angle_z = 0;
    switch (dir)
    {
    case 1:
        while (angle_z < angleTurn)
        {
            dirOut = turn_V;
            run();
        }
        break;
    case -1:
        while (angle_z > -angleTurn)
        {
            dirOut = -turn_V;
            run();
        }
        break;
    default:
        break;
    }
    dirOut = 0;
    Flag_turn = close;
    Flag_dirPID = open;
    _BEE_OFF
}

void turnAngle2(int angleTurn,int dir,int KP)
{   
    _BEE_ON
    Flag_turn = open;
    Flag_dirPID = close;
    angle_z = 0;
    Encoder_accumulate = 0;
    switch (dir)
    {
    case 1:
        while (Encoder_accumulate < 30000)
        {
            dirOut = KP * (angleTurn - angle_z);
            run();
        }
        break;
    case -1:
        while (Encoder_accumulate < 30000)
        {
            dirOut = -KP * (angleTurn + angle_z);
            run();
        }
        break;
    default:
        break;
    }
    dirOut = 0;
    Flag_turn = close;
    Flag_dirPID = open;
    _BEE_OFF
}

/*****************************************电机控制(H桥)*******************************************/
void MotorCtrl(int16_t motor1, int16_t motor2)
{
    if (motor1 > 0)
    {
        pwm_duty_updata(PWM_TIM, PWM_L_CH1, motor1);        // 计算占空比
        pwm_duty_updata(PWM_TIM, PWM_L_CH2, 0);                             // 同一时间 一个电机只能输出一个PWM 另一通道保持低电平
    }
    else
    {
        pwm_duty_updata(PWM_TIM, PWM_L_CH1, 0);									// 同一时间 一个电机只能输出一个PWM 另一通道保持低电平
		pwm_duty_updata(PWM_TIM, PWM_L_CH2, -motor1);		// 计算占空比
    }

    if (motor2 > 0)
    {
        pwm_duty_updata(PWM_TIM, PWM_R_CH1, motor2);		// 计算占空比
		pwm_duty_updata(PWM_TIM, PWM_R_CH2, 0);									// 同一时间 一个电机只能输出一个PWM 另一通道保持低电平
    }
    else
    {
        pwm_duty_updata(PWM_TIM, PWM_R_CH1, 0);									// 同一时间 一个电机只能输出一个PWM 另一通道保持低电平
		pwm_duty_updata(PWM_TIM, PWM_R_CH2, -motor2);		// 计算占空比
    }
}

/*****************************************等待冲♂击*******************************************/
void rushB()
{
    setSpeed = gear[0];
    while (Encoder_accumulate < 980)        //受到冲击开始rushB
    {
        speed_Left = tim_encoder_get_count(TIM_3);              //左电机 注意插槽位置！
	    tim_counter_rst (TIM_3);
	    speed_Right = -tim_encoder_get_count(TIM_4);            //右电机 
	    tim_counter_rst (TIM_4);
        Encoder_accumulate = Encoder_accumulate + speed_Left + speed_Right;
        systick_delay_ms(50);
    }
    uart_putstr(UART_4,"x");                //X表示停车，O表示开跑(小写字母)
    uart_putstr(UART_4,"x");
    uart_putstr(UART_4,"x");
    setSpeed = 20;
}

/*****************************************让三轮开跑*******************************************/
void rushA()
{
    uart_putstr(UART_4,"o"); //X表示停车，O表示开跑
    uart_putstr(UART_4,"o");
    uart_putstr(UART_4,"o");
    oled_p6x8str(0, 0, "STOP!!!!");    
    while (1)
    {
        MotorCtrl(0, 0);
    }
}

/*****************************************发车前2m*******************************************/
void first2M()
{
    _BEE_ON
    Encoder_accumulate = 0;
    setSpeed = 150;
    while (Encoder_accumulate < 9600*15)
    {
        run();
    }
    setSpeed = gear[2];
    Flag_branch = false;
    Flag_circle = 0;
    _BEE_OFF
}

/*************************************************************************
 * 功能说明: 开局确定方向
 * 参数说明: 无
 * 函数返回：无
 * 有毒吧,要写好多,懒得写了
*************************************************************************/
void keyInput(void)
{
    //写菜单太麻烦,懒得写了
}

