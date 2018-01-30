// Copyright 2018 Oleg Petrochenko
// 
// This file is part of Hwdg.
// 
// Hwdg is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
// 
// Hwdg is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Hwdg. If not, see <http://www.gnu.org/licenses/>.

#include "ResetController.h"
#include "Timer.h"

#ifndef RESPONSE_DEF_TIMEOUT
// Default response timeout, ms
#define RESPONSE_DEF_TIMEOUT   ((uint32_t)90000UL)
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
// EXTI occured
#define EXTI_OCCURRED          ((uint8_t)(1U << 5U))

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
// The period of time we disable EXTI after first interrupt occurred
#define EXTI_DISABLE_TIMEOUT   ((uint16_t)1000)

ResetController::ResetController(Uart& uart, Rebooter& rb, LedController& ledController, Exti& exti) :
	eventsEnabled(false),
	uart(uart),
	rebooter(rb),
	ledController(ledController),
	exti(exti),
	counter(INITIAL),
	counterExti(INITIAL),
	counterms(INITIAL),
	state(INITIAL),
	responseTimeout(RESPONSE_DEF_TIMEOUT),
	rebootTimeout(REBOOT_DEF_TIMEOUT),
	sAttempt(SR_ATTEMPTS),
	hAttempt(HR_ATTEMPTS),
	sAttemptCurr(SR_ATTEMPTS),
	hAttemptCurr(HR_ATTEMPTS)
{
	rebooter.GetTimer().SubscribeOnElapse(*this);
}

ResetController::~ResetController()
{
	rebooter.GetTimer().UnsubscribeOnElapse(*this);
}

uint32_t ResetController::GetStatus()
{
	uint32_t result = INITIAL;
	uint8_t* rs = reinterpret_cast<uint8_t*>(&result);
	rs[0] = (rebootTimeout - REBOOT_MIN_TIMEOUT) / HR_TIMEBASE;
	rs[1] = (responseTimeout / SR_TIMEBASE - 1) << 2 | (state & 0x03);
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
	if (state & HDD_MONITOR)
	{
		exti.SubscribeOnExti(*this);
		state &= ~EXTI_OCCURRED;
	}
	return StartOk;
}

Response ResetController::Stop()
{
	ledController.Glow();
	state &= ~(ENABLED | RESPONSE_ELAPSED | LED_STARDED);
	if (state & HDD_MONITOR)
	{
		exti.UnsubscribeOnExti();
		state &= ~EXTI_OCCURRED;
	}
	return StopOk;
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

Response ResetController::EnableHddLedMonitor()
{
	if (state & ENABLED) return Busy;
	exti.SubscribeOnExti(*this);
	state |= HDD_MONITOR;
	state &= ~EXTI_OCCURRED;
	counter = INITIAL;
	return EnableHddMonitorOk;
}

Response ResetController::DisableHddLedMonitor()
{
	if (state & ENABLED) return Busy;
	exti.UnsubscribeOnExti();
	state &= ~(HDD_MONITOR | EXTI_OCCURRED);
	return DisableHddMonitorOk;
}

Response ResetController::TestHardReset()
{
	Stop();
	return rebooter.HardReset();
}

Response ResetController::TestSoftReset()
{
	Stop();
	return rebooter.SoftReset();
}

Response ResetController::EnableEvents()
{
	eventsEnabled = true;
	return EnableEventsOk;
}

Response ResetController::DisableEvents()
{
	eventsEnabled = false;
	return DisableEventsOk;
}

bool ResetController::IsEventsEnabled()
{
	return eventsEnabled;
}

Rebooter& ResetController::GetRebooter()
{
	return rebooter;
}

LedController& ResetController::GetLedController()
{
	return ledController;
}

void ResetController::Callback(uint8_t data)
{
	// WatchdogOk event logic
	if (eventsEnabled)
	{
		if (++counterms >= EVENT_HWDGOK_TIMEOUT)
		{
			counterms = INITIAL;
			uart.SendByte(WatchdogOk);
		}
	}

	// EXTI logic (after EXTI interrupt occurred it would be
	// better to disable EXTI interrupts for a while in order
	// prevent multiple EXTI handler call at short period of time.)
	if (state & EXTI_OCCURRED)
	{
		if (++counterExti >= EXTI_DISABLE_TIMEOUT)
		{
			state &= ~EXTI_OCCURRED;
			counterExti = INITIAL;
			exti.SubscribeOnExti(*this);
		}
	}

	// Reset controller FSM logic
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
		if (eventsEnabled)
			uart.SendByte(FirstResetOccurred);
	}
	else if (state & RESPONSE_ELAPSED && counter >= rebootTimeout)
	{
		counter = INITIAL;
		if (sAttempt > 0)
		{
			sAttempt--;
			rebooter.SoftReset();
			if (eventsEnabled)
				uart.SendByte(SoftResetOccurred);
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
			if (eventsEnabled)
				uart.SendByte(HardResetOccurred);
		}
		else
		{
			ledController.Glow();
			state &= ~(ENABLED | RESPONSE_ELAPSED | LED_STARDED);
			if (eventsEnabled)
				uart.SendByte(MovedToIdle);
		}
	}
}

void ResetController::OnExtiInterrupt()
{
	if (!(state & ENABLED) || state & RESPONSE_ELAPSED) return;
	counter = INITIAL;
	state |= EXTI_OCCURRED;
	exti.UnsubscribeOnExti();
}
