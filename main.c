#include <stdio.h>
#include <stdint.h>

#include <crc.h>

int32_t main(void)
{

    uint8_t buf[] = {0xDE, 0xAD, 0xBE, 0xEF};

    #if CRC_WIDTH_BYTES == 1 
    printf("CRC8: 0x%02X\n", crc8(buf, 4));
    #elif CRC_WIDTH_BYTES == 2
    printf("CRC16: 0x%04X\n", crc16(buf, 4));
    #elif CRC_WIDTH_BYTES == 4
    printf("CRC32: 0x%08X\n", crc32(buf, 4));
    #elif CRC_WIDTH_BYTES == 8
    printf("CRC64: 0x%16X\n", crc64(buf, 4));
    #endif

    return 0; 
}