#include "Uart.h"
#include "Clock.h"
#include "GpioRebooter.h"
#include "ResetController.h"
#include "CommandManager.h"



int main()
{
	// Hardware init.
	Clock::SetCpuFreq(Freq16Mhz);
	Uart::Init(9600);
	Uart::SendByte(0x33);

	// Create reset controller with GPIO rebooter.
	GpioRebooter rebooter;
	ResetController controller(rebooter);
	CommandManager mgr(controller);

	for (;;)
		;
}

