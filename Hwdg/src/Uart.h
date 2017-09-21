#pragma once
#include <stdint.h>
#include "ISubscriber.h"
#include "STM8S003F3.h"

#ifdef __IAR_SYSTEMS_ICC__
#define _virtual
#else
#define _virtual virtual
#endif

class Uart
{
public:
	
	/**
	* \brief Initialize UART with specified baud rate and cpu frequency.
	* \param baudrate Target baud rate.
	*/
	Uart(uint32_t baudrate);

	/**
	 * \brief Dispose UART
	 */
	_virtual ~Uart() {}

	/**
	* \brief Add handler on byte recivied.
	* \param sbcr Subscriber.
	*/
	_virtual void SubscribeOnByteReceived(ISubscriber& sbcr);
        
	/**
	* \brief Remove handler on byte recivied.
	*/
	_virtual void UnsubscribeOnByteReceived();
        
	/**
	* \brief Send byte through the UART.
	* \param data Data to be sent.
	*/
	_virtual void SendByte(uint8_t data);

	/**
	* \brief Send byte array through the UART.
	* \param data Byte array pointer.
	* \param len Number of bytes to send.
	*/
	_virtual void SendData(uint8_t * data, uint8_t len);

private:
	__interrupt static void OnByteReceived();
	static ISubscriber* subscriber;
};
