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

#include <stdint-gcc.h>
#include <avr/io.h>

#define RST_DDR DDRB
#define RST_PIN PINB

#define LED_DDR DDRB
#define LED_PIN PINB

#define RST_VAL (1 << 3)
#define LED_VAL (1 << 4)

/**
* \brief Initialize GPIO.
* \note This function calls once and we can save
* couple of bytes by inlining this function.
*/
__inline void GpioInit(void)
{
	// Turn LED on.
//	LED_DDR |= LED_VAL;
//	LED_PIN |= LED_VAL;

	// Ensure reset pins are in input mode.
	RST_PIN &= ~RST_VAL;
	RST_DDR &= ~RST_VAL;
}

/**
* \brief Drive reset pin low.
*/
__inline void GpioDriveResetLow(void)
{
	RST_DDR |= RST_VAL;
}

/**
* \brief Release reset pin.
*/
__inline void GpioReleaseReset(void)
{
	RST_DDR &= ~RST_VAL;
}

/**
* \brief Drive LED pin low.
* \note This function converts to a single sbi assembly
* instruction, so we should make this function inline.
*/
__inline void GpioDriveLedLow(void)
{
	LED_PIN |= LED_VAL;
}

/**
* \brief Drive LED pin high.
* \note This function converts to a single cbi assembly
* instruction, so we should make this function inline.
*/
__inline void GpioDriveLedHigh(void)
{
	LED_PIN &= ~LED_VAL;
}
