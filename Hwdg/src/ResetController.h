#pragma once
#include <stdint.h>
#include "ISubscriber.h"
#include "Rebooter.h"
#include "Timer.h"
#include "LedController.h"
#include "Response.h"
#include "IExtiInterruptable.h"
#include "Exti.h"

#ifdef __IAR_SYSTEMS_ICC__
#define _virtual
#define _override
#else
#define _virtual virtual
#define _override override
#endif

class IResetControllerEventHandler;

/**
 * \brief Reset controller assumes Callback() calls every 1 ms.
 */
class ResetController : ISubscriber, IExtiInterruptable
{
public:

	/**
	 * \brief Create istnce of Reset controller.
	 * \param rb Rebooter reference.
	 */
	ResetController(Timer & timer, Rebooter & rb, LedController & ledController, Exti & exti);
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
	* \brief Starts Hdd monitoring.
	* \return Returns command response.
	*/
	_virtual Response EnableHddMonitor();

	/**
	* \brief Stops Hdd monitoring.
	* \return Returns command response.
	*/
	_virtual Response DisableHddMonitor();

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
	 * \param timeout Timeout (0-59).
	 * \remarks see https://hwdg.ru/hardware-watchdog-api/setresponsetimeout/ for more details.
	 */
	_virtual Response SetResponseTimeout(uint8_t timeout);

	/**
	* \brief Set reset timeout.
	* \param timeout Timeout(0-118).
	* \remarks see https://hwdg.ru/hardware-watchdog-api/setreboottimeout/ for more details.
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

	/**
	 * \brief Subscribe event handler.
	 * \param eventHandler Event handler to subscribe.
	 */
	_virtual void SubscribeOnEvents(IResetControllerEventHandler & eventHandler);

	/**
	 * \brief Unsubscribe all event handlers.
	 */
	_virtual void UnSubscribeOnEvents();
	/**
	 * \brief Get rebooter reference.
	 */
	_virtual Rebooter& GetRebooter();
private:
	void Callback(uint8_t data) _override;
	void OnExtiInterrupt() _override;
	IResetControllerEventHandler * eventHandler;
	Timer & timer;
	Rebooter & rebooter;
	LedController & ledController;
	Exti & exti;
	uint32_t counter;
	uint_least8_t state;
	uint32_t responseTimeout;
	uint32_t rebootTimeout;
	uint8_t sAttempt;
	uint8_t hAttempt;
	uint8_t sAttemptCurr;
	uint8_t hAttemptCurr;
};
