#include "sp9500_fpga_widget.h"
#include "ui_sp9500_fpga.h"
#include <QFileDialog>

using namespace NS_SP9500;

#define K7_0 SP3301->get_sp2401(2)->k7()
#define K7_1 SP3301->get_sp2401(0)->k7()
#define S6   SP3301->get_sp2401(0)->s6()

Q_FPGA_Widget::Q_FPGA_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Q_SP9500_FPGA_Widget)
{
    ui->setupUi(this);

    ui->lineEditOffsetK7_0->setText(DecimalToHexString(0));
    ui->lineEditOffsetK7_1->setText(DecimalToHexString(0));
    ui->lineEditOffsetS6_BAR0->setText(DecimalToHexString(0));
    ui->lineEditOffsetS6_BAR1->setText(DecimalToHexString(0));
}

Q_FPGA_Widget::~Q_FPGA_Widget()
{
    delete ui;
}

void Q_FPGA_Widget::on_pushButtonWriteK7_0_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetK7_0->text().toUInt(nullptr,16);
    quint32 data = ui->lineEditDataK7_0->text().toUInt(nullptr,16);

    status = K7_0->w32(pci_dev::AS_BAR0,offset << 2,data);
    ui->lineEditStatusK7_0->setText(DecimalToHexString(status));
    Log.add_reg(status,K7_0->descriptor(),offset,data);
}

void Q_FPGA_Widget::on_pushButtonReadK7_0_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetK7_0->text().toUInt(nullptr,16);
    quint32 data = 0;

    status = K7_0->r32(pci_dev::AS_BAR0,offset << 2,&data);
    ui->lineEditDataK7_0->setText(DecimalToHexString(data));
    ui->lineEditStatusK7_0->setText(DecimalToHexString(status));
    Log.add_reg(status,K7_0->descriptor(),offset,-1,data);
}

void Q_FPGA_Widget::on_pushButtonWriteK7_1_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetK7_1->text().toUInt(nullptr,16);
    quint32 data = ui->lineEditDataK7_1->text().toUInt(nullptr,16);

    status = K7_1->w32(pci_dev::AS_BAR0,offset << 2,data);
    ui->lineEditStatusK7_1->setText(DecimalToHexString(status));
    Log.add_reg(status,K7_1->descriptor(),offset,data);
}

void Q_FPGA_Widget::on_pushButtonReadK7_1_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetK7_1->text().toUInt(nullptr,16);
    quint32 data = 0;

    status = K7_1->r32(pci_dev::AS_BAR0,offset << 2,&data);
    ui->lineEditDataK7_1->setText(DecimalToHexString(data));
    ui->lineEditStatusK7_1->setText(DecimalToHexString(status));
    Log.add_reg(status,K7_1->descriptor(),offset,-1,data);
}

void Q_FPGA_Widget::on_pushButtonWriteS6_BAR0_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetS6_BAR0->text().toUInt(nullptr,16);
    quint32 data = ui->lineEditDataS6_BAR0->text().toUInt(nullptr,16);

    status = S6->w32(pci_dev::AS_BAR0,offset << 2,data);
    ui->lineEditStatusS6_BAR0->setText(DecimalToHexString(status));
    Log.add_reg(status,S6->descriptor(),offset,data);
}

void Q_FPGA_Widget::on_pushButtonReadS6_BAR0_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetS6_BAR0->text().toUInt(nullptr,16);
    quint32 data = 0;

    status = S6->r32(pci_dev::AS_BAR0,offset << 2,&data);
    ui->lineEditDataS6_BAR0->setText(DecimalToHexString(data));
    ui->lineEditStatusS6_BAR0->setText(DecimalToHexString(status));
    Log.add_reg(status,S6->descriptor(),offset,-1,data);
}

void Q_FPGA_Widget::on_pushButtonWriteS6_BAR1_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetS6_BAR0->text().toUInt(nullptr,16);
    quint32 data = ui->lineEditDataS6_BAR0->text().toUInt(nullptr,16);

    status = S6->w32(pci_dev::AS_BAR1,offset << 2,data);
    ui->lineEditStatusS6_BAR1->setText(DecimalToHexString(status));
    Log.add_reg(status,S6->descriptor(),offset,data);
}

void Q_FPGA_Widget::on_pushButtonReadS6_BAR1_clicked()
{
    qint32 status = 0;
    quint32 offset = ui->lineEditOffsetS6_BAR0->text().toUInt(nullptr,16);
    quint32 data = 0;

    status = S6->r32(pci_dev::AS_BAR1,offset << 2,&data);
    ui->lineEditDataS6_BAR1->setText(DecimalToHexString(data));
    ui->lineEditStatusS6_BAR1->setText(DecimalToHexString(status));
    Log.add_reg(status,S6->descriptor(),offset,-1,data);
}

void Q_FPGA_Widget::on_pushButtonSetPathK7_0_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select Bit File"),"",tr("Bit Files (*.bit)"));
    if (!path.isEmpty()) {
        ui->lineEditPathK7_0->setText(path);
    }
}

void NS_SP9500::Q_FPGA_Widget::on_pushButtonProgramK7_0_clicked()
{
    SP3301->get_sp2401(2)->set_fpga_bit(ui->lineEditPathK7_0->toPlainText().toStdString().c_str());
}

void NS_SP9500::Q_FPGA_Widget::on_pushButtonSetPathK7_1_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select Bit File"),"",tr("Bit Files (*.bit)"));
    if (!path.isEmpty()) {
        ui->lineEditPathK7_1->setText(path);
    }
}

void NS_SP9500::Q_FPGA_Widget::on_pushButtonProgramK7_1_clicked()
{
    SP3301->get_sp2401(0)->set_fpga_bit(ui->lineEditPathK7_1->toPlainText().toStdString().c_str());
}
