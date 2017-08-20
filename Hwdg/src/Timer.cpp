#include "Timer.h"
#include "Clock.h"
#include <stdint.h>

#define ARR_VAL 0x7C


Timer::Timer()
{
	disposed = false;
	for (auto i = 0; i < MAX_TIMER_SUBSCRIBERS; i++)
		subscribers[i] = 0;
}

void Timer::Run()
{
	// Run timer logic
}

void Timer::Stop()
{
	// Stop timer logic
}

void Timer::SubscribeOnElapse(ISubscriber& sbcr)
{
	for (uint_fast8_t i = 0; i < MAX_TIMER_SUBSCRIBERS; i++)
		if (subscribers[i] == 0)
		{
			subscribers[i] = &sbcr;
			return;
		}
}

void Timer::UnsubscribeOnElapse(ISubscriber& sbcr)
{
	for (uint_fast8_t i = 0; i < MAX_TIMER_SUBSCRIBERS; i++)
		if (subscribers[i] == &sbcr) subscribers[i] = 0;
}

#pragma vector=TIM4_OVR_UIF_vector
void Timer::OnElapse()
{
	for (uint_fast8_t i = 0; i < MAX_TIMER_SUBSCRIBERS; i++)
		if (subscribers[i] != 0) subscribers[i]->Callback(0);
}