#include "GpioDriver.h"

#ifdef __IAR_SYSTEMS_ICC__
#include "STM8S003F3.h"
#define PWR (1 << 2)
#define RST (1 << 3)
#define LED (1 << 6)
#endif



GpioDriver::GpioDriver()
{
#ifdef __IAR_SYSTEMS_ICC__
	GPIOD->ODR |= PWR | RST;
	GPIOD->DDR |= PWR | RST;
	GPIOC->ODR |= LED;
	GPIOC->DDR |= LED;
#endif
}

void GpioDriver::DriveResetLow()
{
#ifdef __IAR_SYSTEMS_ICC__
	GPIOD->ODR &= ~RST;
#endif
}

void GpioDriver::DriveResetHigh()
{
#ifdef __IAR_SYSTEMS_ICC__
	GPIOD->ODR |= PWR;
#endif
}

void GpioDriver::DrivePowerLow()
{
#ifdef __IAR_SYSTEMS_ICC__
	GPIOD->ODR &= ~PWR;
#endif
}

void GpioDriver::DrivePowerHigh()
{
#ifdef __IAR_SYSTEMS_ICC__
	GPIOD->ODR |= LED;
#endif
}

void GpioDriver::DriveLedLow()
{
#ifdef __IAR_SYSTEMS_ICC__
	GPIOC->ODR &= ~LED;
#endif
}

void GpioDriver::DriveLedHigh()
{
#ifdef __IAR_SYSTEMS_ICC__
	GPIOC->ODR |= RST;
#endif
}
