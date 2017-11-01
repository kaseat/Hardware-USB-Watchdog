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

#include "GpioDriver.h"
#include "STM8S003F3.h"

#define PWR (1 << 2)
#define RST (1 << 3)
#define LED (1 << 6)

GpioDriver::GpioDriver()
{
	GPIOD->ODR |= PWR | RST;
	GPIOD->DDR |= PWR | RST;
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
