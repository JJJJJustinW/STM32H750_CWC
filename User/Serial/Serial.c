#include "stm32h7xx_hal.h"
#include "usart.h"
#include "Serial.h"

UART_HandleTypeDef *huart_debug = &huart1; ///< Debug����
UART_HandleTypeDef *huart_screen = &huart5;

uint8_t tmp_Byte=0;//Used to avoid empty pointer


//////////////////////////////////////////////////////////////////////
/*---------------------------UART SEND------------------------------*/
//////////////////////////////////////////////////////////////////////
/*
 * Send uint8_t content to UART4(huart_debug).
 */
void Serial_SendByte(uint8_t Byte)
{
	tmp_Byte=Byte;
	HAL_UART_Transmit(huart_debug,&tmp_Byte,sizeof(Byte),10);
//	UART_WaitOnFlagUntilTimeout(huart_debug,UART_FLAG_TC,SET,1,10);
	while(__HAL_UART_GET_FLAG(huart_debug,UART_FLAG_TC)!=SET);
	
}

/*
 * Send uint8_t content to a given UART port.
 */
void Serial_SendByte_t(uint8_t Byte,UART_HandleTypeDef *huart)
{
	tmp_Byte=Byte;
	HAL_UART_Transmit(huart,&tmp_Byte,sizeof(Byte),10);
//	UART_WaitOnFlagUntilTimeout(huart_debug,UART_FLAG_TC,SET,1,10);
	while(__HAL_UART_GET_FLAG(huart,UART_FLAG_TC)!=SET);
	
}



/*
Send arrays of uint8_t items to UART4.
*/
void Serial_SendArr(uint8_t *array,uint16_t length)
{
	for(uint16_t itor_sndarr=0;itor_sndarr<length;itor_sndarr++)
	{
		Serial_SendByte(array[itor_sndarr]);
	}
	
}

/*
 * Send arrays to a given UART port.
 */
void Serial_SendArr_t(uint8_t *array,uint16_t length,UART_HandleTypeDef *huart)
{
	for(uint16_t itor_sndarr=0;itor_sndarr<length;itor_sndarr++)
	{
		Serial_SendByte_t(array[itor_sndarr],huart);
	}
	
}




//IMPLEMENTED IN NORMAL METHOD
//uint32_t Serial_Power(uint32_t base,uint32_t pow)
//{
//	uint32_t res=1;
//	while(pow--)
//	{
//		res*=base;
//	}
//	return res;
//}


/*
Implement the mathematical power through RECURSION
*/
uint32_t Serial_Power(uint32_t base,uint32_t exp)
{
	if(exp==0){return 1;}
	if(exp%2==0)
	{
		uint32_t halfPow=Serial_Power(base,exp/2);
		return halfPow*halfPow;
	}
	else
	{
		return base*Serial_Power(base,exp-1);
	}
}

/*
Send Numbers to UART4 port
*/
void Serial_SendNum(uint32_t num)
{
	uint16_t length=0;
	for(uint32_t stp_sendNum=0;stp_sendNum<65535;stp_sendNum++)
	{
		if(num/(Serial_Power(10,stp_sendNum))==0)
		{
			length=stp_sendNum;
			break;
		}
	}
	for(uint16_t stp_sndnuml=0;stp_sndnuml<length;stp_sndnuml++)
	{
		Serial_SendByte(num/Serial_Power(10,length-1-stp_sndnuml)%10+'0');
	}
}


/*
Send strings to UART4 port
*/
void Serial_SendStr(char *string)
{
	for(uint16_t itor_sndstr=0;string[itor_sndstr]!=0;itor_sndstr++)
	{
		Serial_SendByte(string[itor_sndstr]);
	}
}

/*
 * Send strings to given UART port.
 */
void Serial_SendStr_t(char *string,UART_HandleTypeDef *huart)
{
	for(uint16_t itor_sndstr=0;string[itor_sndstr]!=0;itor_sndstr++)
	{
		Serial_SendByte_t(string[itor_sndstr],huart);
	}
}


//////////////////////////////////////////////////////////////////////////
/*--------------------------printf redirect-----------------------------*/
//////////////////////////////////////////////////////////////////////////

/*
 * REPLACES the fputc() function used in the implementation of printf()
*/
// int fputc(int ch,FILE *f)
// {
// 	Serial_SendByte(ch);
// 	return ch;
// }
#if   defined( __GNUC__ )//GCC
/*	With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
	set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#elif defined( __CC_ARM )//ARMCC
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#else
#error "Unknown compiler"
#endif
static uint8_t s_usart_tmp;


#if defined(__CC_ARM)
int fputc(int ch,FILE *f)
{
	s_usart_tmp=(uint8_t)(ch);
	HAL_UART_Transmit(huart_debug,&s_usart_tmp,1,1);
	//	HAL_UART_Transmit_IT(huart_debug,&s_usart_tmp,1);
	return ch;
}
#elif defined( __GNUC__ )
int __io_putchar(int ch)
{
	s_usart_tmp=(uint8_t)(ch);
	HAL_UART_Transmit(huart_debug,&s_usart_tmp,1,1);
	//	HAL_UART_Transmit_IT(huart_debug,&s_usart_tmp,1);
	return ch;
}
#endif




/*
*Encapsulated printf function for formatted strings to all ports.
NOTE:
If wish to use Chinese characters, first add "--no-multibyte-chars" argument to the Misc Control in Target Options,
then change the serial port coding to "UTF-8" compatible.
Or use GB2310 compatible in Configuration and change the port to "GBK".
*/
void Serial_printf(const char *formatted,...)
{
	char String[100];
	va_list	args;
	va_start(args,formatted);
	vsprintf(String,formatted,args);
	va_end(args);
	Serial_SendStr(String);
}

/*
 * Variant of printf for other UART ports
 */
void Serial_printf_t(UART_HandleTypeDef *huart, const char *formatted,...)
{
	char String[100];
	va_list	args;
	va_start(args,formatted);
	vsprintf(String,formatted,args);
	va_end(args);
	Serial_SendStr_t(String,huart);

	//Send to UART4
	Serial_SendStr(String);
}


//////////////////////////////////////////////////////////////////
/*-----------------------SEND TO SCREEN-------------------------*/
//////////////////////////////////////////////////////////////////


//usually no need to use this, since the commands for the screen are better inputted like strings.
//THIS ONE INCLUDES THE EXTRA BIT NEEDED FOR COMMAND INPUTTING
void Screen_SendArr(uint8_t *array,uint16_t length)
{
	
	//FEEDBACK TO UART4
	Serial_SendStr_t("\r\nSent to screen:\r\n",huart_debug);
	for(uint16_t itor_sndarr=0;itor_sndarr<length;itor_sndarr++)
	{
		Serial_SendByte_t(array[itor_sndarr],huart_debug);
	}
	Serial_SendByte_t(0xFF,huart_debug);
	Serial_SendByte_t(0xFF,huart_debug);
	Serial_SendByte_t(0xFF,huart_debug);
	Serial_SendStr_t("\r\n",huart_debug);
	
	
	//SEND TO UART5
	for(uint16_t itor_scrsndarr=0;itor_scrsndarr<length;itor_scrsndarr++)
	{
		Serial_SendByte_t(array[itor_scrsndarr],huart_screen);
	}
	Serial_SendByte_t(0xFF,huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
}

/*
 * Send an array to UART5(serial screen)'s display message(integrated command)
 * FB:FB_ON|FB_OFF, indicates whether send to UART4 or not
 */
void Screen_SendArrToShow(uint8_t *array,uint16_t length,uint8_t FB)
{
	
	//FEEDBACK TO UART4
	if(FB==FB_ON)
	{
		Serial_SendStr_t("\r\nSent to screen:\r\n",huart_debug);
		Serial_SendStr_t("recv_txt.txt=\"",huart_debug);
		for(uint16_t itor_sndarr=0;itor_sndarr<length;itor_sndarr++)
		{
			Serial_SendByte_t(array[itor_sndarr],huart_debug);
		}
		Serial_SendStr_t("\"",huart_debug);
		Serial_SendByte_t(0xFF,huart_debug);
		Serial_SendByte_t(0xFF,huart_debug);
		Serial_SendByte_t(0xFF,huart_debug);
		Serial_SendStr_t("\r\n",huart_debug);
	}
	
	//SEND TO UART5
	Serial_SendStr_t("recv_txt.txt=\"",huart_screen);
	for(uint16_t itor_scrsndarr=0;itor_scrsndarr<length;itor_scrsndarr++)
	{
		Serial_SendByte_t(array[itor_scrsndarr],huart_screen);
	}
	Serial_SendStr_t("\"",huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
}

/*
 * Send a string to UART5(command postfix attached)
 */
void Screen_SendStr(char *string,uint8_t FB)
{
	//FEEDBACK TO UART4
	if(FB==FB_ON)
	{
		Serial_SendStr_t("\r\nSent to screen:\r\n",huart_debug);
		for(uint16_t itor_scrsndstr=0;string[itor_scrsndstr]!=0;itor_scrsndstr++)
		{
			Serial_SendByte_t(string[itor_scrsndstr],huart_debug);
		}
		Serial_SendByte_t(0xFF,huart_debug);
		Serial_SendByte_t(0xFF,huart_debug);
		Serial_SendByte_t(0xFF,huart_debug);
		Serial_SendStr_t("\r\n",huart_debug);
	}
	
	//SEND TO UART5
	for(uint16_t itor_scrsndstr=0;string[itor_scrsndstr]!=0;itor_scrsndstr++)
	{
		Serial_SendByte_t(string[itor_scrsndstr],huart_screen);
	}
	Serial_SendByte_t(0xFF,huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
}

/*
 * Send a string to UART5(no postfix attached)
 */
void Screen_SendStr_b(char *string)
{

	//SEND TO UART5
	for(uint16_t itor_scrsndstr=0;string[itor_scrsndstr]!=0;itor_scrsndstr++)
	{
		Serial_SendByte_t(string[itor_scrsndstr],huart_screen);
	}
}

/*
 * Send a string to UART5(serial screen)'s display message(integrated command)
 */
void Screen_SendStrToShow(char *string,uint8_t FB)
{
	//FEEDBACK TO UART4
	if(FB==FB_ON)
	{
		Serial_SendStr_t("\r\nSent to screen:\r\n",huart_debug);
		Serial_SendStr_t("recv_txt=\"",huart_debug);
		for(uint16_t itor_scrsndstr=0;string[itor_scrsndstr]!=0;itor_scrsndstr++)
		{
			Serial_SendByte_t(string[itor_scrsndstr],huart_debug);
		}
		Serial_SendStr_t("\"",huart_debug);
		Serial_SendByte_t(0xFF,huart_debug);
		Serial_SendByte_t(0xFF,huart_debug);
		Serial_SendByte_t(0xFF,huart_debug);
		Serial_SendStr_t("\r\n",huart_debug);
	}
	
	
	//SEND TO UART5
	Serial_SendStr_t("recv_txt.txt=\"",huart_screen);
	for(uint16_t itor_scrsndstr=0;string[itor_scrsndstr]!=0;itor_scrsndstr++)
	{
		Serial_SendByte_t(string[itor_scrsndstr],huart_screen);
	}
	Serial_SendStr_t("\"",huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
}


/*
 * Send a number to UART5(no command)
 */
void Screen_SendNum(uint32_t num)
{
	uint16_t length=0;
	for(uint32_t stp_sendNum=0;stp_sendNum<65535;stp_sendNum++)
	{
		if(num/(Serial_Power(10,stp_sendNum))==0)
		{
			length=stp_sendNum;
			break;
		}
	}
	for(uint16_t stp_scrsndnuml=0;stp_scrsndnuml<length;stp_scrsndnuml++)
	{
		Serial_SendByte_t(num/Serial_Power(10,length-1-stp_scrsndnuml)%10+'0',huart_screen);
	}
}


/*
 * Send a number to UART5(serial screen)'s display message
 */
void Screen_SendNumToShow(uint32_t num)
{
	uint16_t length=0;
	for(uint32_t stp_sendNum=0;stp_sendNum<65535;stp_sendNum++)
	{
		if(num/(Serial_Power(10,stp_sendNum))==0)
		{
			length=stp_sendNum;
			break;
		}
	}
	Serial_SendStr_t("recv_txt.txt=\"",huart_screen);
	for(uint16_t stp_scrsndnuml=0;stp_scrsndnuml<length;stp_scrsndnuml++)
	{
		Serial_SendByte_t(num/Serial_Power(10,length-1-stp_scrsndnuml)%10+'0',huart_screen);
	}
	Serial_SendStr_t("\"",huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
	Serial_SendByte_t(0xFF,huart_screen);
	
	
//	Serial_SendStr_t("recv_txt.txt=\"",huart_debug);
//	for(uint16_t stp_scrsndnuml=0;stp_scrsndnuml<length;stp_scrsndnuml++)
//	{
//		Serial_SendByte_t(num/Serial_Power(10,length-1-stp_scrsndnuml)%10+'0',huart_debug);
//	}
//	Serial_SendStr_t("\"",huart_debug);
//	Serial_SendByte_t(0xFF,huart_debug);
//	Serial_SendByte_t(0xFF,huart_debug);
//	Serial_SendByte_t(0xFF,huart_debug);
//	Serial_SendStr_t("\r\n",huart_debug);
	
}



///////////////////////////////////////////////////////////////////////////////////
//RECEIVE PART


uint8_t USART4_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�. An Array! Use the sendArr function
uint8_t USART5_RX_BUF[USART_REC_LEN];

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ-----ָ��д��RX_BUF��index
uint16_t USART4_RX_STA=0;       //����״̬���	
uint8_t aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

uint16_t USART5_RX_STA=0;
uint8_t aRxBuffer5[RXBUFFERSIZE];

uint16_t uart_rx_len=0;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//Serial_printf("RxCpltCallback() tag\r\n");
	if(huart->Instance==USART1)//����Ǵ���4
	{
		if((USART4_RX_STA&0x8000)==0)//����δ���
		{
			if(USART4_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART4_RX_STA=0;//���մ���,���¿�ʼ
				else 
				{	
					USART4_RX_STA|=0x8000;	//��������� 
					//printf("USART4_RX_BUF[%d]:%s\r\n",USART4_RX_STA,USART4_RX_BUF);	
				}				
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer[0]==0x0d)
					USART4_RX_STA|=0x4000;
				else
				{
					USART4_RX_BUF[USART4_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART4_RX_STA++;
					if(USART4_RX_STA>(USART_REC_LEN-1))
					{	
						USART4_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
							//reply_er();//�ݲ�����
					}
				}		 
			}
		}
	}
	
	if(huart->Instance==UART5)//����Ǵ���5
	{
		if((USART5_RX_STA&0x8000)==0)//����δ���
		{
			if(USART5_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer5[0]!=0x0a)USART5_RX_STA=0;//���մ���,���¿�ʼ
				else 
				{	
					USART5_RX_STA|=0x8000;	//��������� 
					//printf("USART4_RX_BUF[%d]:%s\r\n",USART4_RX_STA,USART4_RX_BUF);	
				}				
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer5[0]==0x0d)
					USART5_RX_STA|=0x4000;
				else
				{
					USART5_RX_BUF[USART5_RX_STA&0X3FFF]=aRxBuffer5[0] ;
					USART5_RX_STA++;
					if(USART5_RX_STA>(USART_REC_LEN-1))
					{	
						USART5_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
							//reply_er();//�ݲ�����
					}
				}		 
			}
		}

	}
	
}

//Modified UART4&UART5 handler
// void UART4_IRQHandler(void)
// {
// 	//Serial_printf("UART4_IRQHandler() tag\r\n");
// 	uint32_t timeout=0;
// 	uint32_t maxDelay=0x1FFFF;
//
// 	HAL_UART_IRQHandler(huart_debug);	//����HAL���жϴ����ú���
//
// 	timeout=0;
// 	while (HAL_UART_GetState(huart_debug) != HAL_UART_STATE_READY)//�ȴ�����
// 	{
// 	 timeout++;////��ʱ����
// 		 if(timeout>maxDelay)
// 		{
// 			//reply_er();
// 			break;
// 		}
// 	}
//
// 	timeout=0;
// 	while(HAL_UART_Receive_IT(huart_debug, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
// 	{
// 	 timeout++; //��ʱ����
// 	 if(timeout>maxDelay)
// 	 {
// 		 //reply_er();
// 		break;
// 	 }
// 	}
//
//
// }
//
//
// void UART5_IRQHandler(void)
// {
// 	//Serial_printf("UART5_IRQHandler() tag\r\n");
// 	uint32_t timeout=0;
// 	uint32_t maxDelay=0x1FFFF;
//
// 	HAL_UART_IRQHandler(huart_screen);	//����HAL���жϴ����ú���
//
// 	timeout=0;
// 	while (HAL_UART_GetState(huart_screen) != HAL_UART_STATE_READY)//�ȴ�����
// 	{
// 	 timeout++;////��ʱ����
// 		 if(timeout>maxDelay)
// 		{
// 			//reply_er();
// 			break;
// 		}
// 	}
//
// 	timeout=0;
// 	while(HAL_UART_Receive_IT(huart_screen, (uint8_t *)aRxBuffer5, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
// 	{
// 	 timeout++; //��ʱ����
// 	 if(timeout>maxDelay)
// 	 {
// 		 //reply_er();
// 		break;
// 	 }
// 	}
//
// }

//SEND THE DATA FROM UART4 TO BOTH UART4 and UART5
void print4serial(void)
{
	if(USART4_RX_STA&0x8000)
	{
		uart_rx_len=USART4_RX_STA&0x3fff;
		Serial_printf("\r\nsent data:\r\n");
//		HAL_UART_Transmit(&huart4,(uint8_t*)USART_RX_BUF,uart_rx_len,1000);  //Original print

		Serial_SendArr((uint8_t*)USART4_RX_BUF,uart_rx_len);

		/*---If the array is a command it will work---*/
		Screen_SendArr((uint8_t*)USART4_RX_BUF,uart_rx_len);
		Serial_SendByte_t(0xFF,huart_screen);
		Serial_SendByte_t(0xFF,huart_screen);
		Serial_SendByte_t(0xFF,huart_screen);

		Screen_SendArrToShow((uint8_t*)USART4_RX_BUF,uart_rx_len,FB_OFF);
		Serial_printf("\r\n");
		while(__HAL_UART_GET_FLAG(&huart4,UART_FLAG_TC)!=SET);		
		USART4_RX_STA=0;
	}
}


//SEND THE DATA FROM UART5 TO UART4
void print4screen(void)
{
	if(USART5_RX_STA&0x8000)
	{
		uart_rx_len=USART5_RX_STA&0x3fff;
		Serial_printf("\r\ndata from screen:\r\n");
		Serial_SendArr((uint8_t*)USART5_RX_BUF,uart_rx_len);
		Serial_printf("\r\n");
		while(__HAL_UART_GET_FLAG(&huart5,UART_FLAG_TC)!=SET);		
		USART5_RX_STA=0;
	}
}



