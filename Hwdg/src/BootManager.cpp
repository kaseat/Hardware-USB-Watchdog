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
#define LED_DISABLED         ((uint8_t)(1U << 0U))
#define DISABLE_TOKEN        ((uint8_t)0x55)
#define ENABLE_TOKEN         ((uint8_t)0xAA)
#define BOOT_PULSE_TIMEOUT   ((uint16_t)3000)
#define MID_PULSE_TIMEOUT    ((uint16_t)2000)
#define CMD_PULSE_TIMEOUT    ((uint16_t)1000)
#define INITIAL              ((uint32_t)0)

__no_init uint8_t __eeprom EnPwrPulse;
__no_init uint8_t __eeprom EnRstPulse;
__no_init uint8_t __eeprom ApplyUserSettings;

uint8_t __eeprom settings0 = 0x1C;
uint8_t __eeprom settings1 = 0x44;
uint8_t __eeprom settings2 = 0x48;
uint8_t __eeprom settings3 = 0x00;

BootManager::BootManager(ResetController& rctrl, Timer& timer) : rctrl(rctrl), counter(INITIAL)
{
	ApplyUserSettings == ENABLE_TOKEN
		? settings3 & LED_DISABLED
			? BootManager::rctrl.GetLedController().Disable()
			: BootManager::rctrl.GetLedController().Enable()
		: BootManager::rctrl.GetLedController().Enable();

	// Subscribe on timer to get 1 ms timebase.
	timer.SubscribeOnElapse(*this);

	// If we're going to send any pulse we need to wait few seconds before send pulse.
	if (EnPwrPulse == ENABLE_TOKEN || EnRstPulse == ENABLE_TOKEN)
		while (counter < BOOT_PULSE_TIMEOUT)
			;

	// Send PWR pulse
	counter = INITIAL;
	if (EnPwrPulse == ENABLE_TOKEN)
		while (BootManager::rctrl.GetRebooter().PwrPulse() == Busy)
			if (counter > CMD_PULSE_TIMEOUT) return;

	// If we send pulse both PWR and RST we need to wait few seconds between pulses.
	if (EnPwrPulse == ENABLE_TOKEN && EnRstPulse == ENABLE_TOKEN)
		while (counter < BOOT_PULSE_TIMEOUT)
			;

	// Send RST pulse
	counter = INITIAL;
	if (EnRstPulse == ENABLE_TOKEN)
		while (BootManager::rctrl.GetRebooter().SoftReset() == Busy)
			if (counter > CMD_PULSE_TIMEOUT) return;

	// Unsubscribe on timer elapse as we no longer need time base.
	timer.UnsubscribeOnElapse(*this);
}

BootManager::~BootManager()
{
}

Response BootManager::PwrPulseOnStartupEnable()
{
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (EnPwrPulse == ENABLE_TOKEN)
		return PwrPulseOnStartupEnableOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	EnPwrPulse = ENABLE_TOKEN;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return EnPwrPulse == ENABLE_TOKEN
		       ? PwrPulseOnStartupEnableOk
		       : SaveSettingsError;
}

Response BootManager::PwrPulseOnStartupDisable()
{
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (EnPwrPulse == DISABLE_TOKEN)
		return PwrPulseOnStartupDisableOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	EnPwrPulse = DISABLE_TOKEN;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return EnPwrPulse == DISABLE_TOKEN
		       ? PwrPulseOnStartupDisableOk
		       : SaveSettingsError;
}

Response BootManager::RstPulseOnStartupEnable()
{
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (EnRstPulse == ENABLE_TOKEN)
		return RstPulseOnStartupEnableOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	EnRstPulse = ENABLE_TOKEN;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return EnRstPulse == ENABLE_TOKEN
		       ? RstPulseOnStartupEnableOk
		       : SaveSettingsError;
}

Response BootManager::RstPulseOnStartupDisable()
{
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (EnRstPulse == DISABLE_TOKEN)
		return RstPulseOnStartupDisableOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	EnRstPulse = DISABLE_TOKEN;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return EnRstPulse == DISABLE_TOKEN
		       ? RstPulseOnStartupDisableOk
		       : SaveSettingsError;
}

bool BootManager::SaveUserSettings(uint32_t status)
{
	uint8_t buffer[4];
	*reinterpret_cast<uint32_t*>(buffer) = status;

	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (settings0 == buffer[0] &&
		settings1 == buffer[1] &&
		settings2 == buffer[2] &&
		settings3 == buffer[3])
		return true;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	settings0 = buffer[0];
	settings1 = buffer[1];
	settings2 = buffer[2];
	settings3 = buffer[3];
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return settings0 == buffer[0] &&
		settings1 == buffer[1] &&
		settings2 == buffer[2] &&
		settings3 == buffer[3];
}

uint32_t BootManager::FetchUserSettings()
{
	uint32_t result = INITIAL;
	uint8_t* rs = reinterpret_cast<uint8_t*>(&result);
	rs[0] = settings0;
	rs[1] = settings1;
	rs[2] = settings2;
	rs[3] = settings3;
	return result;
}

Response BootManager::ApplyUserSettingsAtStartup()
{
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (ApplyUserSettings == ENABLE_TOKEN)
		return ApplyUserSettingsAtStartupOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	ApplyUserSettings = ENABLE_TOKEN;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return ApplyUserSettings == ENABLE_TOKEN
		? ApplyUserSettingsAtStartupOk
		: SaveSettingsError;
}

Response BootManager::LoadDefaultSettingsAtStartup()
{
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (ApplyUserSettings == DISABLE_TOKEN)
		return ApplyUserSettingsAtStartupOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	ApplyUserSettings = DISABLE_TOKEN;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return ApplyUserSettings == DISABLE_TOKEN
		? LoadDefaultSettingsAtStartupOk
		: SaveSettingsError;
}

bool BootManager::IsUserSettingsAppliedAtStartup()
{
	return ApplyUserSettings == ENABLE_TOKEN;
}

void BootManager::Callback(uint8_t data)
{
	counter++;
}
