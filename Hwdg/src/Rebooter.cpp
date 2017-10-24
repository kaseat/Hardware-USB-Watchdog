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

#include "Rebooter.h"
#include "Timer.h"

#ifndef RST_TIM
// Soft reset pin low level duration
#define RST_TIM             ((uint_fast16_t)200U)
#endif

#ifndef HR_LO_TIM
// Hard reset pin low level duration
#define HR_LO_TIM           ((uint_fast16_t)6000U)
#endif

#ifndef HR_HI_TIM
// Hard reset pin high level duration
#define HR_HI_TIM           ((uint_fast16_t)2000U)
#endif
// Reboot is in proccess
#define IN_PROCESS          ((uint_least8_t)0x10U)
// Soft reset marker
#define SOFT_RESET          ((uint_least8_t)0x01U)
// Hard reset marker
#define HARD_RESET          ((uint_least8_t)0x02U)
// Hard reset HR_LO_TIM elapsed
#define HR_LO_ELAPSED       ((uint_least8_t)0x04U)
// Hard reset HR_HI_TIM elapsed
#define HR_HI_ELAPSED       ((uint_least8_t)0x08U)
// Reset value
#define INITIAL             ((uint_least8_t)0x00U)

Rebooter::Rebooter(Timer& tmr, GpioDriver& driver) : timer(tmr), driver(driver), state(INITIAL), counter(INITIAL)
{
	timer.SubscribeOnElapse(*this);
}

Rebooter::~Rebooter()
{
	timer.UnsubscribeOnElapse(*this);
}

Response Rebooter::HardReset()
{
	if (state & IN_PROCESS) return Busy;
	state = IN_PROCESS | HARD_RESET;
	driver.DrivePowerLow();
	counter = INITIAL;
	return TestHardResetOk;
}

Response Rebooter::SoftReset()
{
	if (state & IN_PROCESS) return Busy;
	state = IN_PROCESS | SOFT_RESET;
	driver.DriveResetLow();
	counter = INITIAL;
	return TestSoftResetOk;
}

void Rebooter::Callback(uint8_t data)
{
	if (state & SOFT_RESET)
	{
		if (++counter >= RST_TIM)
		{
			state & HR_HI_ELAPSED
				? driver.ReleasePower()
				: driver.ReleaseReset();
			state = INITIAL;
			return;
		}
	}

	if (state & HARD_RESET)
	{
		if (++counter >= HR_LO_TIM)
		{
			driver.ReleasePower();
			state |= HR_LO_ELAPSED;
			counter = INITIAL;
			return;
		}
	}

	if (state & HR_LO_ELAPSED)
	{
		if (++counter >= HR_HI_TIM)
		{
			driver.DrivePowerLow();
			state = IN_PROCESS | SOFT_RESET | HR_HI_ELAPSED;
			counter = INITIAL;
		}
	}
}
