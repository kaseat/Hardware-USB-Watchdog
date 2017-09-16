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
	Uart(uint32_t baudrate);
        
	/**
	* \brief Add handler on byte recivied.
	* \param sbcr Subscriber.
	*/
	void SubscribeOnByteReceived(ISubscriber& sbcr);
        
	/**
	* \brief Remove handler on byte recivied.
	*/
	void UnsubscribeOnByteReceived();
        
	/**
	* \brief Send byte through the UART.
	* \param data Data to be sent.
	*/
	void SendByte(uint8_t data);

	/**
	* \brief Send byte array through the UART.
	* \param data Byte array pointer.
	* \param len Number of bytes to send.
	*/
	void SendData(uint8_t * data, uint8_t len);

private:
	static void OnByteReceived();
	static ISubscriber* subscriber;
};