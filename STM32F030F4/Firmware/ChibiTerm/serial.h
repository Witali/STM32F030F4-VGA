#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdio.h>
#include <stdint.h>
#include "stm32f0xx.h"
#include "system_stm32f0xx.h"
#include "fifo.h"

#define UART_BAUD								115200

//#define AUTOBAUD

/* Buffer in 2^n */
#define RX_FIFO_Size						64
#define TX_FIFO_Size						16
#define USART_IRQ_PRIORITY  		3

extern const uint32_t USART_BAUD_RATES[];


typedef struct  
{
	uint32_t BAUD_RATE;
	uint8_t OPTIONS;
	
} USART_CONFIG;

void USART_Init(USART_CONFIG *cfg);
void USART_Disable(void);
void Putchar(uint8_t data);
void PutStr(const char *string);
void Putint(uint8_t n);
void Puthex(uint8_t n);




extern FIFO_Data_t RxBuf[], TxBuf[];
#endif
