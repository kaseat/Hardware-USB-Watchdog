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

#pragma once
#include <stdint-gcc.h>
#include "Common.h"

#define SETTINGS_DEFAULT_0            ((uint8_t)0x1C)
#define SETTINGS_DEFAULT_1            ((uint8_t)0x44)
#define SETTINGS_DEFAULT_2            ((uint8_t)0x48)
#define SETTINGS_DEFAULT_3            ((uint8_t)0x00)

#define LED_DISABLED                  ((uint8_t)(1U << 0U))
#define EVENTS_ENABLED                ((uint8_t)(1U << 1U))
#define APPLY_SETTINGS_AT_STARTUP     ((uint8_t)(1U << 2U))
#define RST_PULSE_ENABLED             ((uint8_t)(1U << 3U))
#define PWR_PULSE_ENABLED             ((uint8_t)(1U << 4U))

/**
 * \brief Save user HWDG settings into NVRAM.
 * \param status Status to be saved.
 * \return Returns true if save operation succeeded, otherwise false.
 */
uint8_t SettingsManagerSaveUserSettings(Status_t* status);

/**
 * \brief Fetch user HWDG settings from NVRAM.
 * \return Returns user settings.
 */
Status_t SettingsManagerObtainUserSettings(void);

/**
 * \brief Apply user settings at startup.
 * \return Returns operation status.
 */
Response_t SettingsManagerApplyUserSettingsAtStartup(void);

/**
 * \brief Load default settings at startup.
 * \return Returns operation status.
 */
Response_t SettingsManagerLoadDefaultSettingsAtStartup(void);

/**
 * \brief Restore factory settings.
 * \return Returns operation statuss.
 */
uint8_t SettingsManagerRestoreFactory(void);

/**
 * \brief Get boot settings.
 */
uint8_t SettingsManagerGetBootSettings(void);
