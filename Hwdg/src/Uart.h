#pragma once
#include <stdint.h>
#include "ISubscriber.h"

class Uart
{
public:
	/**
	* \brief Initialize UART with specified baud rate and cpu frequency.
	* \param baudrate Target baud rate.
	*/
	static void Init(uint32_t baudrate);
        
	/**
	* \brief Add handler on byte recivied.
	* \param sbcr Subscriber.
	*/
	static void SubscribeOnByteRecivied(ISubscriber& sbcr);
        
	/**
	* \brief Remove handler on byte recivied.
	*/
	static void UnsubscribeOnByteRecivied();
        
	/**
	* \brief Send byte through the UART.
	* \param data Data to be sent.
	*/
	static void SendByte(uint8_t data);

	/**
	* \brief Send byte array through the UART.
	* \param data Byte array pointer.
	* \param len Number of bytes to send.
	*/
	static void SendData(uint8_t * data, uint8_t len);

private:
	static void OnByteRecivied();
	static ISubscriber* subscriber;
};