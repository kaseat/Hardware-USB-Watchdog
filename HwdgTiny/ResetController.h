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

/**
* \brief Gets ResetController status.
* \remarks see https://hwdg.ru/developer/hwdg-api/getstatus/ for more details.
*/
uint32_t ResetControllerGetStatus(void);

/**
* \brief Allow watchdog restart computer via reset button.
* \remarks see https://hwdg.ru/developer/hwdg-api/start/ for more details.
*/
Response_t ResetControllerStart(void);

/**
* \brief Disable watchdog restart computer via reset button.
* \remarks see https://hwdg.ru/developer/hwdg-api/stop/ for more details.
*/
Response_t ResetControllerStop(void);

/**
* \brief Ping watchdog.
* \remarks see https://hwdg.ru/developer/hwdg-api/ping/ for more details.
*/
Response_t ResetControllerPing(void);

/**
* \brief Set response timeout.
* \param timeout Timeout (0-59).
* \remarks see https://hwdg.ru/developer/hwdg-api/setresponsetimeout/ for more details.
*/
Response_t ResetControllerSetResponseTimeout(uint8_t timeout);

/**
* \brief Set reboot timeout.
* \param timeout Timeout(0-118).
* \remarks see https://hwdg.ru/developer/hwdg-api/setreboottimeout/ for more details.
*/
Response_t ResetControllerSetRebootTimeout(uint8_t timeout);

/**
* \brief Set soft reset attempts count.
* \param attempts Attempts count.
* \remarks see https://hwdg.ru/developer/hwdg-api/setsoftresetattempts/ for more details.
*/
Response_t ResetControllerSetSoftResetAttempts(uint8_t attempts);

/**
* \brief Test soft reset.
*/
Response_t ResetControllerTestSoftReset(void);
