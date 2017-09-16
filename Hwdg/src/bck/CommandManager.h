#pragma once
#include <stdint.h>
#include "ISubscriber.h"
#include  "ResetController.h"
#include "Uart.h"

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
	virtual ~CommandManager();

private:
	Uart& uart;
	ResetController& resetController;
	void Callback(uint8_t data);
};
