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

#include "Uart.h"

#ifdef __ICCSTM8__
#include "Clock.h"
#include "STM8S003F3.h"
#endif

#ifdef _M_IX86
#define UART_REGISTER 5
#endif

#ifdef __AVR__
#include "Arduino.h"
#endif

ISubscriber* Uart::subscriber = nullptr;

Uart::Uart(uint32_t baudrate)
{
#ifdef __ICCSTM8__
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

Uart::~Uart()
{
}

void Uart::SubscribeOnByteReceived(ISubscriber& sbcr)
{
	subscriber = &sbcr;
#ifdef __ICCSTM8__
	UART1->CR2 |= UART_CR2_RIEN;
#endif
}

void Uart::UnsubscribeOnByteReceived()
{
	subscriber = nullptr;
#ifdef __ICCSTM8__
	UART1->CR2 &= ~(UART_CR2_RIEN);
#endif
}

void Uart::SendByte(uint8_t data)
{
#ifdef __ICCSTM8__
	while (!(UART1->SR & UART_SR_TXE))
		;
	UART1->DR = data;
#endif
#ifdef __AVR__
	Serial.write(data);
#endif
}

void Uart::SendData(uint8_t* data, uint8_t len)
{
#ifdef __ICCSTM8__
	while (len--) SendByte(*data++);
#endif
#ifdef __AVR__
	Serial.write(data, len);
#endif
}


#ifdef __AVR__
void serialEvent()
{
	Uart::OnByteReceived();
}
#endif
#ifdef __ICCSTM8__
#pragma vector=UART1_R_RXNE_ISR
#endif
__interrupt void Uart::OnByteReceived()
{
	if (subscriber == nullptr) return;
#ifdef __ICCSTM8__
	subscriber->Callback(UART1->DR);
#endif
#ifdef _M_IX86
	subscriber->Callback(UART_REGISTER);
#endif
#ifdef __AVR__
	subscriber->Callback(Serial.read());
#endif
}
