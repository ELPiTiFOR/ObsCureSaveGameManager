#include "my_crc.h"

#define POLY 0xEDB88320

unsigned int crc32(unsigned char *msg, size_t len)
{
    unsigned int res = 0xFFFFFFFF;

    for (size_t i = 0; i < len; i++)
    {
        res = res ^ msg[i];

        for (size_t j = 0; j < 8; j++)
        {
            unsigned int mask = -(res & 1);
            res = (res >> 1) ^ (POLY & mask);
        }
    }

    return ~res;
}