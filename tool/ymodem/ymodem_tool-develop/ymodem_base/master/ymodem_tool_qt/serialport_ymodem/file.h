#ifndef FILE_H
#define FILE_H
#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QDateTime>

class File
{
private:
    QFile file;

public:
    QString _filePath = ""; //文件绝对路径
    QString _fileName = ""; //文件名+文件后缀
    int _fileLength = 0; //文件大小
    QString _fileCreateDate = ""; //文件创建日期
    QString _fileLastModifyTime = ""; //文件最后修改日期
    uint8_t *_binaryDataBuffer; //文件二进制内容
    uint32_t _fileCrc32 = 0; //文件CRC32校验值

    bool GetBinFile();
};


#endif // FILE_H
