#include "mainwindow.h"
#include "press_event_combobox.hpp"
#include <QVBoxLayout>

using namespace std;
using namespace ns_serial_dev;

MainWindow *g_MainW = nullptr;

SerialPortConfigDelegate::SerialPortConfigDelegate(QObject *parent)
{
    sd = new serial_dev_vi;

    widgetPortConfigBegin = new QWidget;
//    widgetPortConfigBegin->setFrameShape(QFrame::HLine);
//    widgetPortConfigBegin->setFrameShadow(QFrame::Sunken);


    widgetPortConfigEnd   = new QWidget;

    comboBoxPort = new Q_Press_Event_ComboBox;
    connect(comboBoxPort,SIGNAL(clicked()),this,SLOT(resetPort()));
    connect(comboBoxPort,SIGNAL(activated(const QString &)),this,SLOT(open(const QString &)));

    lineEditBaudRate = new QLineEdit;
    connect(lineEditBaudRate,SIGNAL(textEdited(const QString &)),this,SLOT(setBaudRate(const QString &)));

    lineEditDataBits = new QLineEdit;
    connect(lineEditDataBits,SIGNAL(textEdited(const QString &)),this,SLOT(setDataBits(const QString &)));

    comboBoxStopBit = new QComboBox;
    COMBOBOX_MAP_FROM_BETTER_ENUM(comboBoxStopBit,stop_bit_t);
    connect(comboBoxStopBit,SIGNAL(activated(int)),this,SLOT(setStopBit(int)));

    comboBoxParityBit = new QComboBox;
    COMBOBOX_MAP_FROM_BETTER_ENUM(comboBoxParityBit,parity_bit_t);
    connect(comboBoxParityBit,SIGNAL(activated(int)),this,SLOT(setParityBit(int)));

    widgetReadConfigBegin = new QWidget;
    widgetReadConfigEnd   = new QWidget;

    checkBoxReadAuto = new QCheckBox;
    checkBoxReadAuto->setChecked(true);

    lineEditTimeout = new QLineEdit;
    connect(lineEditTimeout,SIGNAL(textEdited(const QString &)),this,SLOT(setTimeout(const QString &)));

    comboBoxReadEndMode = new QComboBox;
    COMBOBOX_MAP_FROM_BETTER_ENUM(comboBoxReadEndMode,read_end_mode_t);
    connect(comboBoxReadEndMode,SIGNAL(activated(int)),this,SLOT(setReadEndMode(int)));

    lineEditReadTerminateChar = new QLineEdit;
    connect(lineEditReadTerminateChar,SIGNAL(textEdited(const QString &)),this,SLOT(setReadTerminateChar(const QString &)));

    widgetWriteConfigBegin = new QWidget;
}

SerialPortConfigDelegate::~SerialPortConfigDelegate()
{
    if (sd) {
        sd->close();
    }
}

void SerialPortConfigDelegate::resetPort()
{
    if (sd == nullptr) {
        return;
    }

    vector<string> ports;
    vector<string>::const_iterator iterPorts;

    sd->get_devs(ports);

    comboBoxPort->clear();

    for (iterPorts = ports.cbegin();iterPorts != ports.cend();++ iterPorts) {
        comboBoxPort->addItem(QString::fromStdString(*iterPorts));
    }
}

void SerialPortConfigDelegate::open(const QString &port)
{
    if (sd == nullptr) {
        return;
    }

    uint32_t baudRate = 0;
    uint8_t dataBits = 0;
    serial_dev_stop_bit_t stopBit = serial_dev_stop_bit__1;
    serial_dev_parity_bit_t parityBit = serial_dev_parity_bit_none;

    uint32_t timeout = 0;
    serial_dev_read_end_mode_t readEndMode = serial_dev_read_end_mode_none;
    bool termCharEn = false;
    char termChar = '\0';

    sd->close();
    sd->open(port.toStdString());
    sd->get_baud_rate(baudRate);
    sd->get_data_bits(dataBits);
    sd->get_stop_bit(stopBit);
    sd->get_parity_bit(parityBit);

    sd->get_timeout(timeout);
    sd->get_read_end_mode(readEndMode);
    sd->get_terminate_char_en(termCharEn);
    sd->get_terminate_char(termChar);

    lineEditBaudRate->setText(QString("%1").arg(baudRate));
    lineEditDataBits->setText(QString("%1").arg(dataBits));
    comboBoxStopBit->setCurrentIndex(stopBit);
    comboBoxParityBit->setCurrentIndex(parityBit);

    lineEditTimeout->setText(QString("%1").arg(timeout));
    comboBoxReadEndMode->setCurrentIndex(readEndMode);
    lineEditReadTerminateChar->setEnabled(readEndMode == serial_dev_read_end_mode_termchar);
    lineEditReadTerminateChar->setText(QString("%1").arg(termChar));
}

void SerialPortConfigDelegate::setBaudRate(const QString &rate)
{
    sd->set_baud_rate(rate.toUInt());
}

void SerialPortConfigDelegate::setDataBits(const QString &bits)
{
    sd->set_data_bits(bits.toUInt());
}

void SerialPortConfigDelegate::setStopBit(int index)
{
    sd->set_stop_bit(serial_dev_stop_bit_t(stop_bit_t::_from_index(index)._to_integral()));
}

void SerialPortConfigDelegate::setParityBit(int index)
{
    sd->set_parity_bit(serial_dev_parity_bit_t(parity_bit_t::_from_index(index)._to_integral()));
}

void SerialPortConfigDelegate::setTimeout(const QString &timeout)
{
    sd->set_timeout(timeout.toUInt());
}

void SerialPortConfigDelegate::setReadEndMode(int index)
{
    sd->set_read_end_mode(serial_dev_read_end_mode_t(read_end_mode_t::_from_index(index)._to_integral()));

    lineEditReadTerminateChar->setEnabled(index == serial_dev_read_end_mode_termchar);
}

void SerialPortConfigDelegate::setReadTerminateChar(const QString &termChar)
{
    if (termChar.length() == 0) {
        sd->set_terminate_char_en(false);
        return;
    }
    if (termChar.length() == 1) {
        sd->set_terminate_char_en(true);
        sd->set_terminate_char(termChar.front().toLatin1());
        return;
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableViewConfigPort->setItemDelegate(&sdConfigDelegate);
    ui->tableViewConfigPort->setModel(&sdConfigModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exit()
{
    QApplication::exit();
}

void MainWindow::on_pushButtonSend_clicked()
{
    if (sdConfigDelegate.sd == nullptr) {
        return;
    }

    QString sendString = ui->plainTextEditSend->toPlainText();

    sdConfigDelegate.sd->write(sendString.toStdString());
    ui->plainTextEditReceive->insertPlainText(QString("%1\n").arg(sendString));
    ui->plainTextEditSend->clear();

    if (sdConfigDelegate.checkBoxReadAuto->isChecked() == false) {
        return;
    }

    if (sdConfigDelegate.comboBoxReadEndMode->currentIndex() != serial_dev_read_end_mode_termchar) {
        while (receive() == 0) { }
    } else {
        receive();
    }

}


void MainWindow::on_pushButtonReceive_clicked()
{
    if (sdConfigDelegate.sd == nullptr) {
        return;
    }

    if (sdConfigDelegate.checkBoxReadAuto->isChecked()) {
        while (receive() == 0) {

        }
    } else {
        receive();
    }
}

qint32 MainWindow::receive()
{
    string buf;
    qint32 status = 0;

    status = sdConfigDelegate.sd->read(buf);
    ui->plainTextEditReceive->insertPlainText(QString::fromStdString(buf));
    ui->plainTextEditReceive->moveCursor(QTextCursor::End);
    return status;
}
