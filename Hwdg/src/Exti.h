#pragma once
#include "ISubscriber.h"
#include "STM8S003F3.h"

#ifdef __IAR_SYSTEMS_ICC__
#define _virtual
#else
#define _virtual virtual
#endif


class Exti
{
public:
	_virtual ~Exti() {}
	/**
	* \brief Initialize EXTI.
	*/
	Exti();
	/**
	* \brief Add handler on external interrupt occurrence.
	* \param sbcr Subscriber.
	*/
	_virtual void SubscribeOnExti(ISubscriber& sbcr);

	/**
	* \brief Remove handler on external interrupt occurrence.
	*/
	_virtual void UnsubscribeOnExti();

	/**
	 * \brief Calls when external intterupt occurs.
	 */
	__interrupt static void OnExti();
private:
	static ISubscriber* subscriber;
};
