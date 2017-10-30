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

#pragma once

enum CpuFreq
{
	Freq2Mhz = 2000000U,
	Freq4Mhz = 4000000U,
	Freq8Mhz = 8000000U,
	Freq16Mhz = 16000000U
};

/**
 * \brief Represents clock control driver.
 */
class Clock
{
public:
	/**
	 * \brief Get current CPU frequency.
	 * \return Returns current CPU freqency.
	 */
	static CpuFreq GetCpuFreq();

	/**
	 * \brief Set CPU frequency.
	 * \param freq Freqency we wanna set.
	 */
	static void SetCpuFreq(CpuFreq freq);

private:
	static CpuFreq currentFrq;
};
