// Copyright 2017 Oleg Petrochenko
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
	*(__near uint8_t*)(uint16_t)dst = v;
}

void __eeprom_program_long(unsigned char __near * dst, unsigned long v)
{
	FLASH->CR2 |= FLASH_CR2_WPRG;
	FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NWPRG);

	*((__near uint8_t*)(uint16_t)dst) = *((uint8_t*)(&v));
	*(((__near uint8_t*)(uint16_t)dst) + 1) = *((uint8_t*)(&v) + 1);
	*(((__near uint8_t*)(uint16_t)dst) + 2) = *((uint8_t*)(&v) + 2);
	*(((__near uint8_t*)(uint16_t)dst) + 3) = *((uint8_t*)(&v) + 3);
}
