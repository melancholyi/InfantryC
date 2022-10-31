#include "cmsis_os.h"
#include "gimbal.hpp"
#include "can.h"
#include "transporter_can.hpp"
#include "main.h"
#include "can.h"
#include "cascade_pid.hpp"
#include "general_pid.hpp"
#include "motor.hpp"
////ʵ��������
communication::TransporterCan can1(hcan1, communication::FIFO0_E);
communication::TransporterCan can2(hcan2, communication::FIFO0_E);

controller::CascadePID pos1(100,100,50,1000,30000,3000);
motor::Motor yaw(motor::GM_YAW_E, motor::M6020_E,  motor::CAN1_E, motor::ONE_E, can1, &pos1);

extern "C" void StartGimbalTask(void const * argument)
{
    /* USER CODE BEGIN StartGimbalTask */
    ////��canͨ��
    if(!can1.isOpen()){
        can1.open();
    }
    if(!can2.isOpen()){
        can2.open();
    }
    ////����motor��PID
    ////����ǰ��ȡ���ĽǶ�����ΪĿ��Ƕȣ������Ͳ����ת��
    yaw.ctrl_->setTarget(yaw.encoder_.getEncoder().angelAll,controller::POS_E); //����Ŀ��λ��
    yaw.ctrl_->setPID(2,0,0,controller::POS_E);   //����pos�� PID
    yaw.ctrl_->setPID(70,6.5,0,controller::VEC_E); //�����ٶȻ� PID
    /* Infinite loop */
    for(;;)
    {
        ////����ѭ�������͵���
        yaw.ctrl_->ctrlLoop();
        //10.���͵���
        motor::Motor::sendAllMotorCur();
        osDelay(1);
    }
    /* USER CODE END StartGimbalTask */
}

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    ////��ȡ�ײ�canͨ�ŵ���Ϣ
    can1.read();
    can2.read();
    ////ѭ�����е������
    yaw.encoder_.encoderSloveLoop();
    //gimbal.encoderLoop();
}

