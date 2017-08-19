#pragma once

#include <stdint.h>
#include "IRebooter.h"
#include "ISubscriber.h"
#include "../GpioDriver.h"

class GpioRebooter : public ISubscriber, public IRebooter
{
public:
	GpioRebooter(GpioDriver driver);
	~GpioRebooter();
	void HardReset();
	void SoftReset();

private:
	GpioDriver& driver;
	uint_least8_t state;
	uint_fast16_t counter;
	void Callback(uint8_t data);
};
