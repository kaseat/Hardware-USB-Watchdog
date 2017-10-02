#include "LedController.h"

#ifndef SLOW_BLINK_TIMEOUT
#define SLOW_BLINK_TIMEOUT 1000U
#endif

#ifndef MID_BLINK_TIMEOUT
#define MID_BLINK_TIMEOUT 500U
#endif

#ifndef FAST_BLINK_TIMEOUT
#define FAST_BLINK_TIMEOUT 250U
#endif

#define INITIAL 0x00U
#define FAST_BLINK 0x01U
#define MID_BLINK 0x02U
#define SLOW_BLINK 0x04U
#define IS_LED_HIGH 0x08U

LedController::LedController(Timer& timer, GpioDriver& driver): timer(timer), driver(driver), state(INITIAL), counter(INITIAL)
{
	timer.SubscribeOnElapse(*this);
}

LedController::~LedController()
{
	timer.UnsubscribeOnElapse(*this);
}

void  LedController::Off()
{
	state = INITIAL;
	driver.DriveLedLow();
}

void  LedController::Glow()
{
	state = INITIAL;
	driver.DriveLedHigh();
}

void  LedController::BlinkFast()
{
	state = FAST_BLINK;
	driver.DriveLedLow();
}

void  LedController::BlinkMid()
{
	state = MID_BLINK;
	driver.DriveLedLow();
}

void  LedController::BlinkSlow()
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
