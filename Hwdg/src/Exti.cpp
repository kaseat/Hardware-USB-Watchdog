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

#include "Exti.h"
#include <stdint.h>
IExtiInterruptable* Exti::subscriber = nullptr;
#define HDD_PIN ((uint8_t)(1 << 7))
#define HDD_PORT GPIOC

Exti::Exti()
{
#ifdef __IAR_SYSTEMS_ICC__
	EXTI->CR1 = EXTI_CR1_PCIS_0;
	HDD_PORT->DDR &= ~HDD_PIN;
	HDD_PORT->CR1 &= ~HDD_PIN;
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
#pragma vector=EXTI2_ISR
#endif
__interrupt void Exti::OnExti()
{
	if (subscriber == nullptr) return;
	subscriber->OnExtiInterrupt();
}
