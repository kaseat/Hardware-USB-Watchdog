#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"

#include "../Hwdg/src/Timer.h"
#include "../Hwdg/src/ResetController.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;


namespace HwdgTests
{
	TEST_CLASS(ResetControllerTests)
	{
	public:
		/**
		* \brief ID:500001 Verify no soft and hard reset without Start and HRon commands
		*/
		TEST_METHOD(VerifyRebooterNeverResetAfterPowerOn)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer,rb, rs);

			// Act
			for (auto i = 0; i < 100000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Never();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500002 Verify no soft and hard reset with specified attempts/timeout, without Start and HRon
		*/
		TEST_METHOD(VerifyRebooterNeverResetAfterPowerOnWithSettings)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer, rb, rs);

			// Act
			rc.SetHardResetAttempts(5);
			rc.SetSoftResetAttempts(7);
			rc.SetResponseTimeout(50);
			rc.SetResetTimeout(120);
			for (auto i = 0; i < 100000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Never();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500003 Verify default response timeout (10 seconds) + no hard reset
		*/
		TEST_METHOD(VerifyRebooterDefaultResponseTimeout)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer, rb, rs);

			// Act
			rc.EnableSoftReset();
			for (auto i = 0; i < 10000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500004 Verify min response timeout (1 second) + no hard reset
		*/
		TEST_METHOD(VerifyRebooterMinResponseTimeout)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer, rb, rs);

			// Act
			rc.SetResponseTimeout(0);
			rc.EnableSoftReset();
			for (auto i = 0; i < 1000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500005 Verify max response timeout (64 seconds) + no hard reset
		*/
		TEST_METHOD(VerifyRebooterMaxResponseTimeout)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer, rb, rs);

			// Act
			rc.SetResponseTimeout(127);
			rc.EnableSoftReset();
			for (auto i = 0; i < 64000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500006 Verify any specified response timeout (23 seconds) + no hard reset
		*/
		TEST_METHOD(VerifyRebooterRestartsIn23Seconds)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer, rb, rs);

			// Act
			rc.SetResponseTimeout(22);
			rc.EnableSoftReset();
			for (auto i = 0; i < 23000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500007 Verify default reset timeout + default soft reset
		*/
		TEST_METHOD(VerifyRebooterDefaultResetTimeout)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer, rb, rs);

			// Act
			rc.EnableSoftReset();
			rc.EnableHardReset();
			for (auto i = 0; i < 1000000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Exactly(3);
		}

		/**
		* \brief ID:500011 Verify default soft reset attempts (3 attempts) + no hard reset
		*/
		TEST_METHOD(VerifyRebooterRestarts3Times)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer, rb, rs);

			// Act
			rc.EnableSoftReset();
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
		TEST_METHOD(VerifyRebooterRestarts3TimesSoftAnd3TimesHard)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer, rb, rs);

			// Act
			rc.EnableSoftReset();
			rc.EnableHardReset();
			for (auto i = 0; i < 10000000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Exactly(3);
		}

		/**
		* \brief ID:500014 Verify any specified soft reset attempts + no hard reset
		*/
		TEST_METHOD(VerifyRebooterRestartsWithSpecifiedAttempts2Soft)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer, rb, rs);

			// Act
			rc.EnableSoftReset();
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
		TEST_METHOD(VerifyRebooterRestartsWithSpecifiedAttempts5SoftAnd1Hard)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer, rb, rs);

			// Act
			rc.EnableSoftReset();
			rc.EnableHardReset();
			rc.SetSoftResetAttempts(5);			
			rc.SetHardResetAttempts(1);
			for (auto i = 0; i < 10000000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(6);
			Verify(Method(rebooter, HardReset)).Exactly(2);
		}

		/**
		* \brief ID:500026 Verify no soft/hard reset after Start + Stop command with default soft reset + no counter if possible
		*/
		TEST_METHOD(VerifyRebooterNoSoftResetWhenSoftDisabled)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer, rb, rs);

			// Act
			rc.EnableSoftReset();
			for (auto i = 0; i < 500; i++)
				timer.OnElapse();

			rc.DisableSoftReset();
			for (auto i = 0; i < 50000; i++)
				timer.OnElapse();

			// Assert
			Verify(Method(rebooter, SoftReset)).Never();
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500027 Verify no soft/hard reset after Start + HRon + Stop commands with default hard reset
		*/
		TEST_METHOD(VerifyRebooterNoHardResetWhenHardDisabled)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();
			auto& rb = rebooter.get();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			auto& rs = ledController.get();
			Timer timer = {};
			ResetController rc(timer,rb, rs);

			// Act
			rc.EnableSoftReset();
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
