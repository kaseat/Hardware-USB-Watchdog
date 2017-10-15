#pragma once
#include <stdint.h>
#include "ISubscriber.h"
#include "Rebooter.h"
#include "Timer.h"
#include "LedController.h"
#include "Response.h"

#ifdef __IAR_SYSTEMS_ICC__
#define _virtual
#define _override
#else
#define _virtual virtual
#define _override override
#endif

/**
 * \brief Reset controller assumes Callback() calls every 1 ms.
 */
class ResetController : ISubscriber
{
public:

	/**
	 * \brief Create istnce of Reset controller.
	 * \param rb Rebooter reference.
	 */
	ResetController(Timer & timer, Rebooter & rb, LedController & ledController);

	/**
	 * \brief Dispose reset controller.
	 */
	~ResetController();

	/**
	* \brief Gets ResetController status.
	*/
	_virtual uint32_t GetStatus();

	/**
	 * \brief Allow watchdog restart computer via reset button.
	 */
	_virtual Response Start();

	/**
	 * \brief Disable watchdog restart computer via reset button.
	 */
	_virtual Response Stop();

	/**
	* \brief Allow watchdog restart computer via reset button.
	*/
	_virtual Response EnableHardReset();

	/**
	* \brief Disable watchdog restart computer via reset button.
	*/
	_virtual Response DisableHardReset();

	/**
	 * \brief Ping watchdog.
	 */
	_virtual Response Ping();

	/**
	 * \brief Set response timeout.
	 * \param timeout Timeout (0-63).
	 */
	_virtual Response SetResponseTimeout(uint8_t timeout);

	/**
	* \brief Set reset timeout.
	* \param timeout Timeout(0-127).
	*/
	_virtual Response SetRebootTimeout(uint8_t timeout);

	/**
	 * \brief Set soft reset attempts count.
	 * \param attempts Attempts count.
	 */
	_virtual Response SetSoftResetAttempts(uint8_t attempts);

	/**
	 * \brief Set hard reset attempts count.
	 * \param attempts Attempts count.
	 */
	_virtual Response SetHardResetAttempts(uint8_t attempts);
private:
	void Callback(uint8_t data) _override;
	Timer & timer;
	Rebooter & rebooter;
	LedController & ledController;
	uint32_t counter;
	uint_least8_t state;
	uint32_t responseTimeout;
	uint32_t rebootTimeout;
	uint8_t sAttempt;
	uint8_t hAttempt;
	uint8_t sAttemptCurr;
	uint8_t hAttemptCurr;
};
