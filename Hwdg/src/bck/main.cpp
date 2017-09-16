#include "Uart.h"
#include "Clock.h"
#include "Rebooter.h"
#include "ResetController.h"
#include "CommandManager.h"
#include "GpioDriver.h"

Uart uart(9600);

int main()
{
	// Hardware init.
	Clock::SetCpuFreq(Freq16Mhz);
	uart.SendByte(0x33);
	Timer timer = {};
	timer.Run();
	// Create reset controller with GPIO rebooter.
	GpioDriver drw = {};
	Rebooter rebooter(timer,drw);
	ResetController controller(timer, rebooter);
	CommandManager mgr(uart,controller);

	for (;;)
		;
}

