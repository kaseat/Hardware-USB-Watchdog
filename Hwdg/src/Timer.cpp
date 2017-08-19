#include "Timer.h"
#include "Clock.h"
#include <stdint.h>

#define ARR_VAL 0x7C
#if MAX_SUBSCRIBERS == 1
ISubscriber* Timer::subscribers[MAX_SUBSCRIBERS] = { 0 };
#elif MAX_SUBSCRIBERS == 2
ISubscriber* Timer::subscribers[MAX_SUBSCRIBERS] = { 0, 0 };
#elif MAX_SUBSCRIBERS == 3
ISubscriber* Timer::subscribers[MAX_SUBSCRIBERS] = { 0, 0, 0 };
#elif MAX_SUBSCRIBERS == 4
ISubscriber* Timer::subscribers[MAX_SUBSCRIBERS] = { 0, 0, 0, 0 };
#elif MAX_SUBSCRIBERS == 5
ISubscriber* Timer::subscribers[MAX_SUBSCRIBERS] = { 0, 0, 0, 0, 0 };
#else
#error Too much subscribers defined!
#endif

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
	for (uint_fast8_t i = 0; i < MAX_SUBSCRIBERS; i++)
		if (subscribers[i] == 0)
		{
			subscribers[i] = &sbcr;
			return;
		}
}

void Timer::UnsubscribeOnElapse(ISubscriber& sbcr)
{
	for (uint_fast8_t i = 0; i < MAX_SUBSCRIBERS; i++)
		if (subscribers[i] == &sbcr) subscribers[i] = 0;
}

#pragma vector=TIM4_OVR_UIF_vector
void Timer::OnElapse()
{
	for (uint_fast8_t i = 0; i < MAX_SUBSCRIBERS; i++)
		if (subscribers[i] != 0) subscribers[i]->Callback(0);
}