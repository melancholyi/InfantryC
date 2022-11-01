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
            ////打印电机编码信息
//            printf("A:%.2f,%.2f,%d\n",
//                   rammer.encoder_.getEncoder().angelAll,
//                   rammer.encoder_.getEncoder().speed,
//                   rammer.encoder_.getEncoder().torque
//            );

            ////打印电机控制信息
            //可以根据自己的需求写一个debug函数，参数为电机类型，传入哪个电机类型就打印哪个
            //位置环debug 目标角度 实际角度 输出速度(目标速度) 实际速度 输出扭矩
            printf("A:%.2f,%.2f,%.2f,%.2f,%.2f\n",
                   yaw.ctrl_->getDebugParam(controller::POS_E).target_,  //目标角度
                   yaw.ctrl_->getDebugParam(controller::POS_E).input_,//实际角度
                   yaw.ctrl_->getDebugParam(controller::POS_E).output_,//输出速度(目标速度)
                   yaw.ctrl_->getDebugParam(controller::VEC_E).input_,//实际速度
                   yaw.ctrl_->getDebugParam(controller::VEC_E).output_//输出扭矩
            );

//            //速度环debug 目标速度 实际速度 输出扭矩
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
