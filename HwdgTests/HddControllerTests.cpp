#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"

#define DEFAULT_HDD_TIMEOUT 300000UL
#include "../Hwdg/src/HddController.h"

#include "../Hwdg/src/Timer.h"
#include "../Hwdg/src/Rebooter.h"

#include "../Hwdg/src/Exti.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

namespace HwdgTests
{
	extern Timer timer;
	Exti exti = {};
	TEST_CLASS(HddControllerTests)
	{
		static void Wait(uint32_t ms)
		{
			for (uint32_t i = 0; i < ms; i++)
				timer.OnElapse();
		}
	public:

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyHddControllerNeverRerboots)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			HddController hddController(exti, timer, rebooter.get());

			// Act & Assert
			Wait(DEFAULT_HDD_TIMEOUT * 50);
			VerifyNoOtherInvocations(rebooter);

			Wait(DEFAULT_HDD_TIMEOUT * 50);
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyHddControllerRebootsCorrectly)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			HddController hddController(exti, timer, rebooter.get());
			hddController.StartHddMonitor();

			// Act & Assert
			Wait(DEFAULT_HDD_TIMEOUT);
			VerifyNoOtherInvocations(rebooter);

			Wait(1);
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();

			Wait(DEFAULT_HDD_TIMEOUT);
			VerifyNoOtherInvocations(rebooter);

			Wait(1);
			Verify(Method(rebooter, SoftReset)).Twice();
			Verify(Method(rebooter, HardReset)).Never();

			Wait(DEFAULT_HDD_TIMEOUT);
			VerifyNoOtherInvocations(rebooter);

			Wait(1);
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();

			Wait(DEFAULT_HDD_TIMEOUT * 50);
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyHddControllerReconfigureWorksCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			HddController hddController(exti, timer, rebooter.get());

			// Act & Assert
			hddController.StartHddMonitor();
			Wait(DEFAULT_HDD_TIMEOUT * 3 + 3);
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();

			Wait(DEFAULT_HDD_TIMEOUT * 3 + 3);
			VerifyNoOtherInvocations(rebooter);

			Wait(DEFAULT_HDD_TIMEOUT * 3 + 3);
			VerifyNoOtherInvocations(rebooter);

			hddController.StartHddMonitor();
			Wait(DEFAULT_HDD_TIMEOUT * 3 + 3);
			Verify(Method(rebooter, SoftReset)).Exactly(6);
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyHddControllerCanStopAfterRun)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			HddController hddController(exti, timer, rebooter.get());

			// Act & Assert
			hddController.StartHddMonitor();
			Wait(DEFAULT_HDD_TIMEOUT * 2 + 2);
			Verify(Method(rebooter, SoftReset)).Exactly(2);
			Verify(Method(rebooter, HardReset)).Never();

			hddController.StopHddMonitor();
			Wait(DEFAULT_HDD_TIMEOUT * 2 + 2);
			VerifyNoOtherInvocations(rebooter);

			hddController.StartHddMonitor();
			Wait(DEFAULT_HDD_TIMEOUT);
			VerifyNoOtherInvocations(rebooter);

			Wait(1);
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();

			Wait(DEFAULT_HDD_TIMEOUT * 2 + 2);

			Verify(Method(rebooter, SoftReset)).Exactly(5);
			Verify(Method(rebooter, HardReset)).Never();

			Wait(DEFAULT_HDD_TIMEOUT * 20 + 20);
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyHddControllerCanResetCounterByExti)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			HddController hddController(exti, timer, rebooter.get());

			// Act & Assert
			hddController.StartHddMonitor();
			Wait(DEFAULT_HDD_TIMEOUT - 50);
			Exti::OnExti();
			VerifyNoOtherInvocations(rebooter);

			Wait(50);
			VerifyNoOtherInvocations(rebooter);

			Wait(DEFAULT_HDD_TIMEOUT - 50);
			VerifyNoOtherInvocations(rebooter);

			Wait(1);
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();
			hddController.StopHddMonitor();

			Wait(DEFAULT_HDD_TIMEOUT * 10 + 50);
			VerifyNoOtherInvocations(rebooter);

			Exti::OnExti();
			VerifyNoOtherInvocations(rebooter);

			Wait(DEFAULT_HDD_TIMEOUT * 10 + 50);
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyHddControllerSetsTimeoutCorrectly)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			HddController hddController(exti, timer, rebooter.get());

			// Act & Assert
			hddController.SetTimeout(0x08);
			hddController.StartHddMonitor();
			Wait(3 * 60000);
			VerifyNoOtherInvocations(rebooter);

			Wait(1);
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();

			hddController.StopHddMonitor();
			hddController.SetTimeout(0x0F);
			hddController.StartHddMonitor();

			Wait((3 + 7) * 60000);
			VerifyNoOtherInvocations(rebooter);

			Wait(1);
			Verify(Method(rebooter, SoftReset)).Twice();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyHddControllerCantEditSettingsDuringExecution)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			HddController hddController(exti, timer, rebooter.get());

			// Act & Assert
			hddController.SetTimeout(0x0A);
			hddController.StartHddMonitor();
			Wait((3 + 2) * 60000);
			VerifyNoOtherInvocations(rebooter);

			Wait(1);
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();

			Assert::IsTrue(Busy == hddController.SetTimeout(0x0B));

			hddController.StopHddMonitor();
			Assert::IsTrue(SetHddTimeoutOk == hddController.SetTimeout(0x0B));
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyHddControllerGetStatusReturnsCorrectValue)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			HddController hddController(exti, timer, rebooter.get());

			// Act & Assert
			Assert::AreEqual(uint8_t(0x12), hddController.GetStatus());

			Assert::IsTrue(EnableHddMonitorOk == hddController.StartHddMonitor());
			Assert::AreEqual(uint8_t(0x32), hddController.GetStatus());

			Assert::IsTrue(DisableHddMonitorOk == hddController.StopHddMonitor());
			Assert::IsTrue(SetHddTimeoutOk == hddController.SetTimeout(0));
			Assert::AreEqual(uint8_t(0x10), hddController.GetStatus());

			Assert::IsTrue(EnableHddMonitorOk == hddController.StartHddMonitor());
			Wait((3 + 0) * 60000 * 4 + 3);
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();
			Assert::AreEqual(uint8_t(0x30), hddController.GetStatus());

			Wait(1);
			VerifyNoOtherInvocations(rebooter);
			Assert::AreEqual(uint8_t(0x10), hddController.GetStatus());

			Assert::IsTrue(SetHddAttemptsOk == hddController.SetRebootAttempts(0x00));
			Assert::AreEqual(uint8_t(0x00), hddController.GetStatus());

			Assert::IsTrue(EnableHddMonitorOk == hddController.StartHddMonitor());
			Assert::AreEqual(uint8_t(0x20), hddController.GetStatus());

			Wait((3 + 0) * 60000 * 1 + 0);
			VerifyNoOtherInvocations(rebooter);

			hddController.PauseHddMonitor();
			Wait((3 + 0) * 60000 * 10 + 0);
			VerifyNoOtherInvocations(rebooter);
			Assert::AreEqual(uint8_t(0x60), hddController.GetStatus());

			hddController.ResumeHddMonitor();
			Assert::AreEqual(uint8_t(0x20), hddController.GetStatus());

			Wait(1);
			Verify(Method(rebooter, SoftReset)).Exactly(4);
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyHddControllerSetAttemptsCorrectly)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			HddController hddController(exti, timer, rebooter.get());

			// Act & Assert
			hddController.SetRebootAttempts(0x00);
			hddController.StartHddMonitor();
			Wait(5 * 60000);
			VerifyNoOtherInvocations(rebooter);

			Wait(1);
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();

			hddController.StopHddMonitor();
			hddController.SetRebootAttempts(0x0F);
			hddController.StartHddMonitor();

			Wait(5 * 60000 * 4 + 3);
			Verify(Method(rebooter, SoftReset)).Exactly(4);
			Verify(Method(rebooter, HardReset)).Never();

			Wait(1);
			Verify(Method(rebooter, SoftReset)).Exactly(5);
			Verify(Method(rebooter, HardReset)).Never();
		}
	};
}