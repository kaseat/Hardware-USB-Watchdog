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

#include "GpioDriver.h"
#include "STM8S003F3.h"

#define PWR (1 << 2)
#define RST (1 << 3)
#define LED (1 << 6)

GpioDriver::GpioDriver()
{
	GPIOD->ODR |= PWR | RST;
	GPIOD->DDR |= PWR | RST;
	GPIOC->ODR |= LED;
	GPIOC->DDR |= LED;
	GPIOC->CR1 |= LED;
}

GpioDriver::~GpioDriver()
{
}

void GpioDriver::DriveResetLow()
{
	GPIOD->ODR &= ~RST;
}

void GpioDriver::ReleaseReset()
{
	GPIOD->ODR |= RST;
}

void GpioDriver::DrivePowerLow()
{
	GPIOD->ODR &= ~PWR;
}

void GpioDriver::ReleasePower()
{
	GPIOD->ODR |= PWR;
}

void GpioDriver::DriveLedLow()
{
	GPIOC->ODR &= ~LED;
}

void GpioDriver::DriveLedHigh()
{
	GPIOC->ODR |= LED;
}
