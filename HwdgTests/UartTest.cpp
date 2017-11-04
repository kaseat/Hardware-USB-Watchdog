// Copyright 2017 Oleg Petrochenko
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"

#include "../Hwdg/src/Uart.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HwdgTests
{
	TEST_CLASS(UartTest)
	{
	public:

		/**
		* \brief ID:
		*/
		TEST_METHOD(VerifyCallbackFiredOnByteReceived)
		{
			// Arrange
			fakeit::Mock<ISubscriber> subscriber;
			fakeit::When(Method(subscriber, Callback)).AlwaysReturn();
			auto& sb = subscriber.get();
			Uart uart(9600);

			// Act
			uart.SubscribeOnByteReceived(sb);
			uart.OnByteReceived();
			uart.OnByteReceived();

			// Assert
			fakeit::Verify(Method(subscriber, Callback)).Twice();
			uart.UnsubscribeOnByteReceived();
		}
	};
}
