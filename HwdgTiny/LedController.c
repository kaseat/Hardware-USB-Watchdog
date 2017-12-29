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

#include "LedController.h"
#include "Gpio.h"


#ifndef SLOW_BLINK_TIMEOUT
#define SLOW_BLINK_TIMEOUT  ((uint16_t)1000U)
#endif

#ifndef MID_BLINK_TIMEOUT
#define MID_BLINK_TIMEOUT   ((uint16_t)500U)
#endif

#ifndef FAST_BLINK_TIMEOUT
#define FAST_BLINK_TIMEOUT  ((uint16_t)250U)
#endif

#define INITIAL             ((uint8_t)0x00U)
#define FAST_BLINK          ((uint8_t)0x01U)
#define MID_BLINK           ((uint8_t)0x02U)
#define SLOW_BLINK          ((uint8_t)0x04U)
#define IS_LED_HIGH         ((uint8_t)0x08U)
#define IS_DISABLED         ((uint8_t)0x10U)
#define OFF                 ((uint8_t)0x20U)
#define GLOW                ((uint8_t)0x40U)

static uint8_t state;
static uint8_t disabled;
static uint16_t counter;

Response_t LedControlerEnable(void)
{
	disabled = 0;
	if (state & GLOW) GpioDriveLedHigh();
	return EnableLedOk;
}

uint8_t LedControlerIsEnabled(void)
{
	return !disabled;
}

Response_t LedControlerDisable(void)
{
	disabled = 1;
	GpioDriveLedLow();
	return DisableLedOk;
}

void LedControlerOff(void)
{
	state = OFF;
	if (!disabled) GpioDriveLedLow();
}

void LedControlerGlow(void)
{
	state = GLOW;
	if (!disabled) GpioDriveLedHigh();
}

void LedControlerBlinkFast(void)
{
	state = FAST_BLINK;
	if (!disabled) GpioDriveLedLow();
}

void LedControlerBlinkMid(void)
{
	state = MID_BLINK;
	if (!disabled) GpioDriveLedLow();
}

void LedControlerBlinkSlow(void)
{
	state = SLOW_BLINK;
	if (!disabled) GpioDriveLedLow();
}

void LedControlerBlink(const uint16_t timeout)
{
	if (++counter >= timeout)
	{
		(state ^= IS_LED_HIGH) & IS_LED_HIGH
			? GpioDriveLedHigh()
			: GpioDriveLedLow();
		counter = INITIAL;
	}
}

/**
 * \brief This function must be called every 1ms.
 */
void LedControlerTimebase(void)
{
	if (disabled || state & OFF || state & GLOW) return;

	state & FAST_BLINK
		? LedControlerBlink(FAST_BLINK_TIMEOUT)
		: state & MID_BLINK
		? LedControlerBlink(MID_BLINK_TIMEOUT)
		: LedControlerBlink(SLOW_BLINK_TIMEOUT);
}
