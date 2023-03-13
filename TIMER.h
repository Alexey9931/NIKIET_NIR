#ifndef __TIMER_H
#define __TIMER_H


#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>
#include <MDR32F9Qx_timer.h>
#include "UART.h"
#include "ADC.h"
#include <stdio.h>

void TIMER1_IRQHandler(void);
void TIMER1_init(void);


#endif /*__TIMER_H */