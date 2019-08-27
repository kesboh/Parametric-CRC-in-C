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
 * Parameterized CRC implementation 
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

#if CRC_USE_TABLE == 1 && CRC_USER_PROVIDES_TABLE == 0
static crc_t crc_table[256];
#endif //CRC_USE_TABLE == 1

#if CRC_REFLECT_INPUT == 1
inline static uint8_t reflect_byte(uint8_t val);
#endif

#if CRC_REFLECT_OUTPUT == 1
inline static crc_t reflect_crc(crc_t crc_in);
#endif

/**
 * Parameterized CRC implementation 
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
        } //End of non-table driven calculation

        #else //Use table
        #if CRC_REFLECT_INPUT == 1
        uint8_t tmp = reflect_byte(data[curr_byte]) ^ (crc >> ((CRC_WIDTH_BYTES*8)-8));
        #else
        uint8_t tmp = data[curr_byte] ^ (crc >> ((CRC_WIDTH_BYTES*8)-8));
        #endif //CRC_REFLECT_INPUT == 1
        crc = crc_table[tmp] ^ (crc << 8);
        #endif //CRC_USE_TABLE == 0

    }

    #if CRC_REFLECT_OUTPUT == 1
    return reflect_crc(crc ^ CRC_FINAL_XOR_VAL);
    #else
    return crc ^ CRC_FINAL_XOR_VAL;
    #endif

}


#if (CRC_REFLECT_INPUT == 1) || (CRC_REFLECT_OUTPUT == 1)
/**
 * Reflects the bits of a byte and returns it
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
 * Reflects the bits of a provided CRC and returns it
 * 
 * @param crc_in The CRC to reflect
 * 
 * @return The reflected CRC
 */
inline static crc_t reflect_crc(crc_t crc_in)
{
    crc_t refl = 0;
    uint8_t i;

    for (i = 0; i < sizeof(crc_t); i++) {
        refl |= reflect_byte((uint8_t)((crc_in >> i*8) & 0xFF)) << ((sizeof(crc_t) - 1 - i)*8);
    }

    return refl;
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
