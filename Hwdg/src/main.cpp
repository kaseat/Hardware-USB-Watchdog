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

#if defined(__ICCSTM8__) || defined(_M_IX86)

#include "Uart.h"
#include "Clock.h"
#include "Rebooter.h"
#include "ResetController.h"
#include "CommandManager.h"
#include "GpioDriver.h"
#include "BootManager.h"

int main()
{
	// Hardware init.
	Clock::SetCpuFreq(Freq16Mhz);

	Timer timer;
	timer.Run();
	GpioDriver drw;
	Rebooter rebooter(timer, drw);
	LedController ldCtr(timer, drw);
	Uart uart(9600);
	Exti exti;
	ResetController controller(uart, rebooter, ldCtr, exti);
	controller.EnableHddLedMonitor();

#ifdef __ICCSTM8__
	asm("RIM");
#endif
	
	SettingsManager settingsManager;
	BootManager btmgr(controller, settingsManager);
	btmgr.ProceedBoot();
	CommandManager mgr(uart, controller, settingsManager);

	for (;;)
		;
}
#endif
