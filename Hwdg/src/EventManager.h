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
#include "IResetControllerEventHandler.h"
#include "ResetController.h"
#include "Uart.h"

class EventManager : IResetControllerEventHandler
{
public:
	/**
	* \brief Create instance of event manager.
	*/
	EventManager(Uart& uart, ResetController& rstController);

	/**
	* \brief Dispose Reset controller.
	*/
	~EventManager();

	/**
	 * \brief Enable events.
	 */
	_virtual void EnableEvents();

	/**
	 * \brief Disable events.
	 */
	_virtual void DisableEvents();

	/**
	 * \brief Detect if events are enabled.
	 */
	_virtual bool IsEnabled();
private:
	/**
	* \brief Fires when Reset Controller throws an event.
	* \param cause Event cause.
	*/
	inline void OnUpdted(uint8_t cause) _override;
	uint_fast16_t counter;
	ResetController& rstController;
	Uart& uart;
	bool enabled;
};
