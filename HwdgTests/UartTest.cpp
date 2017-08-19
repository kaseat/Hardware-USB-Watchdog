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
		
		TEST_METHOD(VerifyCallbackFiredOnByteRecivied)
		{
			// Arrange
			fakeit::Mock<ISubscriber> subscriber;
			fakeit::When(Method(subscriber, Callback)).AlwaysReturn();
			auto& sb = subscriber.get();
			Uart::SubscribeOnByteRecivied(sb);

			// Act
			Uart::OnByteRecivied();
			Uart::OnByteRecivied();

			// Assert
			fakeit::Verify(Method(subscriber, Callback)).Twice();
			Uart::UnsubscribeOnByteRecivied();
		}
	};
}
