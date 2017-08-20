#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"

#include "../Hwdg/src/Timer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

namespace HwdgTests
{
	TEST_CLASS(TimerTests)
	{
	public:

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyCallbackFiredOnTimerElapse)
		{
			// Arrange
			Mock<ISubscriber> subscriber;
			When(Method(subscriber, Callback)).AlwaysReturn();
			auto& sbcr = subscriber.get();
			Timer timer = {};
			timer.SubscribeOnElapse(sbcr);

			// Act
			timer.OnElapse();
			timer.OnElapse();

			// Assert
			Verify(Method(subscriber, Callback)).Twice();
			timer.UnsubscribeOnElapse(sbcr);
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyCallbackNotFiredAfterUnsubscribe)
		{
			// Arrange
			Mock<ISubscriber> subscriber;
			When(Method(subscriber, Callback)).AlwaysReturn();
			auto& sbcr = subscriber.get();
			Timer timer = {};
			timer.SubscribeOnElapse(sbcr);

			// Act
			timer.OnElapse();

			// Assert
			Verify(Method(subscriber, Callback)).Once();

			// Act
			timer.UnsubscribeOnElapse(sbcr);
			timer.OnElapse();

			// Assert
			Verify(Method(subscriber, Callback)).Once();
			timer.UnsubscribeOnElapse(sbcr);
		}

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyMultipleCallbackFires)
		{
			// Arrange
			Mock<ISubscriber> subscriber1;
			When(Method(subscriber1, Callback)).AlwaysReturn();
			auto& sbcr1 = subscriber1.get();

			Mock<ISubscriber> subscriber2;
			When(Method(subscriber2, Callback)).AlwaysReturn();
			auto& sbcr2 = subscriber2.get();

			Mock<ISubscriber> subscriber3;
			When(Method(subscriber3, Callback)).AlwaysReturn();
			auto& sbcr3 = subscriber3.get();
			Timer timer = {};

			// Act
			timer.SubscribeOnElapse(sbcr1);
			timer.OnElapse();

			// Assert
			Verify(Method(subscriber1, Callback)).Once();
			Verify(Method(subscriber2, Callback)).Never();
			Verify(Method(subscriber3, Callback)).Never();

			timer.SubscribeOnElapse(sbcr2);
			timer.OnElapse();

			Verify(Method(subscriber1, Callback)).Twice();
			Verify(Method(subscriber2, Callback)).Once();
			Verify(Method(subscriber3, Callback)).Never();

			timer.SubscribeOnElapse(sbcr3);
			timer.OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(3);
			Verify(Method(subscriber2, Callback)).Twice();
			Verify(Method(subscriber3, Callback)).Once();

			timer.UnsubscribeOnElapse(sbcr1);
			timer.OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(3);
			Verify(Method(subscriber2, Callback)).Exactly(3);
			Verify(Method(subscriber3, Callback)).Exactly(2);

			timer.UnsubscribeOnElapse(sbcr3);
			timer.OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(3);
			Verify(Method(subscriber2, Callback)).Exactly(4);
			Verify(Method(subscriber3, Callback)).Exactly(2);

			timer.UnsubscribeOnElapse(sbcr3);
			timer.OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(3);
			Verify(Method(subscriber2, Callback)).Exactly(5);
			Verify(Method(subscriber3, Callback)).Exactly(2);

			timer.SubscribeOnElapse(sbcr3);
			timer.OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(3);
			Verify(Method(subscriber2, Callback)).Exactly(6);
			Verify(Method(subscriber3, Callback)).Exactly(3);

			timer.SubscribeOnElapse(sbcr1);
			timer.UnsubscribeOnElapse(sbcr2);
			timer.UnsubscribeOnElapse(sbcr3);
			timer.OnElapse();
			timer.OnElapse();
			timer.OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(6);
			Verify(Method(subscriber2, Callback)).Exactly(6);
			Verify(Method(subscriber3, Callback)).Exactly(3);

			timer.UnsubscribeOnElapse(sbcr1);
			timer.OnElapse();
			timer.OnElapse();
			timer.OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(6);
			Verify(Method(subscriber2, Callback)).Exactly(6);
			Verify(Method(subscriber3, Callback)).Exactly(3);

			timer.SubscribeOnElapse(sbcr3);
			timer.OnElapse();
			timer.OnElapse();
			timer.OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(6);
			Verify(Method(subscriber2, Callback)).Exactly(6);
			Verify(Method(subscriber3, Callback)).Exactly(6);
			timer.UnsubscribeOnElapse(sbcr1);
			timer.UnsubscribeOnElapse(sbcr2);
			timer.UnsubscribeOnElapse(sbcr3);
		}
	};
}
