#pragma once

#include <stdint.h>
#include "IRebooter.h"
#include "ISubscriber.h"
#include "GpioDriver.h"

class Rebooter : public ISubscriber, public IRebooter
{
public:
	Rebooter(GpioDriver& driver);
	~Rebooter();
	void HardReset();
	void SoftReset();

private:
	GpioDriver& driver;
	uint_least8_t state;
	uint_fast16_t counter;
	void Callback(uint8_t data);
};
