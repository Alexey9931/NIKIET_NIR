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
	UARTInit();
	//UART_send_string("Ch0\tCh1\tCh2\tCh3\tCh4\tCh5\tCh6\tCh7\t\r");

	while(1)
	{
		
	}
}
