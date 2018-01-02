// Copyright 2018 Oleg Petrochenko
// 
// This file is part of HwdgTiny.
// 
// HwdgTiny is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
// 
// HwdgTiny is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with HwdgTiny. If not, see <http://www.gnu.org/licenses/>.

#include "BootManager.h"
#include "SettingsManager.h"
#include "LedController.h"
#include "ResetController.h"

void BootManagerProceedBoot()
{
	// Obtain settings data.
	const Status_t settings = SettingsManagerObtainUserSettings();

	// If we apply user settings at startup we must config
	// reset controller and LED controller.
	if (settings.LoadUserSettings)
	{
		settings.IsLedDisabled
			? LedControllerDisable()
			: LedControllerEnable();
		ResetControllerSetRebootTimeout(settings.RebootTimeout);
		ResetControllerSetResponseTimeout(settings.ResponseTimeout);
		ResetControllerSetSoftResetAttempts(settings.ResetAttempts);
	}
	// Otherwise we just turn LED on.
	else
	{
		LedControllerEnable();
	}
}
