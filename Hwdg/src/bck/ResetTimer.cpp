#include "ResetTimer.h"
#include "Clock.h"


ISubscriber* ResetTimer::subscriber = 0;

void ResetTimer::Run(uint16_t timebase)
{
	// Set TIM4 overflow interrupt every 1 ms.
	switch (Clock::GetCpuFreq())
	{
	case Freq2Mhz:

		break;
	case Freq4Mhz:

		break;
	case Freq8Mhz:

		break;
	case Freq16Mhz:

		break;
	}

}

void ResetTimer::Stop()
{

}

void ResetTimer::SubscribeOnElapse(ISubscriber& sbcr)
{
	subscriber = &sbcr;

}

void ResetTimer::UnsubscribeOnElapse()
{

}

#pragma vector=TIM2_OVR_UIF_vector
void ResetTimer::OnElapse()
{

	subscriber->Callback(0);
}