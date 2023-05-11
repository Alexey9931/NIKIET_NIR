#include "UART.h"

//static char rx_data[200];//принятая телеграмма-запрос
static uint8_t RX_bytes[200];//принятый массив байтов
static int RX_bytes_counter;//счетчик элементов принятого массива

//static uint8_t rx_data_size;

void UARTInit(void)
{
// Включение тактирования UART1
RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
// Объявление структуры для инициализации контроллера UART
UART_InitTypeDef UART_InitStructure;
// Делитель тактовой частоты UART = 1
UART_BRGInit(MDR_UART1, UART_HCLKdiv1);
// Конфигурация UART
// Скорость передачи данных – 115200 бод
UART_InitStructure.UART_BaudRate = 115200;
UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF;
// Количество бит в посылке – 
UART_InitStructure.UART_WordLength = UART_WordLength8b;
// Один стоп-бит
UART_InitStructure.UART_StopBits = UART_StopBits1;
// Без проверки четности
UART_InitStructure.UART_Parity = UART_Parity_No;
// Выключить работу буфера FIFO приемника и передатчика,
// т.е. передача осуществляется по одному байту
UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF;
// Разрешить прием и передачу данных
UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
// Инициализация UART1 с заданными параметрами
UART_Init(MDR_UART1, &UART_InitStructure);
// Включить сконфигурированный UART
UART_Cmd(MDR_UART1, ENABLE);

UART_ITConfig( MDR_UART1, UART_IT_RX, ENABLE );
NVIC_EnableIRQ( UART1_IRQn );
}

void UART_send_string(char *data)
{
	for (int i = 0; i < strlen(data); i++)
	{
		UART_SendData(MDR_UART1, data[i]);
		while (UART_GetFlagStatus(MDR_UART1, UART_FLAG_TXFF) == SET);
	}
}
void UART_send_array_bytes(uint8_t data[], uint8_t data_size)
{
	for (int i = 0; i < data_size; i++)
	{
		UART_SendData(MDR_UART1, data[i]);
		while (UART_GetFlagStatus(MDR_UART1, UART_FLAG_TXFF) == SET);
	}
}
void UART1_IRQHandler(void)
{
	uint8_t rx_byte;//принятый байт
	//char RX_byte[10] = {0}; //принятый байт в виде строки

	if (UART_GetITStatusMasked(MDR_UART1, UART_IT_RX) == SET)
	{
			rx_byte = UART_ReceiveData(MDR_UART1);	
			UART_ClearITPendingBit(MDR_UART1, UART_IT_RX);
			while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE)!= SET){}	
			//если принят конец пакета
			if((rx_byte == 0xAA)&&(RX_bytes[RX_bytes_counter-1] == 0xAA))
			{
				RX_bytes[RX_bytes_counter] = rx_byte;
				//UART_send_array_bytes(RX_bytes, RX_bytes_counter+1);
				MODBUS_read_request_and_send_response(RX_bytes, RX_bytes_counter+1);				
				memset(RX_bytes,0,sizeof(RX_bytes)/sizeof(RX_bytes[0]));
				RX_bytes_counter = 0;
			}
			else
			{
				RX_bytes[RX_bytes_counter] = rx_byte;
				RX_bytes_counter++;
				//sprintf(RX_byte,"%x",rx_byte);
				//strcat(rx_data,RX_byte);
				//rx_data_size++;
			}	
  }
}