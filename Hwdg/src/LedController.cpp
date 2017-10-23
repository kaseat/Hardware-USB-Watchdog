// Copyright (c) 2017, Oleg Petrochenko
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the HWDG nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.

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

LedController::LedController(Timer& timer, GpioDriver& driver): timer(timer), driver(driver), state(INITIAL),
                                                                counter(INITIAL)
{
	timer.SubscribeOnElapse(*this);
}

LedController::~LedController()
{
	timer.UnsubscribeOnElapse(*this);
}

void LedController::Off()
{
	state = INITIAL;
	driver.DriveLedLow();
}

void LedController::Glow()
{
	state = INITIAL;
	driver.DriveLedHigh();
}

void LedController::BlinkFast()
{
	state = FAST_BLINK;
	driver.DriveLedLow();
}

void LedController::BlinkMid()
{
	state = MID_BLINK;
	driver.DriveLedLow();
}

void LedController::BlinkSlow()
{
	state = SLOW_BLINK;
	driver.DriveLedLow();
}

void LedController::Callback(uint8_t data)
{
	if (!state) return;

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
