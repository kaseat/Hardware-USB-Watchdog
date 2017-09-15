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
ISubscriber* Timer::subscribers[MAX_TIMER_SUBSCRIBERS] = { nullptr, nullptr, nullptr };
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
__interrupt void Timer::OnElapse()
{
TIM4->SR &= ~TIM4_SR_UIF;
#else
void Timer::OnElapse()
{
#endif
	for (uint_fast8_t i = 0; i < MAX_TIMER_SUBSCRIBERS; i++)
		if (subscribers[i] != nullptr) subscribers[i]->Callback(0);
}