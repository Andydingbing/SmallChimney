#include "sp9500x_fpga_widget.h"
#include "ui_sp9500x_fpga.h"
#include <QFileDialog>

using namespace ns_sp9500x;
using namespace NS_SP9500X;

#define V9 SP1403->v9()
#define S6 SP2406->s6()

Q_FPGA_Widget::Q_FPGA_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Q_SP9500X_FPGA_Widget)
{
    ui->setupUi(this);

    ui->lineEditOffsetRFU_V9->setText(DecimalToHexString(0));
    ui->lineEditOffsetBBU_V9->setText(DecimalToHexString(0));
    ui->lineEditOffsetS6->setText(DecimalToHexString(0));
    ui->lineEditAD998X_Addr->setText(DecimalToHexString(quint16(0)));
}

Q_FPGA_Widget::~Q_FPGA_Widget()
{
    delete ui;
}

void Q_FPGA_Widget::on_pushButtonWriteRFU_V9_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetRFU_V9->text().toUInt(nullptr,16);
    quint32 data = ui->lineEditDataRFU_V9->text().toUInt(nullptr,16);

    status = V9->w32(pci_dev::AS_BAR0,offset << 2,data);
    ui->lineEditStatusRFU_V9->setText(DecimalToHexString(status));
    Log.add_reg(status,V9->descriptor(),offset,data);
}

void Q_FPGA_Widget::on_pushButtonReadRFU_V9_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetRFU_V9->text().toUInt(nullptr,16);
    quint32 data = 0;

    status = V9->r32(pci_dev::AS_BAR0,offset << 2,&data);
    ui->lineEditDataRFU_V9->setText(DecimalToHexString(data));
    ui->lineEditStatusRFU_V9->setText(DecimalToHexString(status));
    Log.add_reg(status,V9->descriptor(),offset,-1,data);
}

void Q_FPGA_Widget::on_pushButtonWriteBBU_V9_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetBBU_V9->text().toUInt(nullptr,16);
    quint32 data = ui->lineEditDataBBU_V9->text().toUInt(nullptr,16);

    status = V9->w32(pci_dev::AS_BAR0,offset << 2,data);
    ui->lineEditStatusBBU_V9->setText(DecimalToHexString(status));
    Log.add_reg(status,V9->descriptor(),offset,data);
}

void Q_FPGA_Widget::on_pushButtonReadBBU_V9_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetBBU_V9->text().toUInt(nullptr,16);
    quint32 data = 0;

    status = V9->r32(pci_dev::AS_BAR0,offset << 2,&data);
    ui->lineEditDataBBU_V9->setText(DecimalToHexString(data));
    ui->lineEditStatusBBU_V9->setText(DecimalToHexString(status));
    Log.add_reg(status,V9->descriptor(),offset,-1,data);
}

void Q_FPGA_Widget::on_pushButtonWriteS6_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetS6->text().toUInt(nullptr,16);
    quint32 data = ui->lineEditDataS6->text().toUInt(nullptr,16);

    status = S6->w32(pci_dev::AS_BAR0,offset << 2,data);
    ui->lineEditStatusS6->setText(DecimalToHexString(status));
    Log.add_reg(status,S6->descriptor(),offset,data);
}

void Q_FPGA_Widget::on_pushButtonReadS6_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetS6->text().toUInt(nullptr,16);
    quint32 data = 0;

    status = S6->r32(pci_dev::AS_BAR0,offset << 2,&data);
    ui->lineEditDataS6->setText(DecimalToHexString(data));
    ui->lineEditStatusS6->setText(DecimalToHexString(status));
    Log.add_reg(status,S6->descriptor(),offset,-1,data);
}

void Q_FPGA_Widget::on_pushButtonSetPathRFU_V9_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select Bit File"),"",tr("Bit Files (*.bit)"));
    if (!path.isEmpty()) {
        ui->lineEditPathRFU_V9->setText(path);
    }
}

void Q_FPGA_Widget::on_pushButtonProgramRFU_V9_clicked()
{
    SP2406->program(sp2406::RFU,ui->lineEditPathRFU_V9->toPlainText().toStdString().c_str());
}

void Q_FPGA_Widget::on_pushButtonSetPathBBU_V9_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select Bit File"),"",tr("Bit Files (*.bit)"));
    if (!path.isEmpty()) {
        ui->lineEditPathBBU_V9->setText(path);
    }
}

void Q_FPGA_Widget::on_pushButtonProgramBBU_V9_clicked()
{
    SP2406->program(sp2406::BBU,ui->lineEditPathBBU_V9->toPlainText().toStdString().c_str());
}

void NS_SP9500X::Q_FPGA_Widget::on_pushButtonWriteAD998X_clicked()
{
    uint16_t addr = ui->lineEditAD998X_Addr->text().toUShort(nullptr,16);
    uint8_t  data = uint8_t(ui->lineEditAD998X_Data->text().toUShort(nullptr,16));

    SP1403->set_ad998x_reg(addr,data);
}

void NS_SP9500X::Q_FPGA_Widget::on_pushButtonReadAD998X_clicked()
{
    uint16_t addr = ui->lineEditAD998X_Addr->text().toUShort(nullptr,16);
    uint8_t  data = 0;

    SP1403->get_ad998x_reg(addr,data);
    ui->lineEditAD998X_Data->setText(DecimalToHexString(data));
}
