#include "CommandManager.h"
#include "Uart.h"

CommandManager::CommandManager(IResetController& rctr): resetController(rctr)
{
	Uart::SubscribeOnByteRecivied(*this);
}

CommandManager::~CommandManager()
{
	Uart::UnsubscribeOnByteRecivied();
}

void CommandManager::Callback(uint8_t data)
{
	// Check if we have timeout change command.
	if (data >> 7 == 1)
	{
		resetController.SetHardResetTimeout(data);
		Uart::SendByte(0x20);
	}
	else if (data >> 6 == 1)
	{
		resetController.SetSoftResetTimeout(data);
		Uart::SendByte(0x21);
	}
	else if (data >> 3 == 2)
	{
		resetController.SetSoftResetAttempts(data);
		Uart::SendByte(0x22);
	}
	else if (data >> 3 == 3)
	{
		resetController.SetHardResetAttempts(data);
		Uart::SendByte(0x23);
	}
	else
	{
		switch (data)
		{
		case 0x01:
			resetController.EnableSoftReset();
			Uart::SendByte(0x24);
			break;
		case 0x02:
			resetController.DisableSoftReset();
			Uart::SendByte(0x25);
			break;
		case 0x03:
			resetController.EnableHardReset();
			Uart::SendByte(0x26);
			break;
		case 0x04:
			resetController.DisableHardReset();
			Uart::SendByte(0x27);
			break;
		case 0x05:
			resetController.Ping();
			Uart::SendByte(0x28);
			break;
		default:
			Uart::SendByte(0x29);
			break;
		}
	}
}