#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"

#include "../Hwdg/src/Timer.h"
#include "../Hwdg/src/ResetController.h"

#define RESPONSE_DEF_TIMEOUT 10000U
#define REBOOT_DEF_TIMEOUT 150000U
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

#define INFINITE 1000000U

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
		* \brief ID:11001 Verify neither soft nor hard reset occurs before Start command received.
		*/
		TEST_METHOD(VerifyResetControllerNeverRebootsAfterPowerOn)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
			VerifyNoOtherInvocations(ledController);
		}

		/**
		* \brief ID:11002 Verify neither soft nor hard reset occurs after configuring ResetController.
		*/
		TEST_METHOD(VerifyResetControllerNeverRebootsAfterPowerOnWithSettings)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
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
		* \brief ID:11003 Verify default ResetController behaviour after Start command received (10s+150s+150s).
		*/
		TEST_METHOD(VerifyResetControllerStartDefaultSequence)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.Start();

			for (auto i = 0; i < 3; i++)
			{
				// Act
				if (i == 0)  Wait(RESPONSE_DEF_TIMEOUT - 1);
				else Wait(REBOOT_DEF_TIMEOUT - 1);

				// Assert
				VerifyNoOtherInvocations(rebooter);

				// Act
				Wait(1);

				// Assert
				Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
				Verify(Method(rebooter, HardReset)).Never();
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:11004 Verify default ResetController behaviour after enabling HardReset
		* and sending Start command received. SoftReset (10s+150s+150s) + HardReset (150s+150s+150s).
		*/
		TEST_METHOD(VerifyResetControllerHrdResetDefaultSequence)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.EnableHardReset();
			rc.Start();

			for (auto i = 0; i < 6; i++)
			{
				// Act
				if (i == 0)  Wait(RESPONSE_DEF_TIMEOUT - 1);
				else Wait(REBOOT_DEF_TIMEOUT - 1);

				// Assert
				VerifyNoOtherInvocations(rebooter);

				// Act
				Wait(1);

				// Assert
				if (i < 3)
				{
					Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
					Verify(Method(rebooter, HardReset)).Never();
				}
				else
				{
					Verify(Method(rebooter, SoftReset)).Exactly(3);
					Verify(Method(rebooter, HardReset)).Exactly(i - 2);
				}
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:500005 Verify default response timeout + no hard reset
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
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.Start();
			Wait(RESPONSE_DEF_TIMEOUT - 1);

			// Assert
			VerifyNoOtherInvocations(rebooter);

			// Act
			Wait(1);

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();

			// Act
			Wait(INFINITE);

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500006 Verify min response timeout (1 second) + no hard reset
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
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.SetResponseTimeout(0);
			rc.Start();

			Wait(5000 - 1);

			// Assert
			VerifyNoOtherInvocations(rebooter);

			// Act
			Wait(1);

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();

			// Act
			Wait(INFINITE);

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500007 Verify max response timeout (320 seconds) + no hard reset
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
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.SetResponseTimeout(63);
			rc.Start();

			Wait(320000 - 1);

			// Assert
			VerifyNoOtherInvocations(rebooter);

			// Act
			Wait(1);

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();

			// Act
			Wait(INFINITE);

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500008 Verify any specified response timeout (110 seconds) + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerRestartsIn110Seconds)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.SetResponseTimeout(21);
			rc.Start();

			Wait(110000 - 1);

			// Assert
			VerifyNoOtherInvocations(rebooter);

			// Act
			Wait(1);

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();

			// Act
			Wait(INFINITE);

			// Assert
			Verify(Method(rebooter, SoftReset)).Exactly(3);
			Verify(Method(rebooter, HardReset)).Never();
		}

		/**
		* \brief ID:500009 Verify default soft reset count (3 times) + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerSetSoftResetAttemptsDefaultCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.Start();

			for (auto i = 0; i < 3; i++)
			{
				// Act
				if (i == 0)  Wait(RESPONSE_DEF_TIMEOUT - 1);
				else Wait(REBOOT_DEF_TIMEOUT - 1);

				// Assert
				VerifyNoOtherInvocations(rebooter);

				// Act
				Wait(1);

				// Assert

				Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
				Verify(Method(rebooter, HardReset)).Never();
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:500010 Verify min soft reset count (1 time) + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerSetSoftResetAttemptsMinCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.SetSoftResetAttempts(0);
			rc.Start();

			Wait(RESPONSE_DEF_TIMEOUT - 1);

			// Assert
			VerifyNoOtherInvocations(rebooter);

			// Act
			Wait(1);

			// Assert
			Verify(Method(rebooter, SoftReset)).Once();
			Verify(Method(rebooter, HardReset)).Never();

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:500011 Verify max soft reset count (8 times) + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerSetSoftResetAttemptsMaxCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.SetSoftResetAttempts(7);
			rc.Start();

			for (auto i = 0; i < 8; i++)
			{
				// Act
				if (i == 0)  Wait(RESPONSE_DEF_TIMEOUT - 1);
				else Wait(REBOOT_DEF_TIMEOUT - 1);

				// Assert
				VerifyNoOtherInvocations(rebooter);

				// Act
				Wait(1);

				// Assert

				Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
				Verify(Method(rebooter, HardReset)).Never();
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:500012 Verify any specified soft reset count (6 times) + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerSetSoftResetAttemptsAnyCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.SetSoftResetAttempts(5);
			rc.Start();

			for (auto i = 0; i < 6; i++)
			{
				// Act
				if (i == 0)  Wait(RESPONSE_DEF_TIMEOUT - 1);
				else Wait(REBOOT_DEF_TIMEOUT - 1);

				// Assert
				VerifyNoOtherInvocations(rebooter);

				// Act
				Wait(1);

				// Assert

				Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
				Verify(Method(rebooter, HardReset)).Never();
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:500013 Verify default hard reset count (3 times)
		*/
		TEST_METHOD(VerifyResetControllerSetHardResetAttemptsDefaultCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.EnableHardReset();
			rc.Start();

			for (auto i = 0; i < 6; i++)
			{
				// Act
				if (i == 0)  Wait(RESPONSE_DEF_TIMEOUT - 1);
				else Wait(REBOOT_DEF_TIMEOUT - 1);

				// Assert
				VerifyNoOtherInvocations(rebooter);

				// Act
				Wait(1);

				// Assert
				if (i < 3)
				{
					Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
					Verify(Method(rebooter, HardReset)).Never();
				}
				else
				{
					Verify(Method(rebooter, SoftReset)).Exactly(3);
					Verify(Method(rebooter, HardReset)).Exactly(i - 2);
				}
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:500014 Verify min hard reset count (1 time)
		*/
		TEST_METHOD(VerifyResetControllerSetHardResetAttemptsMinCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.EnableHardReset();
			rc.SetHardResetAttempts(0);
			rc.Start();

			for (auto i = 0; i < 4; i++)
			{
				// Act
				if (i == 0)  Wait(RESPONSE_DEF_TIMEOUT - 1);
				else Wait(REBOOT_DEF_TIMEOUT - 1);

				// Assert
				VerifyNoOtherInvocations(rebooter);

				// Act
				Wait(1);

				// Assert
				if (i < 3)
				{
					Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
					Verify(Method(rebooter, HardReset)).Never();
				}
				else
				{
					Verify(Method(rebooter, SoftReset)).Exactly(3);
					Verify(Method(rebooter, HardReset)).Exactly(i - 2);
				}
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:500015 Verify max hard reset count (8 times)
		*/
		TEST_METHOD(VerifyResetControllerSetHardResetAttemptsMaxCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.EnableHardReset();
			rc.SetHardResetAttempts(0);
			rc.Start();

			for (auto i = 0; i < 4; i++)
			{
				// Act
				if (i == 0)  Wait(RESPONSE_DEF_TIMEOUT - 1);
				else Wait(REBOOT_DEF_TIMEOUT - 1);

				// Assert
				VerifyNoOtherInvocations(rebooter);

				// Act
				Wait(1);

				// Assert
				if (i < 3)
				{
					Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
					Verify(Method(rebooter, HardReset)).Never();
				}
				else
				{
					Verify(Method(rebooter, SoftReset)).Exactly(3);
					Verify(Method(rebooter, HardReset)).Exactly(i - 2);
				}
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:500016 Verify any specified hard reset count (5 times)
		*/
		TEST_METHOD(VerifyResetControllerSetHardResetAttemptsAnyCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.EnableHardReset();
			rc.SetHardResetAttempts(4);
			rc.Start();

			for (auto i = 0; i < 8; i++)
			{
				// Act
				if (i == 0)  Wait(RESPONSE_DEF_TIMEOUT - 1);
				else Wait(REBOOT_DEF_TIMEOUT - 1);

				// Assert
				VerifyNoOtherInvocations(rebooter);

				// Act
				Wait(1);

				// Assert
				if (i < 3)
				{
					Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
					Verify(Method(rebooter, HardReset)).Never();
				}
				else
				{
					Verify(Method(rebooter, SoftReset)).Exactly(3);
					Verify(Method(rebooter, HardReset)).Exactly(i - 2);
				}
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:500017 Verify options modification avaliable after reboot sequence.
		*/
		TEST_METHOD(VerifyResetControllerResetSequenceAfterReboot)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.EnableHardReset();
			rc.SetHardResetAttempts(1);
			rc.SetSoftResetAttempts(3);
			rc.SetResponseTimeout(15);
			rc.SetRebootTimeout(36);
			rc.Start();

			for (auto i = 0; i < 6; i++)
			{
				// Act
				if (i == 0)  Wait(80000 - 1);
				else Wait(190000 - 1);

				// Assert
				VerifyNoOtherInvocations(rebooter);

				// Act
				Wait(1);

				// Assert
				if (i < 4)
				{
					Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
					Verify(Method(rebooter, HardReset)).Never();
				}
				else
				{
					Verify(Method(rebooter, SoftReset)).Exactly(4);
					Verify(Method(rebooter, HardReset)).Exactly(i - 3);
				}
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);

			// Arrange
			rebooter.Reset();
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			// Act
			Assert::IsTrue(rc.DisableHardReset() == Response::DisableHardResetOk);
			Assert::IsTrue(rc.SetSoftResetAttempts(5) == Response::SetSoftResetAttemptsOk);
			Assert::IsTrue(rc.SetRebootTimeout(10) == Response::SetRebootTimeoutOk);
			Assert::IsTrue(rc.Start() == Response::StartOk);

			for (auto i = 0; i < 6; i++)
			{
				// Act
				if (i == 0)  Wait(80000 - 1);
				else Wait(60000 - 1);

				// Assert
				VerifyNoOtherInvocations(rebooter);

				// Act
				Wait(1);

				// Assert
				Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
				Verify(Method(rebooter, HardReset)).Never();
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(rebooter);
		}

		/**
		* \brief ID:500018 Verify LED blink sequence correct during soft reset.
		*/
		TEST_METHOD(VerifyResetControllerLedSoftResetSequenceCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.Start();
			Verify(Method(ledController, BlinkSlow)).Once();
			Verify(Method(ledController, Off)).Never();
			Verify(Method(ledController, Glow)).Never();
			Verify(Method(ledController, BlinkFast)).Never();
			Verify(Method(ledController, BlinkMid)).Never();

			for (auto i = 0; i < 4; i++)
			{
				// Act
				if (i == 0)  Wait(RESPONSE_DEF_TIMEOUT - 1);
				else Wait(REBOOT_DEF_TIMEOUT - 1);

				// Assert
				VerifyNoOtherInvocations(ledController);

				// Act
				Wait(1);
				if (i < 3)
				{
					Verify(Method(ledController, BlinkSlow)).Once();
					Verify(Method(ledController, Off)).Never();
					Verify(Method(ledController, Glow)).Never();
					Verify(Method(ledController, BlinkFast)).Never();
					Verify(Method(ledController, BlinkMid)).Once();
				}
				else
				{
					Verify(Method(ledController, BlinkSlow)).Once();
					Verify(Method(ledController, Off)).Never();
					Verify(Method(ledController, Glow)).Once();
					Verify(Method(ledController, BlinkFast)).Never();
					Verify(Method(ledController, BlinkMid)).Once();
				}
			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(ledController);
		}

		/**
		* \brief ID:500019 Verify LED blink sequence correct during hard reset.
		*/
		TEST_METHOD(VerifyResetControllerLedHardResetSequenceCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());

			// Act
			rc.EnableHardReset();
			rc.Start();
			Verify(Method(ledController, BlinkSlow)).Once();
			Verify(Method(ledController, Off)).Never();
			Verify(Method(ledController, Glow)).Never();
			Verify(Method(ledController, BlinkFast)).Never();
			Verify(Method(ledController, BlinkMid)).Never();

			for (auto i = 0; i < 7; i++)
			{
				// Act
				if (i == 0)  Wait(RESPONSE_DEF_TIMEOUT - 1);
				else Wait(REBOOT_DEF_TIMEOUT - 1);

				// Assert
				VerifyNoOtherInvocations(ledController);

				// Act
				Wait(1);

				if (i < 3)
				{
					Verify(Method(ledController, BlinkSlow)).Once();
					Verify(Method(ledController, Off)).Never();
					Verify(Method(ledController, Glow)).Never();
					Verify(Method(ledController, BlinkFast)).Never();
					Verify(Method(ledController, BlinkMid)).Once();
				}
				else if (i < 6)
				{
					Verify(Method(ledController, BlinkSlow)).Once();
					Verify(Method(ledController, Off)).Never();
					Verify(Method(ledController, Glow)).Never();
					Verify(Method(ledController, BlinkFast)).Once();
					Verify(Method(ledController, BlinkMid)).Once();
				}
				else
				{
					Verify(Method(ledController, BlinkSlow)).Once();
					Verify(Method(ledController, Off)).Never();
					Verify(Method(ledController, Glow)).Once();
					Verify(Method(ledController, BlinkFast)).Once();
					Verify(Method(ledController, BlinkMid)).Once();
				}

			}

			// Act
			Wait(INFINITE);

			// Assert
			VerifyNoOtherInvocations(ledController);
		}


		/**
		* \brief ID:500019 Verify GetResponse command works propertly.
		*/
		TEST_METHOD(VerifyResetControllerResponseCorrect)
		{
			// Arrange
			Mock<Rebooter> rebooter;
			When(Method(rebooter, SoftReset)).AlwaysReturn();
			When(Method(rebooter, HardReset)).AlwaysReturn();

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();

			ResetController rc(timer, rebooter.get(), ledController.get());
			// Act & Assert
			Assert::IsTrue(0x0048049C == rc.GetStatus());
		}






		/**
		* \brief ID:500014 Verify any specified soft reset attempts + no hard reset
		*/
		TEST_METHOD(VerifyResetControllerRestartsWithSpecifiedAttempts2Soft)
		{
			// Arrange
			Mock<Rebooter> rebooter;

			Mock<LedController> ledController;
			When(Method(ledController, Off)).AlwaysReturn();
			When(Method(ledController, Glow)).AlwaysReturn();
			When(Method(ledController, BlinkFast)).AlwaysReturn();
			When(Method(ledController, BlinkMid)).AlwaysReturn();
			When(Method(ledController, BlinkSlow)).AlwaysReturn();
			ResetController rc(timer, rebooter.get(), ledController.get());

//#pragma omp parallel for
//			for (auto rebootTimeout = 0; rebootTimeout < 3; rebootTimeout++)
//				for (auto responseTimeout = 0; responseTimeout < 3; responseTimeout++)
//					for (auto softResetAttemps = 0; softResetAttemps < 8; softResetAttemps++)
//						for (auto hardResetAttempts = 0; hardResetAttempts < 8; hardResetAttempts++)
//						{
//							When(Method(rebooter, SoftReset)).AlwaysReturn();
//							When(Method(rebooter, HardReset)).AlwaysReturn();
//							rc.SetHardResetAttempts(hardResetAttempts);
//							rc.SetSoftResetAttempts(softResetAttemps);
//							rc.SetResponseTimeout(responseTimeout);
//							rc.SetRebootTimeout(rebootTimeout);
//							rc.EnableHardReset();
//							rc.Start();
//							for (auto i = 0; i < softResetAttemps + hardResetAttempts + 2; i++)
//							{
//								// Act
//								if (i == 0)  Wait((responseTimeout + 1) * 5000 - 1);
//								else Wait(10000 + rebootTimeout * 5000 - 1);
//
//								// Assert
//								VerifyNoOtherInvocations(rebooter);
//
//								// Act
//								Wait(1);
//
//								// Assert
//								if (i < softResetAttemps + 1)
//								{
//									Verify(Method(rebooter, SoftReset)).Exactly(i + 1);
//									Verify(Method(rebooter, HardReset)).Never();
//								}
//								else
//								{
//									Verify(Method(rebooter, SoftReset)).Exactly(softResetAttemps + 1);
//									Verify(Method(rebooter, HardReset)).Exactly(i - softResetAttemps);
//								}
//							}
//							// Act
//							Wait(INFINITE);
//
//							// Assert
//							VerifyNoOtherInvocations(rebooter);
//							rebooter.Reset();
//						}
		}
	};
}
