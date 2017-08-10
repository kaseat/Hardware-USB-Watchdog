#include "stdafx.h"
#include "fakeit.hpp"
#include "CppUnitTest.h"
#include "../Hwdg/src/Uart.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HwdgTests
{		
	TEST_CLASS(UartTest), Uart
	{
	public:
		
		TEST_METHOD(VerifyCallbackFiredOnByteRecivied)
		{
			fakeit::Mock<ISubscriber> subscriber;
			fakeit::When(Method(subscriber, Callback)).Return();
			auto& sb = subscriber.get();

			Uart::Init(9600);
			Uart::SubscribeOnByteRecivied(sb);
			Uart::OnByteRecivied();

			fakeit::Verify(Method(subscriber, Callback));
		}

	};
}
