#include "Rebooter.h"
#include "Timer.h"

#ifndef RST_TIM
// Soft reset pin low level duration
#define RST_TIM 200U
#endif

#ifndef HR_LO_TIM
// Hard reset pin low level duration
#define HR_LO_TIM 6000U
#endif

#ifndef HR_HI_TIM
// Hard reset pin high level duration
#define HR_HI_TIM 2000U
#endif
// Reboot is in proccess
#define IN_PROCESS 0x10U
// Soft reset marker
#define SOFT_RESET 0x01U
// Hard reset marker
#define HARD_RESET 0x02U
// Hard reset HR_LO_TIM elapsed
#define HR_LO_ELAPSED 0x04U
// Hard reset HR_HI_TIM elapsed
#define HR_HI_ELAPSED 0x08U
// Reset value
#define INITIAL 0x00U

Rebooter::Rebooter(Timer& tmr, GpioDriver& driver) :timer(tmr), driver(driver), state(INITIAL), counter(INITIAL)
{
	timer.SubscribeOnElapse(*this);
}

Rebooter::~Rebooter()
{
	timer.UnsubscribeOnElapse(*this);
}

void Rebooter::HardReset()
{
	if (state & IN_PROCESS) return;
	state = IN_PROCESS | HARD_RESET;
	driver.DrivePowerLow();
	counter = INITIAL;
}
 
void Rebooter::SoftReset()
{
	if (state & IN_PROCESS) return;
	state = IN_PROCESS | SOFT_RESET;
	driver.DriveResetLow();
	counter = INITIAL;
}

void Rebooter::Callback(uint8_t data)
{
	if (state & SOFT_RESET)
	{
		if (++counter >= RST_TIM)
		{
			state & HR_HI_ELAPSED
				? driver.DrivePowerHigh()
				: driver.DriveResetHigh();
			state = INITIAL;
			return;
		}
	}

	if (state & HARD_RESET)
	{
		if (++counter >= HR_LO_TIM)
		{
			driver.DrivePowerHigh();
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