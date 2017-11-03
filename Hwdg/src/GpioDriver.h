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
#include "PlatformDefinitions.h"

/**
 * \brief Represents Low level GPIO driver.
 */
class GpioDriver
{
public:
	/**
	 * \brief Create instance of GPIO driver.
	 */
	GpioDriver();
	/**
	 * \brief Dispose GPIO driver.
	 */
	_virtual ~GpioDriver();
	/**
	 * \brief Drive reset pin low.
	 */
	_virtual void DriveResetLow();
	/**
	 * \brief Release reset pin.
	 */
	_virtual void ReleaseReset();
	/**
	 * \brief Drive power pin low.
	 */
	_virtual void DrivePowerLow();
	/**
	 * \brief Release power pin.
	 */
	_virtual void ReleasePower();
	/**
	 * \brief Drive LED pin low.
	 */
	_virtual void DriveLedLow();
	/**
	 * \brief Drive LED pin high.
	 */
	_virtual void DriveLedHigh();
};
