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

enum USART_PARITY
{
	USART_PARITY_NONE,
	USART_PARITY_ODD,
	USART_PARITY_EVEN	
};

enum USART_STOP_BIT
{
	USART_STOP_BIT1,
	USART_STOP_BIT1_5,
	USART_STOP_BIT2
};

typedef struct  
{
	uint32_t baud_rate;
	uint8_t parity;
	uint8_t stop_bit;
	
} USART_CONFIG;

void USART_Init(USART_CONFIG *cfg);
void USART_Disable(void);
void Putchar(uint8_t data);
void PutStr(const char *string);
void Putint(uint8_t n);
void Puthex(uint8_t n);




extern FIFO_Data_t RxBuf[], TxBuf[];
#endif
