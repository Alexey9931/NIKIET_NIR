#include "ADC.h"
#include "TIMER.h"
#include "UART.h"
#include "PORTS.h"
#include "DATA_INTERFACE.h"
#include "REGISTER_SPACE.h"


int main(void)
	{
	PortsInit();
	ADCInit();
	ADC_Channel_Init();
	TIMER1_init();
	uart_init();
	//инициализация пространства регистров
	registers_ini();

	while(1)
	{
		MODBUS_read_request_and_send_response();
		
	}
}
