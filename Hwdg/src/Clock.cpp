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
