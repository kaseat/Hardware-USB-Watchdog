#include "Exti.h"

IExtiInterruptable* Exti::subscriber = nullptr;
#define HDD_PIN ((uint8_t)(1 << 7))
#define HDD_PORT GPIOC
Exti::Exti()
{
#ifdef __IAR_SYSTEMS_ICC__
	EXTI->CR1 = EXTI_CR1_PCIS_0;
	HDD_PORT->DDR &= ~HDD_PIN;
	HDD_PORT->CR1 &= ~HDD_PIN;
#endif
}

void Exti::SubscribeOnExti(IExtiInterruptable& sbcr)
{
	subscriber = &sbcr;
#ifdef __IAR_SYSTEMS_ICC__
	HDD_PORT->CR2 |= HDD_PIN;
#endif
}

void Exti::UnsubscribeOnExti()
{
	subscriber = nullptr;
#ifdef __IAR_SYSTEMS_ICC__
	HDD_PORT->CR2 &= ~HDD_PIN;
#endif
}

#ifdef __IAR_SYSTEMS_ICC__
#pragma vector=EXTI2_ISR
#endif
__interrupt void Exti::OnExti()
{
	if (subscriber == nullptr) return;
	subscriber->OnExtiInterrupt();
}