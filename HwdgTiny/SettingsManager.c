// Copyright 2018 Oleg Petrochenko
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

#include "SettingsManager.h"
#include <avr/eeprom.h>

uint8_t SettingsManagerSaveUserSettings(uint32_t status)
{
	uint32_t data = status;
	uint8_t* data_p = (uint8_t*)&data;
	data_p[3] = data_p[3] & 0xFC;
	uint32_t readData;

	eeprom_read_block((uint8_t*)&readData, (uint8_t *)0, sizeof(uint32_t));
	((uint8_t*)&readData)[3] = ((uint8_t*)&readData)[3] & 0x03;

	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (data == readData)
		return 1;

	// Write data to EEPROM.
	eeprom_write_block((uint8_t*)&status, (uint8_t *)0, sizeof(uint32_t));

	// Verify write operation succeeded.
	eeprom_read_block((uint8_t*)&readData, (uint8_t *)0, sizeof(uint32_t));
	((uint8_t*)&readData)[3] = ((uint8_t*)&readData)[3] & 0x03;
	return !(data == readData);
}

uint32_t SettingsManagerObtainUserSettings(void)
{
	uint32_t result = 0;
	eeprom_read_block((uint8_t*)&result, (uint8_t *)0, sizeof(uint32_t));
	return result;
}

Response_t SettingsManagerApplyUserSettingsAtStartup(void)
{
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	const uint8_t data = eeprom_read_byte((uint8_t *)0 + 3);

	if (data & APPLY_SETTINGS_AT_STARTUP)
		return ApplyUserSettingsAtStartupOk;

	// Write data to EEPROM.
	eeprom_write_byte((uint8_t *)0 + 3, data | APPLY_SETTINGS_AT_STARTUP);

	// Verify write operation succeeded.
	return eeprom_read_byte((uint8_t *)0 + 3) & APPLY_SETTINGS_AT_STARTUP
		       ? ApplyUserSettingsAtStartupOk
		       : SaveSettingsError;
}

Response_t SettingsManagerLoadDefaultSettingsAtStartup(void)
{
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	const uint8_t data = eeprom_read_byte((uint8_t *)0 + 3);

	if (!(data & APPLY_SETTINGS_AT_STARTUP))
		return LoadDefaultSettingsAtStartupOk;

	// Write data to EEPROM.
	eeprom_write_byte((uint8_t *)0 + 3, data & ~APPLY_SETTINGS_AT_STARTUP);

	// Verify write operation succeeded.
	return !(eeprom_read_byte((uint8_t *)0 + 3) & APPLY_SETTINGS_AT_STARTUP)
		       ? LoadDefaultSettingsAtStartupOk
		       : SaveSettingsError;
}

uint8_t SettingsManagerRestoreFactory(void)
{
	// If we have the same values in EEPROM we don't need to
	// rewrite existing data, just say operation succeeded.
	if (eeprom_read_byte((uint8_t *)0 + 0) == SETTINGS_DEFAULT_0 &&
		eeprom_read_byte((uint8_t *)0 + 1) == SETTINGS_DEFAULT_1 &&
		eeprom_read_byte((uint8_t *)0 + 2) == SETTINGS_DEFAULT_2 &&
		eeprom_read_byte((uint8_t *)0 + 3) == SETTINGS_DEFAULT_3)
		return 1;

	// Write data to EEPROM.
	eeprom_write_byte((uint8_t *)0 + 0, SETTINGS_DEFAULT_0);
	eeprom_write_byte((uint8_t *)0 + 1, SETTINGS_DEFAULT_1);
	eeprom_write_byte((uint8_t *)0 + 2, SETTINGS_DEFAULT_2);
	eeprom_write_byte((uint8_t *)0 + 3, SETTINGS_DEFAULT_3);

	// Verify write operation succeeded.
	return eeprom_read_byte((uint8_t *)0 + 0) == SETTINGS_DEFAULT_0 &&
		eeprom_read_byte((uint8_t *)0 + 1) == SETTINGS_DEFAULT_1 &&
		eeprom_read_byte((uint8_t *)0 + 2) == SETTINGS_DEFAULT_2 &&
		eeprom_read_byte((uint8_t *)0 + 3) == SETTINGS_DEFAULT_3;
}

uint8_t SettingsManagerGetBootSettings(void)
{
	return eeprom_read_byte((uint8_t *)0 + 3);
}
