#include "Clock.h"
CpuFreq curentFreq = Freq2Mhz;

CpuFreq Clock::GetCpuFreq()
{
  return curentFreq;
}

void Clock::SetCpuFreq(CpuFreq freq)
{

}