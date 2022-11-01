#include "cmsis_os.h"
#include "gimbal.hpp"
#include "can.h"
#include "transporter_can.hpp"
#include "main.h"
#include "cascade_pid.hpp"
#include "general_pid.hpp"
#include "motor.hpp"

////ʵ��������
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
    ////��canͨ��
    if(!can1.isOpen()){
        can1.open();
    }
    if(!can2.isOpen()){
        can2.open();
    }
    ////����motor��PID

    ////����ǰ��ȡ���ĽǶ�����ΪĿ��Ƕȣ������Ͳ����ʼ����һ���Ƕȶ���ת��(��̨����360ת��ʱ������������ã���ȫ����pitch/�޻�����̨)
    //yaw.ctrl_->setTarget(yaw.encoder_.getEncoder().angelAll,controller::POS_E); //����Ŀ��λ��

    yaw.ctrl_->setTarget(360.0*3,controller::POS_E); //����Ŀ��λ��
    yaw.ctrl_->setPID(2,0,0,controller::POS_E);   //����pos�� PID
    yaw.ctrl_->setPID(70,6.5,0,controller::VEC_E); //�����ٶȻ� PID

    //�ٶȻ�
//    yaw.ctrl_->setPID(70,6.5,0); //����PID
//    yaw.ctrl_->setTarget(100);  //����Ŀ��
    rammer.ctrl_->setPID(9.15,0.25,0.0);
    rammer.ctrl_->setTarget(100);


    /* Infinite loop */
    for(;;)
    {
        ////����ѭ�������͵���
        yaw.ctrl_->ctrlLoop();
        rammer.ctrl_->ctrlLoop();

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
    rammer.encoder_.encoderSloveLoop();
    //gimbal.encoderLoop();
}

