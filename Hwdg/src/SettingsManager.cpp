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

#define INITIAL      ((uint32_t)0)

#ifdef __ICCSTM8__
#include "STM8S003F3.h"
__no_init uint8_t __eeprom dummy;
uint8_t __eeprom settings0 = SETTINGS_DEFAULT_0;
uint8_t __eeprom settings1 = SETTINGS_DEFAULT_1;
uint8_t __eeprom settings2 = SETTINGS_DEFAULT_2;
uint8_t __eeprom settings3 = SETTINGS_DEFAULT_3;
#endif
#ifdef __AVR__
#include <EEPROM.h>
#include <Arduino.h>
const uint8_t CompileTime[] PROGMEM = __DATE__ " " __TIME__;
#endif

SettingsManager::SettingsManager()
{
#ifdef __AVR__
	uint8_t f, e = -1, *p = const_cast<uint8_t*>(CompileTime) - 1;
	while (f = pgm_read_byte(++p))
	{
		if (f != EEPROM[++e])
		{
			EEPROM[sizeof CompileTime + 1] = SETTINGS_DEFAULT_0;
			EEPROM[sizeof CompileTime + 2] = SETTINGS_DEFAULT_1;
			EEPROM[sizeof CompileTime + 3] = SETTINGS_DEFAULT_2;
			EEPROM[sizeof CompileTime + 4] = SETTINGS_DEFAULT_3;

			while (f = pgm_read_byte(p++)) EEPROM[e++] = f;
			break;
		}
	}
#endif
}

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
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (EEPROM[sizeof CompileTime + 4] & PWR_PULSE_ENABLED)
		return PwrPulseOnStartupEnableOk;

	// Write data to EEPROM.
	EEPROM[sizeof CompileTime + 4] |= PWR_PULSE_ENABLED;

	// Verify write operation succeeded.
	return EEPROM[sizeof CompileTime + 4] & PWR_PULSE_ENABLED
		? PwrPulseOnStartupEnableOk
		: SaveSettingsError;
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
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (!(EEPROM[sizeof CompileTime + 4] & PWR_PULSE_ENABLED))
		return PwrPulseOnStartupDisableOk;

	// Write data to EEPROM.
	EEPROM[sizeof CompileTime + 4] &= ~PWR_PULSE_ENABLED;

	// Verify write operation succeeded.
	return !(EEPROM[sizeof CompileTime + 4] & PWR_PULSE_ENABLED)
		? PwrPulseOnStartupEnableOk
		: SaveSettingsError;
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
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (EEPROM[sizeof CompileTime + 4] & RST_PULSE_ENABLED)
		return RstPulseOnStartupEnableOk;

	// Write data to EEPROM.
	EEPROM[sizeof CompileTime + 4] |= RST_PULSE_ENABLED;

	// Verify write operation succeeded.
	return EEPROM[sizeof CompileTime + 4] & RST_PULSE_ENABLED
		? RstPulseOnStartupEnableOk
		: SaveSettingsError;
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
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (!(EEPROM[sizeof CompileTime + 4] & RST_PULSE_ENABLED))
		return RstPulseOnStartupDisableOk;

	// Write data to EEPROM.
	EEPROM[sizeof CompileTime + 4] &= ~RST_PULSE_ENABLED;

	// Verify write operation succeeded.
	return !(EEPROM[sizeof CompileTime + 4] & RST_PULSE_ENABLED)
		? RstPulseOnStartupDisableOk
		: SaveSettingsError;
#endif
}

bool SettingsManager::SaveUserSettings(uint32_t status)
{
	uint8_t buffer[4];
	*reinterpret_cast<uint32_t*>(buffer) = status;
#ifdef __ICCSTM8__
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
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (EEPROM[sizeof CompileTime + 1] == buffer[0] &&
		EEPROM[sizeof CompileTime + 2] == buffer[1] &&
		EEPROM[sizeof CompileTime + 3] == buffer[2] &&
		EEPROM[sizeof CompileTime + 4] & 0x03 == buffer[3])
		return true;

	// Write data to EEPROM.
	EEPROM[sizeof CompileTime + 1] = buffer[0];
	EEPROM[sizeof CompileTime + 2] = buffer[1];
	EEPROM[sizeof CompileTime + 3] = buffer[2];
	EEPROM[sizeof CompileTime + 4] = EEPROM[sizeof CompileTime + 4] & 0xFC | buffer[3];

	// Verify write operation succeeded.
	return EEPROM[sizeof CompileTime + 1] == buffer[0] &&
		EEPROM[sizeof CompileTime + 2] == buffer[1] &&
		EEPROM[sizeof CompileTime + 3] == buffer[2] &&
		(EEPROM[sizeof CompileTime + 4] & 0x03) == buffer[3];
#endif
}

uint32_t SettingsManager::ObtainUserSettings()
{
	uint32_t result = INITIAL;
	uint8_t* rs = reinterpret_cast<uint8_t*>(&result);
#ifdef __ICCSTM8__
	rs[0] = settings0;
	rs[1] = settings1;
	rs[2] = settings2;
	rs[3] = settings3;
	return result;
#endif
#ifdef _M_IX86
	rs[0] = SETTINGS_DEFAULT_0;
	rs[1] = SETTINGS_DEFAULT_1;
	rs[2] = SETTINGS_DEFAULT_2;
	rs[3] = SETTINGS_DEFAULT_3;
	return result;
#endif
#ifdef __AVR__
	rs[0] = EEPROM[sizeof CompileTime + 1];
	rs[1] = EEPROM[sizeof CompileTime + 2];
	rs[2] = EEPROM[sizeof CompileTime + 3];
	rs[3] = EEPROM[sizeof CompileTime + 4];
	return result;
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
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (EEPROM[sizeof CompileTime + 4] & APPLY_SETTINGS_AT_ATARTUP)
		return ApplyUserSettingsAtStartupOk;

	// Write data to EEPROM.
	EEPROM[sizeof CompileTime + 4] |= APPLY_SETTINGS_AT_ATARTUP;

	// Verify write operation succeeded.
	return EEPROM[sizeof CompileTime + 4] & APPLY_SETTINGS_AT_ATARTUP
		? ApplyUserSettingsAtStartupOk
		: SaveSettingsError;
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
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (!(EEPROM[sizeof CompileTime + 4] & APPLY_SETTINGS_AT_ATARTUP))
		return LoadDefaultSettingsAtStartupOk;

	// Write data to EEPROM.
	EEPROM[sizeof CompileTime + 4] &= ~APPLY_SETTINGS_AT_ATARTUP;

	// Verify write operation succeeded.
	return !(EEPROM[sizeof CompileTime + 4] & APPLY_SETTINGS_AT_ATARTUP)
		? LoadDefaultSettingsAtStartupOk
		: SaveSettingsError;
#endif
}

bool SettingsManager::RestoreFactory()
{
#ifdef __ICCSTM8__
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (settings0 == SETTINGS_DEFAULT_0 &&
		settings1 == SETTINGS_DEFAULT_1 &&
		settings2 == SETTINGS_DEFAULT_2 &&
		settings3 == SETTINGS_DEFAULT_3)
		return true;

	// Write data to EEPROM.
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	settings0 = SETTINGS_DEFAULT_0;
	settings1 = SETTINGS_DEFAULT_1;
	settings2 = SETTINGS_DEFAULT_2;
	settings3 = SETTINGS_DEFAULT_3;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);

	// Verify write operation succeeded.
	return settings0 == SETTINGS_DEFAULT_0 &&
		settings1 == SETTINGS_DEFAULT_1 &&
		settings2 == SETTINGS_DEFAULT_2 &&
		settings3 == SETTINGS_DEFAULT_3;
#endif
#ifdef _M_IX86
	return true;
#endif
#ifdef __AVR__
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (EEPROM[sizeof CompileTime + 1] == SETTINGS_DEFAULT_0 &&
		EEPROM[sizeof CompileTime + 2] == SETTINGS_DEFAULT_1 &&
		EEPROM[sizeof CompileTime + 3] == SETTINGS_DEFAULT_2 &&
		EEPROM[sizeof CompileTime + 4] == SETTINGS_DEFAULT_3)
		return true;

	// Write data to EEPROM.
	EEPROM[sizeof CompileTime + 1] = SETTINGS_DEFAULT_0;
	EEPROM[sizeof CompileTime + 2] = SETTINGS_DEFAULT_1;
	EEPROM[sizeof CompileTime + 3] = SETTINGS_DEFAULT_2;
	EEPROM[sizeof CompileTime + 4] = SETTINGS_DEFAULT_3;

	// Verify write operation succeeded.
	return EEPROM[sizeof CompileTime + 1] == SETTINGS_DEFAULT_0 &&
		EEPROM[sizeof CompileTime + 2] == SETTINGS_DEFAULT_1 &&
		EEPROM[sizeof CompileTime + 3] == SETTINGS_DEFAULT_2 &&
		EEPROM[sizeof CompileTime + 4] == SETTINGS_DEFAULT_3;
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
	return EEPROM[sizeof CompileTime + 4];
#endif
}
