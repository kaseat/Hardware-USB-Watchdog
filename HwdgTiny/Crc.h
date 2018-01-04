#pragma once

/**
 * \brief Calculate crc7 checksum.
 * \param buffer Buffer pointer.
 * \param length Buffer length.
 * \return Returns CRC7 checksum.
 */
uint8_t GetCrc7(uint8_t* buffer, uint8_t length);