#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); //支持4K等高分辨率屏幕
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QString("IAP升级(Ymodem_base) V0.01"));
    w.show();

    return a.exec();
}
