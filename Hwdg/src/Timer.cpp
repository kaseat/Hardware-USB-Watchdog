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

#include "Timer.h"
#include <stdint.h>

#ifdef __IAR_SYSTEMS_ICC__
#include "STM8S003F3.h"
#include "Clock.h"
#define ARR_VAL 0x7C
#endif

#if MAX_TIMER_SUBSCRIBERS == 1
ISubscriber* Timer::subscribers[MAX_TIMER_SUBSCRIBERS] = { nullptr };
#elif MAX_TIMER_SUBSCRIBERS == 2
ISubscriber* Timer::subscribers[MAX_TIMER_SUBSCRIBERS] = { nullptr, nullptr };
#elif MAX_TIMER_SUBSCRIBERS == 3
ISubscriber* Timer::subscribers[MAX_TIMER_SUBSCRIBERS] = {nullptr, nullptr, nullptr};
#elif MAX_TIMER_SUBSCRIBERS == 4
ISubscriber* Timer::subscribers[MAX_TIMER_SUBSCRIBERS] = { nullptr, nullptr, nullptr, nullptr };
#elif MAX_TIMER_SUBSCRIBERS == 5
ISubscriber* Timer::subscribers[MAX_TIMER_SUBSCRIBERS] = { nullptr, nullptr, nullptr, nullptr, nullptr };
#else
#error Too much subscribers defined!
#endif

void Timer::Run()
{
#ifdef __IAR_SYSTEMS_ICC__
	// Set TIM4 overflow interrupt every 1 ms.
	switch (Clock::GetCpuFreq())
	{
	case Freq2Mhz:
		TIM4->PSCR = 0x04;
		break;
	case Freq4Mhz:
		TIM4->PSCR = 0x05;
		break;
	case Freq8Mhz:
		TIM4->PSCR = 0x06;
		break;
	case Freq16Mhz:
		TIM4->PSCR = 0x07;
		break;
	}
	TIM4->ARR = ARR_VAL;
	TIM4->IER &= ~TIM4_IER_UIE;
	TIM4->CR1 = TIM4_CR1_CEN;
#endif
}

void Timer::Stop()
{
#ifdef __IAR_SYSTEMS_ICC__
	TIM4->CR1 &= ~TIM4_CR1_CEN;
#endif
}

void Timer::SubscribeOnElapse(ISubscriber& sbcr)
{
#ifdef __IAR_SYSTEMS_ICC__
	TIM4->IER |= TIM4_IER_UIE;
#endif
	for (uint_fast8_t i = 0; i < MAX_TIMER_SUBSCRIBERS; i++)
		if (subscribers[i] == nullptr)
		{
			subscribers[i] = &sbcr;
			return;
		}
}

void Timer::UnsubscribeOnElapse(ISubscriber& sbcr)
{
#ifdef __IAR_SYSTEMS_ICC__
	TIM4->IER &= ~TIM4_IER_UIE;
#endif
	for (uint_fast8_t i = 0; i < MAX_TIMER_SUBSCRIBERS; i++)
		if (subscribers[i] == &sbcr) subscribers[i] = nullptr;
}

#ifdef __IAR_SYSTEMS_ICC__
#pragma vector=TIM4_OVR_UIF_ISR
#endif
__interrupt void Timer::OnElapse()
{
#ifdef __IAR_SYSTEMS_ICC__
	TIM4->SR &= ~TIM4_SR_UIF;
#endif
	for (uint_fast8_t i = 0; i < MAX_TIMER_SUBSCRIBERS; i++)
		if (subscribers[i] != nullptr) subscribers[i]->Callback(0);
}
