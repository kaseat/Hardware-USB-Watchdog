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

#define RST_TIM 10U
#define HR_LO_TIM 60U
#define HR_HI_TIM 20U
#define INFINITE 10000U

#include "../Hwdg/src/Timer.h"
#include "../Hwdg/src/Rebooter.h"
// ReSharper disable once CppUnusedIncludeDirective
#include "../Hwdg/src/Rebooter.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

namespace HwdgTests
{
	TEST_CLASS(RebooterTests)
	{
	public:
		Timer timer = {};

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverNeverDrivesResetPin)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, DriveResetHigh)).AlwaysReturn();

			auto& dr = driver.get();
			Rebooter rebooter(timer, dr);

			// Act
			for (auto i = 0; i < 100000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveResetLow)).Never();
			Verify(Method(driver, DriveResetHigh)).Never();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverDriveSoftReset)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, DriveResetHigh)).AlwaysReturn();

			auto& dr = driver.get();
			Rebooter rebooter(timer, dr);

			// Act
			rebooter.SoftReset();
			for (auto i = 0; i < INFINITE; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveResetLow) + Method(driver, DriveResetHigh)).Once();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverDriveSoftResetTiming)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, DriveResetHigh)).AlwaysReturn();

			auto& dr = driver.get();
			Rebooter rebooter(timer, dr);

			// Act
			rebooter.SoftReset();
			for (auto i = 0; i < RST_TIM; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DriveResetLow) + Method(driver, DriveResetHigh)).Once();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverDriveHardReset)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DrivePowerLow)).AlwaysReturn();
			When(Method(driver, DrivePowerHigh)).AlwaysReturn();

			auto& dr = driver.get();
			Rebooter rebooter(timer, dr);

			// Act
			rebooter.HardReset();
			for (auto i = 0; i < INFINITE; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(driver, DrivePowerLow) +
				Method(driver, DrivePowerHigh) +
				Method(driver, DrivePowerLow) +
				Method(driver, DrivePowerHigh)).Once();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverDriveHardResetTiming)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DrivePowerLow)).AlwaysReturn();
			When(Method(driver, DrivePowerHigh)).AlwaysReturn();

			auto& dr = driver.get();
			Rebooter rebooter(timer, dr);

			// Act & Assert
			rebooter.HardReset();
			Verify(Method(driver, DrivePowerLow)).Once();
			Verify(Method(driver, DrivePowerHigh)).Never();

			for (auto i = 0; i < HR_LO_TIM; i++)
				timer.OnElapse();

			Verify(Method(driver, DrivePowerLow) +
				Method(driver, DrivePowerHigh)).Once();

			for (auto i = 0; i < HR_HI_TIM; i++)
				timer.OnElapse();

			Verify(Method(driver, DrivePowerLow) +
				Method(driver, DrivePowerHigh) +
				Method(driver, DrivePowerLow)).Once();

			for (auto i = 0; i < RST_TIM; i++)
				timer.OnElapse();

			Verify(Method(driver, DrivePowerLow) +
				Method(driver, DrivePowerHigh) +
				Method(driver, DrivePowerLow) +
				Method(driver, DrivePowerHigh)).Once();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyGpioDriverRejectMethodsRecallDuringResetSequence)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, DriveResetHigh)).AlwaysReturn();
			When(Method(driver, DrivePowerLow)).AlwaysReturn();
			When(Method(driver, DrivePowerHigh)).AlwaysReturn();

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
				Method(driver, DrivePowerHigh) +
				Method(driver, DrivePowerLow) +
				Method(driver, DrivePowerHigh) +
				Method(driver, DriveResetLow) +
				Method(driver, DriveResetHigh)).Once();
		}
	};
}
