#include "stm32h7xx_hal.h"
//#include "lptim.h"
#include "Key2.h"
#include "Serial.h"
#include "delay.h"


uint8_t led1_status=0,led2_status=0;

//NORMAL IT APPROACH


//DEBOUNCED through LPTIM
//OBSOLETE
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	HAL_LPTIM_Counter_Start_IT(&hlptim1,120000);
//}
//void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef* hlptim)
//{
//	HAL_LPTIM_Counter_Stop_IT(hlptim); //Stop LPTIM1 and its interruption
//	
//	if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3))//PE3
//	{
//		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
//		if(led5_status==0)
//		{
//			Screen_SendStr("led5.val=1");
//			led5_status=1;
//		}
//		else
//		{
//			Screen_SendStr("led5.val=0");
//			led5_status=0;
//		}
//	}//PC7
//		
//	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12))
//	{
//		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
//		if(led4_status==0)
//		{
//			Screen_SendStr("led4.val=1");
//			led4_status=1;
//		}
//		else
//		{
//			Screen_SendStr("led4.val=0");
//			led4_status=0;
//		}
//			
//	}//?? PC6
//}


