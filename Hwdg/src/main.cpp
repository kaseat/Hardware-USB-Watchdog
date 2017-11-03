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

int main()
{
	// Hardware init.
	Clock::SetCpuFreq(Freq16Mhz);

	Timer timer;
	timer.Run();
	GpioDriver drw;
	Rebooter rebooter(timer, drw);
	LedController ldCtr(timer, drw);
	ResetController controller(rebooter, ldCtr);

#ifdef __IAR_SYSTEMS_ICC__
	asm("RIM");
#endif

	SettingsManager settingsManager;
	Uart uart(9600);
	CommandManager mgr(uart, controller, settingsManager);

	for (;;);
}
