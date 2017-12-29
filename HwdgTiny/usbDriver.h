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
#include "usbdrv.h"
#include <util/delay.h>
/**
* \brief This routine initializes USB driver.
*/
__inline void UsbInit(void)
{
	usbInit();
	usbDeviceDisconnect();
	_delay_ms(250);
	usbDeviceConnect();
}
/**
* \brief This function must be called at regular intervals from the
* main loop. Maximum delay between calls is somewhat less than 50ms
* (USB timeout for accepting a Setup message). Otherwise the device
* will not be recognized.
*/
__inline void UsbPoll(void)
{
	usbPoll();
}

/**
 * \brief This function is called by USB driver
 * when a new command received from the host.
 * \param data Received command.
 */
void OnCommandReceived(uint8_t data);
