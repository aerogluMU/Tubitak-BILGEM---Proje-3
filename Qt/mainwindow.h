#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static void sleep(unsigned long secs){QThread::sleep(secs);}

private slots:
    void on_btnConnect_clicked();
    void receiveMessage();
    void on_btnDisconnect_clicked();
    void on_btnRefreshPorts_clicked();
    void on_btnClear_clicked();
    void on_btnLedOn_clicked();
    void on_btnLedOff_clicked();
    void on_btnLedBlink_clicked();
    void on_btnSendBlinkParameter_clicked();
    void on_btnLedPWM_clicked();
    void on_btnSendPWMParameter_clicked();
    void on_btnResetAll_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort serialPort;
    QSerialPortInfo info;
    QString buffer;
    QString code;
    int codeSize;
};
#endif // MAINWINDOW_H
