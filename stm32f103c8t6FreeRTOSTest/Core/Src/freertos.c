/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

#include <stdio.h>
#include <string.h>

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

extern UART_HandleTypeDef huart1;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask03 */
osThreadId_t myTask03Handle;
const osThreadAttr_t myTask03_attributes = {
  .name = "myTask03",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for buttonTaskHandl */
osThreadId_t buttonTaskHandlHandle;
const osThreadAttr_t buttonTaskHandl_attributes = {
  .name = "buttonTaskHandl",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ButtonSemaphore */
osSemaphoreId_t ButtonSemaphoreHandle;
const osSemaphoreAttr_t ButtonSemaphore_attributes = {
  .name = "ButtonSemaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */



/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);
void ButtonTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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

  /* Create the semaphores(s) */
  /* creation of ButtonSemaphore */
  ButtonSemaphoreHandle = osSemaphoreNew(1, 1, &ButtonSemaphore_attributes);

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* creation of myTask03 */
  myTask03Handle = osThreadNew(StartTask03, NULL, &myTask03_attributes);

  /* creation of buttonTaskHandl */
  buttonTaskHandlHandle = osThreadNew(ButtonTask, NULL, &buttonTaskHandl_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  char buffer[30];
  snprintf(buffer, sizeof(buffer), "FreeRTOS Started \r\n");
  for(;;)
  {
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
  uint8_t ledState = 0;

  char buffer[30];
	snprintf(buffer, sizeof(buffer), "LED Control Task Started \r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);

  /* Infinite loop */
  for(;;)
  {

    uint32_t notification = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    if (notification > 0) {
      ledState = !ledState; 

      if (ledState) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); 
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
        snprintf(buffer, sizeof(buffer), "First_LED ON / Second_LED OFF \r\n");
      } else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); 
        snprintf(buffer, sizeof(buffer), "First_LED OFF / Second_LED ON \r\n");
      }
      HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
    }
    else {
      snprintf(buffer, sizeof(buffer), "Button not pressed \r\n");
      HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
    }
    
    osDelay(500); 
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
  char buffer[30];
  uint32_t heartbeat_count = 0;
  
  snprintf(buffer, sizeof(buffer), "Heartbeat Task Started\r\n");
  HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
  
  /* Infinite loop */
  for(;;)
  {
    heartbeat_count++;
    snprintf(buffer, sizeof(buffer), "Heartbeat: %lu\r\n", heartbeat_count);
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
    osDelay(5000); // 5초마다 하트비트
  }
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_ButtonTask */
/**
* @brief Function implementing the buttonTaskHandl thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ButtonTask */
void ButtonTask(void *argument)
{
  /* USER CODE BEGIN ButtonTask */

    uint32_t last_press_time = 0;
    const uint32_t debounce_delay = 50; // 50ms 디바운싱
    uint32_t button_count = 0;
    char buffer[50];
    
    snprintf(buffer, sizeof(buffer), "Button Task Started\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);

  /* Infinite loop */
  for(;;)
  {
    /* CubeMX 생성 세마포어로부터 신호 대기 */
    if (osSemaphoreAcquire(ButtonSemaphoreHandle, osWaitForever) == osOK)
    {
      uint32_t current_time = osKernelGetTickCount();
            
      /* 디바운싱 처리 */
      if ((current_time - last_press_time) > debounce_delay)
        {
          button_count++;
                
          snprintf(buffer, sizeof(buffer), "Button Pressed! Count: %lu\r\n", button_count);
          HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
                
          /* LED 제어 태스크에 알림 전송 */
          xTaskNotifyGive(myTask02Handle);
                
          last_press_time = current_time;
        }
        else
        {
          snprintf(buffer, sizeof(buffer), "Button bounce ignored\r\n");
          HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), 100);
        }
    }
    osDelay(10);
  }
  /* USER CODE END ButtonTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

