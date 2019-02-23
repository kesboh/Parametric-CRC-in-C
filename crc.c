#include <crc.h>


#if (CRC_WIDTH_BYTES != 1 && CRC_WIDTH_BYTES != 2 && CRC_WIDTH_BYTES != 4 && CRC_WIDTH_BYTES != 8)
#error "CRC_WIDTH_BYTES must be set to either 1, 2, 4 or 8"
#endif


#if CRC_WIDTH_BYTES == 1
typedef uint8_t crc_t;
#elif CRC_WIDTH_BYTES == 2
typedef uint16_t crc_t;
#elif CRC_WIDTH_BYTES == 4
typedef uint32_t crc_t;
#elif CRC_WIDTH_BYTES == 8
typedef uint64_t crc_t;
#endif


/**
 * @brief Parameterized CRC implementation 
 * 
 * @param *data Pointer to the data for which to calculate the CRC
 * @param d_len Length of data pointed to by *data
 * 
 * @return The calculated CRC
 * 
 * @note Use the appropriate wrapper function defined elsewhere in this file
 */
static crc_t __calc_crc(const uint8_t *data, uint32_t len);


#define CRC_MSB (1 << ((CRC_WIDTH_BYTES*8)-1))

#if CRC_USE_TABLE == 1
#if CRC_USER_PROVIDES_TABLE == 0
static crc_t crc_table[256];
#endif //CRC_USER_PROVIDES_TABLE == 0
#if CRC_CHECK_TABLE_INIT == 1
static uint8_t table_is_init = 0;
#endif //CRC_CHECK_TABLE_INIT == 1
#endif //CRC_USE_TABLE == 1

#if CRC_REFLECT_INPUT == 1
inline static uint8_t reflect_byte(uint8_t val);
#endif

#if CRC_REFLECT_OUTPUT == 1
inline static crc_t reflect_crc(uint64_t word, uint32_t num_bytes);
#endif

/**
 * @brief Parameterized CRC implementation 
 * 
 * @param *data Pointer to the data for which to calculate the CRC
 * @param d_len Length of data pointed to by *data
 * 
 * @return The calculated CRC
 * 
 * @note Use the appropriate wrapper functions defined elsewhere in this file
 */
static crc_t __calc_crc(const uint8_t *data, uint32_t len)
{
    crc_t crc = CRC_INIT_VAL;	

    for (uint32_t curr_byte = 0; curr_byte < len; ++curr_byte) {
        #if CRC_USE_TABLE == 0
        #if CRC_REFLECT_INPUT == 1
        crc ^= reflect_byte(data[curr_byte]) << ((CRC_WIDTH_BYTES*8) - 8);
        #else //Don't reflect
        crc ^= data[curr_byte] << ((CRC_WIDTH_BYTES*8) - 8);
        #endif //CRC_REFLECT_INPUT == 1
        for (uint8_t bit = 8; bit > 0; --bit) {
            if (crc & CRC_MSB) {
                crc = (crc << 1) ^ CRC_POLY;
            } else {
                crc = (crc << 1);
            }
        } //END OF NON-TABLE DRIVEN CALCULATION

        #else //Use table
        #if CRC_CHECK_TABLE_INIT == 1
        if (table_is_init == 0) {
            crc_init_table();
        }
        #endif //CRC_CHECK_TABLE_INIT == 1
        #if CRC_REFLECT_INPUT == 1
        uint8_t tmp = reflect_byte(data[curr_byte]) ^ (crc >> ((CRC_WIDTH_BYTES*8)-8));
        #else //Don't reflect
        uint8_t tmp = data[curr_byte] ^ (crc >> ((CRC_WIDTH_BYTES*8)-8));
        #endif //CRC_REFLECT_INPUT == 1
        crc = crc_table[tmp] ^ (crc << 8);
        #endif //CRC_USE_TABLE == 0

    }

    //Guessing most compilers optimize out the XOR if its constant 0x00
    #if CRC_REFLECT_OUTPUT == 1
    return reflect_crc(crc ^ CRC_FINAL_XOR_VAL, sizeof(crc_t));
    #else //Don't reflect
    return crc ^ CRC_FINAL_XOR_VAL;
    #endif

}


#if CRC_REFLECT_INPUT == 1
/**
 * @brief Reflects the bits of a byte and returns it
 * 
 * @param val The byte which shall have its bits reflected
 * 
 * @return The reflected byte
 */
inline static uint8_t reflect_byte(uint8_t val)
{
    return (uint8_t)(((val * 0x0802LU & 0x22110LU) | (val * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16);
}
#endif

#if CRC_REFLECT_OUTPUT == 1
/**
 * @brief Reflect bits in a word of size num_bytes
 * 
 * See post by Matt J / SirGuy:
 * https://stackoverflow.com/questions/746171/most-efficient-algorithm-for-bit-reversal-from-msb-lsb-to-lsb-msb-in-c
 * 
 * @param word The word to reflect
 * @param num_bytes sizeof(word)
 * 
 * @return Reflected word / CRC
 * 
 * @note: Could probably replace this with something more portable; BSWAP not available
 * on all archs
 */
inline static crc_t reflect_crc(uint64_t word, uint32_t num_bytes)
{
    __asm__("BSWAP %0" : "=r"(word) : "0"(word));
    word >>= ((sizeof(uint64_t) - num_bytes) * 8);
    word = ((word & 0xaaaaaaaaaaaaaaaa) >> 1) | ((word & 0x5555555555555555) << 1);
    word = ((word & 0xcccccccccccccccc) >> 2) | ((word & 0x3333333333333333) << 2);
    word = ((word & 0xf0f0f0f0f0f0f0f0) >> 4) | ((word & 0x0f0f0f0f0f0f0f0f) << 4);
    return word;
}
#endif


#if (CRC_USE_TABLE == 1 && CRC_USER_PROVIDES_TABLE == 0)
void crc_init_table(void)
{
    crc_t rem;

    for (uint32_t divdn = 0; divdn < 256; ++divdn) {
        rem = divdn << ((CRC_WIDTH_BYTES*8) - 8);

        for (uint8_t bit = 8; bit > 0; --bit) {
            if (rem & CRC_MSB) {
                rem = (rem << 1) ^ CRC_POLY;
            } else {
                rem = (rem << 1);
            }
        }

        crc_table[divdn] = rem;
    }
    #if CRC_CHECK_TABLE_INIT == 1
    table_is_init = 1;
    #endif
}
#endif


#if CRC_WIDTH_BYTES == 1
uint8_t crc8(const uint8_t *data, uint32_t len) {
    return __calc_crc(data, len);
}
#elif CRC_WIDTH_BYTES == 2
uint16_t crc16(const uint8_t *data, uint32_t len) {
    return __calc_crc(data, len);
}
#elif CRC_WIDTH_BYTES == 4
uint32_t crc32(const uint8_t *data, uint32_t len) {
    return __calc_crc(data, len);
}
#elif CRC_WIDTH_BYTES == 8
uint64_t crc64(const uint8_t *data, uint32_t len) {
    return __calc_crc(data, len);
}
#endif //CRC_WIDTH_BYTES
