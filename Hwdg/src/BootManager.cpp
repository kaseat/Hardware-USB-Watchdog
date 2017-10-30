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

#include "BootManager.h"

#define DISABLE_TOKEN        ((uint8_t)0x55)
#define ENABLE_TOKEN         ((uint8_t)0xAA)
#define FLASH_RASS_KEY1      ((uint8_t)0xAE)
#define FLASH_RASS_KEY2      ((uint8_t)0x56)
#define CMD_TIMEOUT          ((uint16_t)40000)

__no_init uint8_t __eeprom EnPwrPulse;
__no_init uint8_t __eeprom EnRstPulse;

BootManager::BootManager(Rebooter& rebooter, Timer& timer) : rebooter(rebooter), counter(0), lock(1)
{
	timer.SubscribeOnElapse(*this);

	if (EnPwrPulse == ENABLE_TOKEN || EnRstPulse == ENABLE_TOKEN)
		counter = 0;

	while (counter < 1000)
		;

	counter = 0;
	if (EnPwrPulse == ENABLE_TOKEN)
		while (BootManager::rebooter.PwrPulse() == Busy)
			if (counter > 1000) return;

	counter = 0;
	if (EnRstPulse == ENABLE_TOKEN)
		while (BootManager::rebooter.SoftReset() == Busy)
			if (counter > 1000) return;

	timer.UnsubscribeOnElapse(*this);
}

BootManager::~BootManager()
{
}

Response BootManager::PwrPulseOnStartupEnable()
{
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	EnPwrPulse = ENABLE_TOKEN;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);
	return EnRstPulse == ENABLE_TOKEN
		       ? PwrPulseOnStartupEnableOk
		       : SaveSettingsError;
}

Response BootManager::PwrPulseOnStartupDisable()
{
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	EnPwrPulse = DISABLE_TOKEN;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);
	return EnRstPulse == DISABLE_TOKEN
		       ? PwrPulseOnStartupDisableOk
		       : SaveSettingsError;
}

Response BootManager::RstPulseOnStartupEnable()
{
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	EnRstPulse = ENABLE_TOKEN;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);
	return EnRstPulse == ENABLE_TOKEN
		       ? RstPulseOnStartupEnableOk
		       : SaveSettingsError;
}

Response BootManager::RstPulseOnStartupDisable()
{
	FLASH->DUKR = FLASH_RASS_KEY1;
	FLASH->DUKR = FLASH_RASS_KEY2;
	EnRstPulse = DISABLE_TOKEN;
	FLASH->IAPSR = uint8_t(~FLASH_IAPSR_DUL);
	return EnRstPulse == DISABLE_TOKEN
		       ? RstPulseOnStartupDisableOk
		       : SaveSettingsError;
}

void BootManager::Callback(uint8_t data)
{
	counter++;
}
