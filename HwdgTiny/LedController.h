// Copyright 2017 Oleg Petrochenko
// 
// This file is part of HwdgTiny.
// 
// HwdgTiny is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
// 
// HwdgTiny is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with HwdgTiny. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "Response.h"
#include <stdint-gcc.h>

/**
* \brief Enable LED.
*/
Response_t LedControllerEnable(void);
/**
* \brief Get current enable status.
*/
uint8_t LedControllerIsEnabled(void);
/**
* \brief Disable LED.
*/
Response_t LedControllerDisable(void);
/**
* \brief Turn LED off.
*/
void LedControllerOff(void);

/**
* \brief Turn LED on.
*/
void LedControllerGlow(void);

/**
* \brief Start fast LED blink.
*/
void LedControllerBlinkFast(void);

/**
* \brief Start medium LED blink.
*/
void LedControllerBlinkMid(void);

/**
* \brief Start slow LED blink.
*/
void LedControllerBlinkSlow(void);
