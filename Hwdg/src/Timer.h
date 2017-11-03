// Copyright 2017 Oleg Petrochenko
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once
#include "ISubscriber.h"
#include "PlatformDefinitions.h"

#ifndef MAX_TIMER_SUBSCRIBERS
#define MAX_TIMER_SUBSCRIBERS 4
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
	_virtual ~Timer()
	{
	}

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
