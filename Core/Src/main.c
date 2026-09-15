/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "usb_device.h"
#include "usbd_hid.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct{
		uint8_t MODIFIER;
		uint8_t RESERVED;
		uint8_t KEYCODE1;
		uint8_t KEYCODE2;
		uint8_t KEYCODE3;
		uint8_t KEYCODE4;
		uint8_t KEYCODE5;
		uint8_t KEYCODE6;
}keyboardHID;
/* USER CODE END 0 */
#define R1_PORT GPIOA
#define R1_PIN GPIO_PIN_6
#define R2_PORT GPIOA
#define R2_PIN GPIO_PIN_5
#define R3_PORT GPIOA
#define R3_PIN GPIO_PIN_4
#define R4_PORT GPIOA
#define R4_PIN GPIO_PIN_3
/**
  * @brief  The application entry point.
  * @retval int
  */
#define C1_PORT GPIOA
#define C1_PIN GPIO_PIN_2
#define C2_PORT GPIOA
#define C2_PIN GPIO_PIN_1
#define C3_PORT GPIOA
#define C3_PIN GPIO_PIN_0


char keypad_Scan(void)
{
    char keymap[4][3] =
    {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'},
        {'*', '0', '#'}
    };

    // Đưa tất cả hàng lên HIGH
    HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_SET);

    // Quét từng hàng
    for (int row = 0; row < 4; row++)
    {
        // Đưa tất cả hàng lên HIGH trước
        HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_SET);

        // Kéo hàng hiện tại xuống LOW
        if (row == 0)
        {
            HAL_GPIO_WritePin(R1_PORT, R1_PIN, GPIO_PIN_RESET);
        }
        else if (row == 1)
        {
            HAL_GPIO_WritePin(R2_PORT, R2_PIN, GPIO_PIN_RESET);
        }
        else if (row == 2)
        {
            HAL_GPIO_WritePin(R3_PORT, R3_PIN, GPIO_PIN_RESET);
        }
        else
        {
            HAL_GPIO_WritePin(R4_PORT, R4_PIN, GPIO_PIN_RESET);
        }

        // Đợi tín hiệu ổn định
        HAL_Delay(1);

        // Đọc C1
        if (HAL_GPIO_ReadPin(C1_PORT, C1_PIN) == GPIO_PIN_RESET)
        {
            HAL_Delay(20);

            if (HAL_GPIO_ReadPin(C1_PORT, C1_PIN) == GPIO_PIN_RESET)
            {
                while (HAL_GPIO_ReadPin(C1_PORT, C1_PIN) == GPIO_PIN_RESET)
                {
                }

                return keymap[row][0];
            }
        }

        // Đọc C2
        if (HAL_GPIO_ReadPin(C2_PORT, C2_PIN) == GPIO_PIN_RESET)
        {
            HAL_Delay(20);

            if (HAL_GPIO_ReadPin(C2_PORT, C2_PIN) == GPIO_PIN_RESET)
            {
                while (HAL_GPIO_ReadPin(C2_PORT, C2_PIN) == GPIO_PIN_RESET)
                {
                }

                return keymap[row][1];
            }
        }

        // Đọc C3
        if (HAL_GPIO_ReadPin(C3_PORT, C3_PIN) == GPIO_PIN_RESET)
        {
            HAL_Delay(20);

            if (HAL_GPIO_ReadPin(C3_PORT, C3_PIN) == GPIO_PIN_RESET)
            {
                while (HAL_GPIO_ReadPin(C3_PORT, C3_PIN) == GPIO_PIN_RESET)
                {
                }

                return keymap[row][2];
            }
        }
    }

    return '\0';
}

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  char key = keypad_Scan();
	  if (key != '\0'){
		  keyboardHID keyboardhid = {0}; // Khởi tạo gói tin HID ban đầu = 0

		          // Ánh xạ phím từ ma trận sang USB HID Keycode
		          if (key >= '1' && key <= '9') {
		              keyboardhid.KEYCODE1 = 0x1E + (key - '1');
		          }
		          else if (key == '0') {
		              keyboardhid.KEYCODE1 = 0x27;
		          }
		          else if (key == '*') {
		              // Ví dụ cho phím '*' (hoặc ký tự A, B tùy bạn đổi trong keymap)
		              keyboardhid.MODIFIER = 0x02; // Left Shift
		              keyboardhid.KEYCODE1 = 0x04; // Phím 'a'
		          }
		          else if (key == '#') {
		              // Xử lý cho phím '#'
		              keyboardhid.KEYCODE1 = 0x28; // Ví dụ: Enter (0x28)
		          }

		          // Gửi báo cáo nhấn phím lên máy tính
		          USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&keyboardhid, sizeof(keyboardhid));

		          HAL_Delay(50); // Giữ trạng thái nhấn

		          // Gửi gói tin thả phím (tất cả bằng 0) để tránh kẹt phím
		          memset(&keyboardhid, 0, sizeof(keyboardhid));
		          USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*)&keyboardhid, sizeof(keyboardhid));

		          HAL_Delay(100);
		      }

		      HAL_Delay(10);

	  }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */


  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA3 PA4 PA5 PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);
  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
