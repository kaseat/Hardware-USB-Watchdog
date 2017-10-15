#include "HddController.h"

#ifndef DEFAULT_HDD_TIMEOUT
#define DEFAULT_HDD_TIMEOUT            ((uint32_t)300000UL)
#endif

#define DEFAULT_HDD_REBOOT_ATTEMPTS    ((uint8_t)3)
#define HDD_REBOOT_ATTEMPTS_MASK       ((uint8_t)3)
#define HDD_MONITORING                 ((uint8_t)(1 << 2))
#define HDD_DEFAULT                    ((uint8_t)0)
#define CALLBACK_TIMER                 ((uint8_t)0)
#define CALLBACK_EXTI                  ((uint8_t)1)
#define HDD_TIMEOUT_MASK               ((uint8_t)0x07U)
#define HDD_TIMEOUT_TIMEBASE           ((uint32_t)60000UL)
#define HDD_TIMEOUT_MIN                ((uint8_t)3)

HddController::HddController(Exti & exti, Timer & timer, Rebooter & rebooter)
	:exti(exti),
	timer(timer),
	rebooter(rebooter),
	counter(HDD_DEFAULT),
	hddTimeout(DEFAULT_HDD_TIMEOUT),
	status(DEFAULT_HDD_REBOOT_ATTEMPTS)
{
	HddController::exti.SubscribeOnExti(*this);
	HddController::timer.SubscribeOnElapse(*this);
}

Response HddController::StartHddMonitor()
{
	if (status & HDD_MONITORING) return Busy;
	status = DEFAULT_HDD_REBOOT_ATTEMPTS | HDD_MONITORING;
	counter = HDD_DEFAULT;
	return EnableHddMonitorOk;
}

Response HddController::StopHddMonitor()
{
	if (!(status & HDD_MONITORING)) return Busy;
	status &= ~HDD_MONITORING;
	return DisableHddMonitorOk;
}

uint8_t HddController::GetStatus()
{
	return status << 3 | hddTimeout / HDD_TIMEOUT_TIMEBASE - HDD_TIMEOUT_MIN;
}

Response HddController::SetTimeout(uint8_t command)
{
	if (status & HDD_MONITORING) return Busy;
	hddTimeout = ((command & HDD_TIMEOUT_MASK) + HDD_TIMEOUT_MIN) * HDD_TIMEOUT_TIMEBASE;
	return SetHddTimeoutOk;
}

HddController::~HddController()
{
	exti.UnsubscribeOnExti();
	timer.UnsubscribeOnElapse(*this);
}

void HddController::Callback(uint8_t data)
{
	if (data == CALLBACK_TIMER && status & HDD_MONITORING && ++counter > hddTimeout)
	{
		counter = HDD_DEFAULT;
		status-- & HDD_REBOOT_ATTEMPTS_MASK ? rebooter.SoftReset() : status = HDD_DEFAULT;
	}

	if (data == CALLBACK_EXTI)
	{
		counter = HDD_DEFAULT;
	}
}
