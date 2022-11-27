#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker.h"
#include "download.h"
#include <QAbstractItemView> //用于combobox自适应文本长度


File *file = new File; //存放文件升级相关信息

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* 界面初始化 */
    transmitionModeItemInit(); //传输模式项初始化
    baudRateItemInit(); //波特率项初始化
    scanSerialPort(); //扫描系统的串口
    downloadProgressInit();
    initStartDownloadButtonLabel();
    setStartDownloadButtonLabel(false);
    initOpenFileLabel();
    initOpenFileButton();

    /* 下载线程初始化 */
    downloadThreadInit();

    /* 信号槽连接 */
    connect(ui->pushButton_openFile, SIGNAL(clicked()), this, SLOT(openFilePushButtonClicked()));
    connect(openFileLabel, SIGNAL(clicked(ClickedLabel*)), this, SLOT(openFilePushButtonClicked()));
    connect(ui->pushButton_startDownload, SIGNAL(clicked()), this, SLOT(startDownloadPushButtonClicked()));
    connect(startDownloadLabel, SIGNAL(clicked(ClickedLabel*)), this, SLOT(startDownloadPushButtonClicked()));
    connect(ui->pushButton_scanSerialPort, SIGNAL(clicked()), this, SLOT(scanSerialPortPushButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*********************************************************************
* @brief  : 打开升级文件按钮初始化
*********************************************************************/
void MainWindow::initOpenFileButton()
{
    QPixmap pixmap(":/images/Dark_blue_button_64px.png");
    QPixmap fitpixmap = pixmap.scaled(61, 141, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->pushButton_openFile->setIcon(QIcon(fitpixmap));
    ui->pushButton_openFile->setIconSize(QSize(61, 141));
}

/*********************************************************************
* @brief  : 打开升级文件按钮文本初始化
*********************************************************************/
void MainWindow::initOpenFileLabel()
{
    openFileLabel = new ClickedLabel(this);
    openFileLabel->setGeometry(QRect(10+280+22, 165+30+18, 40, 100));

    QFont font("微软雅黑", 14, QFont::Bold);
    openFileLabel->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::white);
    openFileLabel->setPalette(pe);

    openFileLabel->setText("打\r\n开\r\n文\r\n件");
}

/*********************************************************************
* @brief  : 开始文件下载按钮文本初始化
*********************************************************************/
void MainWindow::initStartDownloadButtonLabel()
{
    startDownloadLabel = new ClickedLabel(this);
    startDownloadLabel->setGeometry(QRect(10+240+12, 355+80+22, 80, 30));

    QFont font("微软雅黑", 14, QFont::Bold);
    startDownloadLabel->setFont(font);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::white);
    startDownloadLabel->setPalette(pe);

    startDownloadLabel->setText("开始下载");
}

/*********************************************************************
* @brief  : 设置开始下载按钮文本
*---------------------------------------------------------------------
* @param  : bool downloadStatus  ,   下载状态
*---------------------------------------------------------------------
* @retval : none
*********************************************************************/
void MainWindow::setStartDownloadButtonLabel(bool downloadStatus)
{
    if(downloadStatus == false)
    {
        QPixmap pixmap(":/images/Light_blue_button_64px.png");
        QPixmap fitpixmap = pixmap.scaled(100, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->pushButton_startDownload->setIcon(QIcon(fitpixmap));
        ui->pushButton_startDownload->setIconSize(QSize(100, 80));

        startDownloadLabel->setText("开始下载");
    }
    else
    {
        QPixmap pixmap(":/images/Red_button_64px.png");
        QPixmap fitpixmap = pixmap.scaled(100, 80, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->pushButton_startDownload->setIcon(QIcon(fitpixmap));
        ui->pushButton_startDownload->setIconSize(QSize(100, 80));

        startDownloadLabel->setText("停止下载");
    }
}

/*********************************************************************
* @brief  : 显示下载消息槽函数
*---------------------------------------------------------------------
* @param  : QString sMsg  ,   消息
*---------------------------------------------------------------------
* @retval : none
*********************************************************************/
void MainWindow::slotShowDowndloadMsg(QString sMsg)
{
    if(sMsg == "\r\n") //若收到空数据则清空结果栏
    {
        ui->textBrowser_ResultDisplay->clear();
    }
    else
    {
        ui->textBrowser_ResultDisplay->insertPlainText(sMsg);
    }
}

/*********************************************************************
* @brief  : 显示下载进度槽函数
*---------------------------------------------------------------------
* @param  : int processValue  ,   下载进度
*---------------------------------------------------------------------
* @retval : none
*********************************************************************/
void MainWindow::slotShowDowndloadProcess(int processValue)
{
    ui->progressBar->setValue(processValue);
}

/*********************************************************************
* @brief  : 下载结束消息槽函数
*---------------------------------------------------------------------
* @param  : QString sMsg  ,   消息
*---------------------------------------------------------------------
* @retval : none
*********************************************************************/
void MainWindow::slotShowDowndloadEnd(QString sMsg)
{
    _fileDownloadStatus = false;
    setStartDownloadButtonLabel(false);
}

/*********************************************************************
* @brief  : 波特率列表项初始化
*********************************************************************/
void MainWindow::baudRateItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"1200"<<"2400"<<"4800"<<"9600"
       <<"19200"<<"38400"<<"57600"
      <<"115200"<<"230400"<<"460800"
     <<"921600";
    for (int i = 0; i < 11; i++)
    {
        ui->comboBox_baudRate->addItem(list[i]);
    }
    ui->comboBox_baudRate->setCurrentIndex(3);
}

/*********************************************************************
* @brief  : 传输模式列表项初始化
*********************************************************************/
void MainWindow::transmitionModeItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"Ymodem"<<"Ymodem-1K";
    for (int i = 0; i < 2; i++)
    {
        ui->comboBox_transmitionMode->addItem(list[i]);
    }
    ui->comboBox_transmitionMode->setCurrentIndex(0);
}

/*********************************************************************
* @brief  : 通过当前串口信息来获取串口号
*---------------------------------------------------------------------
* @param  : QString currentPortInfo  ,  当前串口信息
*---------------------------------------------------------------------
* @retval : QString com              ,  串口号
*********************************************************************/
QString MainWindow::getSerailPortNum(QString currentPortInfo)
{
    QString com = currentPortInfo;
    QRegExp rx("COM\\d+");
    qDebug() << "comInfo: " << com;
    int comIndex = rx.indexIn(com);
    if(comIndex >= 0)
    {
        com.remove(0, comIndex);
        int bracketIndex = com.indexOf(')');
        if(bracketIndex >= 0)
        {
            com.remove(bracketIndex, 1);
            qDebug() << "com: " << com;
        }
    }
    return com;
}

/*********************************************************************
* @brief  : 扫描可用串口
*********************************************************************/
void MainWindow::scanSerialPort()
{
    QString portInfo = "";
    int maxLength = 0;

    ui->comboBox_serialPort->clear();

    //查找可用串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        portInfo = info.description() + " (" + info.portName() + ")";
        ui->comboBox_serialPort->addItem(portInfo);
        if(portInfo.length() > maxLength)
        {
            maxLength = portInfo.length();
        }
    }
    QFont font;
    font.setFamily("微软雅黑");
    font.setPointSize(10);//文字大小
    ui->comboBox_serialPort->setFont(font);

    //调整下拉框宽度以适应显示最长内容
    ui->comboBox_serialPort->view()->setFixedWidth(8*maxLength);
}

/*********************************************************************
* @brief  : 扫描可用串口按钮槽函数
*********************************************************************/
void MainWindow::scanSerialPortPushButtonClicked()
{
    scanSerialPort();
}

/*********************************************************************
* @brief  : 下载进度初始化
*********************************************************************/
void MainWindow::downloadProgressInit()
{
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
}

/*********************************************************************
* @brief  : 打开升级文件按钮槽函数
*********************************************************************/
void MainWindow::openFilePushButtonClicked()
{
    QString temp;

    bool ret = file->GetBinFile();
    if(ret == true)
    {
        _loadUpdateFileStatus = true;

        ui->textBrowser->clear();
        temp = "文件路径： " + file->_filePath;
        ui->textBrowser->insertPlainText(temp + "\r\n");
        temp = "文件长度： " + QString::number(file->_fileLength);
        ui->textBrowser->insertPlainText(temp + "\r\n");
        temp = "文件创建日期： " + file->_fileCreateDate;
        ui->textBrowser->insertPlainText(temp + "\r\n");
        temp = "文件最后修改时间： " + file->_fileLastModifyTime;
        ui->textBrowser->insertPlainText(temp + "\r\n");
    }
    else
    {
        _loadUpdateFileStatus = false;

        ui->textBrowser->clear();
        temp = "添加BIN文件出错！";
        ui->textBrowser->insertPlainText(temp + "\r\n");
    }
}

/*********************************************************************
* @brief  : 开始文件下载按钮槽函数
*********************************************************************/
void MainWindow::startDownloadPushButtonClicked()
{
    //确认升级文件加载合法性
    if (_loadUpdateFileStatus == false)
    {
        QMessageBox::about(NULL, "错误", "请先加载升级文件！");
        return;
    }

    //若正在下载中，则终止下载
    if(_fileDownloadStatus == true)
    {
        qDebug("Cancel Stop\r\n");

        /* 停止耗时工作，跳出耗时工作的循环 */
        download->stopWork();
        _fileDownloadStatus = false;
        setStartDownloadButtonLabel(false);

        ui->textBrowser_ResultDisplay->insertPlainText("下载终止");
    }
    else //开始新的下载
    {
        downloadProgressInit();
        ui->textBrowser_ResultDisplay->clear();
        _fileDownloadStatus = true;
        setStartDownloadButtonLabel(true);

        emit this->startWork(getSerailPortNum(ui->comboBox_serialPort->currentText()), ui->comboBox_baudRate->currentText().toInt(), file->_fileLength, _yModemPackageSize);
    }
}

/*********************************************************************
* @brief  : 下载线程初始化
*********************************************************************/
void MainWindow::downloadThreadInit()
{
    /* 类实例化 */
    download = new Download;

    /* 将worker类移至线程workerThread */
    download->moveToThread(&download->downloadThread);
    download->downloadThread.moveToThread(&download->downloadThread);

    /* 线程完成销毁对象 */
    connect(&download->downloadThread, SIGNAL(finished()), download, SLOT(deleteLater()));
    connect(&download->downloadThread, SIGNAL(finished()), &download->downloadThread, SLOT(deleteLater()));

    /* 发送开始工作的信号，开始工作 */
    connect(this, SIGNAL(startWork(QString,int,int,int)), download, SLOT(doWork1(QString,int,int,int)));
    connect(download, SIGNAL(signalSendDownloadMsg(QString)), this, SLOT(slotShowDowndloadMsg(QString)));
    connect(download, SIGNAL(signalSendDownloadProcess(int)), this, SLOT(slotShowDowndloadProcess(int)));
    connect(download, SIGNAL(signalSendDownloadEnd(QString)), this, SLOT(slotShowDowndloadEnd(QString)));

    downloadStart();
}

/*********************************************************************
* @brief  : 下载线程启动
*********************************************************************/
void MainWindow::downloadStart()
{
    /* 判断线程是否在运行 */
    if(!download->downloadThread.isRunning())
    {
        /* 开启线程 */
        download->downloadThread.start();
    }
}

/*********************************************************************
* @brief  : 下载线程结束
*********************************************************************/
void MainWindow::downloadStop()
{
    /* 如果线程在运行 */
    if(download->downloadThread.isRunning())
    {
        /* 停止耗时工作，跳出耗时工作的循环 */
        download->stopWork();
    }
}

/*********************************************************************
* @brief  : 传输模式当前索引变化槽函数
*---------------------------------------------------------------------
* @param  : QString currentPortInfo  ,  当前串口信息
*---------------------------------------------------------------------
* @retval : QString com              ,  串口号
*********************************************************************/
void MainWindow::on_comboBox_transmitionMode_currentIndexChanged(int index)
{
    if(index == 0) //"Ymodem"
    {
        _yModemPackageSize = 128;
    }
    else if(index == 1) //"Ymodem-1K"
    {
        _yModemPackageSize = 1024;
    }
}
