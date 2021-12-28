#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "global.h"
#include "libdriver.h"
#include <QMainWindow>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include "ui_mainwindow.h"

CONFIG_MODEL(SerialPortConfigModel,
             "Port Config :",
             "Port",
             "Baud Rate",
             "Data Bits",
             "Stop Bit",
             "Parity Bit",
             "",
             "Read Config :",
             "Auto",
             "Timeout(ms)",
             "End Mode",
             "Terminate Char",
             "",
             "Write Config :")

class SerialPortConfigDelegate : public Q_Config_Table_Delegate
{
    Q_OBJECT

public:
    SerialPortConfigDelegate(QObject *parent = nullptr);
    ~SerialPortConfigDelegate();

    FIRST_CONFIG_WIDGET(QWidget,widgetPortConfigBegin);
    QComboBox *comboBoxPort;
    QLineEdit *lineEditBaudRate;
    QLineEdit *lineEditDataBits;
    QComboBox *comboBoxStopBit;
    QComboBox *comboBoxParityBit;
    QWidget   *widgetPortConfigEnd;

    QWidget   *widgetReadConfigBegin;
    QCheckBox *checkBoxReadAuto;
    QLineEdit *lineEditTimeout;
    QComboBox *comboBoxReadEndMode;
    QLineEdit *lineEditReadTerminateChar;
    QWidget   *widgetReadConfigEnd;

    QWidget   *widgetWriteConfigBegin;

public slots:
    void resetPort();
    void open(const QString &port);
    void setBaudRate(const QString &rate);
    void setDataBits(const QString &bits);
    void setStopBit(int index);
    void setParityBit(int index);

    void setTimeout(const QString &timeout);
    void setReadEndMode(int index);
    void setReadTerminateChar(const QString &termChar);

public:
    serial_dev *sd;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    qint32 receive();

public slots:
    void exit();

private slots:
    void on_pushButtonSend_clicked();
    void on_pushButtonReceive_clicked();

public:
    SerialPortConfigModel sdConfigModel;
    SerialPortConfigDelegate sdConfigDelegate;

    Ui::MainWindow *ui;
};

extern MainWindow *g_MainW;

#endif // MAIN_WINDOW_H
