#include "HddController.h"

#ifndef DEFAULT_HDD_TIMEOUT
#define DEFAULT_HDD_TIMEOUT            ((uint32_t)300000UL)
#endif

#define DEFAULT_HDD_REBOOT_ATTEMPTS    ((uint8_t)3)
#define HDD_REBOOT_ATTEMPTS_MASK       ((uint8_t)3)
#define HDD_MONITORING                 ((uint8_t)(1 << 5))
#define HDD_PAUSE                      ((uint8_t)(1 << 6))
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
	hddAttempt(DEFAULT_HDD_REBOOT_ATTEMPTS),
	status(DEFAULT_HDD_REBOOT_ATTEMPTS)
{
	HddController::exti.SubscribeOnExti(*this);
	HddController::timer.SubscribeOnElapse(*this);
}

Response HddController::StartHddMonitor()
{
	__disableInterrupts
	auto result = Busy;
	if(!(status & HDD_MONITORING))
	{
		status = HDD_MONITORING;
		counter = HDD_DEFAULT;
		result = EnableHddMonitorOk;
	}
	__enablrInterrupts
	return result;
}

Response HddController::StopHddMonitor()
{
	__disableInterrupts
	auto result = Busy;
	if (status & HDD_MONITORING)
	{
		status &= ~HDD_MONITORING;
		result = DisableHddMonitorOk;
	}
	__enablrInterrupts
	return result;
}

void HddController::PauseHddMonitor()
{
	__disableInterrupts
	status |= HDD_PAUSE;
	__enablrInterrupts
}

void HddController::ResumeHddMonitor()
{
	__disableInterrupts
	status &= ~HDD_PAUSE;
	__enablrInterrupts
}

uint8_t HddController::GetStatus()
{
	__disableInterrupts
	auto result = ((hddAttempt - 1) << 3) |
		(status & (HDD_MONITORING | HDD_PAUSE)) |
		(hddTimeout / HDD_TIMEOUT_TIMEBASE - HDD_TIMEOUT_MIN);
	__enablrInterrupts
	return result;
}

Response HddController::SetTimeout(uint8_t command)
{
	__disableInterrupts
	auto result = Busy;
	if (!(status & HDD_MONITORING))
	{
		hddTimeout = ((command & HDD_TIMEOUT_MASK) + HDD_TIMEOUT_MIN) * HDD_TIMEOUT_TIMEBASE;
		result = SetHddTimeoutOk;
	}
	__enablrInterrupts
	return result;
}

Response HddController::SetRebootAttempts(uint8_t command)
{
	__disableInterrupts
	auto result = Busy;
	if (!(status & HDD_MONITORING))
	{
		hddAttempt = (command & HDD_REBOOT_ATTEMPTS_MASK) + 1;
		result = SetHddAttemptsOk;
	}
	__enablrInterrupts
	return result;
}

HddController::~HddController()
{
	exti.UnsubscribeOnExti();
	timer.UnsubscribeOnElapse(*this);
}

void HddController::Callback(uint8_t data)
{
	__disableInterrupts
	if(!(status&HDD_PAUSE))
	{
		if (data == CALLBACK_TIMER && status & HDD_MONITORING && ++counter > hddTimeout)
		{
			counter = HDD_DEFAULT;
			hddAttempt > (status++ & HDD_REBOOT_ATTEMPTS_MASK) ? rebooter.SoftReset() : status = HDD_DEFAULT;
		}

		if (data == CALLBACK_EXTI)
		{
			counter = HDD_DEFAULT;
		}
	}
	__enablrInterrupts
}