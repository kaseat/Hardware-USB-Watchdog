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
