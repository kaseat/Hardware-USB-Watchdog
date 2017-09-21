#pragma once
#include "ISubscriber.h"
#include "GpioDriver.h"
#include "Timer.h"

#ifdef __IAR_SYSTEMS_ICC__
#define _virtual
#define _override
#else
#define _virtual virtual
#define _override override
#endif

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
	_virtual void Off();

	/**
	 * \brief Turn LED on.
	 */
	_virtual void Glow();

	/**
	 * \brief Start fast LED blink.
	 */
	_virtual void BlinkFast();

	/**
	* \brief Start slow LED blink.
	*/
	_virtual void BlinkSlow();
private:
	Timer& timer;
	GpioDriver& driver;
	uint_least8_t state;
	uint_fast16_t counter;
	void Callback(uint8_t data) _override;
	void Blink(uint32_t timeout);
};
