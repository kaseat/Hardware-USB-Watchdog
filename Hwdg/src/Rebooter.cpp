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
// Soft reset token
#define SOFT_RESET          ((uint_least8_t)0x01U)
// Hard reset token
#define HARD_RESET          ((uint_least8_t)0x02U)
// Hard reset HR_LO_TIM elapsed
#define HR_LO_ELAPSED       ((uint_least8_t)0x04U)
// Hard reset HR_HI_TIM elapsed
#define HR_HI_ELAPSED       ((uint_least8_t)0x08U)
// Power pulse token
#define POWER_PULSE         ((uint_least8_t)0x20U)
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

Response Rebooter::PwrPulse()
{
	if (state & IN_PROCESS) return Busy;
	state = IN_PROCESS | SOFT_RESET | POWER_PULSE;
	driver.DrivePowerLow();
	counter = INITIAL;
	return PowerPulseOk;
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
			state & HR_HI_ELAPSED || state & POWER_PULSE
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
