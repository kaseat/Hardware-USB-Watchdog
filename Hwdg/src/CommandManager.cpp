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
#include "Uart.h"
#include "Crc.h"

CommandManager::
CommandManager(Uart& uart, ResetController& resetController, BootManager& btmgr) :
	uart(uart),
	resetController(resetController),
	bootManager(btmgr)
{
	CommandManager::uart.SubscribeOnByteReceived(*this);
}

CommandManager::~CommandManager()
{
	uart.UnsubscribeOnByteReceived();
	resetController.UnSubscribeOnEvents();
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
		? TestHardReset()
		: data == 0x7F // TestSoftReset command
		? TestSoftReset()
		: data == 0x02 // EnableEvents command
		? EnableEvents()
		: data == 0x03 // DisableEvents command
		? DisableEvents()

		: data == 0x3F // RstPulseOnStartupDisable command
		? uart.SendByte(bootManager.RstPulseOnStartupDisable())
		: data == 0x3E // RstPulseOnStartupEnable command
		? uart.SendByte(bootManager.RstPulseOnStartupEnable())
		: data == 0x3D // PwrPulseOnStartupDisable command
		? uart.SendByte(bootManager.PwrPulseOnStartupDisable())
		: data == 0x3C // PwrPulseOnStartupEnable command
		? uart.SendByte(bootManager.PwrPulseOnStartupEnable())

		: data == 0xF7 // Reserved command
		? uart.SendByte(UnknownCommand)
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

inline void CommandManager::OnUpdted(uint8_t reason)
{
	uart.SendByte(reason);
}

inline void CommandManager::GetStatus()
{
	uint8_t buffer[4];
	*reinterpret_cast<uint32_t*>(buffer) = resetController.GetStatus();
	buffer[3] = CrcCalculator::GetCrc7(buffer, 3);
	uart.SendData(buffer, 4);
}

inline void CommandManager::TestHardReset()
{
	resetController.Stop();
	uart.SendByte(resetController.GetRebooter().HardReset());
}

inline void CommandManager::TestSoftReset()
{
	resetController.Stop();
	uart.SendByte(resetController.GetRebooter().SoftReset());
}

inline void CommandManager::EnableEvents()
{
	resetController.SubscribeOnEvents(*this);
	uart.SendByte(EnableEventsOk);
}

inline void CommandManager::DisableEvents()
{
	resetController.UnSubscribeOnEvents();
	uart.SendByte(DisableEventsOk);
}
