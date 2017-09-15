#include "GpioDriver.h"
#include "STM8S003F3.h"

#define PWR (1 << 2)
#define RST (1 << 3)
#define LED (1 << 6)

GpioDriver::GpioDriver()
{
	GPIOD->ODR |= PWR | RST;
	GPIOD->DDR |= PWR | RST;
	GPIOC->ODR |= LED;
	GPIOC->DDR |= LED;
}

void GpioDriver::DriveResetLow()
{
	GPIOD->ODR &= ~RST;
}

void GpioDriver::DriveResetHigh()
{
	GPIOD->ODR |= PWR;
}

void GpioDriver::DrivePowerLow()
{
	GPIOD->ODR &= ~PWR;
}

void GpioDriver::DrivePowerHigh()
{
	GPIOD->ODR |= LED;
}

void GpioDriver::DriveLedLow()
{
	GPIOC->ODR &= ~LED;
}

void GpioDriver::DriveLedHigh()
{
	GPIOC->ODR |= RST;
}
