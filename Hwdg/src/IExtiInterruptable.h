#pragma once

class IExtiInterruptable
{
public:
	virtual void OnExtiInterrupt() = 0;

protected:
	virtual ~IExtiInterruptable() {}
};
