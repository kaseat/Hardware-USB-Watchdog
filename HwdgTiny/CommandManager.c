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

#include "CommandManager.h"
#include "Response.h"
#include "ResetController.h"
#include "LedController.h"

void OnCommandReceived(uint8_t data)
{
	data == 0xFB // Ping command
		? ResetControllerPing()
		//	: data == 0xF8 // IsAlive command
		//	? SoftwareVersion
		//	: data == 0x01 // GetStatus command
		//	? GetStatus()
		: data == 0xF9 // Start command
		? ResetControllerStart()
		: data == 0xFA // Stop command
		? ResetControllerStop()
		: data == 0xFE // Enable LED
		? LedControllerEnable()
		: data == 0xFF // Disable LED
		? LedControllerDisable()
		: data == 0x7F // TestSoftReset command
		? ResetControllerTestSoftReset()

		//	: data == 0x3F // RstPulseOnStartupDisable command
		//	? settingsManager.RstPulseOnStartupDisable()
		//	: data == 0x3E // RstPulseOnStartupEnable command
		//	? settingsManager.RstPulseOnStartupEnable()
		//	: data == 0x3D // PwrPulseOnStartupDisable command
		//	? settingsManager.PwrPulseOnStartupDisable()
		//	: data == 0x3C // PwrPulseOnStartupEnable command
		//	? settingsManager.PwrPulseOnStartupEnable()
		//
		//	: data == 0x3B // ApplyUserSettingsAtStartup command
		//	? settingsManager.ApplyUserSettingsAtStartup()
		//	: data == 0x3A // LoadDefaultSettingsAtStartup command
		//	? settingsManager.LoadDefaultSettingsAtStartup()
		//	: data == 0x39 // SaveCurrentSettings command
		//	? SaveCurrentSettings()


		: data >> 7 == 1 // SetRebootTimeout command
		? ResetControllerSetRebootTimeout(data)
		: data >> 6 == 1 // SetResponseTimeout command
		? ResetControllerSetResponseTimeout(data)
		: data >> 3 == 2 // SetSoftResetAttempts command
		? ResetControllerSetSoftResetAttempts(data)
		: UnknownCommand;
}
