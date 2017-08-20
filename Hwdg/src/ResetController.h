#pragma once
#include <stdint.h>
#include "ISubscriber.h"
#include "Rebooter.h"
#include "Timer.h"

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
	ResetController(Timer & timer, Rebooter & rb);

	/**
	 * \brief Dispose reset controller.
	 */
	~ResetController();

	/**
	 * \brief Allow watchdog restart computer via reset button.
	 */
	virtual void EnableSoftReset();

	/**
	 * \brief Disable watchdog restart computer via reset button.
	 */
	virtual void DisableSoftReset();

	/**
	* \brief Allow watchdog restart computer via reset button.
	*/
	virtual void EnableHardReset();

	/**
	* \brief Disable watchdog restart computer via reset button.
	*/
	virtual void DisableHardReset();

	/**
	 * \brief Ping watchdog.
	 */
	virtual void Ping();

	/**
	 * \brief Set response timeout.
	 * \param timeout Timeout (0-63).
	 */
	virtual void SetResponseTimeout(uint8_t timeout);
        
	 /**
	 * \brief Set reset timeout.
	 * \param timeout Timeout(0-127).
	 */
	virtual void SetResetTimeout(uint8_t timeout);

	/**
	 * \brief Set soft reset attempts count.
	 * \param attempts Attempts count.
	 */
	virtual void SetSoftResetAttempts(uint8_t attempts);

	/**
	 * \brief Set hard reset attempts count.
	 * \param attempts Attempts count.
	 */
	virtual void SetHardResetAttempts(uint8_t attempts);
private:
	void Callback(uint8_t data);
	Timer & timer;
	Rebooter& rebooter;
	uint32_t counter;
	uint32_t softResetTimeout;
	uint32_t hardResetTimeout;
	bool softRebootEnabled;
	bool hardRebootEnabled;
	bool softRebootOccured;
	uint8_t sAttempt;
	uint8_t hAttempt;
	uint8_t sAttemptDef;
	uint8_t hAttemptDef;
};
