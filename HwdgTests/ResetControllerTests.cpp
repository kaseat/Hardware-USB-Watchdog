#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"

#include "../Hwdg/src/Timer.h"
#include "../Hwdg/src/ResetController.h"
//#include "../Hwdg/src/ResetController.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

#define INFINITE 100000U

namespace HwdgTests
{
	Timer timer;
	TEST_CLASS(ResetControllerTests)
	{
		static void Wait(uint32_t ms)
		{
			for (uint32_t i = 0; i < ms; i++)
				timer.OnElapse();
		}

	public:
		/**
		* \brief ID:500001 Verify neither soft nor hard reset occurs before Start command received.
		*/
		TEST_METHOD(VerifyResetControllerNeverResetAfterPowerOn)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
			VerifyNoOtherInvocations(ledController);
		}

		/**
		* \brief ID:500002 Verify neither soft nor hard reset occurs when we configure ResetController.
		*/
		TEST_METHOD(VerifyResetControllerNeverResetAfterPowerOnWithSettings)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.SetHardResetAttempts(5);
			rc.SetSoftResetAttempts(7);
			rc.SetResponseTimeout(50);
			rc.SetRebootTimeout(120);
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
			VerifyNoOtherInvocations(ledController);
		}

		/**
		* \brief ID:500003 Verify default response timeout (10 seconds) + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerDefaultResponseTimeout)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.Start();
			Wait(10000);

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();

			// Act
			Wait(INFINITE);
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:500004 Verify min response timeout (1 second) + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerMinResponseTimeout)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.SetResponseTimeout(0);
			rc.Start();
			for (auto i = 0; i < 1000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500005 Verify max response timeout (64 seconds) + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerMaxResponseTimeout)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.SetResponseTimeout(127);
			rc.Start();
			for (auto i = 0; i < 64000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500006 Verify any specified response timeout (23 seconds) + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerRestartsIn23Seconds)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.SetResponseTimeout(22);
			rc.Start();
			for (auto i = 0; i < 23000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500007 Verify default reset timeout + default soft reset
		*/
		TEST_METHOD(VerifyResetControllerDefaultResetTimeout)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.EnableHardReset();
			rc.Start();

			for (auto i = 0; i < 1000000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Exactly(3);
		}

		/**
		* \brief ID:500011 Verify default soft reset attempts (3 attempts) + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerRestarts3Times)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.Start();
			for (auto i = 0; i < 10000000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500019 Verify default soft reset (3 attempts/10 seconds) + default hard reset (3 attempts/120 seconds)
		* todo: update description!
		*/
		TEST_METHOD(VerifyResetControllerRestarts3TimesSoftAnd3TimesHard)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.EnableHardReset();
			rc.Start();

			for (auto i = 0; i < 10000000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Exactly(3);
		}

		/**
		* \brief ID:500014 Verify any specified soft reset attempts + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerRestartsWithSpecifiedAttempts2Soft)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.Start();
			rc.SetSoftResetAttempts(2);
			for (auto i = 0; i < 200000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(2);
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500022 Verify soft reset (5 attempts) + hard reset (1 attempt)
		*/
		TEST_METHOD(VerifyResetControllerRestartsWithSpecifiedAttempts5SoftAnd1Hard)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.EnableHardReset();
			rc.SetSoftResetAttempts(5);			
			rc.SetHardResetAttempts(1);
			rc.Start();
			for (auto i = 0; i < 10000000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(6);
			Verify(Method(rebooter, HardReset)).Exactly(2);
		}

		/**
		* \brief ID:500026 Verify no soft/hard reset after Start + Stop command with default soft reset + no counter if possible
		*/
		TEST_METHOD(VerifyResetControllerNoSoftResetWhenSoftDisabled)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.Start();
			for (auto i = 0; i < 500; i++)
				timer.OnElapse();

			rc.Stop();
			for (auto i = 0; i < 50000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Never();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500027 Verify no soft/hard reset after Start + HRon + Stop commands with default hard reset
		*/
		TEST_METHOD(VerifyResetControllerNoHardResetWhenHardDisabled)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.Start();
			rc.EnableHardReset();

			for (auto i = 0; i < 5000; i++)
				timer.OnElapse();

			rc.DisableHardReset();
			for (auto i = 0; i < 2000000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();
		}
	};
}
