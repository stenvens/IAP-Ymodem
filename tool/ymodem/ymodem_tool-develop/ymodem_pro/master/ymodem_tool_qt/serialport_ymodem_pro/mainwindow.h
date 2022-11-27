#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFileDialog>

#include "download.h"
#include "file.h"
#include "clicklabel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ClickedLabel *startDownloadLabel;
    ClickedLabel *openFileLabel;
    QSerialPort *serialPort;
    Download *download;

    bool _loadUpdateFileStatus = false; //加载升级文件状态 (true,加载成功 / false,未加载)
    QString _serialPortNum; //串口号
    bool _fileDownloadStatus = false; //文件下载状态
    int _yModemPackageSize = 128; //Ymodem文件包大小

    void downloadThreadInit();
    void baudRateItemInit();
    void transmitionModeItemInit();
    void scanSerialPort();
    void downloadProgressInit();
    QString getSerailPortNum(QString currentPortInfo);
    void setStartDownloadButtonLabel(bool downloadStatus);
    void initStartDownloadButtonLabel();
    void initOpenFileLabel();
    void initOpenFileButton();

private slots:
    void openFilePushButtonClicked();
    void startDownloadPushButtonClicked();
    void scanSerialPortPushButtonClicked();
    void slotShowDowndloadMsg(QString sMsg);
    void slotShowDowndloadProcess(int porcessValue);
    void slotShowDowndloadEnd(QString sMsg);
    void on_comboBox_transmitionMode_currentIndexChanged(int index);

    void on_pushButton_openFile_clicked();

private:
    void downloadStart();
    void downloadStop();

signals:
    void startWork(const QString portName, const int baudRate, int fileSize, int yModemPackageSize);
};

extern File *file;

#endif // MAINWINDOW_H
