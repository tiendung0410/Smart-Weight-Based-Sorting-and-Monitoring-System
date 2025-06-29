#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QJsonDocument>
#include <QDateTime>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serialPort(new QSerialPort(this))
    , dataTimer(new QTimer(this))
{
    ui->setupUi(this);

    // Initialize buffer for receiving data
    incomingBuffer.clear();

    // Fill port info combobox
    fillPortsInfo();

    // Setup UI initial values
    ui->comboBoxBaudRate->addItems({"9600", "19200", "38400", "57600", "115200"});
    ui->comboBoxBaudRate->setCurrentText("9600");

    ui->comboBoxDataBits->addItems({"5", "6", "7", "8"});
    ui->comboBoxDataBits->setCurrentText("8");

    ui->comboBoxStopBits->addItems({"1", "1.5", "2"});
    ui->comboBoxStopBits->setCurrentText("1");

    ui->comboBoxParity->addItems({"None", "Even", "Odd", "Mark", "Space"});
    ui->comboBoxParity->setCurrentText("None");

    // Initialize with disabled system control
    ui->groupBoxSystemConfig->setEnabled(false);
    ui->groupBoxSystemStatus->setEnabled(false);

    // Create value labels for system status
    createValueLabels();

    // Setup connections
    setupConnections();

    // Setup timer for periodic operations
    dataTimer->setInterval(1000); // 1 second
    connect(dataTimer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
}

MainWindow::~MainWindow()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    delete ui;
}

void MainWindow::createValueLabels()
{
    // Replace LCD numbers with QLabels for system status
    QFont font;
    font.setPointSize(12);
    font.setBold(true);

    // Create labels for threshold values
    valueLabels["Threshold_1"] = new QLabel("0", ui->groupBoxSystemStatus);
    valueLabels["Threshold_2"] = new QLabel("0", ui->groupBoxSystemStatus);
    valueLabels["Last_Weight"] = new QLabel("0", ui->groupBoxSystemStatus);
    valueLabels["Total_Quantity"] = new QLabel("0", ui->groupBoxSystemStatus);

    // Apply styling to all system status labels
    for (auto label : valueLabels.values()) {
        label->setFont(font);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setMinimumWidth(60);
    }

    // Replace LCD widgets with QLabels in the UI layout
    QGridLayout* gridLayout = qobject_cast<QGridLayout*>(ui->groupBoxSystemStatus->layout());
    if (gridLayout) {
        gridLayout->removeWidget(ui->lcdNumberThreshold1);
        ui->lcdNumberThreshold1->hide();
        gridLayout->addWidget(valueLabels["Threshold_1"], 0, 1);

        gridLayout->removeWidget(ui->lcdNumberThreshold2);
        ui->lcdNumberThreshold2->hide();
        gridLayout->addWidget(valueLabels["Threshold_2"], 0, 3);

        gridLayout->removeWidget(ui->lcdNumberTotalCount);
        ui->lcdNumberTotalCount->hide();
        gridLayout->addWidget(valueLabels["Total_Quantity"], 1, 1);

        gridLayout->removeWidget(ui->lcdNumberLastWeight);
        ui->lcdNumberLastWeight->hide();
        gridLayout->addWidget(valueLabels["Last_Weight"], 1, 3);
    }

    // Create labels for bin values
    valueLabels["Light_Preset"] = new QLabel("0", ui->groupBoxBins);
    valueLabels["Medium_Preset"] = new QLabel("0", ui->groupBoxBins);
    valueLabels["Heavy_Preset"] = new QLabel("0", ui->groupBoxBins);
    valueLabels["Light_Cont"] = new QLabel("0", ui->groupBoxBins);
    valueLabels["Medium_Cont"] = new QLabel("0", ui->groupBoxBins);
    valueLabels["Heavy_Cont"] = new QLabel("0", ui->groupBoxBins);

    // Apply styling to all bin labels
    for (auto label : valueLabels.values()) {
        if (!label->parent()) {
            label->setParent(ui->groupBoxBins);
        }
        label->setFont(font);
        label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        label->setMinimumWidth(60);
    }

    // Replace LCD widgets with QLabels in the bins layout
    QGridLayout* binsGridLayout = qobject_cast<QGridLayout*>(ui->groupBoxBins->layout());
    if (binsGridLayout) {
        binsGridLayout->removeWidget(ui->lcdNumberLightPreset);
        ui->lcdNumberLightPreset->hide();
        binsGridLayout->addWidget(valueLabels["Light_Preset"], 1, 1);

        binsGridLayout->removeWidget(ui->lcdNumberMediumPreset);
        ui->lcdNumberMediumPreset->hide();
        binsGridLayout->addWidget(valueLabels["Medium_Preset"], 1, 3);

        binsGridLayout->removeWidget(ui->lcdNumberHeavyPreset);
        ui->lcdNumberHeavyPreset->hide();
        binsGridLayout->addWidget(valueLabels["Heavy_Preset"], 1, 5);

        binsGridLayout->removeWidget(ui->lcdNumberLightCurrent);
        ui->lcdNumberLightCurrent->hide();
        binsGridLayout->addWidget(valueLabels["Light_Cont"], 2, 1);

        binsGridLayout->removeWidget(ui->lcdNumberMediumCurrent);
        ui->lcdNumberMediumCurrent->hide();
        binsGridLayout->addWidget(valueLabels["Medium_Cont"], 2, 3);

        binsGridLayout->removeWidget(ui->lcdNumberHeavyCurrent);
        ui->lcdNumberHeavyCurrent->hide();
        binsGridLayout->addWidget(valueLabels["Heavy_Cont"], 2, 5);
    }
}

void MainWindow::setupConnections()
{
    // Serial port connections
    connect(ui->pushButtonConnect, &QPushButton::clicked, this, &MainWindow::onSerialConnect);
    connect(ui->pushButtonDisconnect, &QPushButton::clicked, this, &MainWindow::onSerialDisconnect);
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::onSerialDataReceived);
    connect(serialPort, &QSerialPort::errorOccurred, this, &MainWindow::onSerialError);

    // Configuration connection
    connect(ui->pushButtonSendConfig, &QPushButton::clicked, this, &MainWindow::onSendConfiguration);

    // Power buttons connection
    connect(ui->pushButtonPowerOn, &QPushButton::clicked, this, &MainWindow::onPowerOn);
    connect(ui->pushButtonPowerOff, &QPushButton::clicked, this, &MainWindow::onPowerOff);

    // Log connections
    connect(ui->pushButtonClearLog, &QPushButton::clicked, this, &MainWindow::onClearLog);

    // Refresh ports
    connect(ui->pushButtonRefreshPorts, &QPushButton::clicked, this, &MainWindow::onRefreshPortList);
}

void MainWindow::fillPortsInfo()
{
    ui->comboBoxPort->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        ui->comboBoxPort->addItem(info.portName());
    }
}

void MainWindow::onSerialConnect()
{
    if (ui->comboBoxPort->currentText().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No COM port selected"));
        return;
    }

    serialPort->setPortName(ui->comboBoxPort->currentText());

    // Set baud rate
    int baudRate = ui->comboBoxBaudRate->currentText().toInt();
    serialPort->setBaudRate(baudRate);

    // Set data bits
    int dataBits = ui->comboBoxDataBits->currentText().toInt();
    switch (dataBits) {
    case 5: serialPort->setDataBits(QSerialPort::Data5); break;
    case 6: serialPort->setDataBits(QSerialPort::Data6); break;
    case 7: serialPort->setDataBits(QSerialPort::Data7); break;
    case 8: serialPort->setDataBits(QSerialPort::Data8); break;
    default: serialPort->setDataBits(QSerialPort::Data8); break;
    }

    // Set stop bits
    QString stopBits = ui->comboBoxStopBits->currentText();
    if (stopBits == "1") {
        serialPort->setStopBits(QSerialPort::OneStop);
    } else if (stopBits == "1.5") {
        serialPort->setStopBits(QSerialPort::OneAndHalfStop);
    } else if (stopBits == "2") {
        serialPort->setStopBits(QSerialPort::TwoStop);
    }

    // Set parity
    QString parity = ui->comboBoxParity->currentText();
    if (parity == "None") {
        serialPort->setParity(QSerialPort::NoParity);
    } else if (parity == "Even") {
        serialPort->setParity(QSerialPort::EvenParity);
    } else if (parity == "Odd") {
        serialPort->setParity(QSerialPort::OddParity);
    } else if (parity == "Mark") {
        serialPort->setParity(QSerialPort::MarkParity);
    } else if (parity == "Space") {
        serialPort->setParity(QSerialPort::SpaceParity);
    }

    // Set flow control to none
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this, tr("Error"), tr("Cannot open serial port: ") + serialPort->errorString());
        return;
    }

    // Clear buffer when connecting
    incomingBuffer.clear();

    // Update UI
    ui->pushButtonConnect->setEnabled(false);
    ui->pushButtonDisconnect->setEnabled(true);
    ui->groupBoxSerialConfig->setEnabled(true);
    ui->groupBoxSystemConfig->setEnabled(true);
    ui->groupBoxSystemStatus->setEnabled(true);

    // Start timer
    dataTimer->start();

    appendToLog("Connected to " + ui->comboBoxPort->currentText() + " at " + ui->comboBoxBaudRate->currentText() + " baud");
}

void MainWindow::onSerialDisconnect()
{
    // Disconnect immediately, regardless of current communication state
    if (serialPort->isOpen()) {
        serialPort->close();
    }

    // Clear buffer on disconnect
    incomingBuffer.clear();

    // Update UI
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonDisconnect->setEnabled(false);
    ui->groupBoxSerialConfig->setEnabled(true);
    ui->groupBoxSystemConfig->setEnabled(false);
    ui->groupBoxSystemStatus->setEnabled(false);

    // Stop timer
    dataTimer->stop();

    appendToLog("Disconnected from serial port");
}

void MainWindow::onSerialDataReceived()
{
    // Read all available data and add to buffer
    QByteArray newData = serialPort->readAll();
    if (newData.isEmpty()) {
        return;
    }

    incomingBuffer.append(newData);

    // Process buffer for complete JSON messages
    processBuffer();
}

void MainWindow::processBuffer()
{
    int startIndex = incomingBuffer.indexOf('{');
    if (startIndex == -1) {
        // No start brace, clear buffer
        incomingBuffer.clear();
        return;
    }

    // Remove any data before the first opening brace
    if (startIndex > 0) {
        incomingBuffer.remove(0, startIndex);
        startIndex = 0;
    }

    // Look for the matching closing brace
    int count = 0;
    int endIndex = -1;

    for (int i = 0; i < incomingBuffer.size(); i++) {
        char c = incomingBuffer.at(i);
        if (c == '{') count++;
        else if (c == '}') count--;

        if (count == 0 && i > startIndex) {
            endIndex = i;
            break;
        }
    }

    if (endIndex == -1) {
        // No complete JSON object yet, wait for more data
        return;
    }

    // Extract the complete JSON message
    QByteArray jsonData = incomingBuffer.mid(startIndex, endIndex - startIndex + 1);

    // Remove processed data from buffer
    incomingBuffer.remove(0, endIndex + 1);

    // Process the complete JSON message
    QString receivedText = QString::fromUtf8(jsonData);
    appendToLog("RX: " + receivedText);

    // Parse JSON
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        updateSystemStatus(jsonDoc.object());
    }

    // Check if there's potentially another JSON object in the buffer
    if (incomingBuffer.contains('{')) {
        processBuffer();
    }
}

void MainWindow::onSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError) {
        return;
    }

    QString errorString = "Serial port error: " + serialPort->errorString();
    appendToLog(errorString);

    if (serialPort->isOpen()) {
        serialPort->close();
        ui->pushButtonConnect->setEnabled(true);
        ui->pushButtonDisconnect->setEnabled(false);
        ui->groupBoxSerialConfig->setEnabled(true);
        ui->groupBoxSystemConfig->setEnabled(false);
        ui->groupBoxSystemStatus->setEnabled(false);
        dataTimer->stop();
    }
}

void MainWindow::onSendConfiguration()
{
    if (!serialPort->isOpen()) {
        return;
    }

    QJsonObject configData;
    configData["Threshold_1"] = ui->spinBoxThreshold1->value();
    configData["Threshold_2"] = ui->spinBoxThreshold2->value();
    configData["Light_Preset"] = ui->spinBoxLightPreset->value();
    configData["Medium_Preset"] = ui->spinBoxMediumPreset->value();
    configData["Heavy_Preset"] = ui->spinBoxHeavyPreset->value();

    // Get current power state
    int powerState = ui->labelSystemStatus->text() == "Running";
    configData["Power"] = powerState;

    sendSerialData(configData);
}

void MainWindow::onPowerOn()
{
    setPower(1);
}

void MainWindow::onPowerOff()
{
    setPower(0);
}

void MainWindow::setPower(int on)
{
    if (!serialPort->isOpen()) {
        return;
    }
    QJsonObject configData;
    configData["Threshold_1"] = ui->spinBoxThreshold1->value();
    configData["Threshold_2"] = ui->spinBoxThreshold2->value();
    configData["Light_Preset"] = ui->spinBoxLightPreset->value();
    configData["Medium_Preset"] = ui->spinBoxMediumPreset->value();
    configData["Heavy_Preset"] = ui->spinBoxHeavyPreset->value();
    configData["Power"] = on;
    sendSerialData(configData);
}

void MainWindow::onClearLog()
{
    ui->textEditLog->clear();
}

void MainWindow::onRefreshPortList()
{
    fillPortsInfo();
}

void MainWindow::onTimerTimeout()
{
    // This could be used for periodic tasks such as requesting status updates
    // For now we'll leave it empty as the ATmega is supposed to send data periodically
}

void MainWindow::updateSystemStatus(const QJsonObject &data)
{
    // Update all values from the JSON data
    for (auto it = data.begin(); it != data.end(); ++it) {
        const QString& key = it.key();

        // Update the corresponding label if it exists
        if (valueLabels.contains(key)) {
            QJsonValue value = it.value();
            if (value.isDouble() || value.isString() || value.isDouble()) {
                valueLabels[key]->setText(value.toVariant().toString());
            } else if (value.isBool()) {
                valueLabels[key]->setText(value.toBool() ? "true" : "false");
            }
        }
    }

    // Update threshold displays
    if (data.contains("Threshold_1")) {
        valueLabels["Threshold_1"]->setText(QString::number(data["Threshold_1"].toDouble()));
    }
    if (data.contains("Threshold_2")) {
        valueLabels["Threshold_2"]->setText(QString::number(data["Threshold_2"].toDouble()));
    }

    // Update last weight
    if (data.contains("Last_Weight")) {
        valueLabels["Last_Weight"]->setText(QString::number(data["Last_Weight"].toDouble()));
    }

    // Update preset values
    if (data.contains("Light_Preset")) {
        valueLabels["Light_Preset"]->setText(QString::number(data["Light_Preset"].toDouble()));
    }
    if (data.contains("Medium_Preset")) {
        valueLabels["Medium_Preset"]->setText(QString::number(data["Medium_Preset"].toDouble()));
    }
    if (data.contains("Heavy_Preset")) {
        valueLabels["Heavy_Preset"]->setText(QString::number(data["Heavy_Preset"].toDouble()));
    }

    // Update current bin weights
    if (data.contains("Light_Cont")) {
        ui->progressBarLight->setValue(data["Light_Cont"].toInt());
        if (data.contains("Light_Preset")) {
            ui->progressBarLight->setMaximum(data["Light_Preset"].toInt());
        }
        valueLabels["Light_Cont"]->setText(QString::number(data["Light_Cont"].toDouble()));
    }
    if (data.contains("Medium_Cont")) {
        ui->progressBarMedium->setValue(data["Medium_Cont"].toInt());
        if (data.contains("Medium_Preset")) {
            ui->progressBarMedium->setMaximum(data["Medium_Preset"].toInt());
        }
        valueLabels["Medium_Cont"]->setText(QString::number(data["Medium_Cont"].toDouble()));
    }
    if (data.contains("Heavy_Cont")) {
        ui->progressBarHeavy->setValue(data["Heavy_Cont"].toInt());
        if (data.contains("Heavy_Preset")) {
            ui->progressBarHeavy->setMaximum(data["Heavy_Preset"].toInt());
        }
        valueLabels["Heavy_Cont"]->setText(QString::number(data["Heavy_Cont"].toDouble()));
    }

    // Update total quantity
    if (data.contains("Total_Quantity")) {
        valueLabels["Total_Quantity"]->setText(QString::number(data["Total_Quantity"].toInt()));
    }

    // Update system status
    if (data.contains("Power")) {
        int power = data["Power"].toInt();
        ui->labelSystemStatus->setText((power==1) ? "Running" : "Stopped");
    }

    // Update full status
    if (data.contains("Full")) {
        int full = data["Full"].toInt();
        ui->labelFullStatus->setText((full==1) ? "YES" : "NO");
        if (full==1) {
            ui->labelFullStatus->setStyleSheet("QLabel { color: red; font-weight: bold; }");
        } else {
            ui->labelFullStatus->setStyleSheet("QLabel { color: green; }");
        }
    }
}

void MainWindow::appendToLog(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss]");
    ui->textEditLog->append(timestamp + " " + message);

    // Scroll to bottom
    QScrollBar *sb = ui->textEditLog->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void MainWindow::sendSerialData(const QJsonObject &data)
{
    if (!serialPort->isOpen()) {
        return;
    }

    QJsonDocument doc(data);
    QString jsonString = doc.toJson(QJsonDocument::Compact);

    serialPort->write(jsonString.toUtf8());
    appendToLog("TX: " + jsonString);
}

QString MainWindow::beautifyJson(const QString &jsonString)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    return doc.toJson(QJsonDocument::Indented);
}
