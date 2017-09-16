#pragma once
#include "ISubscriber.h"
#include "GpioDriver.h"
#include "Timer.h"

/**
 * \brief Represents LED controller (assumes callback called once per ms).
 */
class LedController : ISubscriber
{
public:
	/**
	* \brief Create LED controller instance.
	* \param timer Time base source.
	* \param driver GPIO pin driver.
	*/
	LedController(Timer& timer, GpioDriver& driver);

	/**
	 * \brief Dispose LED controller.
	 */
	~LedController();

	/**
	 * \brief Turn LED off.
	 */
	virtual void Off();

	/**
	 * \brief Turn LED on.
	 */
	virtual void Glow();

	/**
	 * \brief Start fast LED blink.
	 */
	virtual void BlinkFast();

	/**
	* \brief Start slow LED blink.
	*/
	virtual void BlinkSlow();
private:
	Timer& timer;
	GpioDriver& driver;
	uint_least8_t state;
	uint_fast16_t counter;
	void Callback(uint8_t data);
	void Blink(uint32_t timeout);
};
