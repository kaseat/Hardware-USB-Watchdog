#pragma once
#include "ISubscriber.h"

#ifndef MAX_TIMER_SUBSCRIBERS
#define MAX_TIMER_SUBSCRIBERS 3
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
	virtual ~Timer() {}

	/**
	* \brief Run timer as 1 ms timebase.
	*/
	virtual void Run();

	/**
	* \brief Stop timer.
	*/
	virtual void Stop();

	/**
	* \brief Add handler on timer elapse.
	* \param sbcr Subscriber.
	*/
	virtual void SubscribeOnElapse(ISubscriber& sbcr);

	/**
	* \brief Remove handler on timer elapse.
	* \param sbcr Subscriber.
	*/
	virtual void UnsubscribeOnElapse(ISubscriber& sbcr);

	/**
	 * \brief Occures on timer elapse.
	 */
	static void OnElapse();
private:

	static ISubscriber* subscribers[MAX_TIMER_SUBSCRIBERS];
};