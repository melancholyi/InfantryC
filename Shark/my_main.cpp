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
//motor::Motor yaw(motor::GM_YAW_E, motor::M6020_E,  motor::CAN1_E, motor::ONE_E, can1);
//
communication::ComVision vision(huart6);     /**1 创建通信类**/
controller::GeneralPID vec1(1000,30000,3000,50,controller::Ramp_e);
motor::Motor MotorArr[]{
        {motor::GM_YAW_E, motor::M6020_E,  motor::CAN1_E, motor::ONE_E, can1, &vec1}  , //1ff
        {motor::GM_PITCH_E, motor::M6020_E,  motor::CAN1_E, motor::TWO_E, can1} ,//1ff
        {motor::GM_FRIC_0E, motor::M3508_E,  motor::CAN1_E, motor::THREE_E, can1},//200
};

extern "C" void StartDebugTask(void const * argument)
{
    osDelay(500);

    /**
     * 这里是debug发送电流结构体设置的对不对
     */
//    printf("vector:%d\n",motor::Motor::sendMsg_.size());
//    for(int i = 0 ; i < motor::Motor::sendMsg_.size();i++){
//        printf("motor%d:%d,%lu,%d,%.2f\n",i,
//               motor::Motor::sendMsg_[i].can,
//               motor::Motor::sendMsg_[i].stdId,
//               motor::Motor::sendMsg_[i].id,
//               *motor::Motor::sendMsg_[i].current );
//    }


    /* USER CODE BEGIN StartDebugTask */

    vision.open();     /**2 启动底层的huart**/
    can1.open();
    can2.open();
    uint32_t count = 0;

    uint8_t arr[8] = {5000 >> 8, 5000&0xff ,2000 >> 8, 2000&0xff ,3000 >> 8, 3000&0xff ,4000 >> 8, 4000&0xff  };

    MotorArr[0].ctrl_->setTarget(100);
    for(;;){
        count++;
        if(count%500 == 0){
            //printf("target:%.2f\n", MotorArr[0].ctrl_->getDebugParam().input_);
//            printf("ecd:%.2f,%d,%d,%d\n",
//                   MotorArr[0].encoder_.getEncoder().angelAll,
//                   MotorArr[0].encoder_.getEncoder().speed,
//                   MotorArr[0].encoder_.getEncoder().torque,
//                   MotorArr[0].encoder_.getEncoder().tempature);
            LEDR_TOGGLE;
            count = 0;
        }
        else if(count % 10 == 0){
//            printf("ecd:%.2f,%.2f,%.2f\n",MotorArr[0].ctrl_->getDebugParam().target_,
//                   MotorArr[0].ctrl_->getDebugParam().input_,
//                   MotorArr[0].ctrl_->getDebugParam().output_
//                   );
            MotorArr[0].ctrl_->ctrlLoop();
            motor::Motor::sendAllMotorCur();
//            can1.setTxStdID(0x1FF);
//            memcpy((uint8_t*)can1.buf_tx_.getBufPtr(),arr,sizeof(arr));
//            can1.write();
        }
        //
        osDelay(1);
    }
    /* USER CODE END StartDebugTask */
}
extern "C" void USART6_IRQHandler(void)
{
    /* USER CODE BEGIN USART6_IRQn 0 */
    vision.read(); /** 5 读取串口传输的原始数据 , 保存至类的成员变量中 FixedPacket<8> buf_rx_**/
    vision.slovePacket(); /**3 解包函数，需要根据实际情况自定义**/
    /* USER CODE END USART6_IRQn 0 */
   HAL_UART_IRQHandler(&huart6);
    /* USER CODE BEGIN USART6_IRQn 1 */

    /* USER CODE END USART6_IRQn 1 */
}

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    can1.read();
    can2.read();
    MotorArr[0].encoder_.encoderSloveLoop();

    //yaw.encoder_.encoderSloveLoop();
//    /*!< CAN receive buffer */
//    CAN_RxHeaderTypeDef rx_header;
//    uint8_t rx_data[8];
//    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
//    int16_t ID = rx_header.StdId;
//    int16_t pos, vel, current_back, tmp;
//    uint16_t cool_rate, cool_limit, shoot_spd_max;
//    uint8_t color, smallOrBig;
//
//    /* Switch to user call back function. */
//    if (hcan == &hcan1) {
//        pos = (int16_t) (rx_data[0] << 8 | rx_data[1]);
//        vel = (int16_t) (rx_data[2] << 8 | rx_data[3]);
//        current_back = (int16_t) ((rx_data)[4] << 8 | rx_data[5]);
//        tmp = (int16_t) rx_data[6];
//        printf("pos:%d\n",pos);
//    }
}


