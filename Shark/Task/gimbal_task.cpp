#include "cmsis_os.h"
#include "gimbal.hpp"
#include "can.h"
#include "transporter_can.hpp"
#include "main.h"
#include "cascade_pid.hpp"
#include "general_pid.hpp"
#include "motor.hpp"

////实例化对象
communication::TransporterCan can1(hcan1, communication::FIFO0_E);
communication::TransporterCan can2(hcan2, communication::FIFO0_E);

controller::CascadePID yaw_pos(100,100,50,1000,30000,3000);
controller::GeneralPID yaw_vec(1000,30000,3000,50,controller::Ramp_e);
controller::GeneralPID rammer_vec(1000,10000,2000,50,controller::Ramp_e);

motor::Motor yaw(motor::GM_YAW_E, motor::M6020_E,  motor::CAN1_E, motor::ONE_E, can1, &yaw_pos);
motor::Motor rammer(motor::GM_RAMMER_E, motor::M2006_E,  motor::CAN1_E, motor::ONE_E, can1, &rammer_vec);

extern "C" void StartGimbalTask(void const * argument)
{
    /* USER CODE BEGIN StartGimbalTask */
    ////打开can通信
    if(!can1.isOpen()){
        can1.open();
    }
    if(!can2.isOpen()){
        can2.open();
    }
    ////设置motor的PID

    ////将当前读取到的角度设置为目标角度，这样就不会初始化到一个角度而疯转了(云台不能360转的时候可以这样设置，安全例如pitch/无滑环云台)
    //yaw.ctrl_->setTarget(yaw.encoder_.getEncoder().angelAll,controller::POS_E); //设置目标位置

    yaw.ctrl_->setTarget(360.0*3,controller::POS_E); //设置目标位置
    yaw.ctrl_->setPID(2,0,0,controller::POS_E);   //设置pos环 PID
    yaw.ctrl_->setPID(70,6.5,0,controller::VEC_E); //设置速度环 PID

    //速度环
//    yaw.ctrl_->setPID(70,6.5,0); //设置PID
//    yaw.ctrl_->setTarget(100);  //设置目标
    rammer.ctrl_->setPID(9.15,0.25,0.0);
    rammer.ctrl_->setTarget(100);


    /* Infinite loop */
    for(;;)
    {
        ////控制循环并发送电流
        yaw.ctrl_->ctrlLoop();
        rammer.ctrl_->ctrlLoop();

        //10.发送电流
        motor::Motor::sendAllMotorCur();
        osDelay(1);
    }
    /* USER CODE END StartGimbalTask */
}

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    ////读取底层can通信的信息
    can1.read();
    can2.read();
    ////循环进行电机编码
    yaw.encoder_.encoderSloveLoop();
    rammer.encoder_.encoderSloveLoop();
    //gimbal.encoderLoop();
}

