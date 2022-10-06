/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "INS_task.h"
#include "retarget.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId DebugTaskHandle;
osThreadId ImuTaskHandle;
osThreadId CanComTaskHandle;
osThreadId LegTaskHandle;
osThreadId WheelTaskHandle;
osThreadId BehaviorTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDebugTask(void const * argument);
void StartIMUTask(void const * argument);
void StartCanComTask(void const * argument);
void StartLegTask(void const * argument);
void StartWheelTask(void const * argument);
void StartBehaviorTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of DebugTask */
  osThreadDef(DebugTask, StartDebugTask, osPriorityLow, 0, 1024);
  DebugTaskHandle = osThreadCreate(osThread(DebugTask), NULL);

  /* definition and creation of ImuTask */
  osThreadDef(ImuTask, StartIMUTask, osPriorityHigh, 0, 512);
  ImuTaskHandle = osThreadCreate(osThread(ImuTask), NULL);

  /* definition and creation of CanComTask */
  osThreadDef(CanComTask, StartCanComTask, osPriorityAboveNormal, 0, 256);
  CanComTaskHandle = osThreadCreate(osThread(CanComTask), NULL);

  /* definition and creation of LegTask */
  osThreadDef(LegTask, StartLegTask, osPriorityRealtime, 0, 512);
  LegTaskHandle = osThreadCreate(osThread(LegTask), NULL);

  /* definition and creation of WheelTask */
  osThreadDef(WheelTask, StartWheelTask, osPriorityRealtime, 0, 512);
  WheelTaskHandle = osThreadCreate(osThread(WheelTask), NULL);

  /* definition and creation of BehaviorTask */
  osThreadDef(BehaviorTask, StartBehaviorTask, osPriorityHigh, 0, 256);
  BehaviorTaskHandle = osThreadCreate(osThread(BehaviorTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDebugTask */
/**
  * @brief  Function implementing the debugTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDebugTask */
__weak void StartDebugTask(void const * argument)
{
  /* USER CODE BEGIN StartDebugTask */
  /* Infinite loop */
  const float* relu_angel = get_INS_angle_point();
  for(;;)
  {
      //printf("angel:%.2f,%.2f,%.2f,%.2f\n",relu_angel[0],relu_angel[1],relu_angel[2],getIMUTemp());
    osDelay(10);
  }
  /* USER CODE END StartDebugTask */
}

/* USER CODE BEGIN Header_StartIMUTask */
/**
* @brief Function implementing the imuTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartIMUTask */
__weak void StartIMUTask(void const * argument)
{
  /* USER CODE BEGIN StartIMUTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartIMUTask */
}

/* USER CODE BEGIN Header_StartCanComTask */
/**
* @brief Function implementing the CanComTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCanComTask */
__weak void StartCanComTask(void const * argument)
{
  /* USER CODE BEGIN StartCanComTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCanComTask */
}

/* USER CODE BEGIN Header_StartLegTask */
/**
* @brief Function implementing the LegTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLegTask */
__weak void StartLegTask(void const * argument)
{
  /* USER CODE BEGIN StartLegTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartLegTask */
}

/* USER CODE BEGIN Header_StartWheelTask */
/**
* @brief Function implementing the WheelTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartWheelTask */
__weak void StartWheelTask(void const * argument)
{
  /* USER CODE BEGIN StartWheelTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartWheelTask */
}

/* USER CODE BEGIN Header_StartBehaviorTask */
/**
* @brief Function implementing the BehaviorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBehaviorTask */
__weak void StartBehaviorTask(void const * argument)
{
  /* USER CODE BEGIN StartBehaviorTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartBehaviorTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
