// Copyright 2017 Oleg Petrochenko
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include  "ResetController.h"
#ifdef __IAR_SYSTEMS_ICC__
#define _virtual
#define _override
#else
#define _virtual virtual
#define _override override
#endif

/**
 * \brief Represents boot manager.
 */
class BootManager : ISubscriber
{
public:
	/**
	 * \brief Create boot manager instance
	 * \param rebooter rebooter reference
	 */
	BootManager(ResetController& rctrl, Timer& timer);

	/**
	 * \brief Dispose CommandManager.
	 */
	_virtual ~BootManager();

	/**
	 * \brief Enable power pin pulse on chip power on.
	 * \return Returns operation status.
	 */
	_virtual Response PwrPulseOnStartupEnable();

	/**
	 * \brief Disable power pin pulse on chip power on.
	 * \return Returns operation status.
	 */
	_virtual Response PwrPulseOnStartupDisable();

	/**
	 * \brief Enable reset pin pulse on chip power on.
	 * \return Returns operation status.
	 */
	_virtual Response RstPulseOnStartupEnable();

	/**
	 * \brief Disable reset pin pulse on chip power on.
	 * \return Returns operation status.
	 */
	_virtual Response RstPulseOnStartupDisable();

	/**
	 * \brief Save user HWDG settings into EEPROM.
	 * \param status Status to be saved.
	 * \return Returns true if save operation succeeded, otherwise false.
	 */
	_virtual bool SaveUserSettings(uint32_t status);

	/**
	 * \brief Fetch user HWDG settings from EEPROM.
	 * \return Returns user settings.
	 */
	_virtual uint32_t FetchUserSettings();

	/**
	 * \brief Apply user settings at startup.
	 * \return Returns operation status.
	 */
	_virtual Response ApplyUserSettingsAtStartup();

	/**
	 * \brief Load default settings at startup.
	 * \return Returns operation status.
	 */
	_virtual Response LoadDefaultSettingsAtStartup();

	/**
	 * \brief Determine whether user settings applied at startup.
	 * \return Returns true if HWDG applies user settings at startup, otherwise returns false.
	 */
	_virtual bool IsUserSettingsAppliedAtStartup();
private:
	void Callback(uint8_t data) _override;
	ResetController& rctrl;
	uint32_t counter;
};
