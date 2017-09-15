#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"

#define private public
#include "../Hwdg/src/Uart.h"
#include "../Hwdg/src/Uart.cpp"

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
