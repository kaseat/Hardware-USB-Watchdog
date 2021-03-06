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

#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"
#include "../Hwdg/src/LedController.h"

#define SLOW_BLINK_TIMEOUT 50
#define FAST_BLINK_TIMEOUT 30
// ReSharper disable once CppUnusedIncludeDirective
#include "../Hwdg/src/LedController.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

namespace HwdgTests
{
	TEST_CLASS(LedControllerTests)
	{
	public:

		/**
		* \brief ID: 550001
		*/
		TEST_METHOD(VerifyLedControllerOffAndOffMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.Off();
			controller.Off();
			for (auto i = 0; i < 1000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).AtLeastOnce();
			Verify(Method(driver, DriveLedHigh)).Never();
		}

		/**
		* \brief ID: 550002
		*/
		TEST_METHOD(VerifyLedControllerOffAndGlowMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.Off();
			controller.Glow();
			for (auto i = 0; i < 1000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Once();
			Verify(Method(driver, DriveLedHigh)).Once();
		}

		/**
		* \brief ID: 550003
		*/
		TEST_METHOD(VerifyLedControllerOffAndBlinkSlowMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.Off();
			for (auto i = 0; i < 5000; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Once();
			controller.BlinkSlow();
			for (auto i = 0; i < SLOW_BLINK_TIMEOUT * 2; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Exactly(3);
			Verify(Method(driver, DriveLedHigh)).Once();
		}

		/**
		* \brief ID: 550004
		*/
		TEST_METHOD(VerifyLedControllerOffAndBlinkFastMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.Off();
			for (auto i = 0; i < 2000; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Once();
			controller.BlinkFast();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 2; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Exactly(3);
			Verify(Method(driver, DriveLedHigh)).Once();
		}

		/**
		* \brief ID: 550005
		*/
		TEST_METHOD(VerifyLedControllerGlowAndOffMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.Glow();
			for (auto i = 0; i < 3000; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedHigh)).Once();
			controller.Off();
			for (auto i = 0; i < 1000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Once();
			Verify(Method(driver, DriveLedHigh)).Once();
		}

		/**
		* \brief ID: 550006
		*/
		TEST_METHOD(VerifyLedControllerGlowAndGlowMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.Glow();
			for (auto i = 0; i < 4000; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedHigh)).Once();
			controller.Glow();
			for (auto i = 0; i < 1000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Never();
			Verify(Method(driver, DriveLedHigh)).Twice();
		}

		/**
		* \brief ID: 550007
		*/
		TEST_METHOD(VerifyLedControllerGlowAndBlinkSlowMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.Glow();
			for (auto i = 0; i < 2000; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedHigh)).Once();
			controller.BlinkSlow();
			for (auto i = 0; i < SLOW_BLINK_TIMEOUT * 2; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Twice();
			Verify(Method(driver, DriveLedHigh)).Twice();
		}

		/**
		* \brief ID: 550008
		*/
		TEST_METHOD(VerifyLedControllerGlowAndBlinkFastMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.Glow();
			for (auto i = 0; i < 1000; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Never();
			Verify(Method(driver, DriveLedHigh)).Once();
			controller.BlinkFast();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 3; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Twice();
			Verify(Method(driver, DriveLedHigh)).Exactly(3);
		}

		/**
		* \brief ID: 550009
		*/
		TEST_METHOD(VerifyLedControllerBlinkSlowAndOffMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.BlinkSlow();
			for (auto i = 0; i < SLOW_BLINK_TIMEOUT * 2; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Twice();
			Verify(Method(driver, DriveLedHigh)).Once();
			controller.Off();
			for (auto i = 0; i < 2000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Exactly(3);
			Verify(Method(driver, DriveLedHigh)).Once();
		}

		/**
		* \brief ID: 550010
		*/
		TEST_METHOD(VerifyLedControllerBlinkSlowAndGlowMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.BlinkSlow();
			Verify(Method(driver, DriveLedLow)).Once();
			Verify(Method(driver, DriveLedHigh)).Never();
			for (auto i = 0; i < SLOW_BLINK_TIMEOUT * 1; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Once();
			Verify(Method(driver, DriveLedHigh)).Once();
			for (auto i = 0; i < SLOW_BLINK_TIMEOUT * 1; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Twice();
			Verify(Method(driver, DriveLedHigh)).Once();
			controller.Off();
			for (auto i = 0; i < 3000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Exactly(3);
			Verify(Method(driver, DriveLedHigh)).Once();
		}

		/**
		* \brief ID: 550011
		*/
		TEST_METHOD(VerifyLedControllerBlinkSlowAndBlinkSlowMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.BlinkSlow();
			for (auto i = 0; i < SLOW_BLINK_TIMEOUT * 2; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Twice();
			Verify(Method(driver, DriveLedHigh)).Once();
			controller.BlinkSlow();
			for (auto i = 0; i < SLOW_BLINK_TIMEOUT * 1; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Exactly(3);
			Verify(Method(driver, DriveLedHigh)).Twice();
		}

		/**
		* \brief ID: 550012
		*/
		TEST_METHOD(VerifyLedControllerBlinkSlowAndBlinkFastMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.BlinkSlow();
			for (auto i = 0; i < SLOW_BLINK_TIMEOUT * 2; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Twice();
			Verify(Method(driver, DriveLedHigh)).Once();
			controller.BlinkFast();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 2; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Exactly(4);
			Verify(Method(driver, DriveLedHigh)).Twice();
		}

		/**
		* \brief ID: 550013
		*/
		TEST_METHOD(VerifyLedControllerBlinkFastAndOffMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.BlinkFast();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 2; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Twice();
			Verify(Method(driver, DriveLedHigh)).Once();
			controller.Off();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 2; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Exactly(3);
			Verify(Method(driver, DriveLedHigh)).Once();
		}

		/**
		* \brief ID: 550014
		*/
		TEST_METHOD(VerifyLedControllerBlinkFastAndGlowMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.BlinkFast();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 3; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Twice();
			Verify(Method(driver, DriveLedHigh)).Twice();
			controller.Glow();
			for (auto i = 0; i < 1500; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Twice();
			Verify(Method(driver, DriveLedHigh)).Exactly(3);
		}

		/**
		* \brief ID: 550015
		*/
		TEST_METHOD(VerifyLedControllerBlinkFastAndBlinkSlowMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.BlinkFast();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 2; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Twice();
			Verify(Method(driver, DriveLedHigh)).Once();
			controller.BlinkSlow();
			for (auto i = 0; i < SLOW_BLINK_TIMEOUT * 5; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Exactly(5);
			Verify(Method(driver, DriveLedHigh)).Exactly(4);
		}

		/**
		* \brief ID: 550016
		*/
		TEST_METHOD(VerifyLedControllerBlinkFastAndBlinkFastMode)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			auto& drv = driver.get();
			Timer timer = {};
			LedController controller(timer, drv);

			// Act
			controller.BlinkFast();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 6; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Exactly(4);
			Verify(Method(driver, DriveLedHigh)).Exactly(3);
			controller.BlinkFast();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 3; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Exactly(6);
			Verify(Method(driver, DriveLedHigh)).Exactly(5);
		}

		/**
		* \brief ID: 5500xx
		*/
		TEST_METHOD(VerifyLedControllerDisableFeatureBehaviour)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveLedLow)).AlwaysReturn();
			When(Method(driver, DriveLedHigh)).AlwaysReturn();
			Timer timer = {};
			LedController controller(timer, driver.get());

			// Act
			controller.BlinkFast();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 4; i++)
				timer.OnElapse();
			Verify(Method(driver, DriveLedLow)).Exactly(3);
			Verify(Method(driver, DriveLedHigh)).Exactly(2);
			controller.Disable();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 2; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveLedLow)).Exactly(4);
			VerifyNoOtherInvocations(driver);
			controller.Enable();
			for (auto i = 0; i < FAST_BLINK_TIMEOUT * 3; i++)
				timer.OnElapse();

			Verify(Method(driver, DriveLedLow)).Exactly(5);
			Verify(Method(driver, DriveLedHigh)).Exactly(4);
		}
	};
}
