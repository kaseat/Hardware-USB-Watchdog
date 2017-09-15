#include "Clock.h"
#include "STM8S003F3.h"
CpuFreq curentFreq = Freq2Mhz;

CpuFreq Clock::GetCpuFreq()
{
	return curentFreq;
}

void Clock::SetCpuFreq(CpuFreq freq)
{
	curentFreq = freq;
	switch (freq)
	{
	case Freq2Mhz:
		CLK->CKDIVR = 3;
		break;
	case Freq4Mhz:
		CLK->CKDIVR = 2;
		break;
	case Freq8Mhz:
		CLK->CKDIVR = 1;
		break;
	case Freq16Mhz:
		CLK->CKDIVR = 0;
		break;
	}
}