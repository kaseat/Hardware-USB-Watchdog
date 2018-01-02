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

#include "CommandManager.h"
#include "Common.h"
#include "ResetController.h"
#include "LedController.h"
#include "SettingsManager.h"
#include "BootManager.h"

typedef struct
{
	uint8_t LastOperationStatus;

}Communication_t;

uint8_t LastOperationStatus = 0;

static Response_t CommandManagerSaveCurrentSettings(void);
void OnCommandReceived(uint8_t data)
{
	data == 0xFB // Ping command
		? LastOperationStatus = ResetControllerPing()
		//	: data == 0xF8 // IsAlive command
		//	? SoftwareVersion
			: data == 0x01 // GetStatus command
			? BootManagerProceedBoot()
		: data == 0xF9 // Start command
		? LastOperationStatus = ResetControllerStart()
		: data == 0xFA // Stop command
		? LastOperationStatus = ResetControllerStop()
		: data == 0xFE // Enable LED
		? LastOperationStatus = LedControllerEnable()
		: data == 0xFF // Disable LED
		? LastOperationStatus = LedControllerDisable()
		: data == 0x7F // TestSoftReset command
		? LastOperationStatus = ResetControllerTestSoftReset()


		: data == 0x3B // ApplyUserSettingsAtStartup command
		? LastOperationStatus = SettingsManagerApplyUserSettingsAtStartup()
		: data == 0x3A // LoadDefaultSettingsAtStartup command
		? LastOperationStatus = SettingsManagerLoadDefaultSettingsAtStartup()
		: data == 0x39 // SaveCurrentSettings command
		? LastOperationStatus = CommandManagerSaveCurrentSettings()


		: data >> 7 == 1 // SetRebootTimeout command
		? LastOperationStatus = ResetControllerSetRebootTimeout(data)
		: data >> 6 == 1 // SetResponseTimeout command
		? LastOperationStatus = ResetControllerSetResponseTimeout(data)
		: data >> 3 == 2 // SetSoftResetAttempts command
		? LastOperationStatus = ResetControllerSetSoftResetAttempts(data)
		: UnknownCommand;
}

Response_t CommandManagerSaveCurrentSettings(void)
{
	// Get ResetController status
	uint8_t buffer[4];
	*(uint32_t*)buffer = ResetControllerGetStatus();

	// Get LED status.
	if (LedControllerIsEnabled())
	{
		buffer[3] &= ~LED_DISABLED;
	}
	else
	{
		buffer[3] |= LED_DISABLED;
	}
	// Save all settings we got earlier
	return SettingsManagerSaveUserSettings((Status_t*)buffer)
		       ? SaveCurrentSettingsOk
		       : SaveSettingsError;
}
