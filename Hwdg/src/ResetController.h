#pragma once
#include <stdint.h>
#include "ISubscriber.h"
#include "IResetController.h"
#include "IRebooter.h"

/**
 * \brief Reset controller assumes Callback() calls every 1 ms.
 */
class ResetController : public ISubscriber, public IResetController
{
public:

	/**
	 * \brief Create istnce of Reset controller.
	 * \param rb Rebooter reference.
	 */
	ResetController(IRebooter & rb);

	/**
	 * \brief Dispose reset controller.
	 */
	~ResetController();

	/**
	 * \brief Allow watchdog restart computer via reset button.
	 */
	void EnableSoftReset();

	/**
	 * \brief Disable watchdog restart computer via reset button.
	 */
	void DisableSoftReset();

	/**
	* \brief Allow watchdog restart computer via reset button.
	*/
	void EnableHardReset();

	/**
	* \brief Disable watchdog restart computer via reset button.
	*/
	void DisableHardReset();

	/**
	 * \brief Ping watchdog.
	 */
	void Ping();

	/**
	 * \brief Set soft reset timeout.
	 * \param timeout Timeout (1-63).
	 */
	void SetSoftResetTimeout(uint8_t timeout);
        
	 /**
	 * \brief Set hard reset timeout.
	 * \param timeout Timeout(1-127).
	 */
	void SetHardResetTimeout(uint8_t timeout);

	/**
	 * \brief Set soft reset attempts count.
	 * \param attempts Attempts count.
	 */
	void SetSoftResetAttempts(uint8_t attempts);

	/**
	 * \brief Set hard reset attempts count.
	 * \param attempts Attempts count.
	 */
	void SetHardResetAttempts(uint8_t attempts);
private:
	void Callback(uint8_t data);

	IRebooter& rebooter;
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
