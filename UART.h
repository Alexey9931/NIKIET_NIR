#ifndef __UART_H
#define __UART_H


#include <MDR32F9Qx_uart.h>
#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>
#include "string.h"

void UARTInit(void);
void UART_send_string(char *data);
void UART1_IRQHandler(void);

#endif /*__UART_H */



