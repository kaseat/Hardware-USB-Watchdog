// Copyright 2017 Oleg Petrochenko
// 
// This file is part of HwdgTiny.
// 
// HwdgTiny is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
// 
// HwdgTiny is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with HwdgTiny. If not, see <http://www.gnu.org/licenses/>.

#include "Rebooter.h"
#include <stdint-gcc.h>
#include "Gpio.h"

// Soft reset pin low level duration
#define RST_TIM             ((uint8_t)200U)

// Reboot is in proccess
#define IN_PROCESS          ((uint8_t)0x10U)
// Soft reset token
#define SOFT_RESET          ((uint8_t)0x01U)
// Reset value
#define INITIAL             ((uint8_t)0x00U)

static uint8_t state;
static uint8_t counter;

/**
* \brief Start soft reset sequence.
*/
Response_t RebooterSoftReset(void)
{
	if (state & IN_PROCESS) return Busy;
	state = IN_PROCESS | SOFT_RESET;
	GpioDriveResetLow();
	counter = INITIAL;
	return TestSoftResetOk;
}

/**
 * \brief This function must be called every 1ms.
 */
void RebooterTimebase()
{
	if (state & SOFT_RESET && ++counter >= RST_TIM)
	{
		GpioReleaseReset();
		state = INITIAL;
	}
}
