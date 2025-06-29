/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_Left;
    QGroupBox *groupBoxSerialConfig;
    QGridLayout *gridLayout;
    QLabel *labelPort;
    QComboBox *comboBoxPort;
    QPushButton *pushButtonRefreshPorts;
    QLabel *labelBaudRate;
    QComboBox *comboBoxBaudRate;
    QLabel *labelDataBits;
    QComboBox *comboBoxDataBits;
    QLabel *labelStopBits;
    QComboBox *comboBoxStopBits;
    QLabel *labelParity;
    QComboBox *comboBoxParity;
    QHBoxLayout *horizontalLayoutConnect;
    QPushButton *pushButtonConnect;
    QPushButton *pushButtonDisconnect;
    QGroupBox *groupBoxSystemConfig;
    QGridLayout *gridLayout_2;
    QLabel *labelThreshold1;
    QSpinBox *spinBoxThreshold1;
    QLabel *labelThreshold2;
    QSpinBox *spinBoxThreshold2;
    QLabel *labelLightPreset;
    QSpinBox *spinBoxLightPreset;
    QLabel *labelMediumPreset;
    QSpinBox *spinBoxMediumPreset;
    QLabel *labelHeavyPreset;
    QSpinBox *spinBoxHeavyPreset;
    QLabel *labelPower;
    QHBoxLayout *horizontalLayoutPower;
    QPushButton *pushButtonPowerOn;
    QPushButton *pushButtonPowerOff;
    QPushButton *pushButtonSendConfig;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_Right;
    QGroupBox *groupBoxSystemStatus;
    QGridLayout *gridLayout_3;
    QLabel *labelStatusThreshold1;
    QLCDNumber *lcdNumberThreshold1;
    QLabel *labelStatusThreshold2;
    QLCDNumber *lcdNumberThreshold2;
    QLabel *labelTotalCount;
    QLCDNumber *lcdNumberTotalCount;
    QLabel *labelLastWeight;
    QLCDNumber *lcdNumberLastWeight;
    QLabel *labelSystemStatusTitle;
    QLabel *labelSystemStatus;
    QLabel *labelFullStatusTitle;
    QLabel *labelFullStatus;
    QGroupBox *groupBoxBins;
    QGridLayout *gridLayout_4;
    QLabel *labelLightBin;
    QLabel *labelMediumBin;
    QLabel *labelHeavyBin;
    QLabel *labelLightPresetStatus;
    QLCDNumber *lcdNumberLightPreset;
    QLabel *labelMediumPresetStatus;
    QLCDNumber *lcdNumberMediumPreset;
    QLabel *labelHeavyPresetStatus;
    QLCDNumber *lcdNumberHeavyPreset;
    QLabel *labelLightCurrentStatus;
    QLCDNumber *lcdNumberLightCurrent;
    QLabel *labelMediumCurrentStatus;
    QLCDNumber *lcdNumberMediumCurrent;
    QLabel *labelHeavyCurrentStatus;
    QLCDNumber *lcdNumberHeavyCurrent;
    QProgressBar *progressBarLight;
    QProgressBar *progressBarMedium;
    QProgressBar *progressBarHeavy;
    QGroupBox *groupBoxLog;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *textEditLog;
    QPushButton *pushButtonClearLog;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(816, 626);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout_Left = new QVBoxLayout();
        verticalLayout_Left->setObjectName("verticalLayout_Left");
        groupBoxSerialConfig = new QGroupBox(centralwidget);
        groupBoxSerialConfig->setObjectName("groupBoxSerialConfig");
        gridLayout = new QGridLayout(groupBoxSerialConfig);
        gridLayout->setObjectName("gridLayout");
        labelPort = new QLabel(groupBoxSerialConfig);
        labelPort->setObjectName("labelPort");

        gridLayout->addWidget(labelPort, 0, 0, 1, 1);

        comboBoxPort = new QComboBox(groupBoxSerialConfig);
        comboBoxPort->setObjectName("comboBoxPort");

        gridLayout->addWidget(comboBoxPort, 0, 1, 1, 1);

        pushButtonRefreshPorts = new QPushButton(groupBoxSerialConfig);
        pushButtonRefreshPorts->setObjectName("pushButtonRefreshPorts");

        gridLayout->addWidget(pushButtonRefreshPorts, 0, 2, 1, 1);

        labelBaudRate = new QLabel(groupBoxSerialConfig);
        labelBaudRate->setObjectName("labelBaudRate");

        gridLayout->addWidget(labelBaudRate, 1, 0, 1, 1);

        comboBoxBaudRate = new QComboBox(groupBoxSerialConfig);
        comboBoxBaudRate->setObjectName("comboBoxBaudRate");

        gridLayout->addWidget(comboBoxBaudRate, 1, 1, 1, 1);

        labelDataBits = new QLabel(groupBoxSerialConfig);
        labelDataBits->setObjectName("labelDataBits");

        gridLayout->addWidget(labelDataBits, 2, 0, 1, 1);

        comboBoxDataBits = new QComboBox(groupBoxSerialConfig);
        comboBoxDataBits->setObjectName("comboBoxDataBits");

        gridLayout->addWidget(comboBoxDataBits, 2, 1, 1, 1);

        labelStopBits = new QLabel(groupBoxSerialConfig);
        labelStopBits->setObjectName("labelStopBits");

        gridLayout->addWidget(labelStopBits, 3, 0, 1, 1);

        comboBoxStopBits = new QComboBox(groupBoxSerialConfig);
        comboBoxStopBits->setObjectName("comboBoxStopBits");

        gridLayout->addWidget(comboBoxStopBits, 3, 1, 1, 1);

        labelParity = new QLabel(groupBoxSerialConfig);
        labelParity->setObjectName("labelParity");

        gridLayout->addWidget(labelParity, 4, 0, 1, 1);

        comboBoxParity = new QComboBox(groupBoxSerialConfig);
        comboBoxParity->setObjectName("comboBoxParity");

        gridLayout->addWidget(comboBoxParity, 4, 1, 1, 1);

        horizontalLayoutConnect = new QHBoxLayout();
        horizontalLayoutConnect->setObjectName("horizontalLayoutConnect");
        pushButtonConnect = new QPushButton(groupBoxSerialConfig);
        pushButtonConnect->setObjectName("pushButtonConnect");

        horizontalLayoutConnect->addWidget(pushButtonConnect);

        pushButtonDisconnect = new QPushButton(groupBoxSerialConfig);
        pushButtonDisconnect->setObjectName("pushButtonDisconnect");
        pushButtonDisconnect->setEnabled(false);

        horizontalLayoutConnect->addWidget(pushButtonDisconnect);


        gridLayout->addLayout(horizontalLayoutConnect, 5, 0, 1, 3);


        verticalLayout_Left->addWidget(groupBoxSerialConfig);

        groupBoxSystemConfig = new QGroupBox(centralwidget);
        groupBoxSystemConfig->setObjectName("groupBoxSystemConfig");
        groupBoxSystemConfig->setEnabled(true);
        gridLayout_2 = new QGridLayout(groupBoxSystemConfig);
        gridLayout_2->setObjectName("gridLayout_2");
        labelThreshold1 = new QLabel(groupBoxSystemConfig);
        labelThreshold1->setObjectName("labelThreshold1");

        gridLayout_2->addWidget(labelThreshold1, 0, 0, 1, 1);

        spinBoxThreshold1 = new QSpinBox(groupBoxSystemConfig);
        spinBoxThreshold1->setObjectName("spinBoxThreshold1");
        spinBoxThreshold1->setMaximum(9999);
        spinBoxThreshold1->setValue(100);

        gridLayout_2->addWidget(spinBoxThreshold1, 0, 1, 1, 1);

        labelThreshold2 = new QLabel(groupBoxSystemConfig);
        labelThreshold2->setObjectName("labelThreshold2");

        gridLayout_2->addWidget(labelThreshold2, 1, 0, 1, 1);

        spinBoxThreshold2 = new QSpinBox(groupBoxSystemConfig);
        spinBoxThreshold2->setObjectName("spinBoxThreshold2");
        spinBoxThreshold2->setMaximum(9999);
        spinBoxThreshold2->setValue(500);

        gridLayout_2->addWidget(spinBoxThreshold2, 1, 1, 1, 1);

        labelLightPreset = new QLabel(groupBoxSystemConfig);
        labelLightPreset->setObjectName("labelLightPreset");

        gridLayout_2->addWidget(labelLightPreset, 2, 0, 1, 1);

        spinBoxLightPreset = new QSpinBox(groupBoxSystemConfig);
        spinBoxLightPreset->setObjectName("spinBoxLightPreset");
        spinBoxLightPreset->setMaximum(9999);
        spinBoxLightPreset->setValue(1000);

        gridLayout_2->addWidget(spinBoxLightPreset, 2, 1, 1, 1);

        labelMediumPreset = new QLabel(groupBoxSystemConfig);
        labelMediumPreset->setObjectName("labelMediumPreset");

        gridLayout_2->addWidget(labelMediumPreset, 3, 0, 1, 1);

        spinBoxMediumPreset = new QSpinBox(groupBoxSystemConfig);
        spinBoxMediumPreset->setObjectName("spinBoxMediumPreset");
        spinBoxMediumPreset->setMaximum(9999);
        spinBoxMediumPreset->setValue(2000);

        gridLayout_2->addWidget(spinBoxMediumPreset, 3, 1, 1, 1);

        labelHeavyPreset = new QLabel(groupBoxSystemConfig);
        labelHeavyPreset->setObjectName("labelHeavyPreset");

        gridLayout_2->addWidget(labelHeavyPreset, 4, 0, 1, 1);

        spinBoxHeavyPreset = new QSpinBox(groupBoxSystemConfig);
        spinBoxHeavyPreset->setObjectName("spinBoxHeavyPreset");
        spinBoxHeavyPreset->setMaximum(9999);
        spinBoxHeavyPreset->setValue(3000);

        gridLayout_2->addWidget(spinBoxHeavyPreset, 4, 1, 1, 1);

        labelPower = new QLabel(groupBoxSystemConfig);
        labelPower->setObjectName("labelPower");

        gridLayout_2->addWidget(labelPower, 5, 0, 1, 1);

        horizontalLayoutPower = new QHBoxLayout();
        horizontalLayoutPower->setObjectName("horizontalLayoutPower");
        pushButtonPowerOn = new QPushButton(groupBoxSystemConfig);
        pushButtonPowerOn->setObjectName("pushButtonPowerOn");

        horizontalLayoutPower->addWidget(pushButtonPowerOn);

        pushButtonPowerOff = new QPushButton(groupBoxSystemConfig);
        pushButtonPowerOff->setObjectName("pushButtonPowerOff");

        horizontalLayoutPower->addWidget(pushButtonPowerOff);


        gridLayout_2->addLayout(horizontalLayoutPower, 5, 1, 1, 1);

        pushButtonSendConfig = new QPushButton(groupBoxSystemConfig);
        pushButtonSendConfig->setObjectName("pushButtonSendConfig");

        gridLayout_2->addWidget(pushButtonSendConfig, 6, 0, 1, 2);


        verticalLayout_Left->addWidget(groupBoxSystemConfig);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_Left->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_Left);

        verticalLayout_Right = new QVBoxLayout();
        verticalLayout_Right->setObjectName("verticalLayout_Right");
        groupBoxSystemStatus = new QGroupBox(centralwidget);
        groupBoxSystemStatus->setObjectName("groupBoxSystemStatus");
        gridLayout_3 = new QGridLayout(groupBoxSystemStatus);
        gridLayout_3->setObjectName("gridLayout_3");
        labelStatusThreshold1 = new QLabel(groupBoxSystemStatus);
        labelStatusThreshold1->setObjectName("labelStatusThreshold1");

        gridLayout_3->addWidget(labelStatusThreshold1, 0, 0, 1, 1);

        lcdNumberThreshold1 = new QLCDNumber(groupBoxSystemStatus);
        lcdNumberThreshold1->setObjectName("lcdNumberThreshold1");
        lcdNumberThreshold1->setDigitCount(4);

        gridLayout_3->addWidget(lcdNumberThreshold1, 0, 1, 1, 1);

        labelStatusThreshold2 = new QLabel(groupBoxSystemStatus);
        labelStatusThreshold2->setObjectName("labelStatusThreshold2");

        gridLayout_3->addWidget(labelStatusThreshold2, 0, 2, 1, 1);

        lcdNumberThreshold2 = new QLCDNumber(groupBoxSystemStatus);
        lcdNumberThreshold2->setObjectName("lcdNumberThreshold2");
        lcdNumberThreshold2->setDigitCount(4);

        gridLayout_3->addWidget(lcdNumberThreshold2, 0, 3, 1, 1);

        labelTotalCount = new QLabel(groupBoxSystemStatus);
        labelTotalCount->setObjectName("labelTotalCount");

        gridLayout_3->addWidget(labelTotalCount, 1, 0, 1, 1);

        lcdNumberTotalCount = new QLCDNumber(groupBoxSystemStatus);
        lcdNumberTotalCount->setObjectName("lcdNumberTotalCount");
        lcdNumberTotalCount->setDigitCount(5);

        gridLayout_3->addWidget(lcdNumberTotalCount, 1, 1, 1, 1);

        labelLastWeight = new QLabel(groupBoxSystemStatus);
        labelLastWeight->setObjectName("labelLastWeight");

        gridLayout_3->addWidget(labelLastWeight, 1, 2, 1, 1);

        lcdNumberLastWeight = new QLCDNumber(groupBoxSystemStatus);
        lcdNumberLastWeight->setObjectName("lcdNumberLastWeight");
        lcdNumberLastWeight->setDigitCount(4);

        gridLayout_3->addWidget(lcdNumberLastWeight, 1, 3, 1, 1);

        labelSystemStatusTitle = new QLabel(groupBoxSystemStatus);
        labelSystemStatusTitle->setObjectName("labelSystemStatusTitle");

        gridLayout_3->addWidget(labelSystemStatusTitle, 2, 0, 1, 1);

        labelSystemStatus = new QLabel(groupBoxSystemStatus);
        labelSystemStatus->setObjectName("labelSystemStatus");

        gridLayout_3->addWidget(labelSystemStatus, 2, 1, 1, 1);

        labelFullStatusTitle = new QLabel(groupBoxSystemStatus);
        labelFullStatusTitle->setObjectName("labelFullStatusTitle");

        gridLayout_3->addWidget(labelFullStatusTitle, 2, 2, 1, 1);

        labelFullStatus = new QLabel(groupBoxSystemStatus);
        labelFullStatus->setObjectName("labelFullStatus");

        gridLayout_3->addWidget(labelFullStatus, 2, 3, 1, 1);


        verticalLayout_Right->addWidget(groupBoxSystemStatus);

        groupBoxBins = new QGroupBox(centralwidget);
        groupBoxBins->setObjectName("groupBoxBins");
        gridLayout_4 = new QGridLayout(groupBoxBins);
        gridLayout_4->setObjectName("gridLayout_4");
        labelLightBin = new QLabel(groupBoxBins);
        labelLightBin->setObjectName("labelLightBin");
        labelLightBin->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_4->addWidget(labelLightBin, 0, 0, 1, 2);

        labelMediumBin = new QLabel(groupBoxBins);
        labelMediumBin->setObjectName("labelMediumBin");
        labelMediumBin->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_4->addWidget(labelMediumBin, 0, 2, 1, 2);

        labelHeavyBin = new QLabel(groupBoxBins);
        labelHeavyBin->setObjectName("labelHeavyBin");
        labelHeavyBin->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_4->addWidget(labelHeavyBin, 0, 4, 1, 2);

        labelLightPresetStatus = new QLabel(groupBoxBins);
        labelLightPresetStatus->setObjectName("labelLightPresetStatus");

        gridLayout_4->addWidget(labelLightPresetStatus, 1, 0, 1, 1);

        lcdNumberLightPreset = new QLCDNumber(groupBoxBins);
        lcdNumberLightPreset->setObjectName("lcdNumberLightPreset");
        lcdNumberLightPreset->setDigitCount(4);

        gridLayout_4->addWidget(lcdNumberLightPreset, 1, 1, 1, 1);

        labelMediumPresetStatus = new QLabel(groupBoxBins);
        labelMediumPresetStatus->setObjectName("labelMediumPresetStatus");

        gridLayout_4->addWidget(labelMediumPresetStatus, 1, 2, 1, 1);

        lcdNumberMediumPreset = new QLCDNumber(groupBoxBins);
        lcdNumberMediumPreset->setObjectName("lcdNumberMediumPreset");
        lcdNumberMediumPreset->setDigitCount(4);

        gridLayout_4->addWidget(lcdNumberMediumPreset, 1, 3, 1, 1);

        labelHeavyPresetStatus = new QLabel(groupBoxBins);
        labelHeavyPresetStatus->setObjectName("labelHeavyPresetStatus");

        gridLayout_4->addWidget(labelHeavyPresetStatus, 1, 4, 1, 1);

        lcdNumberHeavyPreset = new QLCDNumber(groupBoxBins);
        lcdNumberHeavyPreset->setObjectName("lcdNumberHeavyPreset");
        lcdNumberHeavyPreset->setDigitCount(4);

        gridLayout_4->addWidget(lcdNumberHeavyPreset, 1, 5, 1, 1);

        labelLightCurrentStatus = new QLabel(groupBoxBins);
        labelLightCurrentStatus->setObjectName("labelLightCurrentStatus");

        gridLayout_4->addWidget(labelLightCurrentStatus, 2, 0, 1, 1);

        lcdNumberLightCurrent = new QLCDNumber(groupBoxBins);
        lcdNumberLightCurrent->setObjectName("lcdNumberLightCurrent");
        lcdNumberLightCurrent->setDigitCount(4);

        gridLayout_4->addWidget(lcdNumberLightCurrent, 2, 1, 1, 1);

        labelMediumCurrentStatus = new QLabel(groupBoxBins);
        labelMediumCurrentStatus->setObjectName("labelMediumCurrentStatus");

        gridLayout_4->addWidget(labelMediumCurrentStatus, 2, 2, 1, 1);

        lcdNumberMediumCurrent = new QLCDNumber(groupBoxBins);
        lcdNumberMediumCurrent->setObjectName("lcdNumberMediumCurrent");
        lcdNumberMediumCurrent->setDigitCount(4);

        gridLayout_4->addWidget(lcdNumberMediumCurrent, 2, 3, 1, 1);

        labelHeavyCurrentStatus = new QLabel(groupBoxBins);
        labelHeavyCurrentStatus->setObjectName("labelHeavyCurrentStatus");

        gridLayout_4->addWidget(labelHeavyCurrentStatus, 2, 4, 1, 1);

        lcdNumberHeavyCurrent = new QLCDNumber(groupBoxBins);
        lcdNumberHeavyCurrent->setObjectName("lcdNumberHeavyCurrent");
        lcdNumberHeavyCurrent->setDigitCount(4);

        gridLayout_4->addWidget(lcdNumberHeavyCurrent, 2, 5, 1, 1);

        progressBarLight = new QProgressBar(groupBoxBins);
        progressBarLight->setObjectName("progressBarLight");
        progressBarLight->setValue(0);

        gridLayout_4->addWidget(progressBarLight, 3, 0, 1, 2);

        progressBarMedium = new QProgressBar(groupBoxBins);
        progressBarMedium->setObjectName("progressBarMedium");
        progressBarMedium->setValue(0);

        gridLayout_4->addWidget(progressBarMedium, 3, 2, 1, 2);

        progressBarHeavy = new QProgressBar(groupBoxBins);
        progressBarHeavy->setObjectName("progressBarHeavy");
        progressBarHeavy->setValue(0);

        gridLayout_4->addWidget(progressBarHeavy, 3, 4, 1, 2);


        verticalLayout_Right->addWidget(groupBoxBins);

        groupBoxLog = new QGroupBox(centralwidget);
        groupBoxLog->setObjectName("groupBoxLog");
        verticalLayout_2 = new QVBoxLayout(groupBoxLog);
        verticalLayout_2->setObjectName("verticalLayout_2");
        textEditLog = new QTextEdit(groupBoxLog);
        textEditLog->setObjectName("textEditLog");
        textEditLog->setReadOnly(true);

        verticalLayout_2->addWidget(textEditLog);

        pushButtonClearLog = new QPushButton(groupBoxLog);
        pushButtonClearLog->setObjectName("pushButtonClearLog");

        verticalLayout_2->addWidget(pushButtonClearLog);


        verticalLayout_Right->addWidget(groupBoxLog);


        horizontalLayout->addLayout(verticalLayout_Right);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 816, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "H\341\273\207 th\341\273\221ng ph\303\242n lo\341\272\241i kh\341\273\221i l\306\260\341\273\243ng", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        groupBoxSerialConfig->setTitle(QCoreApplication::translate("MainWindow", "C\341\272\245u h\303\254nh k\341\272\277t n\341\273\221i UART", nullptr));
        labelPort->setText(QCoreApplication::translate("MainWindow", "COM Port:", nullptr));
        pushButtonRefreshPorts->setText(QCoreApplication::translate("MainWindow", "Refresh", nullptr));
        labelBaudRate->setText(QCoreApplication::translate("MainWindow", "Baud Rate:", nullptr));
        labelDataBits->setText(QCoreApplication::translate("MainWindow", "Data Bits:", nullptr));
        labelStopBits->setText(QCoreApplication::translate("MainWindow", "Stop Bits:", nullptr));
        labelParity->setText(QCoreApplication::translate("MainWindow", "Parity:", nullptr));
        pushButtonConnect->setText(QCoreApplication::translate("MainWindow", "K\341\272\277t n\341\273\221i", nullptr));
        pushButtonDisconnect->setText(QCoreApplication::translate("MainWindow", "Ng\341\272\257t k\341\272\277t n\341\273\221i", nullptr));
        groupBoxSystemConfig->setTitle(QCoreApplication::translate("MainWindow", "C\341\272\245u h\303\254nh h\341\273\207 th\341\273\221ng", nullptr));
        labelThreshold1->setText(QCoreApplication::translate("MainWindow", "Threshold 1:", nullptr));
        labelThreshold2->setText(QCoreApplication::translate("MainWindow", "Threshold 2:", nullptr));
        labelLightPreset->setText(QCoreApplication::translate("MainWindow", "Light Preset:", nullptr));
        labelMediumPreset->setText(QCoreApplication::translate("MainWindow", "Medium Preset:", nullptr));
        labelHeavyPreset->setText(QCoreApplication::translate("MainWindow", "Heavy Preset:", nullptr));
        labelPower->setText(QCoreApplication::translate("MainWindow", "Power:", nullptr));
        pushButtonPowerOn->setText(QCoreApplication::translate("MainWindow", "B\341\272\255t ngu\341\273\223n", nullptr));
        pushButtonPowerOff->setText(QCoreApplication::translate("MainWindow", "T\341\272\257t ngu\341\273\223n", nullptr));
        pushButtonSendConfig->setText(QCoreApplication::translate("MainWindow", "G\341\273\255i c\341\272\245u h\303\254nh", nullptr));
        groupBoxSystemStatus->setTitle(QCoreApplication::translate("MainWindow", "Tr\341\272\241ng th\303\241i h\341\273\207 th\341\273\221ng", nullptr));
        labelStatusThreshold1->setText(QCoreApplication::translate("MainWindow", "Threshold 1:", nullptr));
        labelStatusThreshold2->setText(QCoreApplication::translate("MainWindow", "Threshold 2:", nullptr));
        labelTotalCount->setText(QCoreApplication::translate("MainWindow", "T\341\273\225ng s\341\273\221 l\306\260\341\273\243ng:", nullptr));
        labelLastWeight->setText(QCoreApplication::translate("MainWindow", "Kh\341\273\221i l\306\260\341\273\243ng g\341\272\247n nh\341\272\245t:", nullptr));
        labelSystemStatusTitle->setText(QCoreApplication::translate("MainWindow", "Tr\341\272\241ng th\303\241i:", nullptr));
        labelSystemStatus->setText(QCoreApplication::translate("MainWindow", "Stopped", nullptr));
        labelFullStatusTitle->setText(QCoreApplication::translate("MainWindow", "Th\303\271ng \304\221\341\272\247y:", nullptr));
        labelFullStatus->setText(QCoreApplication::translate("MainWindow", "NO", nullptr));
        groupBoxBins->setTitle(QCoreApplication::translate("MainWindow", "T\303\254nh tr\341\272\241ng c\303\241c th\303\271ng", nullptr));
        labelLightBin->setText(QCoreApplication::translate("MainWindow", "Light Bin", nullptr));
        labelMediumBin->setText(QCoreApplication::translate("MainWindow", "Medium Bin", nullptr));
        labelHeavyBin->setText(QCoreApplication::translate("MainWindow", "Heavy Bin", nullptr));
        labelLightPresetStatus->setText(QCoreApplication::translate("MainWindow", "Preset:", nullptr));
        labelMediumPresetStatus->setText(QCoreApplication::translate("MainWindow", "Preset:", nullptr));
        labelHeavyPresetStatus->setText(QCoreApplication::translate("MainWindow", "Preset:", nullptr));
        labelLightCurrentStatus->setText(QCoreApplication::translate("MainWindow", "Current:", nullptr));
        labelMediumCurrentStatus->setText(QCoreApplication::translate("MainWindow", "Current:", nullptr));
        labelHeavyCurrentStatus->setText(QCoreApplication::translate("MainWindow", "Current:", nullptr));
        groupBoxLog->setTitle(QCoreApplication::translate("MainWindow", "Log UART", nullptr));
        pushButtonClearLog->setText(QCoreApplication::translate("MainWindow", "X\303\263a log", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
