#pragma once

#include "ISubscriber.h"
#define MAX_SUBSCRIBERS 3

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
	static ISubscriber* subscriber[MAX_SUBSCRIBERS];
	static void OnElapse();
};