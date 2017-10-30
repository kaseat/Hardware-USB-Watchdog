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

#include "LedController.h"

#ifndef SLOW_BLINK_TIMEOUT
#define SLOW_BLINK_TIMEOUT  ((uint32_t)1000U)
#endif

#ifndef MID_BLINK_TIMEOUT
#define MID_BLINK_TIMEOUT   ((uint32_t)500U)
#endif

#ifndef FAST_BLINK_TIMEOUT
#define FAST_BLINK_TIMEOUT  ((uint32_t)250U)
#endif

#define INITIAL             ((uint8_t)0x00U)
#define FAST_BLINK          ((uint8_t)0x01U)
#define MID_BLINK           ((uint8_t)0x02U)
#define SLOW_BLINK          ((uint8_t)0x04U)
#define IS_LED_HIGH         ((uint8_t)0x08U)
#define IS_DISABLED         ((uint8_t)0x10U)
#define OFF                 ((uint8_t)0x20U)
#define GLOW                ((uint8_t)0x40U)

__no_init uint8_t __eeprom CorrectionValue;

LedController::LedController(Timer& timer, GpioDriver& driver) :
	timer(timer),
	driver(driver),
	disabled(false),
	state(INITIAL),
	counter(INITIAL)
{
	LedController::timer.SubscribeOnElapse(*this);
}

LedController::~LedController()
{
	timer.UnsubscribeOnElapse(*this);
}

Response LedController::Enable()
{
	disabled = false;
	if (state & GLOW) driver.DriveLedHigh();
	return EnableLedOk;
}

Response LedController::Disable()
{
	disabled = true;
	driver.DriveLedLow();
	return DisableLedOk;
}

void LedController::Off()
{
	state = OFF;
	if (!disabled) driver.DriveLedLow();
}

void LedController::Glow()
{
	state = GLOW;
	if (!disabled) driver.DriveLedHigh();
}

void LedController::BlinkFast()
{
	state = FAST_BLINK;
	if (!disabled) driver.DriveLedLow();
}

void LedController::BlinkMid()
{
	state = MID_BLINK;
	if (!disabled) driver.DriveLedLow();
}

void LedController::BlinkSlow()
{
	state = SLOW_BLINK;
	if (!disabled) driver.DriveLedLow();
}

void LedController::Callback(uint8_t data)
{
	if (disabled || state & OFF || state & GLOW) return;

	state & FAST_BLINK
		? Blink(FAST_BLINK_TIMEOUT)
		: state & MID_BLINK
		? Blink(MID_BLINK_TIMEOUT)
		: Blink(SLOW_BLINK_TIMEOUT);
}

void LedController::Blink(uint32_t timeout)
{
	if (++counter >= timeout)
	{
		(state ^= IS_LED_HIGH) & IS_LED_HIGH
			? driver.DriveLedHigh()
			: driver.DriveLedLow();
		counter = INITIAL;
	}
}
