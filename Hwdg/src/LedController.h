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
#include "ISubscriber.h"
#include "GpioDriver.h"
#include "Timer.h"
#include "Response.h"

#ifdef __IAR_SYSTEMS_ICC__
#define _virtual
#define _override
#else
#define _virtual virtual
#define _override override
#endif

/**
 * \brief Represents LED controller (assumes callback called once per ms).
 */
class LedController : ISubscriber
{
public:
	/**
	* \brief Create LED controller instance.
	* \param timer Time base source.
	* \param driver GPIO pin driver.
	*/
	LedController(Timer& timer, GpioDriver& driver);

	/**
	 * \brief Dispose LED controller.
	 */
	~LedController();
	/**
	* \brief Enable LED.
	*/
	_virtual Response Enable();
	/**
	* \brief Get current enable status.
	*/
	_virtual bool IsEnabled();
	/**
	* \brief Disable LED.
	*/
	_virtual Response Disable();
	/**
	 * \brief Turn LED off.
	 */
	_virtual void Off();

	/**
	 * \brief Turn LED on.
	 */
	_virtual void Glow();

	/**
	 * \brief Start fast LED blink.
	 */
	_virtual void BlinkFast();

	/**
	* \brief Start medium LED blink.
	*/
	_virtual void BlinkMid();

	/**
	* \brief Start slow LED blink.
	*/
	_virtual void BlinkSlow();
private:
	Timer& timer;
	GpioDriver& driver;
	uint_least8_t state;
	bool disabled;
	uint_fast16_t counter;
	void Callback(uint8_t data) _override;
	void Blink(uint32_t timeout);
};
