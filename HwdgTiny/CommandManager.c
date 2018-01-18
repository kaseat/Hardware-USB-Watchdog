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
#include "Crc.h"

static Response_t CommandManagerSaveCurrentSettings(void);
static void GetSettings(void);
static __inline void SendUnknownCommand(void);
extern Status_t HwdgStatus;

void OnCommandReceived(uint8_t data)
{

	data == 0xFB // Ping command
		? ResetControllerPing()
		//	: data == 0xF8 // IsAlive command
		//	? SoftwareVersion
		: data == 0x01 // GetStatus command
		? GetSettings()
		: data == 0xF9 // Start command
		? ResetControllerStart()
		: data == 0xFA // Stop command
		? ResetControllerStop()
		: data == 0xFE // Enable LED
		? HwdgStatus.LastCommandStatus = LedControllerEnable()
		: data == 0xFF // Disable LED
		? HwdgStatus.LastCommandStatus = LedControllerDisable()
		: data == 0x7F // TestSoftReset command
		? ResetControllerTestSoftReset()


		: data == 0x3B // ApplyUserSettingsAtStartup command
		? SettingsManagerApplyUserSettingsAtStartup()
		: data == 0x3A // LoadDefaultSettingsAtStartup command
		? SettingsManagerLoadDefaultSettingsAtStartup()
		: data == 0x39 // SaveCurrentSettings command
		? CommandManagerSaveCurrentSettings()


		: data >> 7 == 1 // SetRebootTimeout command
		? ResetControllerSetRebootTimeout(data)
		: data >> 6 == 1 // SetResponseTimeout command
		? ResetControllerSetResponseTimeout(data)
		: data >> 3 == 2 // SetSoftResetAttempts command
		? ResetControllerSetSoftResetAttempts(data)
		: SendUnknownCommand();
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
void GetSettings(void)
{
	HwdgStatus = SettingsManagerObtainUserSettings();
	HwdgStatus.Checksum = GetCrc7((uint8_t*)&HwdgStatus, 4);
	HwdgStatus.LastCommandStatus = 0x00;
}

__inline void SendUnknownCommand(void)
{
	HwdgStatus.LastCommandStatus = (uint8_t)UnknownCommand; 
}