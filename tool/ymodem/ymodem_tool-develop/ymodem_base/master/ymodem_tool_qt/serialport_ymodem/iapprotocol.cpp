#include "iapprotocol.h"

void printHexBuf(char *text, QByteArray array)
{
    QString strPrint("");
    strPrint.clear();
    for(int i=0; i<array.length(); i++)
    {
        char c = array.at(i);
        strPrint += QString::number( ((int)c)&0xFF) + " ";
    }
    qDebug() << text;
    qDebug() << strPrint;
}

IapProtocol::IapProtocol(QWidget *parent)
{
    _recvBuf = (uint8_t*)malloc(RECV_LENGTH_MAXIMUM);
    _packageBuf = (uint8_t*)malloc(PACKAGE_LENGTH_MAXIMUM);
}

IapProtocol::~IapProtocol()
{
    free(_recvBuf);
}

void IapProtocol::ClearRecvBuffer()
{
    memset(_recvBuf, 0x00, RECV_LENGTH_MAXIMUM);
    _recvLength = 0;
}

void IapProtocol::AddDataToBuffer(uint8_t *buf, int length)
{
    if ((_recvLength + length) <= RECV_LENGTH_MAXIMUM)
    {
        for(int i=0; i<length; i++)
        {
            _recvBuf[_recvLength + i] = buf[i];
        }
        _recvLength += length;
    }
}

//组装数据 发送设备重启
QByteArray IapProtocol::FormDataDeviceReboot()
{
    QByteArray array(_featureStrReboot);
    return array;
}

//组装数据 发送开始升级回复
QByteArray IapProtocol::FormDataUpdateAck(int packageNum, int fileSize, uint fileCrc32)
{
    /* "update_ack" + package num (2 byte) + file_size (4 byte) + file_crc32 (4 byte) */
    QByteArray temp(_featureStrDeviceUpdateAck);
    QByteArray byteBuf = temp;
    byteBuf.resize(temp.length() + 10);

    int offset = temp.length();
    //package_num
    byteBuf[offset++] = (uint8_t)(packageNum / 256);
    byteBuf[offset++] = (uint8_t)(packageNum % 256);
    //file_size
    int file_length_byte1 = fileSize / (256 * 256 * 256);
    int file_length_byte2 = fileSize % (256 * 256 * 256) / (256 * 256);
    int file_length_byte3 = fileSize % (256 * 256 * 256) % (256 * 256) / 256;
    int file_length_byte4 = fileSize % (256 * 256 * 256) % (256 * 256) % 256;
    byteBuf[offset++] = (uint8_t)file_length_byte1;
    byteBuf[offset++] = (uint8_t)file_length_byte2;
    byteBuf[offset++] = (uint8_t)file_length_byte3;
    byteBuf[offset++] = (uint8_t)file_length_byte4;
    //file_crc32
    uint file_crc32_byte1 = fileCrc32 / (256 * 256 * 256);
    uint file_crc32_byte2 = fileCrc32 % (256 * 256 * 256) / (256 * 256);
    uint file_crc32_byte3 = fileCrc32 % (256 * 256 * 256) % (256 * 256) / 256;
    uint file_crc32_byte4 = fileCrc32 % (256 * 256 * 256) % (256 * 256) % 256;
    byteBuf[offset++] = (uint8_t)file_crc32_byte1;
    byteBuf[offset++] = (uint8_t)file_crc32_byte2;
    byteBuf[offset++] = (uint8_t)file_crc32_byte3;
    byteBuf[offset++] = (uint8_t)file_crc32_byte4;

    return byteBuf;
}

//组装数据 发送升级数据
QByteArray IapProtocol::FormDataUpdateFileData(int packageIndex, uint32_t packageCrc32, int packageLength, uint8_t *packageBuf)
{
    /* "data" + package_index (2 byte) + package_buf_crc32 (4 byte) + package_len (2 byte) + buf (n byte) */
    QByteArray temp(_featureStrFileData);
    QByteArray byteBuf = temp;
    byteBuf.resize(temp.length() + 8 + packageLength);

    int offset = temp.length();
    //package_index
    byteBuf[offset++] = (uint8_t)(packageIndex / 256);
    byteBuf[offset++] = (uint8_t)(packageIndex % 256);
    //package_buf_crc32
    uint package_buf_crc32_byte1 = packageCrc32 / (256 * 256 * 256);
    uint package_buf_crc32_byte2 = packageCrc32 % (256 * 256 * 256) / (256 * 256);
    uint package_buf_crc32_byte3 = packageCrc32 % (256 * 256 * 256) % (256 * 256) / 256;
    uint package_buf_crc32_byte4 = packageCrc32 % (256 * 256 * 256) % (256 * 256) % 256;
    byteBuf[offset++] = (uint8_t)package_buf_crc32_byte1;
    byteBuf[offset++] = (uint8_t)package_buf_crc32_byte2;
    byteBuf[offset++] = (uint8_t)package_buf_crc32_byte3;
    byteBuf[offset++] = (uint8_t)package_buf_crc32_byte4;
    //package_length
    byteBuf[offset++] = (uint8_t)(packageLength / 256);
    byteBuf[offset++] = (uint8_t)(packageLength % 256);
    //package_buf
    for(int i=0; i<packageLength; i++)
    {
        byteBuf[offset++] = packageBuf[i];
    }

    return byteBuf;
}

bool IapProtocol::RequestDataAnalysis(uint8_t *buf, int length, int offset)
{
    bool ret = false;

    //package_index
    int packageIndex = (int)(buf[offset] * 256) + (int)buf[offset + 1];
    qDebug("recv packageIndex = %d", packageIndex);

    //请求包验证
    if (packageIndex != 0 && packageIndex <= _packageNum)
    {
        _requestPackageIndex = packageIndex;
        ret = true;
    }

    return ret;
}

bool IapProtocol::UpdateRetAnalysis(uint8_t *buf, int length, int offset)
{
    bool ret = false;

    //ret
    int updateRet = (int)buf[offset];
    qDebug("recv update_ret = %d" + updateRet);

    if (updateRet == 0x00)
    {
        _updateRet = false; //升级失败
        ret = true;
    }
    else if (updateRet == 0x01)
    {
        _updateRet = true; //升级成功
        ret = true;
    }

    return ret;
}

void test_printHexBuf(char *text, QByteArray array)
{
    QString strPrint("");
    strPrint.clear();
    for(int i=0; i<array.length(); i++)
    {
        char c = array.at(i);
        strPrint += QString::number( ((int)c)&0xFF) + " ";
    }
    qDebug() << text;
    qDebug() << strPrint;
}

int IapProtocol::UartRecvDataAnalysis(uint8_t *buf, int length)
{
    int cmdType = 0;
    int firstValidCharOffset = length;

    for (int i = 0; i < length; i++)
    {
        if(buf[i] != 0x00)
        {
            firstValidCharOffset = i;
            break;
        }
    }
    if (length == firstValidCharOffset) return 0;

    QByteArray strBuf("");
    //char strBuf[length - firstValidCharOffset + 1] = {0};
    strBuf.resize(length - firstValidCharOffset);
    for(int i=0; i<length - firstValidCharOffset; i++)
    {
        strBuf[i] = buf[firstValidCharOffset + i];
    }
    QString str(strBuf);
    qDebug()<< "str = " << str;
    test_printHexBuf("res : ", strBuf);

    /* update */
    if (str.contains(_featureStrDeviceUpdate))
    {
        //set update status
        _updateStatus = WaitDeviceRequestData;
        cmdType = Update;

        qDebug()<< "dev: update";
    }
    /* request_data + package_index (2 byte) */
    else if (str.contains(_featureStrRequestData))
    {
        if (length >= (strlen(_featureStrRequestData) + str.indexOf(_featureStrRequestData) + 2)) //确认收到特征字符串+数据
        {
            int offset = str.indexOf(_featureStrRequestData) + strlen(_featureStrRequestData);
            RequestDataAnalysis(buf, length, offset);
            cmdType = RequestData;

            qDebug()<< "dev: request_data";
        }
    }
    /* ret + ret (1 byte) */
    else if (str.contains(_featureStrUpdateRet))
    {
        if (length >= (strlen(_featureStrUpdateRet) + str.indexOf(_featureStrUpdateRet) + 1)) //确认收到特征字符串+数据
        {
            int offset = str.indexOf(_featureStrUpdateRet) + strlen(_featureStrUpdateRet);
            UpdateRetAnalysis(buf, length, offset);
            cmdType = UpdateRet;

            qDebug()<< "dev: update_ret";
        }
    }

    return cmdType;
}

//升级文件协议数据解析
bool IapProtocol::UartRecvDataAnalysisProc(int destCmdType)
{
    int cmdType = 0;

    //串口协议的数据接收抓包
    cmdType = UartRecvDataAnalysis(_recvBuf, _recvLength);

    //收到升级结束
    if (cmdType == UpdateRet)
    {
        qDebug()<< "recv update ret";
        _updateStatus = End;
        return true;
    }

    //未识别到有效的操作码则返回失败
    if (cmdType == 0x00 || cmdType != destCmdType)
    {
        return false;
    }

    return true;
}
