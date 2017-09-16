#include "CommandManager.h"
#include "Uart.h"

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
	// Check if we have timeout change command.
	if (data >> 7 == 1)
	{
		resetController.SetResetTimeout(data);
		uart.SendByte(0x20);
	}
	else if (data >> 6 == 1)
	{
		resetController.SetResponseTimeout(data);
		uart.SendByte(0x21);
	}
	else if (data >> 3 == 2)
	{
		resetController.SetSoftResetAttempts(data);
		uart.SendByte(0x22);
	}
	else if (data >> 3 == 3)
	{
		resetController.SetHardResetAttempts(data);
		uart.SendByte(0x23);
	}
	else
	{
		switch (data)
		{
		case 0x01:
			resetController.EnableSoftReset();
			uart.SendByte(0x24);
			break;
		case 0x02:
			resetController.DisableSoftReset();
			uart.SendByte(0x25);
			break;
		case 0x03:
			resetController.EnableHardReset();
			uart.SendByte(0x26);
			break;
		case 0x04:
			resetController.DisableHardReset();
			uart.SendByte(0x27);
			break;
		case 0x05:
			resetController.Ping();
			uart.SendByte(0x28);
			break;
		default:
			uart.SendByte(0x29);
			break;
		}
	}
}