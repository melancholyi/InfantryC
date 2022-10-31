#include "cmsis_os.h"
#include "gimbal.hpp"
#include "can.h"
#include "transporter_can.hpp"
#include "main.h"
#include "can.h"
#include "cascade_pid.hpp"
#include "general_pid.hpp"
#include "motor.hpp"
////实例化对象
communication::TransporterCan can1(hcan1, communication::FIFO0_E);
communication::TransporterCan can2(hcan2, communication::FIFO0_E);

controller::CascadePID pos1(100,100,50,1000,30000,3000);
motor::Motor yaw(motor::GM_YAW_E, motor::M6020_E,  motor::CAN1_E, motor::ONE_E, can1, &pos1);

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
    ////将当前读取到的角度设置为目标角度，这样就不会疯转了
    yaw.ctrl_->setTarget(yaw.encoder_.getEncoder().angelAll,controller::POS_E); //设置目标位置
    yaw.ctrl_->setPID(2,0,0,controller::POS_E);   //设置pos环 PID
    yaw.ctrl_->setPID(70,6.5,0,controller::VEC_E); //设置速度环 PID
    /* Infinite loop */
    for(;;)
    {
        ////控制循环并发送电流
        yaw.ctrl_->ctrlLoop();
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
    //gimbal.encoderLoop();
}

