#include "ResetController.h"
#include "Timer.h"

#define SR_DEF_TIMEOUT 10000U
#define SR_TIMEBASE 1000U
#define SR_MASK 0x3FU
#define SR_ATTEMPTS 3

#define HR_DEF_TIMEOUT 150000U
#define HR_TIMEBASE 5000U
#define HR_MASK 0x7FU
#define HR_MIN_TIMEOUT 10000U
#define HR_ATTEMPTS 3

#define ATTEMPTS_MASK 0x07U

#define CNTR_RST 0

ResetController::ResetController(Timer & time, Rebooter & rb, LedController & ledController)
	: timer(time)
	, rebooter(rb)
	, ledController(ledController)
	, counter(CNTR_RST)
	, softResetTimeout(SR_DEF_TIMEOUT)
	, hardResetTimeout(HR_DEF_TIMEOUT)
	, softRebootEnabled(false)
	, hardRebootEnabled(false)
	, softRebootOccured(false)
	, sAttempt(SR_ATTEMPTS)
	, hAttempt(HR_ATTEMPTS)
	, sAttemptDef(SR_ATTEMPTS)
	, hAttemptDef(HR_ATTEMPTS)
{
	timer.SubscribeOnElapse(*this);
}

ResetController::~ResetController()
{
	timer.UnsubscribeOnElapse(*this);
}

void ResetController::EnableSoftReset()
{
	ledController.BlinkSlow();
	counter = CNTR_RST;
	sAttempt = sAttemptDef;
	hAttempt = hAttemptDef;
	softRebootEnabled = true;
	softRebootOccured = false;
}

void ResetController::DisableSoftReset()
{
	ledController.Off();
	softRebootEnabled = false;
	softRebootOccured = false;
}

void ResetController::EnableHardReset()
{
	counter = CNTR_RST;
	hAttempt = hAttemptDef;
	hardRebootEnabled = true;
}

void ResetController::DisableHardReset()
{
	hardRebootEnabled = false;
}

void ResetController::Ping()
{
	counter = CNTR_RST;
	softRebootOccured = false;
}

void ResetController::SetResponseTimeout(uint8_t timeout)
{
	counter = CNTR_RST;
	softResetTimeout = ((timeout & SR_MASK) + 1) * SR_TIMEBASE;
}

void ResetController::SetResetTimeout(uint8_t timeout)
{
	counter = CNTR_RST;
	hardResetTimeout = HR_MIN_TIMEOUT + (timeout & HR_MASK) * HR_TIMEBASE;
}

void ResetController::SetSoftResetAttempts(uint8_t attempts)
{
	sAttempt = sAttemptDef = (attempts & ATTEMPTS_MASK) + 1;
}

void ResetController::SetHardResetAttempts(uint8_t attempts)
{
	hAttempt = hAttemptDef = (attempts & ATTEMPTS_MASK) + 1;
}

void ResetController::Callback(uint8_t data)
{
	if (softRebootEnabled && !softRebootOccured && ++counter >= softResetTimeout)
	{
		counter = CNTR_RST;
		rebooter.SoftReset();
		ledController.BlinkFast();
		sAttempt--;
		softRebootOccured = true;
	}
	if (softRebootOccured && ++counter >= hardResetTimeout)
	{
		counter = CNTR_RST;
		if (hardRebootEnabled && sAttempt >= 0 && hAttempt > 0)
		{
			hAttempt--;
			rebooter.HardReset();
		}
		else if (sAttempt > 0)
		{
			sAttempt--;
			rebooter.SoftReset();
		}
	}
}