/**
  ******************************************************************************
  * @file           : main.c
  * @author         : melancholycy
  * @attention      : no
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
/* Private includes ----------------------------------------------------------*/
#include <cstring>
#include <vector>
#include "bsp_delay.h"
#include "calibrate_task.h"
#include "retarget.h"
#include "transporter_usart.hpp"
#include "com_vision.hpp"
#include "transporter_can.hpp"
#include "encoder.hpp"
#include "motor.hpp"
#include "cascade_pid.hpp"
#include "general_pid.hpp"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/
void * operator new(size_t size)
{
    auto ptr = pvPortMalloc(size);
    memset(ptr,0,size);
    return ptr;
}

void operator delete(void *m)
{
    vPortFree(m);
}

/**
  * @brief  The application entry point.
  */
int main()
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2C3_Init();
    MX_SPI1_Init();
    MX_TIM10_Init();
    MX_USART6_UART_Init();
    MX_USART3_UART_Init();
    MX_CAN1_Init();
    MX_CAN2_Init();
    MX_USART1_UART_Init();

    /* USER CODE BEGIN 2 */
    RetargetInit(&huart6);
    delay_init();
    cali_param_init();
    printf("init ok\n");
    /* USER CODE END 2 */

    MX_FREERTOS_Init();
    osKernelStart();

    while (1)
    {
        LEDB_TOGGLE;
        HAL_Delay(500);
    }
}

communication::TransporterCan can1(hcan1,communication::FIFO0_E);
communication::TransporterCan can2(hcan2,communication::FIFO0_E);
communication::ComVision vision(huart6);     /**1 创建通信类**/
controller::GeneralPID vec1(1000,30000,3000,50,controller::Ramp_e);
controller::CascadePID pos1(100,100,50,1000,30000,3000);
motor::Motor MotorArr[]{
        {motor::GM_YAW_E, motor::M6020_E,  motor::CAN1_E, motor::ONE_E, can1, &pos1}  , //1ff
        {motor::GM_PITCH_E, motor::M6020_E,  motor::CAN1_E, motor::TWO_E, can1} ,//1ff
        {motor::GM_FRIC_0E, motor::M3508_E,  motor::CAN1_E, motor::THREE_E, can1},//200
};

extern "C" void StartDebugTask(void const * argument)
{
    osDelay(500);
    /* USER CODE BEGIN StartDebugTask */
    vision.open();
    can1.open();
    can2.open();
    uint32_t count = 0;
    MotorArr[0].ctrl_->setTarget(360.0 * 3,controller::POS_E);
    MotorArr[0].ctrl_->setPID(2,0,0,controller::POS_E);
    MotorArr[0].ctrl_->setPID(70,6.5,0,controller::VEC_E);
    for(;;){
        count++;
        if(count%500 == 0){
            LEDR_TOGGLE;
            count = 0;
        }
        else if(count % 10 == 0){
            ////debug
            printf("ecd:%.2f,%.2f,%.2f\n",MotorArr[0].ctrl_->getDebugParam(controller::POS_E).target_,
                   MotorArr[0].ctrl_->getDebugParam(controller::POS_E).input_,
                   MotorArr[0].ctrl_->getDebugParam(controller::POS_E).output_
            );
        }
        MotorArr[0].ctrl_->ctrlLoop();

        motor::Motor::sendAllMotorCur();
        osDelay(1);
    }
    /* USER CODE END StartDebugTask */
}
extern "C" void USART6_IRQHandler(void)
{
    /* USER CODE BEGIN USART6_IRQn 0 */
    vision.read();
    vision.slovePacket();
    /* USER CODE END USART6_IRQn 0 */
   HAL_UART_IRQHandler(&huart6);
    /* USER CODE BEGIN USART6_IRQn 1 */

    /* USER CODE END USART6_IRQn 1 */
}

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    can1.read();
    can2.read();
    MotorArr[0].encoder_.encoderSloveLoop();
}


