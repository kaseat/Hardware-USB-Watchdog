#pragma once
#include "ISubscriber.h"
#include "Exti.h"
#include "Timer.h"
#include "Rebooter.h"
#include "Response.h"

#define __disableInterrupts
#define __enablrInterrupts

class HddController :ISubscriber
{
public:
	/**
	 * \brief Initialize HddController instance.
	 * \param exti External interrupt driver.
	 * \param timer Timer driver.
	 * \param rebooter Rebooter driver.
	 */
	HddController(Exti & exti, Timer & timer, Rebooter & rebooter);

	/**
	 * \brief Starts Hdd monitoring.
	 * \return Returns command response.
	 */
	_virtual Response StartHddMonitor();

	/**
	 * \brief Stops Hdd monitoring.
	 * \return Returns command response.
	 */
	_virtual Response StopHddMonitor();

	/**
	 * \brief Pause Hdd monitoring.
	 */
	_virtual void PauseHddMonitor();

	/**
	 * \brief Resume Hdd monitoring.
	 */
	_virtual void ResumeHddMonitor();

	/**
	 * \brief 
	 * \return Returns HDD monitor state.
	 */
	_virtual uint8_t GetStatus();

	/**
	 * \brief Set hdd resrt timeout (0-7).
	 * \param command SteTimeout command.
	 * \return Returns command response. 
	 */
	_virtual Response SetTimeout(uint8_t command);

	_virtual Response SetRebootAttempts(uint8_t command);

	/**
	 * \brief Dispose HddController.
	 */
	~HddController();
private:
	void Callback(uint8_t data) override;
	Exti & exti;
	Timer & timer;
	Rebooter & rebooter;
	uint32_t counter;
	uint32_t hddTimeout;
	uint8_t hddAttempt;
	uint8_t status;
};
