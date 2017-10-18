#include "ResetController.h"
#include "Timer.h"
#include "IResetControllerEventHandler.h"

#ifndef RESPONSE_DEF_TIMEOUT
// Default response timeout, ms
#define RESPONSE_DEF_TIMEOUT   ((uint32_t)10000UL)
#endif
#ifndef REBOOT_DEF_TIMEOUT
// Default reboot timeout, ms
#define REBOOT_DEF_TIMEOUT     ((uint32_t)150000UL)
#endif
#ifndef REBOOT_MIN_TIMEOUT
// Least reboot timeout value, ms
#define REBOOT_MIN_TIMEOUT     ((uint32_t)10000UL)
#endif

#ifndef SR_TIMEBASE
// Soft reset time base, ms
#define SR_TIMEBASE            ((uint32_t)5000UL)
#endif
#ifndef HR_TIMEBASE
// Hard reset time base, ms
#define HR_TIMEBASE            ((uint32_t)5000UL)
#endif

#ifndef SR_ATTEMPTS
// Default soft reset attempts count
#define SR_ATTEMPTS            ((uint8_t)3U)
#endif
#ifndef HR_ATTEMPTS
// Default hard reset attempts count
#define HR_ATTEMPTS            ((uint8_t)3U)
#endif

// RSM enabled
#define ENABLED                ((uint8_t)(1U << 0U))
// Response timeout elapsed
#define RESPONSE_ELAPSED       ((uint8_t)(1U << 1U))
// Hard reset enabled
#define HR_ENABLED             ((uint8_t)(1U << 2U))
// Response timeout elapsed
#define HDD_MONITOR            ((uint8_t)(1U << 3U))
// Response timeout elapsed
#define LED_STARDED            ((uint8_t)(1U << 4U))
// Output bit
#define OUTPUT_BIT             ((uint8_t)(0U << 7U))


// Mask applied to extract attempts value
#define ATTEMPTS_MASK          ((uint8_t)0x07U)
// Mask applied to extract reboot timeout value
#define REBOOT_MASK            ((uint8_t)0x7FU)
// Mask applied to extract response timeout value
#define RESPONSE_MASK          ((uint8_t)0x3FU)
// Reset value
#define INITIAL                ((uint8_t)0x00U)


// Reset value
#define FIRST_RESET_OCCURED                ((uint8_t)0x30U)
#define SOFT_RESET_OCCURED                ((uint8_t)0x31U)
#define HARD_RESET_OCCURED                ((uint8_t)0x32U)

ResetController::ResetController(Timer & timer, Rebooter & rb, LedController & ledController, Exti & exti)
	: timer(timer)
	, rebooter(rb)
	, ledController(ledController)
	, exti(exti)
	, counter(INITIAL)
	, state(INITIAL)
	, responseTimeout(RESPONSE_DEF_TIMEOUT)
	, rebootTimeout(REBOOT_DEF_TIMEOUT)
	, sAttempt(SR_ATTEMPTS)
	, hAttempt(HR_ATTEMPTS)
	, sAttemptCurr(SR_ATTEMPTS)
	, hAttemptCurr(HR_ATTEMPTS)
{
	eventHandler = nullptr;
	ResetController::timer.SubscribeOnElapse(*this);
}

ResetController::~ResetController()
{
	timer.UnsubscribeOnElapse(*this);
	exti.UnsubscribeOnExti();
}

uint32_t ResetController::GetStatus()
{
	uint32_t result = 0;
	uint8_t* rs = reinterpret_cast<uint8_t*>(&result);
	rs[0] = (rebootTimeout - REBOOT_MIN_TIMEOUT) / HR_TIMEBASE | OUTPUT_BIT;
	rs[1] = (responseTimeout / SR_TIMEBASE - 1) << 2 | state & 0x03;
	rs[2] = (sAttemptCurr - 1) << 5 | (hAttemptCurr - 1) << 2 | (state & 0x0C) >> 2;
	return result;
}

Response ResetController::Start()
{
  if (state & ENABLED) return Busy;
	counter = INITIAL;
	state &= ~(ENABLED | RESPONSE_ELAPSED | LED_STARDED);
	state |= ENABLED;
	sAttempt = sAttemptCurr;
	hAttempt = hAttemptCurr;
	ledController.BlinkSlow();
	return StartOk;
}

Response ResetController::Stop()
{
	ledController.Glow();
	state &= ~(ENABLED | RESPONSE_ELAPSED | LED_STARDED);
	return StopOk;
}

Response ResetController::EnableHddMonitor()
{
	if (state & ENABLED) return Busy;
	exti.SubscribeOnExti(*this);
	state |= HDD_MONITOR;
	counter = INITIAL;
	return EnableHddMonitorOk;
}

Response ResetController::DisableHddMonitor()
{
	if (state & ENABLED) return Busy;
	exti.UnsubscribeOnExti();
	state &= ~HDD_MONITOR;
	return DisableHddMonitorOk;
}

Response ResetController::EnableHardReset()
{
	if (state & ENABLED) return Busy;
	state |= HR_ENABLED;
	return EnableHardResetOk;
}

Response ResetController::DisableHardReset()
{
	if (state & ENABLED) return Busy;
	state &= ~HR_ENABLED;
	return DisableHardResetOk;
}

Response ResetController::Ping()
{
	if (!(state & ENABLED) || state & RESPONSE_ELAPSED) return Busy;
	counter = INITIAL;
	return PingOk;
}

Response ResetController::SetResponseTimeout(uint8_t timeout)
{
	if (state & ENABLED) return Busy;
	responseTimeout = ((timeout & RESPONSE_MASK) + 1) * SR_TIMEBASE;
	return SetResponseTimeoutOk;
}

Response ResetController::SetRebootTimeout(uint8_t timeout)
{
	if (state & ENABLED) return Busy;
	rebootTimeout = REBOOT_MIN_TIMEOUT + (timeout & REBOOT_MASK) * HR_TIMEBASE;
	return SetRebootTimeoutOk;
}

Response ResetController::SetSoftResetAttempts(uint8_t attempts)
{
	if (state & ENABLED) return Busy;
	sAttemptCurr = (attempts & ATTEMPTS_MASK) + 1;
	return SetSoftResetAttemptsOk;
}

Response ResetController::SetHardResetAttempts(uint8_t attempts)
{
	if (state & ENABLED) return Busy;
	hAttemptCurr = (attempts & ATTEMPTS_MASK) + 1;
	return SetHardResetAttemptsOk;
}

void ResetController::SubscribeOnEvents(ISubscriber& eventHandler)
{
	ResetController::eventHandler = &eventHandler;
}

void ResetController::UnSubscribeOnEvents()
{
	eventHandler = nullptr;
}

void ResetController::Callback(uint8_t data)
{
	if (!(state & ENABLED)) return;

	counter++;

	if (!(state & RESPONSE_ELAPSED) && counter >= responseTimeout)
	{
		counter = INITIAL;
		sAttempt--;
		rebooter.SoftReset();
		ledController.BlinkMid();
		state |= RESPONSE_ELAPSED;
		exti.UnsubscribeOnExti();
		if (eventHandler != nullptr)
			eventHandler->Callback(FIRST_RESET_OCCURED);
	}
	else if (state & RESPONSE_ELAPSED && counter >= rebootTimeout)
	{
		counter = INITIAL;
		if (sAttempt > 0)
		{
			sAttempt--;
			rebooter.SoftReset();
			if (eventHandler != nullptr)
				eventHandler->Callback(SOFT_RESET_OCCURED);
		}
		else if (state & HR_ENABLED && hAttempt > 0)
		{
			hAttempt--;
			if (!(state & LED_STARDED))
			{
				state |= LED_STARDED;
				ledController.BlinkFast();
			}
			rebooter.HardReset();
			if (eventHandler != nullptr)
				eventHandler->Callback(HARD_RESET_OCCURED);
		}
		else
		{
			ledController.Glow();
			state &= ~(ENABLED | RESPONSE_ELAPSED | LED_STARDED);
		}
	}
}

void ResetController::OnExtiInterrupt()
{
	if (!(state & ENABLED) || state & RESPONSE_ELAPSED) return;
	counter = INITIAL;
}
