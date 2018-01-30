// Copyright 2018 Oleg Petrochenko
// 
// This file is part of Hwdg.
// 
// Hwdg is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
// 
// Hwdg is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Hwdg. If not, see <http://www.gnu.org/licenses/>.

#pragma once
#include "LedController.h"
#include "Rebooter.h"
#include "Uart.h"
#include "Exti.h"

/**
 * \brief Reset controller assumes Callback() calls every 1 ms.
 */
class ResetController : ISubscriber, IExtiInterruptable
{
public:

	/**
	 * \brief Create istnce of Reset controller.
	 * \param rb Rebooter reference.
	 */
	ResetController(Uart& uart, Rebooter& rb, LedController& ledController, Exti& exti);

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
	* \brief Starts Hdd monitoring.
	* \return Returns command response.
	* \remarks see https://hwdg.ru/hardware-watchdog-api/enablehddledmonitor/ for more details.
	*/
	_virtual Response EnableHddLedMonitor();

	/**
	* \brief Stops Hdd monitoring.
	* \return Returns command response.
	* \remarks see https://hwdg.ru/hardware-watchdog-api/disablehddledmonitor/ for more details.
	*/
	_virtual Response DisableHddLedMonitor();

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
	void OnExtiInterrupt() _override;
	bool eventsEnabled;
	Uart& uart;
	Rebooter& rebooter;
	LedController& ledController;
	Exti& exti;
	uint32_t counter;
	uint16_t counterExti;
	uint16_t counterms;
	uint_least8_t state;
	uint32_t responseTimeout;
	uint32_t rebootTimeout;
	uint8_t sAttempt;
	uint8_t hAttempt;
	uint8_t sAttemptCurr;
	uint8_t hAttemptCurr;
};
