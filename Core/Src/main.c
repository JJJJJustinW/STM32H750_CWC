/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/////////////////////////////////IMPORTANT NOTE: make sure to comment the UART4_HANDLER() in stm32h7xx_it.c
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Serial.h"

#include "Key2.h"

#include "AD9959.h"
#include "AD9834.h"

#include "delay.h"

#include "custom_logger.h"

#include "Mode1.h"
#include "Mode3.h"
#include "Mode4.h"
#include "../../User/DDS/AD9959.h"


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
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/*
GPIO External Interrupt Callback Function
Pins: 	PE2-->LED4(PC6),PE3-->LED5(PC7)

*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    delay_ms(50);
    // if (GPIO_Pin == GPIO_PIN_2) {
    //     if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)) {
    //         HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
    //         if (led1_status == 0) {
    //             Screen_SendStr("led1.val=1", FB_ON); //REMAINDER OF PREVIOUS PIN SETUP
    //             led1_status = 1;
    //         } else {
    //             Screen_SendStr("led1.val=0", FB_ON);
    //             led1_status = 0;
    //         }
    //     } //?? PC6
    // } else if (GPIO_Pin == GPIO_PIN_3) {
    //     if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)) //PE3
    //     {
    //         HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
    //         if (led2_status == 0) {
    //             Screen_SendStr("led2.val=1", FB_ON);
    //             led2_status = 1;
    //         } else {
    //             Screen_SendStr("led2.val=0", FB_ON);
    //             led2_status = 0;
    //         }
    //     } //PC7
    // }
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

    //MODIFIED from main.c's request method

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

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
  MX_DMA_Init();
  MX_UART5_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_DAC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

    //printf("before TIM\r\n");
    //HAL_TIM_Base_Start(&htim1);//tim1 init
    // HAL_TIM_Base_Start(&htim2);//tim2 init
    // printf("before ADC\r\n");

    //HAL_ADC_Start_DMA(&hadc1,g_adc1_dma_data1,ADC_DATA_LENGTH);
    // printf("after ADC\r\n");


    //printf("before delay\r\n");
    delay_init(480);
    delay_ms(100);


    //Serial_printf("DDS1\r\n");
    Init_AD9959();
    IO_Update();
    Write_frequence(0, 200000);
    Write_Amplitude(0, 700);
    Write_Phase(0, 0);
    // Write_frequence(2, 2000000);
    // Write_Amplitude(2, 100);
    // Write_Phase(2, 0);

    //CUSTOM_LOG_V(V_INFO, "DDS2\r\n");
    // float dac_vol = 0;
    // uint32_t dac_val = 0;
    // dac_vol = 1;
    // dac_val = (uint32_t) (dac_vol / 3.3 * 4095);
    // CUSTOM_LOG_V(V_INFO,"State:%d\r\n",HAL_DAC_GetState(&hdac1));
    // HAL_DAC_Start(&hdac1,DAC_CHANNEL_2);
    // HAL_DAC_SetValue(&hdac1,DAC_CHANNEL_2,DAC_ALIGN_12B_R,dac_val);
    // CUSTOM_LOG("DAC_VAL:%d\r\n",dac_val);
    // AD9834_Init();
    // AD9834_Select_Wave(CONTROL_REGISTER);
    // AD9834_Set_Freq(FREQ_0,100000);



    __HAL_UART_ENABLE_IT(huart_debug, UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(huart_screen, UART_IT_RXNE);
    //Serial_printf("\r\nIT_RXNE ENABLED\r\n");

    ADC_DMA_Init(
        &hadda,
        3.0f,
        1.5f,
        0xFFF,
        0x800,
        &htim1,
        240000000,
        10,
        &hadc1,
        &hdac1,
        DAC_CHANNEL_1,
        500000,
        ADC_DMA_BUFSIZE,
        ADC_DMA_RTFunc,
        ADC_DMA_HalfTxFunc,
        ADC_DMA_FullTxFunc);


    CUSTOM_LOG_V(V_INFO, "===========INITIALIZATION COMPLETE==========\r\n");



    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
    delay_ms(150);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
    delay_ms(150);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
    delay_ms(150);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    delay_ms(150);

    //printf("debug flag 1\r\n");
    ADC_DMA_STATE AD_State = ADDA_STATE_IDLE;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        //ADC_DMA_Output();
        if (AD_State == ADDA_STATE_IDLE) {
            switch (print4serial()) {
                case 0x0004: {
                    AD_State = ADDA_STATE_INIT;
                    break;
                }
                case 0x0002: {
                    CUSTOM_LOG_V(V_INFO, "SERIAL INFO\r\n");
                    break;
                }
                default: ;
            }
            switch (print4screen()) {
                /*===MODE1 FREQ OUT===*/
                case 0x0000: {
                    break;
                }
                case 0x1000: {
                    Serial_printf("MODE1 ON\r\n");
                    Write_frequence(0, m1_freq);
                    Write_Amplitude(0, 700);
                    Write_Phase(0, 0);
                    IO_Update();
                    break;
                }
                case 0x2000: {
                    Serial_printf("MODE1 OFF\r\n");
                    Write_Amplitude(0, 0);
                    Write_Phase(0, 0);
                    Write_frequence(0, 0);
                    IO_Update();
                    break;
                }
                case 0x4000: {
                    Mode1_FreqSel(USART_SCR_RX_BUF);
                    CUSTOM_LOG_V(V_INFO, "Freq:%d\r\n", m1_freq);
                    break;
                }
                /*===MODE2 1kHz_2V_Output===*/
                case 0x0100: {
                    CUSTOM_LOG("MODE2 ON\r\n");
                    Write_frequence(0, 1000);
                    Write_Amplitude(0, 170);
                    Write_Phase(0, 0);
                    IO_Update();
                    break;
                }
                case 0x0200: {
                    CUSTOM_LOG("MODE2 OFF\r\n");
                    Write_Amplitude(0, 0);
                    Write_Phase(0, 0);
                    Write_frequence(0, 0);
                    IO_Update();
                    break;
                }
                /*===MODE3 FREQ 1-2V OUTPUT===*/
                case 0x0040: {
                    Mode3_FreqMagSel(USART_SCR_RX_BUF);
                    //CUSTOM_LOG_V(V_INFO, "Freq:%d\r\n", m3_freq);//MIGHT CAUSE PACK LOSS
                    //CUSTOM_LOG_V(V_INFO, "Mag:%d\r\n", m3_DDS_Out); //now outputs analog output requirement.
                    break;
                }
                case 0x0010: {
                    Write_frequence(0, m3_freq);
                    Write_Amplitude(0, m3_DDS_Out);
                    Write_Phase(0, 0);
                    IO_Update();
                    break;
                }
                case 0x0020: {
                    Write_Amplitude(0, 0);
                    Write_Phase(0, 0);
                    Write_frequence(0, 0);
                    IO_Update();
                    break;
                }
                case 0x0001: {
                    AD_State = ADDA_STATE_INIT;
                    break;
                }
                default: ;
            }
        } else if (AD_State == ADDA_STATE_INIT) {
            ADC_DMA_Start(&hadda);
            AD_State = ADDA_STATE_RUNNING;
            CUSTOM_LOG_V(V_INFO, "DMA INIT\r\n");
        } else if (AD_State == ADDA_STATE_RUNNING) {
            switch (print4serial()) {
                case 0x0008: {
                    AD_State = ADDA_STATE_HALTED;
                    CUSTOM_LOG_V(V_INFO, "DMA ENDED\r\n");
                    break;
                }
                default: ;
            }
            switch (print4screen()) {
                case 0x0002: {
                    AD_State = ADDA_STATE_HALTED;
                    CUSTOM_LOG_V(V_INFO, "DMA ENDED\r\n");
                    break;
                }
                default: ;
            }
            ADC_DMA_Main(&hadda);
        } else if (AD_State == ADDA_STATE_HALTED) {
            ADC_DMA_Stop(&hadda);
            AD_State = ADDA_STATE_IDLE;
        }
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
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
