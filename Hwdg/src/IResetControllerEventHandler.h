#pragma once
#include <stdint.h>

class IResetControllerEventHandler
{
public:
	virtual void OnSoftReset() = 0;
	virtual void OnHardReset() = 0;
	virtual void OnUpdted() = 0;

protected:
	virtual ~IResetControllerEventHandler() {}
};
