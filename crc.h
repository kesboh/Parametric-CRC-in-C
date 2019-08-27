#ifndef CRC_H
#define CRC_H

#include <stdint.h>

//If 1, then a table is used, speeding up computation but requiring sizeof(crc_t)*256 bytes of additional memory
#define CRC_USE_TABLE 0

//If 1, the user provides a table by implementing the appropriately sized array crc_table[256], if 0, a table is provided and filled
#define CRC_USER_PROVIDES_TABLE 0

//The width of the calculated CRC. 1, 2, 4, 8, etc
#define CRC_WIDTH_BYTES 2

//CRC polynomial
#define CRC_POLY 0x1021

//Initial value of calculated CRC
#define CRC_INIT_VAL 0x0000

//Calculated CRC is XORed with this value before being returned
#define CRC_FINAL_XOR_VAL 0x0000

//If 1, every input byte is reflected
#define CRC_REFLECT_INPUT 0

//If 1, the calculated CRC is reflected before being returned
#define CRC_REFLECT_OUTPUT 1



#if CRC_USE_TABLE == 1 && CRC_USER_PROVIDES_TABLE == 1
#if CRC_WIDTH_BYTES == 1
extern const uint8_t crc_table[256];
#elif CRC_WIDTH_BYTES == 2
extern const uint16_t crc_table[256];
#elif CRC_WIDTH_BYTES == 4
extern const uint32_t crc_table[256];
#elif CRC_WIDTH_BYTES == 8
extern const uint64_t crc_table[256];
#endif //CRC_WIDTH_BYTES
#endif


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


#if (CRC_USE_TABLE == 1 && CRC_USER_PROVIDES_TABLE == 0)
void crc_init_table(void);
#endif //CRC_USE_TABLE == 1


#endif //CRC_H