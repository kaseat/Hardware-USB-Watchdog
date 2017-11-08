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
#include <stdint.h>
#include "PlatformDefinitions.h"
#include "ISubscriber.h"

class Uart
{
public:

	/**
	* \brief Initialize UART with specified baud rate and cpu frequency.
	* \param baudrate Target baud rate.
	*/
	Uart(uint32_t baudrate);

	/**
	 * \brief Dispose UART
	 */
	_virtual ~Uart();

	/**
	* \brief Add handler on byte received.
	* \param sbcr Subscriber.
	*/
	_virtual void SubscribeOnByteReceived(ISubscriber& sbcr);

	/**
	* \brief Remove handler on byte received.
	*/
	_virtual void UnsubscribeOnByteReceived();

	/**
	* \brief Send byte through the UART.
	* \param data Data to be sent.
	*/
	_virtual void SendByte(uint8_t data);

	/**
	* \brief Send byte array through the UART.
	* \param data Byte array pointer.
	* \param len Number of bytes to send.
	*/
	_virtual void SendData(uint8_t* data, uint8_t len);

	/**
	 * \brief Executes when new byte received.
	 */
	__interrupt static void OnByteReceived();
private:
	static ISubscriber* subscriber;
};
