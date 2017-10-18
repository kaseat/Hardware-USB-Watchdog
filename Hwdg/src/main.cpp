#include "Uart.h"
#include "Clock.h"
#include "Rebooter.h"
#include "ResetController.h"
#include "CommandManager.h"
#include "GpioDriver.h"
#include "Exti.h"


int main()
{
	// Hardware init.
	Clock::SetCpuFreq(Freq16Mhz);

	Exti exti;
	Timer timer;
	GpioDriver drw;
	Uart uart(9600);
	LedController ldCtr(timer, drw);
	Rebooter rebooter(timer, drw);
	ResetController controller(timer, rebooter, ldCtr, exti);
	CommandManager mgr(uart, controller);

	timer.Run();
	ldCtr.Glow();
#ifdef __IAR_SYSTEMS_ICC__
	asm("RIM");
#endif
	for (;;)
		;
}

