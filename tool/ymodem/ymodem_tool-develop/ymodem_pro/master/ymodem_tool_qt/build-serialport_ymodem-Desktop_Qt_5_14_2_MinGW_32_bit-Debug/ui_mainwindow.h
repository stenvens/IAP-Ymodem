/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGroupBox *groupBox;
    QComboBox *comboBox_baudRate;
    QLabel *label;
    QLabel *label_2;
    QComboBox *comboBox_serialPort;
    QPushButton *pushButton_scanSerialPort;
    QLabel *label_3;
    QComboBox *comboBox_transmitionMode;
    QGroupBox *groupBox_2;
    QTextBrowser *textBrowser;
    QPushButton *pushButton_openFile;
    QGroupBox *groupBox_3;
    QTextBrowser *textBrowser_ResultDisplay;
    QPushButton *pushButton_startDownload;
    QProgressBar *progressBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(563, 560);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, 10, 551, 145));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        groupBox->setFont(font);
        groupBox->setFlat(false);
        comboBox_baudRate = new QComboBox(groupBox);
        comboBox_baudRate->setObjectName(QString::fromUtf8("comboBox_baudRate"));
        comboBox_baudRate->setGeometry(QRect(120, 100, 161, 25));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(18, 25, 91, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(12);
        label->setFont(font1);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(18, 105, 91, 20));
        QFont font2;
        font2.setPointSize(12);
        label_2->setFont(font2);
        comboBox_serialPort = new QComboBox(groupBox);
        comboBox_serialPort->setObjectName(QString::fromUtf8("comboBox_serialPort"));
        comboBox_serialPort->setGeometry(QRect(120, 20, 161, 25));
        pushButton_scanSerialPort = new QPushButton(groupBox);
        pushButton_scanSerialPort->setObjectName(QString::fromUtf8("pushButton_scanSerialPort"));
        pushButton_scanSerialPort->setGeometry(QRect(420, 60, 31, 31));
        QFont font3;
        font3.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font3.setPointSize(14);
        pushButton_scanSerialPort->setFont(font3);
        pushButton_scanSerialPort->setStyleSheet(QString::fromUtf8("border-image: url(:/images/refresh_72px.png);"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(18, 65, 91, 20));
        label_3->setFont(font2);
        comboBox_transmitionMode = new QComboBox(groupBox);
        comboBox_transmitionMode->setObjectName(QString::fromUtf8("comboBox_transmitionMode"));
        comboBox_transmitionMode->setGeometry(QRect(120, 60, 161, 25));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 165, 551, 185));
        QFont font4;
        font4.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font4.setPointSize(10);
        groupBox_2->setFont(font4);
        textBrowser = new QTextBrowser(groupBox_2);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(15, 30, 411, 141));
        pushButton_openFile = new QPushButton(groupBox_2);
        pushButton_openFile->setObjectName(QString::fromUtf8("pushButton_openFile"));
        pushButton_openFile->setGeometry(QRect(430, 30, 101, 141));
        QFont font5;
        font5.setPointSize(14);
        pushButton_openFile->setFont(font5);
        pushButton_openFile->setFlat(true);
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 354, 551, 181));
        groupBox_3->setFont(font);
        textBrowser_ResultDisplay = new QTextBrowser(groupBox_3);
        textBrowser_ResultDisplay->setObjectName(QString::fromUtf8("textBrowser_ResultDisplay"));
        textBrowser_ResultDisplay->setGeometry(QRect(10, 70, 421, 111));
        pushButton_startDownload = new QPushButton(groupBox_3);
        pushButton_startDownload->setObjectName(QString::fromUtf8("pushButton_startDownload"));
        pushButton_startDownload->setGeometry(QRect(440, 70, 101, 101));
        pushButton_startDownload->setStyleSheet(QString::fromUtf8(""));
        pushButton_startDownload->setIconSize(QSize(98, 90));
        pushButton_startDownload->setAutoRepeat(false);
        pushButton_startDownload->setFlat(true);
        progressBar = new QProgressBar(groupBox_3);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(10, 30, 531, 35));
        progressBar->setLayoutDirection(Qt::LeftToRight);
        progressBar->setAutoFillBackground(false);
        progressBar->setValue(24);
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        pushButton_startDownload->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\345\217\202\346\225\260\350\256\276\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\344\270\262 \345\217\243 \345\217\267 :", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\263\242 \347\211\271 \347\216\207 :", nullptr));
        pushButton_scanSerialPort->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "\344\274\240\350\276\223\346\226\271\345\274\217:", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\345\215\207\347\272\247\346\226\207\344\273\266\344\277\241\346\201\257", nullptr));
        pushButton_openFile->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\345\215\207\347\272\247\350\277\233\345\272\246", nullptr));
        pushButton_startDownload->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\344\270\213\350\275\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
