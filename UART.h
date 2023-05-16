#ifndef __UART_H
#define __UART_H


#include <MDR32F9Qx_uart.h>
#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>
#include "string.h"

void uart_init(void);
void uart_write(uint8_t data[], uint32_t data_size);
uint8_t uart_read(uint32_t len, uint8_t *data);
void uart_flush();
void uart_set_pos(uint32_t pos);
uint32_t uart_read_pos();
void UART1_IRQHandler(void);

#endif /*__UART_H */



