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
#include <stdint.h>
#include "ISubscriber.h"
#include "STM8S003F3.h"

#ifdef __IAR_SYSTEMS_ICC__
#define _virtual
#else
#define _virtual virtual
#endif

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
	_virtual ~Uart()
	{
	}

	/**
	* \brief Add handler on byte recivied.
	* \param sbcr Subscriber.
	*/
	_virtual void SubscribeOnByteReceived(ISubscriber& sbcr);

	/**
	* \brief Remove handler on byte recivied.
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

private:
	__interrupt static void OnByteReceived();
	static ISubscriber* subscriber;
};
