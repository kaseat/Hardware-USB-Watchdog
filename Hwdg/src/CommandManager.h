#pragma once
#include <stdint.h>
#include "ISubscriber.h"
#include  "ResetController.h"
#include "Uart.h"
#include "IResetControllerEventHandler.h"
#ifdef __IAR_SYSTEMS_ICC__
#define _override
#else
#define _override override
#endif
class CommandManager :ISubscriber,IResetControllerEventHandler
{
public:
	/**
	 * \brief Create instance of command manager.
	 * \param rctr Reset controller.
	 */
	CommandManager(Uart& uart, ResetController& rctr);

	/**
	 * \brief Dispose Reset controller.
	 */
	 ~CommandManager();

private:
	Uart& uart;
	ResetController& resetController;
	inline void Callback(uint8_t data) _override;
	inline void OnUpdted(uint8_t reason) _override;
	inline void GetStatus();
	inline void TestHardReset();
	inline void TestSoftReset();
	inline void EnableEvents();
	inline void DisableEvents();
};
