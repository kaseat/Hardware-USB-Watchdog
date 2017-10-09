#pragma once
#include <stdint.h>
#include "ISubscriber.h"
#include  "ResetController.h"
#include "Uart.h"
#ifdef __IAR_SYSTEMS_ICC__
#define _override
#else
#define _override override
#endif
class CommandManager :ISubscriber
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
	void Callback(uint8_t data) _override;
	void CmdSelector(uint8_t data) const;
};
