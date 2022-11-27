#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QDebug>

#include <QThread>
#include <QMutexLocker>
#include <QMutex>
#include <QSerialPort>


class Download : public QObject
{
    Q_OBJECT

public:
    explicit Download(QObject *parent = 0);
    ~Download();

    QSerialPort *serialPort;
    QThread downloadThread;

signals:
    void signalSendMsg(QString sMsg);
    void signalSendDownloadMsg(QString sMsg);
    void signalSendDownloadProcess(int processValue);
    void signalSendDownloadEnd(QString sMsg);

private:
    const uint8_t SOH = 0x01;  /* start of 128-byte data packet - 133字节长度帧 */
    const uint8_t STX = 0x02;  /* start of 1024-byte data packet - 1024字节长度帧 */
    const uint8_t EOT = 0x04;  /* end of transmission - 文件传输结束命令*/
    const uint8_t ACK = 0x06;  /* acknowledge - 接收正确应答命令 */
    const uint8_t NAK = 0x15;  /* negative acknowledge - 重传当前数据包请求命令 */
    const uint8_t CA = 0x18;   /* two of these in succession aborts transfer - 取消传输命令，连续发送5个该命令 */
    const uint8_t C = 0x43;    /* capital letter C */

private:
    QMutex lock;
    int _serialReadTimeOutMs = 3000; //串口读取超时毫秒数
    int _waitDeviceAckAsMaximum = 2000; //等待设备回复超时时间 (因需要根据设备实际擦除APP代码区大小而定)
    int _updatePackageSize = 128; //升级文件包大小
    int _updateFileLength = 0; //升级文件长度
    bool isCanRun = false;

    int calculateUpdatePackageNum(int fileSize);
    int calculateSendUpdatePackageLength(int packageNum, int currentPackageIndex);
    void sendMsg(char *text);
    void sendDownloadMsg(char *text);
    void sendDownloadProcess(int processValue);
    bool serialPortWaitRead(uint8_t data);
    bool serialPortWaitReadEx(uint8_t data);
    void sendYmodemInitialPacket(uint8_t head, int packetNumber, int dataSize, char *fileName, int fileSize);
    void sendYmodemPacket(uint8_t head, int packetNumber, QByteArray data, int dataSize);
    void sendEndSignalPacket();
    void sendCancelSignalPacket();
    bool YmodemUploadFile(int waitFirstPacketAckMs);
    void sendDeviceRebootPacket();
    void sendStartUpdatePacket();

public slots:
    void doWork1(const QString portName, const int baudRate, int fileSize, int yModemPackageSize);

public:
    void stopWork();  //打断线程（注意此方法不能放在槽函数下）
};

#endif // DOWNLOAD_H
