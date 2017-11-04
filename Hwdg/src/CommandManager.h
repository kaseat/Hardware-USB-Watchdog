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
#include "ResetController.h"
#include "SettingsManager.h"

class CommandManager : ISubscriber
{
public:
	/**
	 * \brief Create instance of command manager.
	 * \param rctr Reset controller.
	 */
	CommandManager(Uart& uart, ResetController& rctr, SettingsManager& btmgr);

	/**
	 * \brief Dispose Reset controller.
	 */
	~CommandManager();

	/**
	 * \brief Fires when UART byte received.
	 * \param data UART data.
	 */
	inline void Callback(uint8_t data) _override;
private:
	Uart& uart;
	ResetController& resetController;
	SettingsManager& settingsManager;

	inline void GetStatus();
	inline Response SaveCurrentSettings();
};
