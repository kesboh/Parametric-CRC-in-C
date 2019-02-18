#ifndef CRC_H
#define CRC_H

#include <stdint.h>


//If 1, then a table is used, speeding up computation but requiring sizeof(crc_t)*256 bytes of additional memory
#define CRC_USE_TABLE 0

//If 1, a function crc_init_table(void) is made available that populates the CRC table
#define CRC_USE_INIT_TABLE_FUNC 0

//If 1, a check is performed in the CRC calculation function verifying that the table is initialized.
#define CRC_CHECK_TABLE_INIT 0

//The width of the calculated CRC. 1, 2, 4, 8, etc
#define CRC_WIDTH_BYTES 2

//CRC polynomial
#define CRC_POLY 0x1021

//Initial value of calculated CRC
#define CRC_INIT_VAL 0xFFFF

//Calculated CRC is XORed with this value before being returned
#define CRC_FINAL_XOR_VAL 0xFFFF

//If 1, every input byte is reflected
#define CRC_REFLECT_INPUT 1

//If 1, the calculated CRC is reflected before being returned
#define CRC_REFLECT_OUTPUT 1



//Make available the apropriate function
#if CRC_WIDTH_BYTES == 1
uint8_t crc8(const uint8_t *data, uint32_t len);
#elif CRC_WIDTH_BYTES == 2
uint16_t crc16(const uint8_t *data, uint32_t len);
#elif CRC_WIDTH_BYTES == 4
uint32_t crc32(const uint8_t *data, uint32_t len);
#elif CRC_WIDTH_BYTES == 8
uint64_t crc64(const uint8_t *data, uint32_t len);
#endif //CRC_WIDTH_BYTES

#if (CRC_USE_TABLE == 1 && CRC_USE_INIT_TABLE_FUNC == 1)
void crc_init_table(void);
#endif //CRC_USE_TABLE == 1

#endif //CRC_H