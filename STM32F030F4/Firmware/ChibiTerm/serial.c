/*
 * serial.c
 *
 * Created: March-09-16, 8:48:47 PM
 *  Author: K. C. Lee
 * Copyright (c) 2015 by K. C. Lee
 
 	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.

	If not, see http://www.gnu.org/licenses/gpl-3.0.en.html
	
 */

#include "serial.h"
#include "vga-min.h"
#include "fifo.h"

FIFO_DECL(RxBuf,RX_FIFO_Size);
FIFO_DECL(TxBuf,TX_FIFO_Size);
const uint32_t USART_BAUD_RATES[] = { 2400, 4800, 9600, 19200, 38400, 57600, 115200 };

void USART_Init(USART_CONFIG *cfg)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;									// Enable UART1 clock
  USART1->BRR = CPU_CLOCK / cfg->baud_rate;										// Set default baud rate
#ifdef AUTOBAUD
	USART1->CR2 = USART_CR2_ABRMODE_0|USART_CR2_ABREN;		// enable autobaud, 1 stop bit
#endif
  USART1->CR1 = USART_CR1_RXNEIE|												// Receive not empty
								USART_CR1_RE|USART_CR1_TE|USART_CR1_UE; // 8-bit, 1 start bit, no parity, 16X oversampling

	FIFO_Clear((FIFO*)RxBuf);
	FIFO_Clear((FIFO*)TxBuf);
	
  // NVIC IRQ
  NVIC_SetPriority(USART1_IRQn,USART_IRQ_PRIORITY);			// Lowest priority																	// Highest priority
  NVIC_EnableIRQ(USART1_IRQn);
}

void USART_Disable(void)
{
	RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;									// Enable UART1 clock
	
  // NVIC IRQ
  NVIC_DisableIRQ(USART1_IRQn);
}

void USART1_IRQHandler(void)
{
	if(USART1->ISR & USART_ISR_RXNE)											// Rx data
	{
		FIFO_Write((FIFO*)RxBuf,USART1->RDR);	
	}
	
	if(USART1->ISR & USART_ISR_TXE)												// Tx empty
	{
		uint8_t TxD;
		
		if(FIFO_Read((FIFO*)TxBuf,&TxD))
			USART1->TDR = TxD;																// TDR is 16-bit!
		else
			USART1->CR1 &= ~USART_CR1_TXEIE;									// Disable Tx Empty interrupt
	}
}

void Putchar( uint8_t data )
{
	while(!FIFO_Write((FIFO*)TxBuf,data))
	  /* busy wait */;
	
  USART1->CR1 |= USART_CR1_TXEIE;												// Enable Tx Empty interrupt
}


void PutStr(const char *string)
{
	while(string && *string)
	  Putchar(*string++);
}

void Putint(uint8_t n)
{	
  if(n>9)
    Putint(n/10);
	Putchar((n%10)+'0');
}

void Puthex(uint8_t n)
{
 uint8_t c;
  c=(n>>4)+'0';
	if (c>'9')
		c+=7;
	Putchar(c);
	c=(n&0x0f)+'0';
	if (c>'9')
		c+=7;
	Putchar(c);
}
