// Copyright (c) 2017, Oleg Petrochenko
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the HWDG nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.

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
	};
}
