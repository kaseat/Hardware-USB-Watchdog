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

#include "SettingsManager.h"

#ifdef __ICCSTM8__

#include "STM8S003F3.h"

#define INITIAL                       ((uint32_t)0)

__no_init uint8_t __eeprom dummy;

uint8_t __eeprom settings0 = 0x1C;
uint8_t __eeprom settings1 = 0x44;
uint8_t __eeprom settings2 = 0x48;
uint8_t __eeprom settings3 = 0x00;

#endif

Response SettingsManager::PwrPulseOnStartupEnable()
{
#ifdef __ICCSTM8__
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (settings3 & PWR_PULSE_ENABLED)
		return PwrPulseOnStartupEnableOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	settings3 |= PWR_PULSE_ENABLED;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return settings3 & PWR_PULSE_ENABLED
		       ? PwrPulseOnStartupEnableOk
		       : SaveSettingsError;
#endif
#ifdef _M_IX86
	return PwrPulseOnStartupEnableOk;
#endif
#ifdef __AVR__
	return SaveSettingsError;
#endif
}

Response SettingsManager::PwrPulseOnStartupDisable()
{
#ifdef __ICCSTM8__
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (!(settings3 & PWR_PULSE_ENABLED))
		return PwrPulseOnStartupDisableOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	settings3 &= ~PWR_PULSE_ENABLED;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return !(settings3 & PWR_PULSE_ENABLED)
		       ? PwrPulseOnStartupDisableOk
		       : SaveSettingsError;
#endif
#ifdef _M_IX86
	return PwrPulseOnStartupDisableOk;
#endif
#ifdef __AVR__
	return SaveSettingsError;
#endif
}

Response SettingsManager::RstPulseOnStartupEnable()
{
#ifdef __ICCSTM8__
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (settings3 & RST_PULSE_ENABLED)
		return RstPulseOnStartupEnableOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	settings3 |= RST_PULSE_ENABLED;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return settings3 & RST_PULSE_ENABLED
		       ? RstPulseOnStartupEnableOk
		       : SaveSettingsError;
#endif
#ifdef _M_IX86
	return RstPulseOnStartupEnableOk;
#endif
#ifdef __AVR__
	return SaveSettingsError;
#endif
}

Response SettingsManager::RstPulseOnStartupDisable()
{
#ifdef __ICCSTM8__
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (!(settings3 & RST_PULSE_ENABLED))
		return RstPulseOnStartupDisableOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	settings3 &= ~RST_PULSE_ENABLED;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return !(settings3 & RST_PULSE_ENABLED)
		       ? RstPulseOnStartupDisableOk
		       : SaveSettingsError;
#endif
#ifdef _M_IX86
	return RstPulseOnStartupDisableOk;
#endif
#ifdef __AVR__
	return SaveSettingsError;
#endif
}

bool SettingsManager::SaveUserSettings(uint32_t status)
{
#ifdef __ICCSTM8__
	uint8_t buffer[4];
	*reinterpret_cast<uint32_t*>(buffer) = status;

	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (settings0 == buffer[0] &&
		settings1 == buffer[1] &&
		settings2 == buffer[2] &&
		settings3 & 0x03 == buffer[3])
		return true;

	
	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	settings0 = buffer[0];
	settings1 = buffer[1];
	settings2 = buffer[2];
	settings3 = settings3 & 0xFC | buffer[3];
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return settings0 == buffer[0] &&
		settings1 == buffer[1] &&
		settings2 == buffer[2]&&
		(settings3 & 0x03) == buffer[3];
#endif
#ifdef _M_IX86
	return true;
#endif
#ifdef __AVR__
	return false;
#endif
}

uint32_t SettingsManager::ObtainUserSettings()
{
#ifdef __ICCSTM8__
	uint32_t result = INITIAL;
	uint8_t* rs = reinterpret_cast<uint8_t*>(&result);
	rs[0] = settings0;
	rs[1] = settings1;
	rs[2] = settings2;
	rs[3] = settings3;
	return result;
#endif
#ifdef _M_IX86
	return 0;
#endif
#ifdef __AVR__
	return 0;
#endif
}

Response SettingsManager::ApplyUserSettingsAtStartup()
{
#ifdef __ICCSTM8__
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (settings3 & APPLY_SETTINGS_AT_ATARTUP)
		return ApplyUserSettingsAtStartupOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	settings3 |= APPLY_SETTINGS_AT_ATARTUP;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return settings3 & APPLY_SETTINGS_AT_ATARTUP
		       ? ApplyUserSettingsAtStartupOk
		       : SaveSettingsError;
#endif
#ifdef _M_IX86
	return ApplyUserSettingsAtStartupOk;
#endif
#ifdef __AVR__
	return SaveSettingsError;
#endif
}

Response SettingsManager::LoadDefaultSettingsAtStartup()
{
#ifdef __ICCSTM8__
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (!(settings3 & APPLY_SETTINGS_AT_ATARTUP))
		return LoadDefaultSettingsAtStartupOk;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	settings3 &= ~APPLY_SETTINGS_AT_ATARTUP;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return !(settings3 & APPLY_SETTINGS_AT_ATARTUP)
		       ? LoadDefaultSettingsAtStartupOk
		       : SaveSettingsError;
#endif
#ifdef _M_IX86
	return LoadDefaultSettingsAtStartupOk;
#endif
#ifdef __AVR__
	return SaveSettingsError;
#endif
}

uint8_t SettingsManager::GetBootSettings()
{
#ifdef __ICCSTM8__
	return settings3;
#endif
#ifdef _M_IX86
	return 0;
#endif
#ifdef __AVR__
	return SaveSettingsError;
#endif
}
