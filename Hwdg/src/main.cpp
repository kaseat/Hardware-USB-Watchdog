#include "Uart.h"
#include "Clock.h"
#include "Rebooter.h"
#include "ResetController.h"
#include "CommandManager.h"
#include "GpioDriver.h"

int main()
{
	// Hardware init.
	Clock::SetCpuFreq(Freq16Mhz);
	Uart::Init(9600);
	Uart::SendByte(0x33);

	// Create reset controller with GPIO rebooter.
	GpioDriver drw = {};
	Rebooter rebooter(drw);
	ResetController controller(rebooter);
	CommandManager mgr(controller);

	for (;;)
		;
}

