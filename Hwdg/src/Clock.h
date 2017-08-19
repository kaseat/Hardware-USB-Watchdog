#pragma once

enum  CpuFreq
{
	Freq2Mhz = 2000000U,
	Freq4Mhz = 4000000U,
	Freq8Mhz = 8000000U,
	Freq16Mhz = 16000000U
};

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