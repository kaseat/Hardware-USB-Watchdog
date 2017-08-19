#include "GpioRebooter.h"
#include "Timer.h"


#define RST_TIM 0xC8U
#define HR_LO_TIM 0x1770U
#define HR_HI_TIM 0x7D0U

GpioRebooter::GpioRebooter(GpioDriver driver) :driver(driver), state(0), counter(0)
{
	Timer::SubscribeOnElapse(*this);
}

GpioRebooter::~GpioRebooter()
{
	Timer::UnsubscribeOnElapse(*this);
}

void GpioRebooter::HardReset()
{
	if (state & 0x10) return;
	driver.DrivePowerLow();
	state = 0x12;
}

void GpioRebooter::SoftReset()
{
	if (state & 0x10) return;
	driver.DriveResetLow();
	state = 0x11;
}

void GpioRebooter::Callback(uint8_t data)
{
	if (state & 0x01U)
	{
		if (++counter > RST_TIM)
		{
			if (state & 0x08U) driver.DrivePowerHigh();
			else driver.DriveResetHigh();
			state = 0;
			counter = 0;
		}
	}

	if (state & 0x02U)
	{
		if (++counter > HR_LO_TIM)
		{
			driver.DrivePowerHigh();
			state = 0x14;
			counter = 0;
		}
	}

	if (state & 0x04U)
	{
		if (++counter > HR_HI_TIM)
		{
			driver.DrivePowerLow();
			state = 0x19;
			counter = 0;
		}
	}
}