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

	for (uint_fast8_t i = 0; i < MAX_TIMER_SUBSCRIBERS; i++)
		if (subscribers[i] == &sbcr) subscribers[i] = nullptr;
	bool hasSubscribers = false;
	for (uint_fast8_t i = 0; i < MAX_TIMER_SUBSCRIBERS; i++)
		if (subscribers[i] != nullptr)
		{
			hasSubscribers = true;
			break;
		}
	if (!hasSubscribers)
	{
#ifdef __IAR_SYSTEMS_ICC__
		TIM4->IER &= ~TIM4_IER_UIE;
#endif
	}
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
