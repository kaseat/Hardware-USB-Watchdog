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

#include <avr/interrupt.h>
#include "usbDriver.h"
#include "HardwareInit.h"

int main(void)
{
	DDRB |= 1 << 3;
	HardwareInit();
	for (;;)
	{
		Usb.Poll();
	}
}

uint16_t counter = 0;
ISR(TIM0_COMPA_vect)
{
	if (++counter > 1000)
	{
		counter = 0;
		PORTB ^= 1 << 3;
	}
}
