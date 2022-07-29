/*
 * term.c
 *
 * Created: March-14-16, 5:18:46 PM
 *  Author: K. C. Lee
 * Copyright (c) 2016 by K. C. Lee 
 
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
 
#include "vga-min.h"
#include "ansi.h"
#include "serial.h"
#include "ps2.h"
#include "ps2 table.h"
#include "settings.h"

enum {MODE_NORMAL, MODE_SETUP};
static int mode = MODE_NORMAL;

void onKeyUp(uint8_t code)
{
	if(code == F12_KEY)
	{
		mode = MODE_SETUP;
		void USART_Disable(void);
	}
	
	if(mode == MODE_SETUP && code == F10_KEY)
	{
		mode = MODE_NORMAL;
		USART_CONFIG cfg = {UART_BAUD, USART_PARITY_NONE, USART_STOP_BIT1};
		USART_Init(&cfg);
	}	
	
}

int main(void)
{
	VGA_Init();

	USART_CONFIG cfg = {UART_BAUD, USART_PARITY_NONE, USART_STOP_BIT1};
  USART_Init(&cfg);
	PS2_Init();
	PS2_Subscribe(onKeyUp);
  ANSI_Init();

	while(1)
	{
		if(mode == MODE_NORMAL)
		{
			if(FIFO_ReadAvail((FIFO*)RxBuf))
				ANSI_FSM(Getc((FIFO*)RxBuf));		

			if(FIFO_ReadAvail((FIFO*)PS2_Buf))
				PS2_Task();
			
			if(Cursor.Update)
				Cursor_Task();
		
		}
		else // SETUP
		{
		
		}
			
	}
}
