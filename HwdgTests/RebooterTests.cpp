#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"

#include "../Hwdg/src/Timer.h"
#include "../Hwdg/src/Rebooter.h"

#define RST_TIM 200U
#define HR_LO_TIM 6000U
#define HR_HI_TIM 2000U



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
			for (auto i = 0; i < HR_LO_TIM + HR_HI_TIM + RST_TIM; i++)
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