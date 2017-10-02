#include "CommandManager.h"
#include "Uart.h"
#include "Crc.h"

CommandManager::CommandManager(Uart& uart, ResetController& rctr): uart(uart), resetController(rctr)
{
	uart.SubscribeOnByteReceived(*this);
}

CommandManager::~CommandManager()
{
	uart.UnsubscribeOnByteReceived();
}

void CommandManager::Callback(uint8_t data)
{
	data >> 7 == 1
		? uart.SendByte(resetController.SetRebootTimeout(data))
		: data >> 6 == 1
		? uart.SendByte(resetController.SetResponseTimeout(data))
		: data >> 3 == 2
		? uart.SendByte(resetController.SetSoftResetAttempts(data))
		: data >> 3 == 3
		? uart.SendByte(resetController.SetHardResetAttempts(data))
		: CmdSelector(data);
}

void CommandManager::CmdSelector(uint8_t data) const
{
	switch (data)
	{
	case 0x00:
	{
		uint8_t buffer[4];
		*reinterpret_cast<uint32_t*>(buffer) = resetController.GetStatus();
		buffer[3] = CrcCalculator::GetCrc7(buffer, 3);
		uart.SendData(buffer, 4);
		break;
	}
	case 0x01:
		uart.SendByte(resetController.Start());
		break;
	case 0x02:
		uart.SendByte(resetController.Stop());
		break;
	case 0x03:
		uart.SendByte(resetController.EnableHardReset());
		break;
	case 0x04:
		uart.SendByte(resetController.DisableHardReset());
		break;
	case 0x05:
		uart.SendByte(resetController.Ping());
		break;
	default:
		uart.SendByte(Error);
		break;
	}
}
