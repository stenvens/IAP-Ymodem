#ifndef IAPPROTOCOL_H
#define IAPPROTOCOL_H
#include <QCoreApplication>
#include <iostream>
#include <QDebug>

class IapProtocol
{
public:
    explicit IapProtocol(QWidget *parent = 0);
    ~IapProtocol();

    /*
    1. tool: reboot
    2. dev: update
    3. tool: update_ack + (package_num + file_size + file_crc32)
    4. dev: request_data + (package_index)
    5. tool: data + (package_index + crc32 for buf + package_len + buf)
    6. dev: ret (ret)
    */
public:
    //升级状态
    enum UpdateStatus
    {
        None = 0,
        WaitDeviceUpdate = 1, //等待设备发起升级
        WaitDeviceRequestData = 2, //等待设备请求数据
        SendFileData = 3, //发送文件数据
        End = 4, //升级结束
    };

    //命令
    enum UpdateCmd
    {
        Reboot = 1,
        Update = 2,
        UpdateAck = 3,
        RequestData = 4,
        Data = 5,
        UpdateRet = 6,
    };

private:
    const char *_featureStrReboot = "reboot";
    const char *_featureStrDeviceUpdate = "update";
    const char *_featureStrDeviceUpdateAck = "update_ack";
    const char *_featureStrRequestData = "request_data";
    const char *_featureStrFileData = "data";
    const char *_featureStrUpdateRet = "ret";

    const int RECV_LENGTH_MAXIMUM = 300; //最大缓存字节
    const int PACKAGE_LENGTH_MAXIMUM = 2100; //最大支持2048字节一包的文件数据

public:
    uint8_t *_recvBuf; //接收缓存
    int _recvLength = 0; //累计接收长度
    uint8_t *_packageBuf;
    int _packageSize = 0; //当前下发包大小
    int _updateStatus = None; //升级状态
    int _requestPackageIndex = 0; //当前请求包序号
    int _fileSize = 0; //升级文件大小
    int _packageNum = 0; //升级文件总包数
    uint32_t _fileCrc32 = 0; //升级文件的CRC32
    bool _updateRet = false; //升级结果

    void ClearRecvBuffer();
    void AddDataToBuffer(uint8_t *buf, int length);
    QByteArray FormDataDeviceReboot();
    QByteArray FormDataUpdateAck(int packageNum, int fileSize, uint fileCrc32);
    QByteArray FormDataUpdateFileData(int packageIndex, uint packageCrc32, int packageLength, uint8_t *packageBuf);
    bool RequestDataAnalysis(uint8_t *buf, int length, int offset);
    bool UpdateRetAnalysis(uint8_t *buf, int length, int offset);
    int UartRecvDataAnalysis(uint8_t *buf, int length);
    bool UartRecvDataAnalysisProc(int destCmdType);
};

#endif // IAPPROTOCOL_H
