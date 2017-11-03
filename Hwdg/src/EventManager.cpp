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

#include "EventManager.h"
#include "SettingsManager.h"

EventManager::EventManager(Uart& uart, ResetController& rstController) :
	counter(0),
	rstController(rstController),
	uart(uart),
	enabled(false)
{
}

EventManager::~EventManager()
{
}

void EventManager::EnableEvents()
{
	rstController.SubscribeOnEvents(*this);
	enabled = true;
	uart.SendByte(EnableEventsOk);
}

void EventManager::DisableEvents()
{
	rstController.UnSubscribeOnEvents();
	enabled = false;
	uart.SendByte(DisableEventsOk);
}

bool EventManager::IsEnabled()
{
	return enabled;
}

void EventManager::OnUpdted(uint8_t cause)
{
	uart.SendByte(cause);
}
