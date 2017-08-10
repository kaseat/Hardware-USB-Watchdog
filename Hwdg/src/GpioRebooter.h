#pragma once

#include <stdint.h>
#include "IRebooter.h"
#include "ISubscriber.h"

class GpioRebooter : public ISubscriber, public IRebooter
{
public:
	GpioRebooter();
	~GpioRebooter();
	void HardReset();
	void SoftReset();

private:
	bool firstRun;
	bool srMarker;
	bool hrMarker;
	void Callback(uint8_t data);
};