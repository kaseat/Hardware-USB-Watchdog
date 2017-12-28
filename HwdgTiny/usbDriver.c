// Copyright 2017 Oleg Petrochenko
// 
// This file is part of HwdgTiny.
// 
// HwdgTiny is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or any
// later version.
// 
// HwdgTiny is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with HwdgTiny. If not, see <http://www.gnu.org/licenses/>.

#include "usbDriver.h"
#include "usbconfig.h"
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "usbdrv.h"
#include <util/delay.h>

void OnCommandReceived(uint8_t data);
void InitUsbDriver(void);

const UsbDriver_t Usb = { usbInit, usbPoll, OnCommandReceived };

#define abs(x) ((x) > 0 ? (x) : (-x))
static uint8_t currentAddress;
static uint8_t bytesRemaining;
static uint8_t reportId;

/**
 * \brief Device descriptor.
 */
PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
	0x06, 0x00, 0xff, // USAGE_PAGE (Generic Desktop)
	0x09, 0x01, // USAGE (Vendor Usage 1)
	0xa1, 0x01, // COLLECTION (Application)
	0x85, 0x01, //   REPORT_ID (1)
	0x09, 0x01, //   USAGE (Vendor Usage 1)
	0x15, 0x00, //   LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00, //   LOGICAL_MAXIMUM (255)
	0x75, 0x08, //   REPORT_SIZE (8)
	0x95, 0x01, //   REPORT_COUNT (1)
	0xb1, 0x82, //   FEATURE (Data,Var,Abs,Vol)
	0x09, 0x01, //   USAGE (Vendor Usage 1)
	0x81, 0x02, //   INPUT (Data,Var,Abs,Vol)
	0x09, 0x01, //   USAGE (Vendor Usage 1)
	0x91, 0x82, //   INPUT (Data,Var,Abs,Vol)
	0xc0 // END_COLLECTION
};

/**
 * \brief Called by V-USB after device reset
 */
void hadUsbReset()
{
	int targetLength = (unsigned)(1499 * (double)F_CPU / 10.5e6 + 0.5);
	int bestDeviation = 9999;
	uint8_t bestCal = 0;

	// do a binary search in regions 0-127 and 128-255 to get optimum OSCCAL
	for (uint8_t region = 0; region <= 1; region++)
	{
		int frameLength = 0;
		uint8_t trialCal = region == 0 ? 0 : 128;

		for (uint8_t step = 64; step > 0; step >>= 1)
		{
			if (frameLength < targetLength) // true for initial iteration
				trialCal += step; // frequency too low
			else
				trialCal -= step; // frequency too high

			OSCCAL = trialCal;

			frameLength = usbMeasureFrameLength();

			if (abs(frameLength - targetLength) < bestDeviation)
			{
				bestCal = trialCal; // new optimum found
				bestDeviation = abs(frameLength - targetLength);
			}
		}
	}
	OSCCAL = bestCal;
}

/**
 * \brief This function is called by the driver to ask the application for a
 * control transfer's payload data (control-in). It is called in chunks of up
 * to 8 bytes each.You should copy the data to the location given by 'data'
 * and return the actual number of bytes copied.If you return less than
 * requested, the control - in transfer is terminated.
 * \param data Outcoming data pointer.
 * \param len Outcoming data length.
 * \return Returns remaining data length.
 */
uint8_t usbFunctionRead(uint8_t* data, uint8_t len)
{
	if (len > bytesRemaining)
		len = bytesRemaining;
	//eeprom_read_block(data, (uint8_t *)0 + currentAddress, len);
	currentAddress += len;
	bytesRemaining -= len;
		data[0] = reportId;
		data[1] = 0x33;
	return len;
}

/**
 * \brief This function is called by the driver to provide a control transfer's
 * payload data(control - out).It is called in chunks of up to 8 bytes.The total
 * count provided in the current control transfer can be obtained from the 'length'
 * property in the setup data.
 * \param data Incoming data pointer.
 * \param len Incoming data length.
 * \return Returns remaining data length.
 */
uint8_t usbFunctionWrite(uint8_t* data, uint8_t len)
{
	if (bytesRemaining == 0)
		return 1; /* end of transfer */
	if (len > bytesRemaining)
		len = bytesRemaining;

	//eeprom_write_block(data, (uint8_t *)0 + currentAddress, len);
	currentAddress += len;
	bytesRemaining -= len;

	if (bytesRemaining == 0)
	{
		OnCommandReceived(data[1]);
	}
	return bytesRemaining == 0; /* return 1 if this was the last chunk */
}

/**
 * \brief This function is called when the driver receives a SETUP transaction
 * from the host which is not answered by the driver itself(in practice : class
 * and vendor requests).All control transfers start with a SETUP transaction
 * where the host communicates the parameters of the following(optional) data
 * transfer.The SETUP data is available in the 'data' parameter which can (and
 * should) be casted to 'usbRequest_t *' for a more user - friendly access to
 * parameters.
 * \param data usbRequest_t data.
 * \return Number of requested bytes.
 */
usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
	usbRequest_t* rq = (void *)data;

	if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS)
	{
		if (rq->bRequest == USBRQ_HID_GET_REPORT)
		{
			reportId = rq->wValue.bytes[0];
			bytesRemaining = 2;
			currentAddress = 0;
			return USB_NO_MSG;
		}
		if (rq->bRequest == USBRQ_HID_SET_REPORT)
		{
			bytesRemaining = 2;
			currentAddress = 0;
			return USB_NO_MSG;
		}
	}
	return 0;
}

__attribute__((weak)) void OnCommandReceived(uint8_t data)
{
	if (data)
	{
		DDRB |= 1 << 3;
		PORTB |= 1 << 3;
	}
	else
	{
		PORTB &= ~(1 << 3);
		DDRB &= ~(1 << 3);
	}
}

void InitUsbDriver(void)
{
	usbInit();
	usbDeviceDisconnect();
	_delay_ms(250);
	usbDeviceConnect();
}
