#include "usart.h"
#include "cmsis_os.h"
#include "gimbal.hpp"
#include "motor.hpp"

extern motor::Motor rammer;
extern motor::Motor yaw;
extern "C" void StartDebugTask(void const * argument)
{
    osDelay(500);
    uint16_t count = 0;
    /* USER CODE BEGIN StartDebugTask */
    motor::eMotorApp type = motor::GM_PITCH_E;

    for(;;){
        count++;
        if(count%500 == 0){
            LEDR_TOGGLE;
            count = 0;
        }
        else if(count % 10 == 0){
            ////debug
            ////��ӡ���������Ϣ
//            printf("A:%.2f,%.2f,%d\n",
//                   rammer.encoder_.getEncoder().angelAll,
//                   rammer.encoder_.getEncoder().speed,
//                   rammer.encoder_.getEncoder().torque
//            );

            ////��ӡ���������Ϣ
            //���Ը����Լ�������дһ��debug����������Ϊ������ͣ������ĸ�������;ʹ�ӡ�ĸ�
            //λ�û�debug Ŀ��Ƕ� ʵ�ʽǶ� ����ٶ�(Ŀ���ٶ�) ʵ���ٶ� ���Ť��
            printf("A:%.2f,%.2f,%.2f,%.2f,%.2f\n",
                   yaw.ctrl_->getDebugParam(controller::POS_E).target_,  //Ŀ��Ƕ�
                   yaw.ctrl_->getDebugParam(controller::POS_E).input_,//ʵ�ʽǶ�
                   yaw.ctrl_->getDebugParam(controller::POS_E).output_,//����ٶ�(Ŀ���ٶ�)
                   yaw.ctrl_->getDebugParam(controller::VEC_E).input_,//ʵ���ٶ�
                   yaw.ctrl_->getDebugParam(controller::VEC_E).output_//���Ť��
            );

//            //�ٶȻ�debug Ŀ���ٶ� ʵ���ٶ� ���Ť��
//            printf("A:%.2f,%.2f,%.2f\n",
//                   yaw.ctrl_->getDebugParam(controller::VEC_E).target_,
//                   yaw.ctrl_->getDebugParam(controller::VEC_E).input_,
//                   yaw.ctrl_->getDebugParam(controller::VEC_E).output_
//            );
        }
        osDelay(1);
    }
    /* USER CODE END StartDebugTask */
}
