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
