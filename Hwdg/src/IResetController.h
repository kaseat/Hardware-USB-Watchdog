#pragma once
#include <stdint.h>

class IResetController
{
public:
	/**
	* \brief Allow watchdog restart computer via reset button.
	*/
	virtual void EnableSoftReset() = 0;

	/**
	* \brief Disable watchdog restart computer via reset button.
	*/
	virtual void DisableSoftReset() = 0;

	/**
	* \brief Allow watchdog restart computer via reset button.
	*/
	virtual void EnableHardReset() = 0;

	/**
	* \brief Disable watchdog restart computer via reset button.
	*/
	virtual void DisableHardReset() = 0;

	/**
	* \brief Ping watchdog.
	*/
	virtual void Ping() = 0;

	/**
	 * \brief Set soft reset timeout.
	 * \param timeout Timeout (1-63).
	 */
	virtual void SetSoftResetTimeout(uint8_t timeout) = 0;
        
	 /**
	 * \brief Set hard reset timeout.
	 * \param timeout Timeout(1-127).
	 */
	virtual void SetHardResetTimeout(uint8_t timeout) = 0;

	/**
	 * \brief Set soft reset attempts count.
	 * \param attempts Attempts count.
	 */
	virtual void SetSoftResetAttempts(uint8_t attempts) = 0;
        
	/**
	 * \brief Set hard reset attempts count.
	 * \param attempts Attempts count.
	 */
	virtual void SetHardResetAttempts(uint8_t attempts) = 0;

protected:
	virtual ~IResetController() {}
};