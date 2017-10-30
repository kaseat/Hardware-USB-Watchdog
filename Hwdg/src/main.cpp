// Copyright 2017 Oleg Petrochenko
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Uart.h"
#include "Clock.h"
#include "Rebooter.h"
#include "ResetController.h"
#include "CommandManager.h"
#include "GpioDriver.h"

#ifndef __IAR_SYSTEMS_ICC__
#define __eeprom
#endif

uint8_t __eeprom settings = 0x55;

int main()
{
	// Hardware init.
	Clock::SetCpuFreq(Freq16Mhz);

	Timer timer;
	timer.Run();
	GpioDriver drw;
	Uart uart(9600);
	Rebooter rebooter(timer, drw);

#ifdef __IAR_SYSTEMS_ICC__
	asm("RIM");
#endif

	BootManager bootManager(rebooter, timer);
	LedController ldCtr(timer, drw);
	ResetController controller(timer, rebooter, ldCtr);
	CommandManager mgr(uart, controller, bootManager);

	ldCtr.Glow();

	for (;;);
}
