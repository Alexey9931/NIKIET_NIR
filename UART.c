#include "UART.h"

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
// Количество бит в посылке – 8
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
}

void UART_send_string(char *data)
{
	for (int i = 0; i < strlen(data); i++)
	{
		UART_SendData(MDR_UART1, data[i]);
		while (UART_GetFlagStatus(MDR_UART1, UART_FLAG_TXFF) == SET);
	}
}