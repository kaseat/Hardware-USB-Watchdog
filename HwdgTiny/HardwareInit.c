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

#include "HardwareInit.h"
#include <avr/interrupt.h>
#include "usbDriver.h"

/**
 * \brief Configure TIM0 as 1ms interrupt source.
 */
__inline void TimerInit(void);

/**
 * \brief Low level hardware initialization.
 */
void HardwareInit(void)
{
	//wdt_enable(WDTO_1S);
	TimerInit();
	Usb.Init();
	sei();
}

__inline void TimerInit(void)
{
	// Ensure we have zero in TCNT0 register.
	TCNT0 = 0;
	// Configure counter resolution to get 1 ms interrupt.
	// Actually we have an interrupt every 1.0055 ms instead.
	// It's the closest value we can achieve on this microcontroller.
	OCR0A = 64;
	// Put timer in CTC mode.
	TCCR0A = 1 << WGM01;
	// Set 256 prescaler.
	TCCR0B = 1 << CS02;
	// Enable Output Compare Match interrupt.
	TIMSK |= 1 << OCIE0A;
}
