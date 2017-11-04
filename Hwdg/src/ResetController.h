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
#include "LedController.h"
#include "Rebooter.h"
#include "Uart.h"

/**
 * \brief Reset controller assumes Callback() calls every 1 ms.
 */
class ResetController : ISubscriber
{
public:

	/**
	 * \brief Create istnce of Reset controller.
	 * \param rb Rebooter reference.
	 */
	ResetController(Uart& uart, Rebooter& rb, LedController& ledController);

	/**
	 * \brief Dispose reset controller.
	 */
	~ResetController();

	/**
	* \brief Gets ResetController status.
	* \remarks see https://hwdg.ru/hardware-watchdog-api/getstatus/ for more details.
	*/
	_virtual uint32_t GetStatus();

	/**
	 * \brief Allow watchdog restart computer via reset button.
	 * \remarks see https://hwdg.ru/hardware-watchdog-api/start/ for more details.
	 */
	_virtual Response Start();

	/**
	 * \brief Disable watchdog restart computer via reset button.
	 * \remarks see https://hwdg.ru/hardware-watchdog-api/stop/ for more details.
	 */
	_virtual Response Stop();

	/**
	* \brief Allow watchdog restart computer via reset button.
	* \remarks see https://hwdg.ru/hardware-watchdog-api/enablehardreset/ for more details.
	*/
	_virtual Response EnableHardReset();

	/**
	* \brief Disable watchdog restart computer via reset button.
	* \remarks see https://hwdg.ru/hardware-watchdog-api/disablehardreset/ for more details.
	*/
	_virtual Response DisableHardReset();

	/**
	 * \brief Ping watchdog.
	 * \remarks see https://hwdg.ru/hardware-watchdog-api/ping/ for more details.
	 */
	_virtual Response Ping();

	/**
	 * \brief Set response timeout.
	 * \param timeout Timeout (0-59).
	 * \remarks see https://hwdg.ru/hardware-watchdog-api/setresponsetimeout/ for more details.
	 */
	_virtual Response SetResponseTimeout(uint8_t timeout);

	/**
	* \brief Set reboot timeout.
	* \param timeout Timeout(0-118).
	* \remarks see https://hwdg.ru/hardware-watchdog-api/setreboottimeout/ for more details.
	*/
	_virtual Response SetRebootTimeout(uint8_t timeout);

	/**
	 * \brief Set soft reset attempts count.
	 * \param attempts Attempts count.
	 * \remarks see https://hwdg.ru/hardware-watchdog-api/setsoftresetattempts/ for more details.
	 */
	_virtual Response SetSoftResetAttempts(uint8_t attempts);

	/**
	 * \brief Set hard reset attempts count.
	 * \param attempts Attempts count.
	 * \remarks see https://hwdg.ru/hardware-watchdog-api/sethardresetattempts/ for more details.
	 */
	_virtual Response SetHardResetAttempts(uint8_t attempts);

	/**
	 * \brief Test hard reset.
	 */
	_virtual Response TestHardReset();

	/**
	 * \brief Test soft reset.
	 */
	_virtual Response TestSoftReset();

	/**
	 * \brief Enable reset controller evets.
	 */
	_virtual Response EnableEvents();

	/**
	* \brief Disable reset controller evets.
	*/
	_virtual Response DisableEvents();

	/**
	* \brief Determine if reset controller evets enabled.
	*/
	_virtual bool IsEventsEnabled();

	/**
	 * \brief Get rebooter reference.
	 */
	_virtual Rebooter& GetRebooter();

	/**
	 * \brief Get LED controller reference.
	 */
	_virtual LedController& GetLedController();
private:
	void Callback(uint8_t data) _override;
	bool eventsEnabled;
	Uart& uart;
	Rebooter& rebooter;
	LedController& ledController;
	uint32_t counter;
	uint16_t counterms;
	uint_least8_t state;
	uint32_t responseTimeout;
	uint32_t rebootTimeout;
	uint8_t sAttempt;
	uint8_t hAttempt;
	uint8_t sAttemptCurr;
	uint8_t hAttemptCurr;
};
