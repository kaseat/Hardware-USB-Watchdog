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
	Uart uart(9600);
	Exti exti;
	//uart.SendByte(0x33);
	Timer timer;
	timer.Run();
	// Create reset controller with GPIO rebooter.
	GpioDriver drw;
	LedController ldCtr(timer, drw);
	ldCtr.Glow();

	Rebooter rebooter(timer,drw);
	ResetController controller(timer, rebooter, ldCtr);
	CommandManager mgr(uart,controller);
#ifdef __IAR_SYSTEMS_ICC__
	asm("RIM");
#endif


	for (;;)
		;
}

