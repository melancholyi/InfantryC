#include "usart.h"
#include "cmsis_os.h"
#include "gimbal.hpp"
#include "motor.hpp"

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


        }
        osDelay(1);
    }
    /* USER CODE END StartDebugTask */
}
