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

#include "GpioDriver.h"

#ifdef __ICCSTM8__
#include "STM8S003F3.h"
#define PWR_PIN (1 << 2)
#define RST_PIN (1 << 3)
#define LED_PIN (1 << 6)
#endif

#ifdef __AVR__
#include "Arduino.h"
#ifndef PWR_PIN
#define PWR_PIN 11
#endif
#ifndef RST_PIN
#define RST_PIN 12
#endif
#ifndef LED_PIN
#define LED_PIN LED_BUILTIN
#endif
#endif

GpioDriver::GpioDriver()
{
#ifdef __ICCSTM8__
	GPIOD->ODR |= PWR_PIN | RST_PIN;
	GPIOD->DDR |= PWR_PIN | RST_PIN;
	GPIOC->DDR |= LED_PIN;
	GPIOC->CR1 |= LED_PIN;
#endif
#ifdef __AVR__
	pinMode(PWR_PIN, INPUT);
	pinMode(RST_PIN, INPUT);
	pinMode(LED_PIN, OUTPUT);
#endif
}

GpioDriver::~GpioDriver()
{
#ifdef __ICCSTM8__
	GPIOD->ODR &= ~(PWR_PIN | RST_PIN);
	GPIOD->DDR &= ~(PWR_PIN | RST_PIN);
	GPIOC->DDR &= ~LED_PIN;
	GPIOC->CR1 &= ~LED_PIN;
#endif
#ifdef __AVR__
	pinMode(PWR_PIN, INPUT);
	pinMode(RST_PIN, INPUT);
	pinMode(LED_PIN, INPUT);
#endif
}

void GpioDriver::DriveResetLow()
{
#ifdef __ICCSTM8__
	GPIOD->ODR &= ~RST_PIN;
#endif
#ifdef __AVR__
	pinMode(RST_PIN, OUTPUT);
#endif
}

void GpioDriver::ReleaseReset()
{
#ifdef __ICCSTM8__
	GPIOD->ODR |= RST_PIN;
#endif
#ifdef __AVR__
	pinMode(RST_PIN, INPUT);
#endif
}

void GpioDriver::DrivePowerLow()
{
#ifdef __ICCSTM8__
	GPIOD->ODR &= ~PWR_PIN;
#endif
#ifdef __AVR__
	pinMode(PWR_PIN, OUTPUT);
#endif
}

void GpioDriver::ReleasePower()
{
#ifdef __ICCSTM8__
	GPIOD->ODR |= PWR_PIN;
#endif
#ifdef __AVR__
	pinMode(PWR_PIN, INPUT);
#endif
}

void GpioDriver::DriveLedLow()
{
#ifdef __ICCSTM8__
	GPIOC->ODR &= ~LED_PIN;
#endif
#ifdef __AVR__
	digitalWrite(LED_PIN, LOW);
#endif
}

void GpioDriver::DriveLedHigh()
{
#ifdef __ICCSTM8__
	GPIOC->ODR |= LED_PIN;
#endif
#ifdef __AVR__
	digitalWrite(LED_PIN, HIGH);
#endif
}
