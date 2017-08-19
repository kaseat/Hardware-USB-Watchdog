#include "Clock.h"
#include <stdint.h>

CpuFreq Clock::currentFrq = Freq2Mhz;

CpuFreq Clock::GetCpuFreq()
{
  return currentFrq;
}

void Clock::SetCpuFreq(CpuFreq freq)
{
	currentFrq = freq;
	// Set CPU freq logic
}