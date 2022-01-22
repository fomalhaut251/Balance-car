#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

extern char txt[32];
//————————————————全局变量————————————————
#define _BEE_ON             GPIO_SetBits(GPIOD,GPIO_Pin_12);
#define _BEE_OFF            GPIO_ResetBits(GPIOD,GPIO_Pin_12);
#define open                1
#define close               0
#define LED1				B13
#define LED2				H2
#define KEY					D0
#define BUZZER_PIN			D12	
#define PWM_CH1				TIM_2_CH1_A00
#define PWM_CH2				TIM_2_CH2_A01
#define PWM_CH3				TIM_2_CH3_A02
#define PWM_CH4				TIM_2_CH4_A03
#define PWM_TIM				TIM_5
#define PWM_L_CH1			TIM_5_CH1_A00
#define PWM_L_CH2			TIM_5_CH2_A01
#define PWM_R_CH1			TIM_5_CH3_A02
#define PWM_R_CH2			TIM_5_CH4_A03
//各种情况阈值
extern int Threshold_derailment;//出轨行为的阈值
extern int Threshold_branch;//出轨行为的阈值

//各种情况标志位
extern int8_t Flag_derailment;
extern int8_t Flag_branch;           //判断是否在三岔路
extern int8_t Flag_branch_number;    //进入三岔路次数
extern int8_t Flag_circle;        //判断是否在环岛
extern int8_t Flag_circle_number;
extern int8_t Flag_dirPID;
extern int8_t Flag_turn;

//速度环相关
extern int  setSpeed;               //设定速度
extern int gear[4]; //挡位
extern int  actualSpeed;            //速度环PI计算参数
extern int  speedControlOutNew;
extern int  speedControlOutOld;
extern float  speedIntegral;
extern int  speedControlOut;
extern int  speedControlPeriod;
extern int  speed_Left,speed_Right;  //编码器脉冲捕捉计数,等效为速度
extern int  err_speed;

//方向环相关
extern uint16 Vol_left1_actual;        //加权计算后左右中电感采集值
extern uint16 Vol_right1_actual;
extern uint16 Vol_left2_actual;        //加权计算后左右中电感采集值
extern uint16 Vol_right2_actual;
extern uint16 Vol_mid_actual;
extern double BiasIndActual;
extern double BiasIndActual_last,dvar;      //方向环PD计算参数
extern int dirControlOut; //方向环占空比
extern int dirOut;
extern float w;

//
extern float  acc_x,acc_z;                  //加速度传感器简单换算后数据
extern float  gyro_y,gyro_z,angle_z;;                       //陀螺仪简单换算后数据
extern float  angle_Filtering;
extern float  angle_acc;                 //重力加速度反三角函数计算得出倾角
extern float  angle_gyro;                //角速度积分计算得出倾角
extern float  err_angle;                 //最终角度
extern float  anglespeed;                //角速度
extern float  dtt;              //MPU6050原始数据采样时间
extern float  K1;               //对加速度计取值的权重
extern short  mpu_acc_x,mpu_acc_y,mpu_acc_z;
extern short  mpu_gyro_x,mpu_gyro_y,mpu_gyro_z;
extern float  angleControlOut;

extern int duty_Left;

extern int Encoder_accumulate;
extern long long Encoder_total;
extern int time_accumulate;
extern char signal;
extern long long lap_length;
/****************函数声明***************/
void ComplementaryFiltering();
void ComplementaryFiltering2();
void SpeedControl();
void OutputSpeedControl();
void directionControl();
void run();
void DetermineDirection();
void Branch();
void leftCircle();




#endif 
