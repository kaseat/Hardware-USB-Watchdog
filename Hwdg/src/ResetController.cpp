#include "ResetController.h"
#include "Timer.h"

#ifndef RESPONSE_DEF_TIMEOUT
// Default response timeout, ms
#define RESPONSE_DEF_TIMEOUT 10000U
#endif
#ifndef REBOOT_DEF_TIMEOUT
// Default reboot timeout, ms
#define REBOOT_DEF_TIMEOUT 150000U
#endif
#ifndef REBOOT_MIN_TIMEOUT
// Least reboot timeout value, ms
#define REBOOT_MIN_TIMEOUT 10000U
#endif

#ifndef SR_TIMEBASE
// Soft reset time base, ms
#define SR_TIMEBASE 5000U
#endif
#ifndef HR_TIMEBASE
// Hard reset time base, ms
#define HR_TIMEBASE 5000U
#endif

#ifndef SR_ATTEMPTS
// Default soft reset attempts count
#define SR_ATTEMPTS 3U
#endif
#ifndef HR_ATTEMPTS
// Default hard reset attempts count
#define HR_ATTEMPTS 3U
#endif

// RSM enabled
#define ENABLED (1U << 0U)
// Hard reset enabled
#define HR_ENABLED (1U << 1U)
// Response timeout elapsed
#define RESPONSE_ELAPSED (1U << 2U)

// Mask applied to extract attempts value
#define ATTEMPTS_MASK 0x07U
// Mask applied to extract reboot timeout value
#define REBOOT_MASK 0x7FU
// Mask applied to extract response timeout value
#define RESPONSE_MASK 0x3FU
// Reset value
#define INITIAL 0x00U

ResetController::ResetController(Timer & time, Rebooter & rb, LedController & ledController)
	: timer(time)
	, rebooter(rb)
	, ledController(ledController)
	, counter(INITIAL)
	, state(INITIAL)
	, responseTimeout(RESPONSE_DEF_TIMEOUT)
	, rebootTimeout(REBOOT_DEF_TIMEOUT)
	, sAttempt(SR_ATTEMPTS)
	, hAttempt(HR_ATTEMPTS)
	, sAttemptCurr(SR_ATTEMPTS)
	, hAttemptCurr(HR_ATTEMPTS)
{
	timer.SubscribeOnElapse(*this);
}

ResetController::~ResetController()
{
	timer.UnsubscribeOnElapse(*this);
}

Response ResetController::Start()
{
	if (state & ENABLED) return Response::Busy;
	counter = INITIAL;
	state |= ENABLED;
	sAttempt = sAttemptCurr;
	hAttempt = hAttemptCurr;
	ledController.BlinkSlow();
	return Response::StartOk;
}

Response ResetController::Stop()
{
	ledController.Glow();
	state &= ~ENABLED;
	return Response::StopOk;
}

Response ResetController::EnableHardReset()
{
	if (state & ENABLED) return Response::Busy;
	state |= HR_ENABLED;
	return Response::EnableHardResetOk;
}

Response ResetController::DisableHardReset()
{
	if (state & ENABLED) return Response::Busy;
	state &= ~(HR_ENABLED);
	return Response::DisableHardResetOk;
}

Response ResetController::Ping()
{
	if (!(state & ENABLED)) return Response::Busy;
	counter = INITIAL;
	state &= ~(RESPONSE_ELAPSED);
	return Response::PingOk;
}

Response ResetController::SetResponseTimeout(uint8_t timeout)
{
	if (state & ENABLED) return Response::Busy;
	responseTimeout = ((timeout & RESPONSE_MASK) + 1) * SR_TIMEBASE;
	return Response::SetResponseTimeoutOk;
}

Response ResetController::SetRebootTimeout(uint8_t timeout)
{
	if (state & ENABLED) return Response::Busy;
	rebootTimeout = REBOOT_MIN_TIMEOUT + (timeout & REBOOT_MASK) * HR_TIMEBASE;
	return Response::SetRebootTimeoutOk;
}

Response ResetController::SetSoftResetAttempts(uint8_t attempts)
{
	if (state & ENABLED) return Response::Busy;
	sAttemptCurr = (attempts & ATTEMPTS_MASK) + 1;
	return Response::SetSoftResetAttemptsOk;
}

Response ResetController::SetHardResetAttempts(uint8_t attempts)
{
	if (state & ENABLED) return Response::Busy;
	hAttemptCurr = (attempts & ATTEMPTS_MASK) + 1;
	return Response::SetHardResetAttemptsOk;
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
		ledController.BlinkFast();
		state |= RESPONSE_ELAPSED;
	}
	else if (state & RESPONSE_ELAPSED && counter >= rebootTimeout)
	{
		counter = INITIAL;
		if (sAttempt > 0)
		{
			sAttempt--;
			rebooter.SoftReset();
		}
		else if (state & HR_ENABLED && hAttempt > 0)
		{
			hAttempt--;
			rebooter.HardReset();
		}
		else
		{
			ledController.Glow();
			state &= ~(ENABLED | RESPONSE_ELAPSED);
		}
	}
}