#include "LedController.h"

#ifndef SLOW_BLINK_TIMEOUT
#define SLOW_BLINK_TIMEOUT 500U
#endif

#ifndef FAST_BLINK_TIMEOUT
#define FAST_BLINK_TIMEOUT 250U
#endif


LedController::LedController(Timer& timer, GpioDriver& driver): timer(timer), driver(driver), state(0), counter(0)
{
	timer.SubscribeOnElapse(*this);
}

LedController::~LedController()
{
	timer.UnsubscribeOnElapse(*this);
}

void LedController::Off()
{
	state = 0x00U;
	driver.DriveLedLow();
}

void LedController::Glow()
{
	state = 0x00U;
	driver.DriveLedHigh();
}

void LedController::BlinkFast()
{
	state = 0x01U;
	driver.DriveLedLow();
}

void LedController::BlinkSlow()
{
	state = 0x02U;
	driver.DriveLedLow();
}

void LedController::Callback(uint8_t data)
{
	if (!state) return;

	state & 0x01U ? Blink(FAST_BLINK_TIMEOUT) : Blink(SLOW_BLINK_TIMEOUT);
}

void LedController::Blink(uint32_t timeout)
{
	if (++counter >= timeout)
	{
		(state ^= 0x04U) & 0x04U
			? driver.DriveLedHigh()
			: driver.DriveLedLow();
		counter = 0x00U;
	}
}
