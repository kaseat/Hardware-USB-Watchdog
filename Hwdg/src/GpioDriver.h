#pragma once

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
	virtual ~GpioDriver() = default;
	/**
	 * \brief Drive reset pin low.
	 */
	virtual void DriveResetLow();
	/**
	 * \brief Release reset pin.
	 */
	virtual void DriveResetHigh();
	/**
	 * \brief Drive power pin low.
	 */
	virtual void DrivePowerLow();
	/**
	 * \brief Releare power pin.
	 */
	virtual void DrivePowerHigh();
	/**
	 * \brief Drive LED pin low.
	 */
	virtual void DriveLedLow();
	/**
	 * \brief Drive LED pin high.
	 */
	virtual void DriveLedHigh();
};
