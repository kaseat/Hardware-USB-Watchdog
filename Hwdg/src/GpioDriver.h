#pragma once

#ifdef __IAR_SYSTEMS_ICC__
#define _virtual
#else
#define _virtual virtual
#endif

/**
 * \brief Represents Low level GPIO driver.
 */
class GpioDriver
{
public:
	/**
	 * \brief Create instance of GPIO driver.
	 */
	GpioDriver();
	/**
	 * \brief Dispose GPIO driver.
	 */
	_virtual ~GpioDriver();
	/**
	 * \brief Drive reset pin low.
	 */
	_virtual void DriveResetLow();
	/**
	 * \brief Release reset pin.
	 */
	_virtual void DriveResetHigh();
	/**
	 * \brief Drive power pin low.
	 */
	_virtual void DrivePowerLow();
	/**
	 * \brief Releare power pin.
	 */
	_virtual void DrivePowerHigh();
	/**
	 * \brief Drive LED pin low.
	 */
	_virtual void DriveLedLow();
	/**
	 * \brief Drive LED pin high.
	 */
	_virtual void DriveLedHigh();
};
