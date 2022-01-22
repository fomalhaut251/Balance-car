#include "headfile.h"
//场上更改

long long lap_length = 93000*20;        //赛道长度

//PID                                 old       400       500
float P_angle = 700;     //直立环KP   400       700       600
float D_angle = 30;      //直立环KD   30        30        30
float P_direction = 1; //方向环KP      3        1         2
float D_direction = 20;  //方向环KD    10       20        30
float P_speed = 30;       //速度环KP   8        30         6
float I_speed = 3;     //速度环KI   0.2         3      0.15

//各种情况阈值
int Threshold_derailment = 500; //出轨行为的阈值
int Threshold_branch = 2000;    //三岔路的阈值
int Threshold_circle = 5000;    //circle的阈值

//各种情况标志位
int8_t Flag_derailment = 0;     
int8_t Flag_branch = 0;        //判断是否在三岔路 0不在 1在
int8_t Flag_branch_number = 0; //进入三岔路次数 用来防止多次误判
int8_t Flag_circle = 0;        //判断是否在环岛
int8_t Flag_circle_number = 1; //遇到的第N次环岛标志位
int8_t Flag_dirPID = 1;        //1为方向环控制，0为屏蔽方向环
int8_t Flag_turn = 0;          //转向用标志位 1开启积分 0屏蔽积分

//角度处理相关
float angleControlOut = 0;
float angle_balance = 61; //平衡位置角度      静止时62.5
float acc_x = 0, acc_z = 0; //加速度传感器简单换算后数据
float gyro_y = 0,gyro_z = 0,angle_z = 0;           //陀螺仪简单换算后数据
float angle_Filtering = 90; //互补滤波得到角度
float angle_acc = 0;        //重力加速度反三角函数计算得出倾角
float angle_gyro = 0;       //角速度积分计算得出倾角
float err_angle = 0;        //最终角度
float anglespeed = 0;       //角速度
float dtt = 0.001;          //MPU9250原始数据采样时间
float K1 = 0.005;           //对加速度计取值的权重
float K2 = 0.2;             //对加速度计取值的权重
float x1=0,x2=0,y1=0;       //运算中间变量

//速度环相关
int setSpeed = 600;       //设定速度
int gear[4] = {0, 200, 400, 500, 600}; //挡位
int actualSpeed = 0;    //速度环PI计算参数
int speedControlOutNew = 0;
int speedControlOutOld = 0;
float speedIntegral = 0;
int speedControlOut = 0;
int speedControlPeriod = 0;
int speed_Left = 0, speed_Right = 0; //编码器脉冲捕捉计数,等效为速度
int err_speed = 0;

//方向环相关
uint16 Vol_left1_actual = 0; //加权计算后左右中电感采集值
uint16 Vol_right1_actual = 0;
uint16 Vol_left2_actual = 0; //加权计算后左右中电感采集值
uint16 Vol_right2_actual = 0;
uint16 Vol_mid_actual = 0;
double BiasIndActual = 0;
double BiasIndActual_last = 0, dvar = 0; //方向环PD计算参数
int dirControlOut = 0; //方向环占空比
int dirOut = 0;
float w = 1;

//占空比相关
int duty_death_left_P = 1000;  //电机输出死区占空比      电机反着用，死区好大。。。
int duty_death_right_P = 1000; //电机输出死区占空比
int duty_death_left_N = -1000;  //电机输出死区占空比
int duty_death_right_N = -1000; //电机输出死区占空比
int duty_Max_P = 6000;         //电机PWM限幅参数   10000
int duty_Max_N = -6000;
int duty_Left = 0;     //叠加得到的左轮占空比
int duty_Right = 0;    //叠加得到的右轮占空比

//其他
int Encoder_accumulate = 0; //延距测量,手动测量10cm = 9200个脉冲，不是特别精确
long long Encoder_total = 0;
int time_accumulate = 0;
char signal = 0;

/*************************************** 互补滤波函数 （包含直立环PD）*********************************************/
/*angle_Filtering = K1 * angle_m+ (1-K1) * (angle_Filtering + gyro_m * dt);
angle_Filtering 是融合后的角度值
angle_acc是加速度测量简单计算得到的角度
angle + angle_gyro * dt是陀螺仪积分得到的角度
dt为采样周期，单位是s，取0.005
K1是滤波器系数，取0.001。
一阶互补滤波也可以看做是加权平均。*/
void ComplementaryFiltering()
{
    if (icm_acc_x >= 0)
        icm_acc_x = -1;                                 //tan90°
    acc_x = - icm_acc_x * 9.8 / 4096.0;                 //转化单位
    acc_z = icm_acc_z * 9.8 / 4096.0;                   //转化单位
    gyro_y = icm_gyro_y / 16.4;                         //转化单位

    anglespeed = - (gyro_y - 0.25);                     //零点偏差
    angle_acc = atan((acc_z) / acc_x) * 57.3;           //加速度得到的角度，可加零点偏差
    
    angle_gyro = angle_Filtering + anglespeed * dtt;    //角速度得到的角度
    angle_Filtering = K1 * angle_acc + (1 - K1) * angle_gyro;
    
    err_angle = angle_Filtering - angle_balance;
    angleControlOut = P_angle * err_angle + D_angle * anglespeed;
}

/*二阶互补滤波(原理是啥?学会了教教我!)*/
void ComplementaryFiltering2()
{
    if (icm_acc_x >= 0)
        icm_acc_x = -1;                             
    acc_x = - icm_acc_x * 9.8 / 4096.0;             //转化单位
    acc_z = icm_acc_z * 9.8 / 4096.0;               //转化单位
    gyro_y = icm_gyro_y / 16.4;                     //转化单位

    anglespeed = - (gyro_y - 0.25);                 //零点偏差
    angle_acc = atan((acc_z) / acc_x) * 57.3;       //加速度得到的角度，可加零点偏差
    
    x1 = (angle_acc - angle_Filtering) * (1 - K2) * (1 - K2);
    y1 = y1 + x1 * dtt;
    x2 = y1 + 2 * (1 - K2) * (angle_acc - angle_Filtering) + anglespeed;
    angle_Filtering = angle_Filtering + x2 * dtt;
    
    err_angle = angle_Filtering - angle_balance;
    angleControlOut = P_angle * err_angle + D_angle * anglespeed;
}

/*****************************************速度闭环控制算法（PI）*******************************************/
void SpeedControl()
{
    actualSpeed = (speed_Left + speed_Right) * 0.5;
    err_speed = setSpeed - actualSpeed;                         //速度偏差
    speedIntegral = speedIntegral + err_speed * I_speed;        //积分

    if(speedIntegral>3000)  speedIntegral=3000;                 //限幅
    if(speedIntegral<-2000)  speedIntegral=-2000;               //限幅

    speedControlOutOld = speedControlOutNew;
    speedControlOutNew = err_speed * P_speed + speedIntegral;
}

void OutputSpeedControl()                           //速度环平滑输出
{
    float fValue;
    fValue = speedControlOutNew - speedControlOutOld;
    speedControlOut = fValue * (speedControlPeriod + 1) / 10.0 + speedControlOutOld;

    if (speedControlOut > 4500)
        speedControlOut = 4500;                     //限幅
    if (speedControlOut < -4500)
        speedControlOut = -4500;                    //限幅
}

/*****************************************方向闭环控制算法(PD)*******************************************/
void directionControl()
{
    dvar = BiasIndActual - BiasIndActual_last;
    dirControlOut = P_direction * BiasIndActual + D_direction * dvar;
    BiasIndActual_last = BiasIndActual;
}

/*****************************************车模运行控制算法*******************************************/
void run()
{
    if (Vol_left1_actual + Vol_right1_actual > Threshold_circle && err_angle < 10 && err_angle > -10)
        dirControlOut *= 0.05;                                  //屏蔽环岛
    if (Flag_dirPID == open)
        dirOut = dirControlOut;                                 //方向环开关
    
    duty_Left = angleControlOut - speedControlOut + dirOut;     //dirOut为正是左转
    duty_Right = angleControlOut - speedControlOut - dirOut;

    if (duty_Left > 0)
        duty_Left = duty_Left + duty_death_left_P;
    if (duty_Left < 0)
        duty_Left = duty_Left + duty_death_left_N;
    if (duty_Right > 0)
        duty_Right = duty_Right + duty_death_left_P;
    if (duty_Right < 0)
        duty_Right = duty_Right + duty_death_left_N;

    if (duty_Left > duty_Max_P)
        duty_Left = duty_Max_P;
    if (duty_Right > duty_Max_P)
        duty_Right = duty_Max_P;
    if (duty_Left < duty_Max_N)
        duty_Left = duty_Max_N;
    if (duty_Right < duty_Max_N)
        duty_Right = duty_Max_N;

    MotorCtrl(-duty_Left, -duty_Right);                         //直立车算是倒着使轮子,所以取负
}


/*****************************************元素标志位判断*********************************/
void DetermineDirection()
{
    //判断三岔路
    if (Vol_left1_actual + Vol_right1_actual < Threshold_branch 
        && Vol_left2_actual > Vol_left1_actual 
        && Vol_right2_actual > Vol_right1_actual)
    {
        Flag_branch = 1;
    }
    //环岛
//    else if (Vol_mid_actual > Threshold_circle && err_angle < 10 && err_angle > -10)
//    {
//        if (Vol_left2_actual > Vol_right2_actual + 500) //左环岛
//        {
//            Flag_circle = 1;
//        }
//        else if (Vol_right2_actual > Vol_left2_actual + 500) //右环岛
//        {
//            Flag_circle = 2;
//        }      
//        
//    }
    else if (Vol_left1_actual + Vol_right1_actual > Threshold_circle
            && err_angle < 5 && err_angle > -5)
    {
        Flag_circle = 3;
    }
    //判断是否出轨
    else if ((Vol_left1_actual < Threshold_derailment) && (Vol_right1_actual < Threshold_derailment))
    {
        while(1)
            MotorCtrl(0,0);    //收手吧阿祖，碳杆都被撞坏了
    }
}

/*****************************************三岔路*******************************************/
void Branch()
{
    if (Flag_branch == true)
    {
        _BEE_ON

//        turnAngle(20,1,800);              //入三岔路planC        
//        //turnAngle2(30,1,60);
//        P_direction *= 1.5;

        Flag_dirPID = close;
        for (int i = 0;i<2000;i++)         //入岔路planB,这三种方案都差不多,不好使...
        {
            run();
            dirOut = 3000;
        }
        P_direction *= 8;
        D_direction *= 7;
        w = 1;
        Flag_dirPID = open;

        
//        Encoder_accumulate = 0;           //清空计数,入岔路planA
//        while (Encoder_accumulate < 9600) //走5cm
//        {
//            Encoder_accumulate = Encoder_accumulate + Read_Encoder(1)  -Read_Encoder(2);
//            dirOut += 15; //危,转起来了，是你先开的
//            if(dirOut > 1000)
//            {
//                dirOut = 1000;
//            }
//            run();
//        }
        
        setSpeed = gear[1];
        Encoder_accumulate = 0;             //清空计数
        while (Encoder_accumulate < 150000) //走1m+
        {
            
            run();
        }
        _BEE_OFF
        rushA();
    }
}

///*****************************************左环岛*******************************************/
//void leftCircle()
//{
//    if (Flag_circle == 1 && Flag_circle_number <= 4) //防止误判
//    {
//        switch (Flag_circle_number % 2)
//        {
//        case 1:
//            _BEE_ON
//            Flag_dirPID = close;   //接过方向环控制
//            for(int i=0;i<5000;i++)            
//            {
//                dirOut = 800;
//                run();
//            }
//            Flag_dirPID = open;    //方向环控制
//            Flag_circle = 0;
//            Flag_branch = false;;   //防止误判三岔路
//            Flag_circle_number++;
//            _BEE_OFF
//            break;
//        case 2:
//            Encoder_accumulate = 0;
//            while (Encoder_accumulate < 40000)
//            {
//                dirOut *= 0; //屏蔽环岛
//                run();
//            }
//            Flag_circle_number++;
//            Flag_circle = 0;

//        default:
//            break;
//        }
//    }
//}

///*****************************************右环岛*******************************************/
//void rightCircle()
//{
//    if (Flag_circle == 2 && Flag_circle_number <= 4) //防止误判
//    {
//        switch (Flag_circle_number % 2)
//        {
//        case 1:
//            _BEE_ON
//            Flag_dirPID = close;   //接过方向环控制
//            for(int i=0;i<5000;i++)            
//            {
//                dirOut = -800;
//                run();
//            }
//            Flag_dirPID = open;    //方向环控制
//            Flag_circle = 0;
//            Flag_branch = false;   //防止误判三岔路
//            Flag_circle_number++;
//            _BEE_OFF
//            break;
//        case 2:
//            Encoder_accumulate = 0;
//            while (Encoder_accumulate < 40000)
//            {
//                dirOut *= 0; //屏蔽环岛
//                run();
//            }
//            Flag_circle_number++;
//            Flag_circle = 0;
//            Flag_branch = false;

//        default:
//            break;
//        }
//    }
//}

/*****************************************固定环岛*******************************************/
/*环岛最终也没写好,所以就用了个固定环岛的方案,提前输入环岛顺序,不判断左右*/
void fixedCircle()
{
    if (Flag_circle == 3 && Flag_circle_number <= 4) //防止误判
    {
        switch (Flag_circle_number % 2)
        {
        case 1:
            _BEE_ON
            Flag_dirPID = close;        //接过方向环控制
            for(int i=0;i<10000;i++)            
            {
                dirOut = -800;
                run();
            }
            Flag_dirPID = open;         //方向环控制
            Flag_circle = 0;
            Flag_branch = false;;       //防止误判三岔路
            Flag_circle_number++;
            _BEE_OFF
            break;
        case 2:
            Encoder_accumulate = 0;
            while (Encoder_accumulate < 40000)
            {
                dirOut *= 0.1;          //屏蔽环岛
                run();
            }
            Flag_circle_number++;
            Flag_circle = 0;
            Flag_branch = false;
        default:
            break;
        }
    }
}


