#include "GpioRebooter.h"
#include "ResetTimer.h"

#define FR_TIMEOUT 0x10U
#define RST_TIM 0x20U
#define HR_LO_TIM 0xC00U
#define HR_HI_TIM 0x600U

GpioRebooter::GpioRebooter()
{

	firstRun = true;
	srMarker = false;
	hrMarker = false;
	ResetTimer::SubscribeOnElapse(*this);
	ResetTimer::Run(FR_TIMEOUT);
}

GpioRebooter::~GpioRebooter()
{
	ResetTimer::UnsubscribeOnElapse();
}

void GpioRebooter::HardReset()
{

	srMarker = false;
	hrMarker = true;
	ResetTimer::Run(HR_LO_TIM);
}

void GpioRebooter::SoftReset()
{

	srMarker = false;
	hrMarker = false;
	ResetTimer::Run(RST_TIM);
}

void GpioRebooter::Callback(uint8_t data)
{
	// This only need for the first timer run, as timer
	// incorrect behaviour during first run is observed.
	if (firstRun)
	{
		firstRun = false;
		srMarker = false;
		hrMarker = false;
		ResetTimer::Stop();
		return;
	}

	ResetTimer::Stop();

	// usually we reach this point after pin been pulled down
	// so we have to bring pin in floating input mode


	// finally make soft reset on PWR pin
	if (srMarker)
	{

		ResetTimer::Run(RST_TIM);
		srMarker = false;
		hrMarker = false;
	}

	// if we have hard reset we should wait couple of
	// seconds and make soft reset on PWR pin (see above)
	if (hrMarker)
	{
		ResetTimer::Run(HR_HI_TIM);
		hrMarker = false;
		srMarker = true;
	}
}