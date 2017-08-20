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
	 * \brief Create Timer instance.
	 */
	Timer();

	/**
	 * \brief Dispose Timer.
	 */
	virtual ~Timer() = default;

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
	void OnElapse();
private:

	ISubscriber* subscribers[MAX_TIMER_SUBSCRIBERS];
	bool disposed;
};