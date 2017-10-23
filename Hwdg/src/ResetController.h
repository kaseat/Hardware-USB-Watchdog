// Copyright (c) 2017, Oleg Petrochenko
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the HWDG nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.

#pragma once
#include <stdint.h>
#include "ISubscriber.h"
#include "Rebooter.h"
#include "Timer.h"
#include "LedController.h"
#include "Response.h"
#include "IExtiInterruptable.h"
#include "Exti.h"

#ifdef __IAR_SYSTEMS_ICC__
#define _virtual
#define _override
#else
#define _virtual virtual
#define _override override
#endif

class IResetControllerEventHandler;

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
	ResetController(Timer& timer, Rebooter& rb, LedController& ledController, Exti& exti);
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
	 * \brief Subscribe event handler.
	 * \param eventHandler Event handler to subscribe.
	 */
	_virtual void SubscribeOnEvents(IResetControllerEventHandler& eventHandler);

	/**
	 * \brief Unsubscribe all event handlers.
	 */
	_virtual void UnSubscribeOnEvents();

	/**
	 * \brief Get rebooter reference.
	 */
	_virtual Rebooter& GetRebooter();
private:
	void Callback(uint8_t data) _override;
	void OnExtiInterrupt() _override;
	IResetControllerEventHandler* eventHandler;
	Timer& timer;
	Rebooter& rebooter;
	LedController& ledController;
	Exti& exti;
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
