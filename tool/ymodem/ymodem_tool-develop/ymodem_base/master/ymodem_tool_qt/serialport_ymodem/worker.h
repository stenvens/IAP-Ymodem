#ifndef WORKER_H
#define WORKER_H

#include <QDebug>

#include <QThread>
#include <QMutexLocker>
#include <QMutex>

/* Worker类，这个类声明了doWork1函数，将整个Worker类移至线程workerThread */
class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QWidget *parent = 0);
    ~Worker();

    /* 全局线程 */
    QThread workerThread;

private:
    /* 互斥锁 */
    QMutex lock;

    /* 标志位 */
    bool isCanRun;

public slots:
    void doWork1(const QString &parameter);

public:
    /* 打断线程（注意此方法不能放在槽函数下） */
    void stopWork();

signals:
    /* 工人工作函数状态的信号 */
    void resultReady(const QString &result);
};

#endif // WORKER_H
