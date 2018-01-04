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

#pragma once

#include <avr/interrupt.h>
#include "usbDriver.h"
#include "Timer.h"
#include "Gpio.h"
#include "BootManager.h"

/**
 * \brief Low level hardware initialization.
 */
__inline void HardwareInit(void)
{
	//wdt_enable(WDTO_1S);
	TimerInit();
	GpioInit();
	UsbInit();
	BootManagerProceedBoot();
	sei();
}