#include "PORTS.h"

void PortsInit(void)
{
// Включение тактирования портов
RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB|RST_CLK_PCLK_PORTC, ENABLE);
	
// Объявление структуры для инициализации портов UART
PORT_InitTypeDef GPIO_init_struct;
// Инициализация порта B для функции UART
// Настройка порта по умолчанию
PORT_StructInit(&GPIO_init_struct);
// Переопределение функции порта
GPIO_init_struct.PORT_FUNC = PORT_FUNC_MAIN;
// Установка короткого фронта
GPIO_init_struct.PORT_SPEED = PORT_SPEED_MAXFAST;
// Цифровой режим работы вывода
GPIO_init_struct.PORT_MODE = PORT_MODE_DIGITAL;
// Инициализация вывода PF1 как UART_TX (передача)
GPIO_init_struct.PORT_Pin = PORT_Pin_0;
GPIO_init_struct.PORT_OE = PORT_OE_OUT;
PORT_Init(MDR_PORTB, &GPIO_init_struct);
// Инициализация вывода PF0 как UART_RX (прием)
GPIO_init_struct.PORT_Pin = PORT_Pin_1;
GPIO_init_struct.PORT_OE = PORT_OE_IN;
PORT_Init(MDR_PORTB, &GPIO_init_struct);

// Объявление структуры для инициализации 0-3 каналов АЦП
PORT_InitTypeDef GPIO_init_ADC_0_3;
PORT_StructInit(&GPIO_init_ADC_0_3);
// Инициализация вывода PC1 как вход канала 3
GPIO_init_ADC_0_3.PORT_Pin = PORT_Pin_1;
GPIO_init_ADC_0_3.PORT_OE = PORT_OE_IN;
GPIO_init_ADC_0_3.PORT_FUNC = PORT_FUNC_PORT;
GPIO_init_ADC_0_3.PORT_MODE = PORT_MODE_ANALOG;
// Инициализация вывода PC2 как вход канала 2
GPIO_init_ADC_0_3.PORT_Pin = PORT_Pin_2;
GPIO_init_ADC_0_3.PORT_OE = PORT_OE_IN;
GPIO_init_ADC_0_3.PORT_FUNC = PORT_FUNC_PORT;
GPIO_init_ADC_0_3.PORT_MODE = PORT_MODE_ANALOG;
// Инициализация вывода PC3 как вход канала 1
GPIO_init_ADC_0_3.PORT_Pin = PORT_Pin_3;
GPIO_init_ADC_0_3.PORT_OE = PORT_OE_IN;
GPIO_init_ADC_0_3.PORT_FUNC = PORT_FUNC_PORT;
GPIO_init_ADC_0_3.PORT_MODE = PORT_MODE_ANALOG;
// Инициализация вывода PC4 как вход канала 0
GPIO_init_ADC_0_3.PORT_Pin = PORT_Pin_4;
GPIO_init_ADC_0_3.PORT_OE = PORT_OE_IN;
GPIO_init_ADC_0_3.PORT_FUNC = PORT_FUNC_PORT;
GPIO_init_ADC_0_3.PORT_MODE = PORT_MODE_ANALOG;

PORT_Init(MDR_PORTC, &GPIO_init_ADC_0_3);

// Объявление структуры для инициализации 4-7 каналов АЦП
PORT_InitTypeDef GPIO_init_ADC_4_7;
PORT_StructInit(&GPIO_init_ADC_4_7);
// Инициализация вывода PB6 как вход канала 7
GPIO_init_ADC_4_7.PORT_Pin = PORT_Pin_6;
GPIO_init_ADC_4_7.PORT_OE = PORT_OE_IN;
GPIO_init_ADC_4_7.PORT_FUNC = PORT_FUNC_PORT;
GPIO_init_ADC_4_7.PORT_MODE = PORT_MODE_ANALOG;
// Инициализация вывода PB7 как вход канала 6
GPIO_init_ADC_4_7.PORT_Pin = PORT_Pin_7;
GPIO_init_ADC_4_7.PORT_OE = PORT_OE_IN;
GPIO_init_ADC_4_7.PORT_FUNC = PORT_FUNC_PORT;
GPIO_init_ADC_4_7.PORT_MODE = PORT_MODE_ANALOG;
// Инициализация вывода PB8 как вход канала 5
GPIO_init_ADC_4_7.PORT_Pin = PORT_Pin_8;
GPIO_init_ADC_4_7.PORT_OE = PORT_OE_IN;
GPIO_init_ADC_4_7.PORT_FUNC = PORT_FUNC_PORT;
GPIO_init_ADC_4_7.PORT_MODE = PORT_MODE_ANALOG;
// Инициализация вывода PB9 как вход канала 4
GPIO_init_ADC_4_7.PORT_Pin = PORT_Pin_9;
GPIO_init_ADC_4_7.PORT_OE = PORT_OE_IN;
GPIO_init_ADC_4_7.PORT_FUNC = PORT_FUNC_PORT;
GPIO_init_ADC_4_7.PORT_MODE = PORT_MODE_ANALOG;

PORT_Init(MDR_PORTB, &GPIO_init_ADC_4_7);


}