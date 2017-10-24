// Copyright (c) 2017, Oleg Petrochenko
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the HWDG nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.

#include "CommandManager.h"
#include "Uart.h"
#include "Crc.h"

CommandManager::
CommandManager(Uart& uart, ResetController& resetController): uart(uart), resetController(resetController)
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
		: data == 0xFE // EnableHddLedMonitor command
		? uart.SendByte(resetController.EnableHddLedMonitor())
		: data == 0xFF // DisableHddLedMonitor command
		? uart.SendByte(resetController.DisableHddLedMonitor())
		: data == 0x7E // TestHardReset command
		? TestHardReset()
		: data == 0x7F // TestSoftReset command
		? TestSoftReset()
		: data == 0x02 // EnableEvents command
		? EnableEvents()
		: data == 0x03 // DisableEvents command
		? DisableEvents()
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
