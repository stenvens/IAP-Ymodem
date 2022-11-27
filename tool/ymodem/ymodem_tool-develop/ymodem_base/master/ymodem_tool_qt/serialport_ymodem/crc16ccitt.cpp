#include "crc16ccitt.h"

const uint16_t poly = 4129;
uint16_t table[256] = {0};
uint16_t initialValue = 0;

void crc16init()
{
    uint16_t temp, a;

    for (int i = 0; i < sizeof(table); ++i)
    {
        temp = 0;
        a = (uint16_t)(i << 8);
        for (int j = 0; j < 8; ++j)
        {
            if (((temp ^ a) & 0x8000) != 0)
            {
                temp = (uint16_t)((temp << 1) ^ poly);
            }
            else
            {
                temp <<= 1;
            }
            a <<= 1;
        }
        table[i] = temp;
    }
}

uint16_t crc16ccitt::computeChecksum(QByteArray pBuf, int pBufSize)
{
    uint16_t crc = initialValue;

    crc16init();

    for (int i = 0; i < pBufSize; ++i)
    {
        crc = (uint16_t)((crc << 8) ^ table[((crc >> 8) ^ (0xff & pBuf[i]))]);
    }
    return crc;
}

