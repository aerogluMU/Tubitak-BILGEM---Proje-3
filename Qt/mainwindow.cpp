#include <mainwindow.h>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Disable maximizing
    setFixedSize(width(), height());

    // Ports
    QList<QSerialPortInfo> ports = info.availablePorts();
    QList<QString> stringPorts;
    for(int i = 0 ; i < ports.size() ; i++){
        stringPorts.append(ports.at(i).portName());
    }
    ui->cbPort->addItems(stringPorts);

    // Baud Rates
    QList<qint32> baudRates = info.standardBaudRates(); // What baudrates does my computer support ?
    QList<QString> stringBaudRates;
    for(int i = 0 ; i < baudRates.size() ; i++){
        stringBaudRates.append(QString::number(baudRates.at(i)));
    }
    ui->cbBaudRate->addItems(stringBaudRates);

    //Led Blink Rate
    for (int var = 100; var <= 1000; var = var + 100) {
        ui->cbLedBlinkRate->addItem(QString::number(var));
    }

    //Led PWM Rate
    for (int var = 0; var <= 1000; var = var + 10) {
        ui->cbLedPWM->addItem(QString::number(var));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnConnect_clicked()
{
    QString portName = ui->cbPort->currentText();
    serialPort.setPortName(portName);

    serialPort.open(QIODevice::ReadWrite);

    if(!serialPort.isOpen()){
        ui->txtCommands->setTextColor(Qt::red);
        ui->txtCommands->append("!!!! Something went Wrong !!!!");
    }
    else {

        QString stringbaudRate = ui->cbBaudRate->currentText();
        int intbaudRate = stringbaudRate.toInt();
        serialPort.setBaudRate(intbaudRate);

        serialPort.setDataBits(QSerialPort::Data8);

        serialPort.setStopBits(QSerialPort::OneStop);

        serialPort.setParity(QSerialPort::NoParity);

        serialPort.setFlowControl(QSerialPort::NoFlowControl);

        connect(&serialPort,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
    }


}

/*
During process of making this program, i encountered a problem.
Library of QSerialPort does not guarantee that received message comes at once.It mostly comes part by part.
Sometimes it comes completely.But it is not guarantee.I tried to solve this issue by giving message a code.
I stored every received message in buffer. Whenever i store message in my buffer,i searched my code in buffer.
If it is found,i display the part of the message until the code. And i remove that message from buffer.
If it is not found i keep storing received message in buffer.
*/


void MainWindow::receiveMessage()
{
    QByteArray dataBA = serialPort.readAll();
    QString data(dataBA);
    buffer.append(data);
    int index = buffer.indexOf(code);
    if(index != -1){
        QString message = buffer.mid(0,index);
        ui->txtCommands->setTextColor(Qt::blue); // Receieved message's color is blue.
        ui->txtCommands->append(message);
        buffer.remove(0,index+codeSize);
    }

}

void MainWindow::on_btnResetAll_clicked()
{
    QString message = "ResetAll";
    ui->txtCommands->setTextColor(Qt::darkGreen); // Color of message to send is green.
    ui->txtCommands->append(message);
    serialPort.write(message.toUtf8());
}

void MainWindow::on_btnLedOn_clicked()
{
    QString message = "LedOn";
    ui->txtCommands->setTextColor(Qt::darkGreen); // Color of message to send is green.
    ui->txtCommands->append(message);
    serialPort.write(message.toUtf8());
}

void MainWindow::on_btnLedOff_clicked()
{
    QString message = "LedOff";
    ui->txtCommands->setTextColor(Qt::darkGreen); // Color of message to send is green.
    ui->txtCommands->append(message);
    serialPort.write(message.toUtf8());
}

void MainWindow::on_btnLedBlink_clicked()
{
    QString message = "Blink";
    ui->txtCommands->setTextColor(Qt::darkGreen); // Color of message to send is green.
    ui->txtCommands->append(message);
    serialPort.write(message.toUtf8());
}

void MainWindow::on_btnSendBlinkParameter_clicked()
{
    QString message = "P " + QString::number(ui->cbLedBlinkRate->currentText().size()) + " " + ui->cbLedBlinkRate->currentText();
    ui->txtCommands->setTextColor(Qt::darkGreen); // Color of message to send is green.
    ui->txtCommands->append(message);
    serialPort.write(message.toUtf8());
}

void MainWindow::on_btnLedPWM_clicked()
{
    QString message = "LedPWM";
    ui->txtCommands->setTextColor(Qt::darkGreen); // Color of message to send is green.
    ui->txtCommands->append(message);
    serialPort.write(message.toUtf8());
}

void MainWindow::on_btnSendPWMParameter_clicked()
{
    QString message = "P " + QString::number(ui->cbLedPWM->currentText().size()) + " " + ui->cbLedPWM->currentText();
    ui->txtCommands->setTextColor(Qt::darkGreen); // Color of message to send is green.
    ui->txtCommands->append(message);
    serialPort.write(message.toUtf8());
}


// Button of Disconnect
void MainWindow::on_btnDisconnect_clicked()
{
    serialPort.close();
}

// Button of Refresh Ports
void MainWindow::on_btnRefreshPorts_clicked()
{
    ui->cbPort->clear();
    QList<QSerialPortInfo> ports = info.availablePorts();
    QList<QString> stringPorts;
    for(int i = 0 ; i < ports.size() ; i++){
        stringPorts.append(ports.at(i).portName());
    }
    ui->cbPort->addItems(stringPorts);
}

// Button of Clear
void MainWindow::on_btnClear_clicked()
{
    ui->txtCommands->clear();
}

