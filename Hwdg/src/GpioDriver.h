#pragma once

class GpioDriver
{
public:
	virtual ~GpioDriver() = default;
	virtual void DriveResetLow();
	virtual void DriveResetHigh();
	virtual void DrivePowerLow();
	virtual void DrivePowerHigh();
};
