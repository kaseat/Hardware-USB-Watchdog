// Copyright 2017 Oleg Petrochenko
// 
// This file is part of HwdgTiny.
// 
// HwdgTiny is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
// 
// HwdgTiny is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with HwdgTiny. If not, see <http://www.gnu.org/licenses/>.

#include "ResetController.h"
#include "LedController.h"
#include "Rebooter.h"

// Default response timeout, ms
#define RESPONSE_DEF_TIMEOUT   ((uint32_t)10000UL/2)
// Default reboot timeout, ms
#define REBOOT_DEF_TIMEOUT     ((uint32_t)15000UL/2)
// Least reboot timeout value, ms
#define REBOOT_MIN_TIMEOUT     ((uint32_t)10000UL/2)
// Soft reset time base, ms
#define SR_TIMEBASE            ((uint32_t)5000UL/2)
// Hard reset time base, ms
#define HR_TIMEBASE            ((uint32_t)5000UL)
// Default soft reset attempts count
#define SR_ATTEMPTS            ((uint8_t)3U)

// RSM enabled
#define ENABLED                ((uint8_t)(1U << 0U))
// Response timeout elapsed
#define RESPONSE_ELAPSED       ((uint8_t)(1U << 1U))
// Response timeout elapsed
#define HDD_MONITOR            ((uint8_t)(1U << 3U))
// Response timeout elapsed
#define LED_STARDED            ((uint8_t)(1U << 4U))


// Mask applied to extract attempts value
#define ATTEMPTS_MASK          ((uint8_t)0x07U)
// Mask applied to extract reboot timeout value
#define REBOOT_MASK            ((uint8_t)0x7FU)
// Mask applied to extract response timeout value
#define RESPONSE_MASK          ((uint8_t)0x3FU)
// Reset value
#define INITIAL                ((uint8_t)0x00U)
// hwdg event WatchdogOk elapse timeout
#define EVENT_HWDGOK_TIMEOUT   ((uint16_t)1000)

static uint32_t counter = INITIAL;
static uint16_t counterms = INITIAL;
static uint_least8_t state = INITIAL;
static uint32_t responseTimeout = RESPONSE_DEF_TIMEOUT;
static uint32_t rebootTimeout = REBOOT_DEF_TIMEOUT;
static uint8_t sAttempt = SR_ATTEMPTS;
static uint8_t sAttemptCurr = SR_ATTEMPTS;


uint32_t ResetControllerGetStatus(void)
{
	uint32_t result = INITIAL;
	uint8_t* rs = (uint8_t*)&result;
	rs[1] = (responseTimeout / SR_TIMEBASE - 1) << 2 | (state & 0x03);
	rs[2] = (sAttemptCurr - 1) << 5 | (state & 0x0C) >> 2;
	return result;
}

Response_t ResetControllerStart(void)
{
	if (state & ENABLED) return Busy;
	counter = INITIAL;
	state &= ~(ENABLED | RESPONSE_ELAPSED | LED_STARDED);
	state |= ENABLED;
	sAttempt = sAttemptCurr;
	LedControllerBlinkSlow();
	return StartOk;
}

Response_t ResetControllerStop(void)
{
	LedControllerGlow();
	state &= ~(ENABLED | RESPONSE_ELAPSED | LED_STARDED);
	return StopOk;
}

Response_t ResetControllerPing(void)
{
	if (!(state & ENABLED) || state & RESPONSE_ELAPSED) return Busy;
	counter = INITIAL;
	return PingOk;
}

Response_t ResetControllerSetResponseTimeout(const uint8_t timeout)
{
	if (state & ENABLED) return Busy;
	responseTimeout = ((timeout & RESPONSE_MASK) + 1) * SR_TIMEBASE;
	return SetResponseTimeoutOk;
}

Response_t ResetControllerSetRebootTimeout(const uint8_t timeout)
{
	if (state & ENABLED) return Busy;
	rebootTimeout = REBOOT_MIN_TIMEOUT + (timeout & REBOOT_MASK) * HR_TIMEBASE;
	return SetRebootTimeoutOk;
}

Response_t ResetControllerSetSoftResetAttempts(const uint8_t attempts)
{
	if (state & ENABLED) return Busy;
	sAttemptCurr = (attempts & ATTEMPTS_MASK) + 1;
	return SetSoftResetAttemptsOk;
}

Response_t ResetControllerTestSoftReset(void)
{
	ResetControllerStop();
	return RebooterSoftReset();
}

/**
 * \brief This function must be called every 1ms.
 */
void ResetControllerTimebase(void)
{
	// Reset controller FSM logic
	if (!(state & ENABLED)) return;

	counter++;

	if (!(state & RESPONSE_ELAPSED) && counter >= responseTimeout)
	{
		counter = INITIAL;
		sAttempt--;
		RebooterSoftReset();
		LedControllerBlinkMid();
		state |= RESPONSE_ELAPSED;
	}
	else if (state & RESPONSE_ELAPSED && counter >= rebootTimeout)
	{
		counter = INITIAL;
		if (sAttempt > 0)
		{
			sAttempt--;
			RebooterSoftReset();
		}
		else
		{
			LedControllerGlow();
			state &= ~(ENABLED | RESPONSE_ELAPSED | LED_STARDED);
		}
	}
}
