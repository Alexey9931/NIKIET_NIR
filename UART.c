#include "UART.h"

#define BUFFER_SIZE 2048
#define BUFFER_MASK (BUFFER_SIZE-1)
//static uint8_t RX_bytes[200];//принятый массив байтов
//static int RX_bytes_counter;//счетчик элементов принятого массива

static uint8_t buffer[BUFFER_SIZE]; //буффер с принятыми байтами
static int buffer_count = 0;//счетчик элементов буффера
static uint32_t read_pos = 0;


void uart_init(void)
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


void uart_write(uint8_t data[], uint32_t data_size)
{
	for (int i = 0; i < data_size; i++)
	{
		UART_SendData(MDR_UART1, data[i]);
		while (UART_GetFlagStatus(MDR_UART1, UART_FLAG_TXFF) == SET);
	}
}
uint8_t uart_read(uint32_t len, uint8_t *data)
{
	if (len > BUFFER_SIZE)
	{
		return 0;
	}
	if (sizeof(buffer) == 0)
	{
		return 0;
	}
	if ((read_pos+len) > BUFFER_SIZE)
	{
		memcpy(data, buffer + read_pos, BUFFER_SIZE-read_pos);
		memcpy(data + BUFFER_SIZE-read_pos, buffer, len-BUFFER_SIZE+read_pos);
		read_pos += len-BUFFER_SIZE;
	}
	else
	{
		memcpy(data, buffer + read_pos, len);
		//read_pos += len;
	}
	
	return 1;
}
void uart_set_pos(uint32_t pos)
{
	read_pos = pos;
}
uint32_t uart_read_pos()
{
	return read_pos;
}
void uart_flush()
{
	memset(buffer, 0, sizeof(buffer));
}
void UART1_IRQHandler(void)
{
	//uint8_t rx_byte;//принятый байт
	if (UART_GetITStatusMasked(MDR_UART1, UART_IT_RX) == SET)
	{
			buffer_count &= BUFFER_MASK;
			buffer[buffer_count] = UART_ReceiveData(MDR_UART1);	
			UART_ClearITPendingBit(MDR_UART1, UART_IT_RX);
			while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE)!= SET){}
			/*if ((buffer[buffer_count] == 0xAA)&&(buffer[buffer_count-1] == 0xAA))
			{
				uart_write(buffer, sizeof(buffer));
			}*/
			buffer_count++;
			/*rx_byte = UART_ReceiveData(MDR_UART1);	
			UART_ClearITPendingBit(MDR_UART1, UART_IT_RX);
			while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE)!= SET){}	
			if((rx_byte == 0xAA)&&(RX_bytes[RX_bytes_counter-1] == 0xAA))
			{
				RX_bytes[RX_bytes_counter] = rx_byte;
				MODBUS_read_request_and_send_response(RX_bytes, RX_bytes_counter+1);				
				memset(RX_bytes,0,sizeof(RX_bytes)/sizeof(RX_bytes[0]));
				RX_bytes_counter = 0;
			}
			else
			{
				RX_bytes[RX_bytes_counter] = rx_byte;
				RX_bytes_counter++;
			}	*/
		
  }
}