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

#include <stdint-gcc.h>

/**
 * \brief Represents USB driver.
 */
typedef struct
{
	/**
	* \brief This routine initializes USB driver.
	*/
	void (*Init)(void);

	/**
	* \brief This function must be called at regular intervals from the
	* main loop. Maximum delay between calls is somewhat less than 50ms
	* (USB timeout for accepting a Setup message). Otherwise the device
	* will not be recognized.
	*/
	void (*Poll)(void);

	/**
	* \brief This routine calls when device received a command via USB.
	* \return Returns command value.
	*/
	void (*OnCommandReceived)(uint8_t data);
} UsbDriver_t;

const UsbDriver_t Usb;
