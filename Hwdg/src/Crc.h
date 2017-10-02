#pragma once
#include <cstdint>

class CrcCalculator
{
public:
	static uint8_t GetCrc7(uint8_t* buffer, uint8_t length);
private:
	static  uint8_t crcTable[256];
	static uint8_t GetCrc7Byte(uint8_t crc, uint8_t data)
	{
		return crcTable[crc << 1 ^ data];
	}
};