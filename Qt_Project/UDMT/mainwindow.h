#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSerialConnect();
    void onSerialDisconnect();
    void onSerialDataReceived();
    void onSerialError(QSerialPort::SerialPortError error);
    void onSendConfiguration();
    void onPowerOn();
    void onPowerOff();
    void onClearLog();
    void onRefreshPortList();
    void onTimerTimeout();

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QTimer *dataTimer;
    QByteArray incomingBuffer; // Buffer for incoming data
    QMap<QString, QLabel*> valueLabels; // Maps value names to corresponding QLabels

    void setupConnections();
    void createValueLabels();
    void fillPortsInfo();
    void processBuffer(); // Method to process the buffer
    void updateSystemStatus(const QJsonObject &data);
    void appendToLog(const QString &message);
    void sendSerialData(const QJsonObject &data);
    QString beautifyJson(const QString &jsonString);
    void setPower(int on);
};
#endif // MAINWINDOW_H
