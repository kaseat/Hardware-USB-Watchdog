#include "Uart.h"

#ifdef __IAR_SYSTEMS_ICC__
#include "Clock.h"
#endif

#define UART_REGISTER 5
ISubscriber* Uart::subscriber = nullptr;

Uart::Uart(uint32_t baudrate)
{
#ifdef __IAR_SYSTEMS_ICC__
	// Configure GPIOs
	GPIOD->ODR |= 1 << 5;
	GPIOD->DDR |= 1 << 5;
	GPIOD->DDR &= ~(1 << 6);
	GPIOD->CR1 &= ~(1 << 6);
	GPIOD->CR2 &= ~(1 << 6);

	// Baudrate configuration.
	uint32_t brr = Clock::GetCpuFreq() / baudrate;
	UART1->BRR2 = brr & 0x000F;
	UART1->BRR2 |= brr >> 12;
	UART1->BRR1 = (brr >> 4) & 0x00FF;

	// UART configuration.
	UART1->CR1 = 0;
	UART1->CR2 = UART_CR2_TEN | UART_CR2_REN;
	UART1->CR3 = 0;
#endif
}

void Uart::SubscribeOnByteReceived(ISubscriber& sbcr)
{
	subscriber = &sbcr;
#ifdef __IAR_SYSTEMS_ICC__
	UART1->CR2 |= UART_CR2_RIEN;
#endif
}

void Uart::UnsubscribeOnByteReceived()
{
	subscriber = nullptr;
#ifdef __IAR_SYSTEMS_ICC__
	UART1->CR2 &= ~(UART_CR2_RIEN);
#endif
}

void Uart::SendByte(uint8_t data)
{
#ifdef __IAR_SYSTEMS_ICC__
	while (!(UART1->SR & UART_SR_TXE))
		;
	UART1->DR = data;
#endif
}

void Uart::SendData(uint8_t* data, uint8_t len)
{
	while (len--) SendByte(*data++);
}

#ifdef __IAR_SYSTEMS_ICC__
#pragma vector=UART1_R_RXNE_ISR
#endif
__interrupt void Uart::OnByteReceived()
{
	if (subscriber == nullptr) return;
#ifdef __IAR_SYSTEMS_ICC__
	subscriber->Callback(UART1->DR);
#else
	subscriber->Callback(UART_REGISTER);
#endif
	
}