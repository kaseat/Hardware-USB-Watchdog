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
#define _override
#else
#define _override override
#endif

/**
 * \brief Implements reboot logic
 */
class Rebooter : ISubscriber
{
public:
	/**
	 * \brief Create Rebooter instance.
	 * \param timer Time base source.
	 * \param driver GPIO pin driver.
	 */
	Rebooter(Timer& timer, GpioDriver& driver);

	/**
	 * \brief Dispose Rebooter.
	 */
	~Rebooter();

	/**
	 * \brief Satrt hard reset sequence.
	 */
	virtual Response HardReset();

	/**
	 * \brief Send pulse on power pin.
	 */
	virtual Response PwrPulse();

	/**
	 * \brief Start soft reset sequence.
	 */
	virtual Response SoftReset();

private:
	Timer& timer;
	GpioDriver& driver;
	uint_least8_t state;
	uint_fast16_t counter;
	void Callback(uint8_t data) _override;
};
