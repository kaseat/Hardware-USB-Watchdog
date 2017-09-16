#pragma once
#include "ISubscriber.h"
#include "GpioDriver.h"
#include "Timer.h"

/**
 * \brief Implements reboot logic
 */
class Rebooter : ISubscriber
{
public:
	/**
	 * \brief Create Rebooter instance.
	 * \param timer Time base source.
	 * \param driver GPIO pin driver.
	 */
	Rebooter(Timer& timer, GpioDriver& driver);

	/**
	 * \brief Dispose Rebooter.
	 */
	~Rebooter();

	/**
	 * \brief Satrt hard reset sequence.
	 */
	virtual void HardReset();

	/**
	 * \brief Start soft reset sequence.
	 */
	virtual void SoftReset();

private:
	Timer& timer;
	GpioDriver& driver;
	uint_least8_t state;
	uint_fast16_t counter;
	void Callback(uint8_t data);
};
