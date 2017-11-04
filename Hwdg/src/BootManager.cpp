// Copyright 2017 Oleg Petrochenko
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "BootManager.h"

#define BOOT_PULSE_TIMEOUT            ((uint_fast16_t)3000)
#define MID_PULSE_TIMEOUT             ((uint_fast16_t)2000)
#define CMD_PULSE_TIMEOUT             ((uint_fast16_t)1000)
#define INITIAL                       ((uint_fast16_t)0)

BootManager::BootManager(ResetController& rctr, SettingsManager& smgr): counter(INITIAL)
{
	uint8_t settings[4];
	*reinterpret_cast<uint32_t*>(settings) = smgr.ObtainUserSettings();

	// If we apply user settings at startupp we must config reset controller,
	// event manager and LED controller.
	if (settings[3] & APPLY_SETTINGS_AT_ATARTUP)
	{
		settings[3] & LED_DISABLED
			? rctr.GetLedController().Disable()
			: rctr.GetLedController().Enable();
		rctr.SetRebootTimeout(settings[0]);
		rctr.SetResponseTimeout(settings[1] >> 2);
		rctr.SetSoftResetAttempts(settings[2] >> 5);
		rctr.SetHardResetAttempts(settings[2] >> 2);

		if (settings[2] & 1) rctr.EnableHardReset();
		if (settings[3] & EVENTS_ENABLED) rctr.EnableEvents();
	}
	else
	{
		rctr.GetLedController().Enable();
	}

	// Subscribe on timer to get 1 ms timebase.
	rctr.GetRebooter().GetTimer().SubscribeOnElapse(*this);

	// If we're going to send any pulse we need to wait few seconds before send pulse.
	if (settings[3] & RST_PULSE_ENABLED || settings[3] & PWR_PULSE_ENABLED)
		while (counter < BOOT_PULSE_TIMEOUT)
			;

	// Send PWR pulse
	counter = INITIAL;
	if (settings[3] & PWR_PULSE_ENABLED)
		while (rctr.GetRebooter().PwrPulse() == Busy)
			if (counter > CMD_PULSE_TIMEOUT) return;

	// If we send pulse both PWR and RST we need to wait few seconds between pulses.
	if (settings[3] & RST_PULSE_ENABLED && settings[3] & PWR_PULSE_ENABLED)
		while (counter < BOOT_PULSE_TIMEOUT)
			;

	// Send RST pulse
	counter = INITIAL;
	if (settings[3] & RST_PULSE_ENABLED)
		while (rctr.GetRebooter().SoftReset() == Busy)
			if (counter > CMD_PULSE_TIMEOUT) return;

	// Unsubscribe on timer elapse as we no longer need time base.
	rctr.GetRebooter().GetTimer().UnsubscribeOnElapse(*this);
}

BootManager::~BootManager()
{
}

void BootManager::Callback(uint8_t data)
{
	counter++;
}
