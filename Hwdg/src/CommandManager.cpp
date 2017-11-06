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

#include "CommandManager.h"
#include "Crc.h"
#include "ChipReset.h"

CommandManager::CommandManager(Uart& uart, ResetController& rstController, SettingsManager& btmgr) :
	uart(uart),
	resetController(rstController),
	settingsManager(btmgr)
{
	CommandManager::uart.SubscribeOnByteReceived(*this);
}

CommandManager::~CommandManager()
{
	uart.UnsubscribeOnByteReceived();
}

inline void CommandManager::Callback(uint8_t data)
{
	data == 0xFB // Ping command
		? uart.SendByte(resetController.Ping())
		: data == 0xF8 // IsAlive command
		? uart.SendByte(SoftwareVersion)
		: data == 0x01 // GetStatus command
		? GetStatus()
		: data == 0xF9 // Start command
		? uart.SendByte(resetController.Start())
		: data == 0xFA // Stop command
		? uart.SendByte(resetController.Stop())
		: data == 0xFC // EnableHardReset command
		? uart.SendByte(resetController.EnableHardReset())
		: data == 0xFD // DisableHardReset command
		? uart.SendByte(resetController.DisableHardReset())
		: data == 0xFE // Enable LED
		? uart.SendByte(resetController.GetLedController().Enable())
		: data == 0xFF // Disable LED
		? uart.SendByte(resetController.GetLedController().Disable())
		: data == 0x7E // TestHardReset command
		? uart.SendByte(resetController.TestHardReset())
		: data == 0x7F // TestSoftReset command
		? uart.SendByte(resetController.TestSoftReset())
		: data == 0x02 // EnableEvents command
		? uart.SendByte(resetController.EnableEvents())
		: data == 0x03 // DisableEvents command
		? uart.SendByte(resetController.DisableEvents())

		: data == 0x3F // RstPulseOnStartupDisable command
		? uart.SendByte(settingsManager.RstPulseOnStartupDisable())
		: data == 0x3E // RstPulseOnStartupEnable command
		? uart.SendByte(settingsManager.RstPulseOnStartupEnable())
		: data == 0x3D // PwrPulseOnStartupDisable command
		? uart.SendByte(settingsManager.PwrPulseOnStartupDisable())
		: data == 0x3C // PwrPulseOnStartupEnable command
		? uart.SendByte(settingsManager.PwrPulseOnStartupEnable())

		: data == 0x3B // ApplyUserSettingsAtStartup command
		? uart.SendByte(settingsManager.ApplyUserSettingsAtStartup())
		: data == 0x3A // LoadDefaultSettingsAtStartup command
		? uart.SendByte(settingsManager.LoadDefaultSettingsAtStartup())
		: data == 0x39 // SaveCurrentSettings command
		? uart.SendByte(SaveCurrentSettings())

		: data == 0x00 // Reset microcntroller
		? ChipReset::ResetImmediately()
		: data == 0xF7 // Restore factory settings
		? RestoreFactory()

		: data >> 7 == 1 // SetRebootTimeout command
		? uart.SendByte(resetController.SetRebootTimeout(data))
		: data >> 6 == 1 // SetResponseTimeout command
		? uart.SendByte(resetController.SetResponseTimeout(data))
		: data >> 3 == 2 // SetSoftResetAttempts command
		? uart.SendByte(resetController.SetSoftResetAttempts(data))
		: data >> 3 == 3 // SetHardResetAttempts command
		? uart.SendByte(resetController.SetHardResetAttempts(data))
		: uart.SendByte(UnknownCommand);
}

inline void CommandManager::GetStatus()
{
	uint8_t buffer[5];
	*reinterpret_cast<uint32_t*>(buffer) = resetController.GetStatus();

	buffer[3] = settingsManager.GetBootSettings() & 0xFC;
	resetController.GetLedController().IsEnabled()
		? buffer[3] &= ~LED_DISABLED
		: buffer[3] |= LED_DISABLED;

	// Get Event status
	resetController.IsEventsEnabled()
		? buffer[3] |= EVENTS_ENABLED
		: buffer[3] &= ~EVENTS_ENABLED;

	buffer[4] = CrcCalculator::GetCrc7(buffer, 4);
	uart.SendData(buffer, 5);
}

Response CommandManager::SaveCurrentSettings()
{
	// Get ResetController status
	uint8_t buffer[4];
	*reinterpret_cast<uint32_t*>(buffer) = resetController.GetStatus();

	// Get LED status.
	resetController.GetLedController().IsEnabled()
		? buffer[3] &= ~LED_DISABLED
		: buffer[3] |= LED_DISABLED;

	// Get Event status
	resetController.IsEventsEnabled()
		? buffer[3] |= EVENTS_ENABLED
		: buffer[3] &= ~EVENTS_ENABLED;

	// Save all settings we got earlier
	return settingsManager.SaveUserSettings(*reinterpret_cast<uint32_t*>(buffer))
		? SaveCurrentSettingsOk
		: SaveSettingsError;
}

void CommandManager::RestoreFactory()
{
	settingsManager.RestoreFactory();
	ChipReset::ResetImmediately();
}
