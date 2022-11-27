#ifndef CRC16CCITT_H
#define CRC16CCITT_H
#include <stdint.h>
#include <QDebug>

class crc16ccitt
{
public:
    uint16_t computeChecksum(QByteArray pBuf, int pBufSize);
};

#endif // CRC32_H
