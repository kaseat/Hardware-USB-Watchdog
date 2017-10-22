#pragma once
#include <stdint.h>

class IResetControllerEventHandler
{
public:
	virtual void OnUpdted(uint8_t reason) = 0;

protected:
	virtual ~IResetControllerEventHandler() {}
};
