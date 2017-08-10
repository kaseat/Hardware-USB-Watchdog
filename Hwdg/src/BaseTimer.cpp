#include "BaseTimer.h"
#include "Clock.h"
#include <stdint.h>

#define ARR_VAL 0x7C

ISubscriber* Timer::subscriber[MAX_SUBSCRIBERS] = { 0, 0, 0 };

void Timer::Run()
{

}

void Timer::Stop()
{

}

void Timer::SubscribeOnElapse(ISubscriber& sbcr)
{
	for (uint_fast8_t i = 0; i < MAX_SUBSCRIBERS; i++)
		if (subscriber[i] == 0)
		{
			subscriber[i] = &sbcr;
			return;
		}
}

void Timer::UnsubscribeOnElapse(ISubscriber& sbcr)
{
	for (uint_fast8_t i = 0; i < MAX_SUBSCRIBERS; i++)
		if (subscriber[i] == &sbcr) subscriber[i] = 0;
}

#pragma vector=TIM4_OVR_UIF_vector
void Timer::OnElapse()
{
	for (uint_fast8_t i = 0; i < MAX_SUBSCRIBERS; i++)
		if (subscriber[i] != 0) subscriber[i]->Callback(0);
}