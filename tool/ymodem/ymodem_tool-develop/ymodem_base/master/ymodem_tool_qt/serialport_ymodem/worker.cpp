#include "worker.h".h"

Worker::Worker(QWidget *parent)
{
    qDebug()<<"Worker init"<<endl;
}

Worker::~Worker()
{
}

void Worker::doWork1(const QString &parameter)
{
    /* 标志位为真 */
    isCanRun = true;

    /* 死循环 */
    while (1) {
        /* 此{}作用是QMutexLocker与lock的作用范围，获取锁后，
         * 运行完成后即解锁 */
        {
            QMutexLocker locker(&lock);
            /* 如果标志位不为真 */
            if (!isCanRun) {
                /* 跳出循环 */
                break;
            }
        }
        /* 使用QThread里的延时函数，当作一个普通延时 */
        QThread::sleep(2);

        emit resultReady(parameter + "doWork1函数");
    }
    /* doWork1运行完成，发送信号 */
    emit resultReady("打断doWork1函数");
}

void Worker::stopWork()
{
    qDebug()<<"打断线程"<<endl;

    /* 获取锁后，运行完成后即解锁 */
    QMutexLocker locker(&lock);
    isCanRun = false;
}
