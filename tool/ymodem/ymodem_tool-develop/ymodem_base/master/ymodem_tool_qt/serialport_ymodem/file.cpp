#include "file.h"

/*********************************************************************
* @brief  : 读取BIN文件到缓存
*---------------------------------------------------------------------
* @param  : none
*---------------------------------------------------------------------
* @retval : true     ,  读取成功
*           false    ,  读取失败
*********************************************************************/
bool File::GetBinFile()
{
    /* 获取文件的路径 */
    QString fileName = QFileDialog::getOpenFileName(NULL,"Open Document", "", "BIN(*.bin)");
    QFileInfo fileInfo = QFileInfo(fileName);

    /* 指向文件 */
    file.setFileName(fileName);

    /* 判断文件是否存在 */
    if (!file.exists())
        return false;

    /* 以读写的方式打开 */
    //if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) //读取非二进制文件
    if (!file.open(QIODevice::ReadOnly)) //读取二进制文件
        return false;

    /* 使用文本流读取文件 */
    QTextStream stream(&file);
    QByteArray array = file.readAll();
    _binaryDataBuffer = (uint8_t*)malloc(array.size());
    for(int i=0; i<array.size(); i++)
    {
        _binaryDataBuffer[i] = array.at(i);
    }
    _fileLength = fileInfo.size();
    _filePath = fileInfo.filePath(); //绝对路径
    _fileName = fileInfo.fileName();
    _fileCreateDate = fileInfo.created().toString("yyyy/MM/dd hh:mm:ss");
    _fileLastModifyTime = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");

    qDebug() << "_fileLength = " << _fileLength;
    qDebug() << "_fileName = " << _fileName;
    qDebug() << "_filePath = " << _filePath;
    qDebug() << "_fileCreateDate = " << _fileCreateDate;
    qDebug() << "_fileLastModifyTime = " << _fileLastModifyTime;

    /* 关闭文件 */
    file.close();

    return true;
}
