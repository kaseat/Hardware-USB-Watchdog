#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"

#include "../Hwdg/src/Rebooter.h"
#include "../Hwdg/src/GpioDriver.h"

#define private public
#include "../Hwdg/src/Timer.h"

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

		TEST_METHOD(VerifyGpioDriverNeverDrivesResetPin)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, DriveResetHigh)).AlwaysReturn();

			Rebooter rebooter(driver.get());

			// Act
			for (auto i = 0; i < 100000; i++)
				Timer::OnElapse();

			// Assert
			Verify(Method(driver, DriveResetLow)).Never();
			Verify(Method(driver, DriveResetHigh)).Never();
		}

		TEST_METHOD(VerifyGpioDriverDriveSoftReset)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, DriveResetHigh)).AlwaysReturn();
			Rebooter rebooter(driver.get());

			// Act
			rebooter.SoftReset();
			for (auto i = 0; i < RST_TIM; i++)
				Timer::OnElapse();

			// Assert
			Verify(Method(driver, DriveResetLow) + Method(driver, DriveResetHigh)).Once();
		}

		TEST_METHOD(VerifyGpioDriverDriveHardReset)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DrivePowerLow)).AlwaysReturn();
			When(Method(driver, DrivePowerHigh)).AlwaysReturn();
			Rebooter rebooter(driver.get());

			// Act
			rebooter.HardReset();
			for (auto i = 0; i < HR_LO_TIM + HR_HI_TIM + RST_TIM; i++)
				Timer::OnElapse();

			// Assert
			Verify(Method(driver, DrivePowerLow) +
				Method(driver, DrivePowerHigh) +
				Method(driver, DrivePowerLow) +
				Method(driver, DrivePowerHigh)).Once();
		}

		TEST_METHOD(VerifyGpioDriverRejectMethodsRecallDuringResetSequence)
		{
			// Arrange
			Mock<GpioDriver> driver;
			When(Method(driver, DriveResetLow)).AlwaysReturn();
			When(Method(driver, DriveResetHigh)).AlwaysReturn();
			When(Method(driver, DrivePowerLow)).AlwaysReturn();
			When(Method(driver, DrivePowerHigh)).AlwaysReturn();
			Rebooter rebooter(driver.get());

			// Act
			rebooter.HardReset();
			for (auto i = 0; i < HR_LO_TIM; i++)
				Timer::OnElapse();
			rebooter.HardReset();
			for (auto i = 0; i < HR_HI_TIM; i++)
				Timer::OnElapse();

			rebooter.SoftReset();
			for (auto i = 0; i < RST_TIM + 10; i++)
				Timer::OnElapse();

			rebooter.SoftReset();
			for (auto i = 0; i < 50; i++)
				Timer::OnElapse();
			rebooter.HardReset();
			for (auto i = 0; i < 50; i++)
				Timer::OnElapse();

			rebooter.SoftReset();
			for (auto i = 0; i < 100; i++)
				Timer::OnElapse();

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