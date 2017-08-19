#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"

#include "../Hwdg/src/ResetController.h"

#define private public
#include "../Hwdg/src/Timer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

namespace HwdgTests
{
	TEST_CLASS(ResetControllerTests)
	{
	public:

		TEST_METHOD(VerifyRebooterNeverResetAfterPowerOn)
		{
			// Arrange
			Mock<IRebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			ResetController rc(rb);

			// Act
			for (auto i = 0; i < 100000; i++)
				Timer::OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Never();
			Verify(Method(rebooter, HardReset)).Never();
		}

		TEST_METHOD(VerifyRebooterRestarts3Times)
		{
			// Arrange
			Mock<IRebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			ResetController rc(rb);
			rc.EnableSoftReset();

			// Act
			for (auto i = 0; i < 10000000; i++)
				Timer::OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();
		}

		TEST_METHOD(VerifyRebooterRestarts3TimesSoftAnd3TimesHard)
		{
			// Arrange
			Mock<IRebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			ResetController rc(rb);
			rc.EnableSoftReset();
			rc.EnableHardReset();

			// Act
			for (auto i = 0; i < 10000000; i++)
				Timer::OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Exactly(3);
		}

		TEST_METHOD(VerifyRebooterRestartsWithSpecifiedAttempts2Soft)
		{
			// Arrange
			Mock<IRebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			ResetController rc(rb);
			rc.EnableSoftReset();
			rc.SetSoftResetAttempts(2);

			// Act
			for (auto i = 0; i < 200000; i++)
				Timer::OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(2);
			Verify(Method(rebooter, HardReset)).Never();
		}

		TEST_METHOD(VerifyRebooterRestartsWithSpecifiedAttempts5SoftAnd1Hard)
		{
			// Arrange
			Mock<IRebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			ResetController rc(rb);
			rc.EnableSoftReset();
			rc.EnableHardReset();
			rc.SetSoftResetAttempts(5);			
			rc.SetHardResetAttempts(1);

			// Act
			for (auto i = 0; i < 10000000; i++)
				Timer::OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(6);
			Verify(Method(rebooter, HardReset)).Exactly(2);
		}

		TEST_METHOD(VerifyNoSoftResetWhenSoftDisabled)
		{
			// Arrange
			Mock<IRebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			ResetController rc(rb);
			rc.EnableSoftReset();
			rc.DisableSoftReset();

			// Act
			for (auto i = 0; i < 100000; i++)
				Timer::OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Never();
			Verify(Method(rebooter, HardReset)).Never();
		}

		TEST_METHOD(VerifyNoHardResetWhenHardDisabled)
		{
			// Arrange
			Mock<IRebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			ResetController rc(rb);
			rc.EnableSoftReset();
			rc.EnableHardReset();
			rc.DisableHardReset();

			// Act
			for (auto i = 0; i < 10000000; i++)
				Timer::OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();
		}
	};
}
