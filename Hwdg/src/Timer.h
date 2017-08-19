#pragma once
#include "ISubscriber.h"

#ifndef MAX_SUBSCRIBERS
#define MAX_SUBSCRIBERS 2
#endif

/**
 * \brief Represents timer class.
 */
class Timer
{
public:
	/**
	* \brief Run timer as 1 ms timebase.
	*/
	static void Run();

	/**
	* \brief Stop timer.
	*/
	static void Stop();

	/**
	* \brief Add handler on timer elapse.
	* \param sbcr Subscriber.
	*/
	static void SubscribeOnElapse(ISubscriber& sbcr);

	/**
	* \brief Remove handler on timer elapse.
	* \param sbcr Subscriber.
	*/
	static void UnsubscribeOnElapse(ISubscriber& sbcr);

private:
	static ISubscriber* subscribers[MAX_SUBSCRIBERS];
	static void OnElapse();
};