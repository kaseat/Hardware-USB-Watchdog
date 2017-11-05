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

#include "CommandManager.h"
#include "BootManager.h"

#define BAUDRATE 9600

Uart uart(BAUDRATE);
Timer timer;
GpioDriver drw;
Rebooter rebooter(timer, drw);
LedController ldCtr(timer, drw);
ResetController controller(uart, rebooter, ldCtr);
SettingsManager settingsManager;
BootManager btmgr(controller, settingsManager);
CommandManager mgr(uart, controller, settingsManager);

void setup()
{
	timer.Run();
	btmgr.ProceedBoot();
	Serial.begin(BAUDRATE);
}

void loop()
{
}
