#pragma once
#include "ISubscriber.h"
#include "STM8S003F3.h"

#ifndef MAX_TIMER_SUBSCRIBERS
#define MAX_TIMER_SUBSCRIBERS 3
#endif

#ifdef __IAR_SYSTEMS_ICC__
#define _virtual
#else
#define _virtual virtual
#endif

/**
 * \brief Represents timer class.
 */
class Timer
{
public:

	/**
	 * \brief Dispose Timer.
	 */
	_virtual ~Timer() {}

	/**
	* \brief Run timer as 1 ms timebase.
	*/
	_virtual void Run();

	/**
	* \brief Stop timer.
	*/
	_virtual void Stop();

	/**
	* \brief Add handler on timer elapse.
	* \param sbcr Subscriber.
	*/
	_virtual void SubscribeOnElapse(ISubscriber& sbcr);

	/**
	* \brief Remove handler on timer elapse.
	* \param sbcr Subscriber.
	*/
	_virtual void UnsubscribeOnElapse(ISubscriber& sbcr);

	/**
	 * \brief Occures on timer elapse.
	 */
	__interrupt static void OnElapse();
private:

	static ISubscriber* subscribers[MAX_TIMER_SUBSCRIBERS];
};