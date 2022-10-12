/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
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
/* USER CODE BEGIN Includes */
#include "bsp_delay.h"
#include "calibrate_task.h"
#include "retarget.h"
#include <cstring>
#include "transporter_usart.hpp"
#include "com_vision.hpp"
#include "transporter_can.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);

void CAN_FilterInit()
{
    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterBank = 0;
    can_filter_st.FilterActivation = ENABLE;
//    can_filter_st.FilterBank =
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
    HAL_CAN_Start(&hcan1);

    can_filter_st.FilterBank = 14;
    can_filter_st.SlaveStartFilterBank = 14;
    HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);
    HAL_CAN_Start(&hcan2);
}

void CAN_Init(CAN_HandleTypeDef* hcan)
{
    assert_param(hcan != NULL);
    HAL_CAN_Start(hcan);
    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    //注意使能中断
    HAL_CAN_ActivateNotification(hcan,CAN_IT_RX_FIFO0_MSG_PENDING);
}



void CAN_DriverInit()
{
    CAN_Init(&hcan1);
    CAN_Init(&hcan2);
    CAN_FilterInit();
}
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void *pvPortMalloc( size_t xWantedSize );
void * operator new(size_t size)
{
//	return pvPortMalloc(size);
    auto ptr = pvPortMalloc(size);
    memset(ptr,0,size);
    return ptr;
}

void operator delete(void *m)
{
    vPortFree(m);
}

void can_filter_init();
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval int
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
extern "C" void StartDebugTask(void const * argument)
{
    /* USER CODE BEGIN StartDebugTask */
    osDelay(500);
    vision.open();     /**2 启动底层的huart**/
    can1.open();
    can2.open();

    for(;;){
        LEDB_TOGGLE;
        vision.slovePacket(); /**3 解包函数，需要根据实际情况自定义**/
        vision.getVisionMsg().head; /**4 需要的位置getMSG来进行获取控制信息 **/
        int16_t pos = 0;
        can1.buf_rx_.getData(0,pos);
        printf("%d\n",pos);

        osDelay(1);
    }
    /* USER CODE END StartDebugTask */
}
extern "C" void USART6_IRQHandler(void)
{
    vision.read(); /** 5 读取串口传输的原始数据 , 保存至类的成员变量中 FixedPacket<8> buf_rx_**/
    /* USER CODE BEGIN USART6_IRQn 0 */

    /* USER CODE END USART6_IRQn 0 */
   HAL_UART_IRQHandler(&huart6);
    /* USER CODE BEGIN USART6_IRQn 1 */

    /* USER CODE END USART6_IRQn 1 */
}

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    can1.read();
    can2.read();

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


