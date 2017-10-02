#pragma once

class IRebooter
{
public:
	/**
	 * \brief Make computer hard reset.
	 */
	virtual void HardReset() {}
	/**
	 * \brief Make computer soft reset.
	 */
	virtual void SoftReset() {}

protected:
	virtual ~IRebooter() {}
};
