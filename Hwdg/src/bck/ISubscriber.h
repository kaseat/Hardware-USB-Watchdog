#pragma once
#include <stdint.h>

class ISubscriber
{
public:
	virtual void Callback(uint8_t data) = 0;

protected:
	virtual ~ISubscriber() {}
};
