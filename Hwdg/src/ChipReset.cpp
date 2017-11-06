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

#include "ChipReset.h"

#ifdef __ICCSTM8__
#include "STM8S003F3.h"
#endif
#ifdef __AVR__
#include <arduino.h>
#define CHIP_RESET_PIN   ((uint8_t)10U)
#endif

void ChipReset::ResetImmediately()
{
#ifdef __ICCSTM8__
	WWDG->CR = WWDG_CR_WDGA;
#endif
#ifdef __AVR__
	pinMode(CHIP_RESET_PIN, OUTPUT);
#endif
}
