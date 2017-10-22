#include "CommandManager.h"
#include "Uart.h"
#include "Crc.h"

CommandManager::CommandManager(Uart& uart, ResetController& resetController): uart(uart), resetController(resetController)
{
	CommandManager::uart.SubscribeOnByteReceived(*this);
	CommandManager::resetController.SubscribeOnEvents(*this);
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
		: data == 0xFE // EnableHddMonitor command
		? uart.SendByte(resetController.EnableHddMonitor())
		: data == 0xFF // DisableHddMonitor command
		? uart.SendByte(resetController.DisableHddMonitor())
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
