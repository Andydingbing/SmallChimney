#include "sp9500x_rrh_widget.h"
#include "ui_sp9500x_rrh.h"

using namespace ns_sp9500x::ns_rrh;
using namespace NS_SP9500X;

Q_SP9500X_RRH_Widget::Q_SP9500X_RRH_Widget(QWidget *parent) :
    Q_RD_Widget(parent),
    ui(new Ui::Q_SP9500X_RRH_Widget)
{
    ui->setupUi(this);

    for (size_t i = 0;i < io_mode::_size();++i) {
        ui->comboBoxTX0IOMode->addItem(QString::fromUtf8(io_mode::_names()[i]));
        ui->comboBoxTX1IOMode->addItem(QString::fromUtf8(io_mode::_names()[i]));
    }
}

Q_SP9500X_RRH_Widget::~Q_SP9500X_RRH_Widget()
{
    delete ui;
}

void Q_SP9500X_RRH_Widget::init()
{
    RD_TRY
    ui->lineEditTXFreq->setText(QString::fromStdString(SP9500X_RRH->tx_freq_string()));
    ui->lineEditRXFreq->setText(QString::fromStdString(SP9500X_RRH->rx_freq_string()));
    RD_LOG_ALL_EXCEPTION

    double TXPwr = 0.0;
    double RXRef = 0.0;
    io_mode_t modeTRX0 = CLOSE;
    io_mode_t modeTRX1 = CLOSE;

    INT_CHECKV(SP9500X_RRH->get_tx_pwr(TXPwr));
    INT_CHECKV(SP9500X_RRH->get_rx_ref(RXRef));
    INT_CHECKV(SP9500X_RRH->get_io_mode(TRX0,modeTRX0));
    INT_CHECKV(SP9500X_RRH->get_io_mode(TRX1,modeTRX1));

    ui->lineEditTXPwr->setText(QString("%1").arg(TXPwr));
    ui->lineEditRXRef->setText(QString("%1").arg(RXRef));
    ui->comboBoxTX0IOMode->setCurrentIndex(modeTRX0);
    ui->comboBoxTX1IOMode->setCurrentIndex(modeTRX1);
    ui->lineEditRegAddr->setText("0x00");
}

void Q_SP9500X_RRH_Widget::on_lineEditTXFreq_textEdited(const QString &arg1)
{
    RD_TRY
    SP9500X_RRH->set_tx_freq(freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP9500X_RRH_Widget::on_pushButtonSetTXPwr_clicked()
{
    SP9500X_RRH->set_tx_pwr(ui->lineEditTXPwr->text().toDouble());
}

void Q_SP9500X_RRH_Widget::on_lineEditTXPwr_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    on_pushButtonSetTXPwr_clicked();
}

void Q_SP9500X_RRH_Widget::on_lineEditRXFreq_textEdited(const QString &arg1)
{
    RD_TRY
    SP9500X_RRH->set_rx_freq(freq_string_to<quint64>(arg1.toStdString()));
    RD_LOG_ALL_EXCEPTION
}

void Q_SP9500X_RRH_Widget::on_pushButtonSetRXRef_clicked()
{
    SP9500X_RRH->set_rx_ref(ui->lineEditRXRef->text().toDouble());
}

void Q_SP9500X_RRH_Widget::on_lineEditRXRef_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    on_pushButtonSetRXRef_clicked();
}

void Q_SP9500X_RRH_Widget::on_pushButtonRegWrite_clicked()
{
    uint8_t addr = quint8(ui->lineEditRegAddr->text().toUShort(nullptr,16));
    uint32_t data = ui->lineEditRegData->text().toULong(nullptr,16);
    SP9500X_RRH->set_reg(addr,data);
}

void Q_SP9500X_RRH_Widget::on_pushButtonRegRead_clicked()
{
    uint32_t data = 0;
    uint8_t addr = quint8(ui->lineEditRegAddr->text().toUShort(nullptr,16));
    SP9500X_RRH->get_reg(addr,data);
    ui->lineEditRegData->setText(DecimalToHexString(data));
}
