#include <stdint.h>
#include "STM8S003F3.h"

#define OPERATION_TIMEOUT   ((uint16_t)0xFFFF)

int __eeprom_wait_for_last_operation(void)
{
	uint8_t flagstatus = 0x00;
	uint16_t timeout = OPERATION_TIMEOUT;
	while ((flagstatus == 0x00) && (timeout != 0x00))
	{
		flagstatus = (uint8_t)(FLASH->IAPSR & (FLASH_IAPSR_EOP | FLASH_IAPSR_WR_PG_DIS));
		timeout--;
	}
	return !!(flagstatus & (uint8_t)(0x04));
}

void __eeprom_program_byte(unsigned char __near * dst, unsigned char v)
{
  *(__near uint8_t*) (uint16_t)dst = v;
}

void __eeprom_program_long(unsigned char __near * dst, unsigned long v)
{
	FLASH->CR2 |= FLASH_CR2_WPRG;
	FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NWPRG);

	/* Write one byte - from lowest address*/
	*((__near uint8_t*)(uint16_t)dst) = *((uint8_t*)(&v));
	/* Write one byte*/
	*(((__near uint8_t*)(uint16_t)dst) + 1) = *((uint8_t*)(&v) + 1);
	/* Write one byte*/
	*(((__near uint8_t*)(uint16_t)dst) + 2) = *((uint8_t*)(&v) + 2);
	/* Write one byte - from higher address*/
	*(((__near uint8_t*)(uint16_t)dst) + 3) = *((uint8_t*)(&v) + 3);
}