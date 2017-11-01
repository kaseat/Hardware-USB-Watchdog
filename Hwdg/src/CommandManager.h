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

#pragma once
#include <stdint.h>
#include "ISubscriber.h"
#include "ResetController.h"
#include "Uart.h"
#include "IResetControllerEventHandler.h"
#include "BootManager.h"
#ifdef __IAR_SYSTEMS_ICC__
#define _override
#else
#define _override override
#endif
class CommandManager : ISubscriber, IResetControllerEventHandler
{
public:
	/**
	 * \brief Create instance of command manager.
	 * \param rctr Reset controller.
	 */
	CommandManager(Uart& uart, ResetController& rctr, BootManager& btmgr);

	/**
	 * \brief Dispose Reset controller.
	 */
	~CommandManager();
	inline void Callback(uint8_t data) _override;
	inline void OnUpdted(uint8_t reason) _override;
private:
	Uart& uart;
	ResetController& resetController;
	BootManager& bootManager;
	uint8_t status;

	inline void GetStatus();
	inline void TestHardReset();
	inline void TestSoftReset();
	inline void EnableEvents();
	inline void DisableEvents();
	inline Response SaveCurrentSettings();
};
