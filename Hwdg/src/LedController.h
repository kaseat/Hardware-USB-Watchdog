#pragma once
#include "ISubscriber.h"
#include "GpioDriver.h"
#include "Timer.h"

class LedController : ISubscriber
{
public:
	/**
	* \brief Create Rebooter instance.
	* \param timer Time base source.
	* \param driver GPIO pin driver.
	*/
	LedController(Timer& timer, GpioDriver& driver);
	~LedController();
	void Off();
	void Glow();
	void BlinkFast();
	void BlinkSlow();
private:
	Timer& timer;
	GpioDriver& driver;
	uint_least8_t state;
	uint_fast16_t counter;
	void Callback(uint8_t data);
	void Blink(uint32_t timeout);
};
