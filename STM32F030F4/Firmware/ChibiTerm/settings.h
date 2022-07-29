
#include <stdint.h>
#include "serial.h"

typedef struct  
{
	USART_CONFIG uart;
	uint8_t code_page;	
	
} SETUP_Item;


SETUP_Item SETUP_Load(void);
void       SETUP_Save(SETUP_Item data);



