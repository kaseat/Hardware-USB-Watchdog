#include "Rebooter.h"
#include "Timer.h"

#define RST_TIM 200U
#define HR_LO_TIM 6000U
#define HR_HI_TIM 2000U

Rebooter::Rebooter(Timer& tmr, GpioDriver& driver) :timer(tmr), driver(driver), state(0), counter(0)
{
	timer.SubscribeOnElapse(*this);
}

Rebooter::~Rebooter()
{
	timer.UnsubscribeOnElapse(*this);
}

void Rebooter::HardReset()
{
	if (state & 0x10) return;
	counter = 0;
	state = 0x12;
	driver.DrivePowerLow();
}

void Rebooter::SoftReset()
{
	if (state & 0x10) return;
	counter = 0;
	state = 0x11;
	driver.DriveResetLow();
}

void Rebooter::Callback(uint8_t data)
{
	if (state & 0x01U)
	{
		if (++counter >= RST_TIM)
		{
			if (state & 0x08U) driver.DrivePowerHigh();
			else driver.DriveResetHigh();
			state = 0;
			counter = 0;
			return;
		}
	}

	if (state & 0x02U)
	{
		if (++counter >= HR_LO_TIM)
		{
			driver.DrivePowerHigh();
			state = 0x14;
			counter = 0;
			return;
		}
	}

	if (state & 0x04U)
	{
		if (++counter >= HR_HI_TIM)
		{
			driver.DrivePowerLow();
			state = 0x19;
			counter = 0;
		}
	}
}