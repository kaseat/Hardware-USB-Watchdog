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

#define RST_TIM 10U
#define HR_LO_TIM 60U
#define HR_HI_TIM 20U
#define INFINITY_RB 10000U

#include "../Hwdg/src/Timer.h"
#include "../Hwdg/src/Rebooter.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "../Hwdg/src/Rebooter.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

namespace HwdgTests
{
	extern Timer timer;
	TEST_CLASS(RebooterTests)
	{
	public:
		static void Wait(uint32_t ms)
		{
			for (uint32_t i = 0; i < ms; i++)
				timer.OnElapse();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverNeverDrivesResetPin)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, ReleaseReset)).AlwaysReturn();

			auto& dr = driver.get();
			Rebooter rebooter(timer, dr);

			// Act
			for (auto i = 0; i < 100000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveResetLow)).Never();
			Verify(Method(driver, ReleaseReset)).Never();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverDriveSoftReset)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, ReleaseReset)).AlwaysReturn();

			auto& dr = driver.get();
			Rebooter rebooter(timer, dr);

			// Act
			rebooter.SoftReset();
			for (auto i = 0; i < INFINITY_RB; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveResetLow) + Method(driver, ReleaseReset)).Once();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverDriveSoftResetTiming)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, ReleaseReset)).AlwaysReturn();

			auto& dr = driver.get();
			Rebooter rebooter(timer, dr);

			// Act
			rebooter.SoftReset();
			for (auto i = 0; i < RST_TIM; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveResetLow) + Method(driver, ReleaseReset)).Once();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverDriveHardReset)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DrivePowerLow)).AlwaysReturn();
			When(Method(driver, ReleasePower)).AlwaysReturn();

			auto& dr = driver.get();
			Rebooter rebooter(timer, dr);

			// Act
			rebooter.HardReset();
			for (auto i = 0; i < INFINITY_RB; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DrivePowerLow) +
				Method(driver, ReleasePower) +
				Method(driver, DrivePowerLow) +
				Method(driver, ReleasePower)).Once();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverDriveHardResetTiming)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DrivePowerLow)).AlwaysReturn();
			When(Method(driver, ReleasePower)).AlwaysReturn();

			auto& dr = driver.get();
			Rebooter rebooter(timer, dr);

			// Act & Assert
			rebooter.HardReset();
			Verify(Method(driver, DrivePowerLow)).Once();
			Verify(Method(driver, ReleasePower)).Never();

			for (auto i = 0; i < HR_LO_TIM; i++)
				timer.OnElapse();

			Verify(Method(driver, DrivePowerLow) +
				Method(driver, ReleasePower)).Once();

			for (auto i = 0; i < HR_HI_TIM; i++)
				timer.OnElapse();

			Verify(Method(driver, DrivePowerLow) +
				Method(driver, ReleasePower) +
				Method(driver, DrivePowerLow)).Once();

			for (auto i = 0; i < RST_TIM; i++)
				timer.OnElapse();

			Verify(Method(driver, DrivePowerLow) +
				Method(driver, ReleasePower) +
				Method(driver, DrivePowerLow) +
				Method(driver, ReleasePower)).Once();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverRejectMethodsRecallDuringResetSequence)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, ReleaseReset)).AlwaysReturn();
			When(Method(driver, DrivePowerLow)).AlwaysReturn();
			When(Method(driver, ReleasePower)).AlwaysReturn();

			auto& dr = driver.get();
			Rebooter rebooter(timer, dr);

			// Act
			rebooter.HardReset();
			for (auto i = 0; i < HR_LO_TIM; i++)
				timer.OnElapse();
			rebooter.HardReset();
			for (auto i = 0; i < HR_HI_TIM; i++)
				timer.OnElapse();

			rebooter.SoftReset();
			for (auto i = 0; i < RST_TIM + 10; i++)
				timer.OnElapse();

			rebooter.SoftReset();
			for (auto i = 0; i < 50; i++)
				timer.OnElapse();
			rebooter.HardReset();
			for (auto i = 0; i < 50; i++)
				timer.OnElapse();

			rebooter.SoftReset();
			for (auto i = 0; i < 100; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DrivePowerLow) +
				Method(driver, ReleasePower) +
				Method(driver, DrivePowerLow) +
				Method(driver, ReleasePower) +
				Method(driver, DriveResetLow) +
				Method(driver, ReleaseReset)).Once();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverPwrPulseCorrectBehaviour)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, ReleaseReset)).AlwaysReturn();
			When(Method(driver, DrivePowerLow)).AlwaysReturn();
			When(Method(driver, ReleasePower)).AlwaysReturn();

			Rebooter rebooter(timer, driver.get());

			// Act & assert
			Assert::IsTrue(rebooter.PwrPulse() == PowerPulseOk);
			Verify(Method(driver, DrivePowerLow)).Once();
			Assert::IsTrue(rebooter.PwrPulse() == Busy);
			Assert::IsTrue(rebooter.SoftReset() == Busy);
			Assert::IsTrue(rebooter.HardReset() == Busy);
			Wait(RST_TIM - 1);
			VerifyNoOtherInvocations(driver);
			Wait(1);
			Verify(Method(driver, ReleasePower)).Once();
			Wait(INFINITY_RB);
			VerifyNoOtherInvocations(driver);
			Assert::IsTrue(rebooter.SoftReset() == TestSoftResetOk);
			Verify(Method(driver, DriveResetLow)).Once();
			Wait(RST_TIM - 1);
			VerifyNoOtherInvocations(driver);
			Wait(1);
			Verify(Method(driver, ReleaseReset)).Once();
			Wait(INFINITY_RB);
			VerifyNoOtherInvocations(driver);
		}
	};
}
