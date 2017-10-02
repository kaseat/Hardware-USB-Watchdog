#pragma once

#pragma once
#include <stdint.h>
#include "ISubscriber.h"

class ResetTimer
{
public:

	/**
	 * \brief Starts timer with specified timebase.
	 * \param timebase Timer time base.
	 */
	static void Run(uint16_t timebase);
        
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
	*/
	static void UnsubscribeOnElapse();
        
	/**
	 * \brief Code we execute on timer elapse;
	 */
        
private:
	static ISubscriber* subscriber;

	static void OnElapse();
};