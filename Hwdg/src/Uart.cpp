#include "Uart.h"

#define UART_REGISTER 5

ISubscriber* Uart::subscriber = 0;

void Uart::Init(uint32_t baudrate)
{
	// UART configuration logic
}

void Uart::SubscribeOnByteRecivied(ISubscriber& sbcr)
{
	subscriber = &sbcr;
}

void Uart::UnsubscribeOnByteRecivied()
{
	subscriber = 0;
}

void Uart::SendByte(uint8_t data)
{
	// Send byte logic
}

void Uart::SendData(uint8_t* data, uint8_t len)
{
	while (len--) SendByte(*data++);
}

#pragma vector=UART1_R_RXNE_vector
void Uart::OnByteRecivied()
{
	if (subscriber == 0) return;
	subscriber->Callback(UART_REGISTER);
}

