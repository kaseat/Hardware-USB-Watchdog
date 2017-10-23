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

#include "Uart.h"

#ifdef __IAR_SYSTEMS_ICC__
#include "Clock.h"
#endif

#define UART_REGISTER 5
ISubscriber* Uart::subscriber = nullptr;

Uart::Uart(uint32_t baudrate)
{
#ifdef __IAR_SYSTEMS_ICC__
	// Configure GPIOs
	GPIOD->ODR |= 1 << 5;
	GPIOD->DDR |= 1 << 5;
	GPIOD->DDR &= ~(1 << 6);
	GPIOD->CR1 &= ~(1 << 6);
	GPIOD->CR2 &= ~(1 << 6);

	// Baudrate configuration.
	uint32_t brr = Clock::GetCpuFreq() / baudrate;
	UART1->BRR2 = brr & 0x000F;
	UART1->BRR2 |= brr >> 12;
	UART1->BRR1 = (brr >> 4) & 0x00FF;

	// UART configuration.
	UART1->CR1 = 0;
	UART1->CR2 = UART_CR2_TEN | UART_CR2_REN;
	UART1->CR3 = 0;
#endif
}

void Uart::SubscribeOnByteReceived(ISubscriber& sbcr)
{
	subscriber = &sbcr;
#ifdef __IAR_SYSTEMS_ICC__
	UART1->CR2 |= UART_CR2_RIEN;
#endif
}

void Uart::UnsubscribeOnByteReceived()
{
	subscriber = nullptr;
#ifdef __IAR_SYSTEMS_ICC__
	UART1->CR2 &= ~(UART_CR2_RIEN);
#endif
}

void Uart::SendByte(uint8_t data)
{
#ifdef __IAR_SYSTEMS_ICC__
	while (!(UART1->SR & UART_SR_TXE))
		;
	UART1->DR = data;
#endif
}

void Uart::SendData(uint8_t* data, uint8_t len)
{
	while (len--) SendByte(*data++);
}

#ifdef __IAR_SYSTEMS_ICC__
#pragma vector=UART1_R_RXNE_ISR
#endif
__interrupt void Uart::OnByteReceived()
{
	if (subscriber == nullptr) return;
#ifdef __IAR_SYSTEMS_ICC__
	subscriber->Callback(UART1->DR);
#else
	subscriber->Callback(UART_REGISTER);
#endif
}
