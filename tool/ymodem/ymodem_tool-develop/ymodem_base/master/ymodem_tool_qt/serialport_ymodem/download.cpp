#include "download.h"
#include "iapprotocol.h"
#include "crc16ccitt.h"
#include "mainwindow.h"


Download::Download(QObject *parent)
{
    qDebug()<<"Download Thread Init"<<endl;
}

Download::~Download()
{
    qDebug()<<"Download Thread Deinit"<<endl;

    downloadThread.quit();

    /* 阻塞线程2000ms，判断线程是否结束 */
    if (downloadThread.wait(2000)) {
        qDebug()<<"线程结束"<<endl;
    }
}

/*********************************************************************
* @brief  : 发送下载消息
*---------------------------------------------------------------------
* @param  : char *text  ,   消息文本
*---------------------------------------------------------------------
* @retval : none
*********************************************************************/
void Download::sendDownloadMsg(char *text)
{
    QString str = text;
    signalSendDownloadMsg(str + "\r\n");
}

/*********************************************************************
* @brief  : 发送下载进度
*---------------------------------------------------------------------
* @param  : int processValue  ,   下载进度百分比
*---------------------------------------------------------------------
* @retval : none
*********************************************************************/
void Download::sendDownloadProcess(int processValue)
{
    signalSendDownloadProcess(processValue);
}

/*********************************************************************
* @brief  : 串口等待读取指定的数据(有数据来了就判断一次然后返回结果)
*---------------------------------------------------------------------
* @param  : uint8_t data  ,   指定的数据
*---------------------------------------------------------------------
* @retval : true     ,  读取成功
*           false    ,  读取失败
*********************************************************************/
bool Download::serialPortWaitRead(uint8_t data)
{
    if(serialPort->waitForReadyRead(_serialReadTimeOutMs)) //等待数据返回
    {
        QByteArray array = serialPort->readAll();
        uint8_t read_data = array.at(0);
        if(read_data == data)
        {
            return true;
        }
        else
        {
            qDebug("waitRead(%d,%c) Fail, Get %d,%c\r\n", data, data, read_data, read_data);
        }
    }
    return false;
}

/*********************************************************************
* @brief  : 串口等待读取指定的数据(在超时时间内一直判断然后返回结果)
*---------------------------------------------------------------------
* @param  : uint8_t data  ,   指定的数据
*---------------------------------------------------------------------
* @retval : true     ,  读取成功
*           false    ,  读取失败
*********************************************************************/
bool Download::serialPortWaitReadEx(uint8_t data)
{
    int current_msec = QDateTime::currentMSecsSinceEpoch(); //当前时间的毫秒数
    int end_msec = current_msec + _serialReadTimeOutMs; //超时时间的毫秒数

    while(current_msec <= end_msec)
    {
        if(serialPort->waitForReadyRead(100)) //等待数据返回
        {
            QByteArray array = serialPort->readAll();
            uint8_t read_data = array.at(0);
            if(read_data == data)
            {
                return true;
            }
            else
            {
                qDebug("ExwaitRead(%d,%c) Fail, Get %d,%c\r\n", data, data, read_data, read_data);
            }
        }

        //更新当前时间
        current_msec = QDateTime::currentMSecsSinceEpoch();
    }

    return false;
}

/*********************************************************************
* @brief  : 发送结束信号数据包
*********************************************************************/
void Download::sendEndSignalPacket()
{
    QByteArray baEndSignal;
    baEndSignal.resize(1);
    baEndSignal[0] = EOT;

    serialPort->write(baEndSignal);
    serialPort->waitForBytesWritten();
}

/*********************************************************************
* @brief  : 发送取消信号数据包
*********************************************************************/
void Download::sendCancelSignalPacket()
{
    QByteArray baEndSignal;
    baEndSignal.resize(5);
    for(int i=0; i<5; i++)
    {
        baEndSignal[i] = CA;
    }

    serialPort->write(baEndSignal);
    serialPort->waitForBytesWritten();
}

/*********************************************************************
* @brief  : 发送Ymodem数据包
*---------------------------------------------------------------------
* @param  : uint8_t head        ,   包头(区分长短包)
*           int packetNumber    ,   文件包数量
*           QByteArray data     ,   包数据内容
*           int dataSize        ,   数据包大小
*---------------------------------------------------------------------
* @retval : none
*********************************************************************/
void Download::sendYmodemPacket(uint8_t head, int packetNumber, QByteArray data, int dataSize)
{
    /*
     * 长数据包(非初始化包): STX 01 FE data[1024] CRC CRC
     *                    STX 02 FD data[1024] CRC CRC
     * 短数据包(非初始化包): SOH 05 FA data[100] 1A[28] CRC CRC
     */

    QByteArray packet = data;
    packet.resize(3 + dataSize + 2);

    //帧头
    packet[0] = head;

    //包号
    uint8_t num = (uint8_t)(packetNumber & 0xFF);
    packet[1] = num;

    //包号反码
    packet[2] = (uint8_t)((~num) & 0xFF);

    //数据区
    for(int i=0; i<dataSize; i++)
    {
        packet[i + 3] = data[i];
    }

    //校验
    crc16ccitt crc16t;
    uint16_t crc = crc16t.computeChecksum(data, dataSize);
    packet[3 + dataSize] = (uint8_t)(crc / 256);
    packet[3 + dataSize + 1] = (uint8_t)(crc % 256);

    //开始串口发送数据
    serialPort->write(packet);
    serialPort->waitForBytesWritten();
}

/*********************************************************************
* @brief  : 发送Ymodem初始化数据包
*---------------------------------------------------------------------
* @param  : uint8_t head        ,   包头(区分长短包)
*           int packetNumber    ,   文件包数量
*           int dataSize        ,   数据包大小
*           char *fileName      ,   文件名
*           int fileSize        ,   文件大小
*---------------------------------------------------------------------
* @retval : true     ,  读取成功
*           false    ,  读取失败
*********************************************************************/
void Download::sendYmodemInitialPacket(uint8_t head, int packetNumber, int dataSize, char *fileName, int fileSize)
{
    //初始化数据包格式: SOH 00 FF "foo.c" "1064" NUL[118] CRC CRC
    QString strFileSize;
    strFileSize = strFileSize.sprintf("%d",fileSize);
    QByteArray data;
    QByteArray baFileSize = strFileSize.toUtf8();

    //添加文件名
    int i;
    for (i = 0; i < strlen(fileName); i++)
    {
        data[i] = fileName[i];
    }
    data[i] = 0;

    //添加文件长度
    int j;
    for (j = 0; j < baFileSize.size(); j++)
    {
        data[(i + 1) + j] = baFileSize[j];
    }
    data[(i + 1) + j] = 0;

    //剩余字节填充0
    for (int k = ((i + 1) + j) + 1; k < dataSize; k++)
    {
        data[k] = 0;
    }

    sendYmodemPacket(head, packetNumber, data, dataSize);
}


/*********************************************************************
* @brief  : Ymodem下载文件流程
*---------------------------------------------------------------------
* @param  : int waitFirstPacketAckMs    ,   等待设备回复初始包的超时时间
*---------------------------------------------------------------------
* @retval : true     ,  下载成功
*           false    ,  下载失败
*********************************************************************/
bool Download::YmodemUploadFile(int waitFirstPacketAckMs)
{
    /*
    传输有效数据时主要考虑的是最后一包数据的是处理，SOH帧和STX帧有不同的处理
    [1] 对于SOH帧，若余下数据小于128字节，则以0x1A填充，该帧长度仍为133字节
    [2] 对于STX帧需考虑几种情况：
        余下数据等于1024字节，以1029长度帧发送；
        余下数据小于1024字节，但大于128字节，以1029字节帧长度发送，无效数据以0x1A填充。
        余下数据等于128字节，以133字节帧长度发送。
        余下数据小于128字节，以133字节帧长度发送，无效数据以0x1A填充。
    */
    int i = 0;
    bool transmission_ret = false; //下载结果
    int progress = 0; //升级进度百分比
    uint8_t packetHead = 0; //数据包头
    int packetNumber = 0; //当前下发数据包序号
    int packageLength = 0; //当前下发包大小
    int dataSize = _updatePackageSize; //升级文件包大小
    int updatePackageNum = calculateUpdatePackageNum(file->_fileLength); //下发数据包总个数

    sendDownloadMsg((char*)"正在升级中...");

    //升级文件包缓存申请空间
    QByteArray data;
    data.resize(dataSize);

    //获取升级文件名
    QByteArray baFileName = file->_fileName.toLatin1();
    char *fileName = baFileName.data();

    //等待设备请求包
    if(serialPortWaitReadEx(C) != true)
    {
        sendDownloadMsg((char*)"失败！原因：等待设备的<请求包>指令超时。");
        goto download_end;
    }

    //发送初始包
    sendYmodemInitialPacket(SOH, packetNumber, 128, fileName, file->_fileLength);
    qDebug("Send Initial Packet\r\n");

    //某些设备在收到InitialPacket后就擦除整个APP区Flash则会需要大量时间，所以加设等待
    if (waitFirstPacketAckMs > 0)
    {
        QThread::msleep(waitFirstPacketAckMs);
    }

    //等待设备回复初始包
    if (serialPortWaitRead(ACK) != true)
    {
        sendDownloadMsg((char*)"失败！原因：等待设备的<回复初始包>指令超时。");
        goto download_end;
    }

    //等待再次握手信号
    if (serialPortWaitRead(C) != true)
    {
        sendDownloadMsg((char*)"失败！原因：等待设备的<再次握手>指令超时。");
        goto download_end;
    }

    //文件数据传输
    while(true)
    {
        //计算当前要下发的数据长度
        packetNumber++;
        packageLength = calculateSendUpdatePackageLength(updatePackageNum, packetNumber);

        //提取文件数据
        for(i=0 ;i<packageLength; i++)
        {
            data[i] = file->_binaryDataBuffer[((packetNumber - 1) * dataSize) + i];
        }
        //qDebug("file_start=%d, len=%d, packetNumber=%d\r\n", ((packetNumber - 1) * dataSize), packageLength, packetNumber);

        //提取文件数据
        if (_updatePackageSize == 128) //"Ymodem"
        {
            if (packageLength < 128)
            {
                for (i = packageLength; i < 128; i++) data[i] = 0x1A;
            }
            packetHead = SOH;
        }
        else if (_updatePackageSize == 1024) //"Ymodem-1K"
        {
            if (packageLength < 128)
            {
                for (int i = packageLength; i < 128; i++) data[i] = 0x1A;
                dataSize = 128;
                packetHead = SOH;
            }
            else if (packageLength < 1024)
            {
                for (int i = packageLength; i < dataSize; i++) data[i] = 0x1A;
                dataSize = 1024;
                packetHead = STX;
            }
            else
            {
                packetHead = STX;
            }
        }

        //更新进度条
        float processValue = (float)packetNumber / (float)updatePackageNum;
        processValue = (int)(processValue * 100);
        sendDownloadProcess(processValue);

        //发送文件数据包
        sendYmodemPacket(packetHead, packetNumber, data, dataSize);

        //等待包接收回复
        if (serialPortWaitRead(ACK) != true)
        {
            sendDownloadMsg((char*)"失败！原因：等待设备的<包接收回复>指令超时。");
            goto download_end;
        }

        //停止下载判断
        if (isCanRun == false) goto download_end;

        //进度刷新
        progress = (int)(((float)dataSize * packetNumber) / file->_fileLength * 100);
        if (progress > 100) progress = 100;
        //qDebug("progress:%d\r\n" + progress);

        //下发结束跳转判断
        if (packetNumber >= updatePackageNum) break;
    };

    //发送结束信号
    sendEndSignalPacket();

    //等待结束信号回复
    if (serialPortWaitRead(ACK) != true)
    {
        sendDownloadMsg((char*)"失败！原因：等待设备的<结束信号回复>指令超时。");
        goto download_end;
    }

    //发送结束包
    packetNumber = 0;
    dataSize = 128;
    for(i=0; i<dataSize; i++)
    {
        data[i] = 0;
    }
    sendYmodemPacket(SOH, packetNumber, data, dataSize);

    //等待结束包回复
    if (serialPortWaitRead(ACK) != true)
    {
        sendDownloadMsg((char*)"失败！原因：等待设备的<结束包回复>指令超时。");
        goto download_end;
    }

    transmission_ret = true;

download_end:
    if (isCanRun == false) //停止下载结果
    {
         sendCancelSignalPacket();
         sendDownloadMsg((char*)"失败！原因：停止下载。");
    }

    return transmission_ret;
}

/*********************************************************************
* @brief  : 开始任务的槽函数
*---------------------------------------------------------------------
* @param  : const QString portName  ,   端口名称
*           const int baudRate      ,   波特率
*           int fileSize            ,   文件大小
*           yModemPackageSize       ,   传输包大小
*---------------------------------------------------------------------
* @retval : none
*********************************************************************/
void Download::doWork1(const QString portName, const int baudRate, int fileSize, int yModemPackageSize)
{
    bool update_ret = false;

    _updateFileLength = fileSize;
    _updatePackageSize = yModemPackageSize;
    isCanRun = true;

    qDebug("download start\r\n");

    //串口设置
    serialPort = new QSerialPort(this);
    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::UnknownStopBits);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    //开启串口
    if (!serialPort->open(QIODevice::ReadWrite))
    {
        qDebug() << "open serialport fail" << endl;
        sendDownloadMsg((char*)"串口无法打开！可能串口已经被占用！");
        goto end;
    }
    else
    {
        qDebug() << "open serialport success" << endl;
    }

    //Ymodem下载流程
    update_ret = YmodemUploadFile(3000);

    //关闭串口
    serialPort->clear(); //关闭串口前清空，预防在通讯中突然关闭导致数据残留而关闭串口失败
    serialPort->close();
    qDebug("close serialPort");

    //升级结果处理
    if (update_ret == true)
    {
        qDebug("File Transmission Succesfully!\r\n");
        sendDownloadMsg((char*)"");
        sendDownloadMsg((char*)"升级成功！");
    }
    else
    {
        qDebug("File Transmission Fail!\r\n");
        sendDownloadMsg((char*)"升级失败！");
    }

end:
    signalSendDownloadEnd(""); //通知线程升级流程结束
    qDebug("download end\r\n");
}

/*********************************************************************
* @brief  : 停止升级流程处理
*********************************************************************/
void Download::stopWork()
{
    qDebug("stopWork");
    isCanRun = false;
}

/*********************************************************************
* @brief  : 计算总共要分发的升级文件包数
*---------------------------------------------------------------------
* @param  : int fileSize  ,   文件大小
*---------------------------------------------------------------------
* @retval : 升级文件包数
*********************************************************************/
int Download::calculateUpdatePackageNum(int fileSize)
{
    int num = 0;
    int temp = fileSize % _updatePackageSize;
    if (temp != 0)
    {
        num = (fileSize / _updatePackageSize) + 1;
    }
    else
    {
        num = fileSize / _updatePackageSize;
    }
    return num;
}

/*********************************************************************
* @brief  : 计算当前要发送升级包的长度
*---------------------------------------------------------------------
* @param  : int packageNum          ,   升级文件包数
*           int currentPackageIndex ,   当前下发包序号
*---------------------------------------------------------------------
* @retval : none
*********************************************************************/
int Download::calculateSendUpdatePackageLength(int packageNum, int currentPackageIndex)
{
   int length = 0;
   if (currentPackageIndex != packageNum) //非最后一包
   {
       length = _updatePackageSize;
   }
   else //最后一包的字节长度
   {
       length = _updateFileLength - (packageNum - 1) * _updatePackageSize;
   }
   return length;
}

