#pragma once

#include <stdint.h>
#include "ISubscriber.h"
#include "IResetController.h"

class CommandManager :ISubscriber
{
public:
	/**
	 * \brief Create instance of command manager.
	 * \param rctr Reset controller.
	 */
	CommandManager(IResetController& rctr);

	/**
	 * \brief Dispose Reset controller.
	 */
	virtual ~CommandManager();

private:
	IResetController& resetController;
	void Callback(uint8_t data);
};