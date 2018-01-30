// Copyright 2018 Oleg Petrochenko
// 
// This file is part of Hwdg.
// 
// Hwdg is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
// 
// Hwdg is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Hwdg. If not, see <http://www.gnu.org/licenses/>.

#include "Exti.h"
#include "STM8S003F3.h"
#include <stdint.h>

IExtiInterruptable* Exti::subscriber = nullptr;
#define HDD_PIN ((uint8_t)(1 << 3))
#define HDD_PORT GPIOA

Exti::Exti()
{
#ifdef __IAR_SYSTEMS_ICC__
	EXTI->CR1 = EXTI_CR1_PAIS_1;
	HDD_PORT->DDR &= ~HDD_PIN;
	HDD_PORT->CR1 |= HDD_PIN;
#endif
}

void Exti::SubscribeOnExti(IExtiInterruptable& sbcr)
{
	subscriber = &sbcr;
#ifdef __IAR_SYSTEMS_ICC__
	HDD_PORT->CR2 |= HDD_PIN;
#endif
}

void Exti::UnsubscribeOnExti()
{
	subscriber = nullptr;
#ifdef __IAR_SYSTEMS_ICC__
	HDD_PORT->CR2 &= ~HDD_PIN;
#endif
}

#ifdef __IAR_SYSTEMS_ICC__
#pragma vector=EXTI0_ISR
#endif
__interrupt void Exti::OnExti()
{
	if (subscriber == nullptr) return;
	subscriber->OnExtiInterrupt();
}
