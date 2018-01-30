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
#define PWR_PIN1 (1 << 4)
#define RST_PIN1 (1 << 4)
#define PWR_PIN2 (1 << 3)
#define RST_PIN2 (1 << 5)
#define LED_PIN (1 << 2)
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
	GPIOB->ODR |= PWR_PIN1 | RST_PIN2;
	GPIOC->ODR |= PWR_PIN2 | RST_PIN1;
	GPIOB->DDR |= PWR_PIN1 | RST_PIN2;
	GPIOC->DDR |= PWR_PIN2 | RST_PIN1;
	GPIOD->DDR |= LED_PIN;
	GPIOD->CR1 |= LED_PIN;
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
	GPIOB->ODR &= ~(PWR_PIN1 | RST_PIN2);
	GPIOC->ODR &= ~(PWR_PIN2 | RST_PIN1);
	GPIOB->DDR &= ~(PWR_PIN1 | RST_PIN2);
	GPIOC->DDR &= ~(PWR_PIN2 | RST_PIN1);
	GPIOD->DDR &= LED_PIN;
	GPIOD->CR1 &= LED_PIN;
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
	GPIOC->ODR &= ~RST_PIN1;
	GPIOB->ODR &= ~RST_PIN2;
#endif
#ifdef __AVR__
	pinMode(RST_PIN, OUTPUT);
#endif
}

void GpioDriver::ReleaseReset()
{
#ifdef __ICCSTM8__
	GPIOC->ODR |= RST_PIN1;
	GPIOB->ODR |= RST_PIN2;
#endif
#ifdef __AVR__
	pinMode(RST_PIN, INPUT);
#endif
}

void GpioDriver::DrivePowerLow()
{
#ifdef __ICCSTM8__
	GPIOC->ODR &= ~PWR_PIN2;
	GPIOB->ODR &= ~PWR_PIN1;
#endif
#ifdef __AVR__
	pinMode(PWR_PIN, OUTPUT);
#endif
}

void GpioDriver::ReleasePower()
{
#ifdef __ICCSTM8__
	GPIOC->ODR |= PWR_PIN2;
	GPIOB->ODR |= PWR_PIN1;
#endif
#ifdef __AVR__
	pinMode(PWR_PIN, INPUT);
#endif
}

void GpioDriver::DriveLedLow()
{
#ifdef __ICCSTM8__
	GPIOD->ODR &= ~LED_PIN;
#endif
#ifdef __AVR__
	digitalWrite(LED_PIN, LOW);
#endif
}

void GpioDriver::DriveLedHigh()
{
#ifdef __ICCSTM8__
	GPIOD->ODR |= LED_PIN;
#endif
#ifdef __AVR__
	digitalWrite(LED_PIN, HIGH);
#endif
}
