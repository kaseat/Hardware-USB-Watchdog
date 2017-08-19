#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"

#define private public
#define MAX_SUBSCRIBERS 3
#include "../Hwdg/src/Timer.h"
#include "../Hwdg/src/Timer.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

namespace HwdgTests
{
	TEST_CLASS(TimerTests)
	{
	public:

		TEST_METHOD(VerifyCallbackFiredOnTimerElapse)
		{
			// Arrange
			Mock<ISubscriber> subscriber;
			When(Method(subscriber, Callback)).AlwaysReturn();
			auto& sbcr = subscriber.get();
			
			Timer::SubscribeOnElapse(sbcr);

			// Act
			Timer::OnElapse();
			Timer::OnElapse();

			// Assert
			Verify(Method(subscriber, Callback)).Twice();
			Timer::UnsubscribeOnElapse(sbcr);
		}

		TEST_METHOD(VerifyCallbackNotFiredAfterUnsubscribe)
		{
			// Arrange
			Mock<ISubscriber> subscriber;
			When(Method(subscriber, Callback)).AlwaysReturn();
			auto& sbcr = subscriber.get();
			Timer::SubscribeOnElapse(sbcr);

			// Act
			Timer::OnElapse();

			// Assert
			Verify(Method(subscriber, Callback)).Once();

			// Act
			Timer::UnsubscribeOnElapse(sbcr);
			Timer::OnElapse();

			// Assert
			Verify(Method(subscriber, Callback)).Once();
			Timer::UnsubscribeOnElapse(sbcr);
		}

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

			Timer::SubscribeOnElapse(sbcr1);

			// Act
			Timer::OnElapse();

			// Assert
			Verify(Method(subscriber1, Callback)).Once();
			Verify(Method(subscriber2, Callback)).Never();
			Verify(Method(subscriber3, Callback)).Never();

			Timer::SubscribeOnElapse(sbcr2);
			Timer::OnElapse();

			Verify(Method(subscriber1, Callback)).Twice();
			Verify(Method(subscriber2, Callback)).Once();
			Verify(Method(subscriber3, Callback)).Never();

			Timer::SubscribeOnElapse(sbcr3);
			Timer::OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(3);
			Verify(Method(subscriber2, Callback)).Twice();
			Verify(Method(subscriber3, Callback)).Once();

			Timer::UnsubscribeOnElapse(sbcr1);
			Timer::OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(3);
			Verify(Method(subscriber2, Callback)).Exactly(3);
			Verify(Method(subscriber3, Callback)).Exactly(2);

			Timer::UnsubscribeOnElapse(sbcr3);
			Timer::OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(3);
			Verify(Method(subscriber2, Callback)).Exactly(4);
			Verify(Method(subscriber3, Callback)).Exactly(2);

			Timer::UnsubscribeOnElapse(sbcr3);
			Timer::OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(3);
			Verify(Method(subscriber2, Callback)).Exactly(5);
			Verify(Method(subscriber3, Callback)).Exactly(2);

			Timer::SubscribeOnElapse(sbcr3);
			Timer::OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(3);
			Verify(Method(subscriber2, Callback)).Exactly(6);
			Verify(Method(subscriber3, Callback)).Exactly(3);

			Timer::SubscribeOnElapse(sbcr1);
			Timer::UnsubscribeOnElapse(sbcr2);
			Timer::UnsubscribeOnElapse(sbcr3);
			Timer::OnElapse();
			Timer::OnElapse();
			Timer::OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(6);
			Verify(Method(subscriber2, Callback)).Exactly(6);
			Verify(Method(subscriber3, Callback)).Exactly(3);

			Timer::UnsubscribeOnElapse(sbcr1);
			Timer::OnElapse();
			Timer::OnElapse();
			Timer::OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(6);
			Verify(Method(subscriber2, Callback)).Exactly(6);
			Verify(Method(subscriber3, Callback)).Exactly(3);

			Timer::SubscribeOnElapse(sbcr3);
			Timer::OnElapse();
			Timer::OnElapse();
			Timer::OnElapse();

			Verify(Method(subscriber1, Callback)).Exactly(6);
			Verify(Method(subscriber2, Callback)).Exactly(6);
			Verify(Method(subscriber3, Callback)).Exactly(6);
			Timer::UnsubscribeOnElapse(sbcr1);
			Timer::UnsubscribeOnElapse(sbcr2);
			Timer::UnsubscribeOnElapse(sbcr3);
		}
	};
}
