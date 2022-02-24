#include "q_test_tx_io_sw_msg_dlg.h"
#include "ui_q_test_tx_io_sw_msg_dlg.h"

QTestTXIOSwMsgDlg::QTestTXIOSwMsgDlg(const QColor TX, const QColor RX, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTestTXIOSwMsgDlg)
{
    ui->setupUi(this);
    ui->ledTX->setColor(TX);
    ui->ledRX->setColor(RX);
}

QTestTXIOSwMsgDlg::~QTestTXIOSwMsgDlg()
{
    delete ui;
}

void QTestTXIOSwMsgDlg::on_pushButtonYes_clicked()
{
    QDialog::accept();
}

void QTestTXIOSwMsgDlg::on_pushButtonNo_clicked()
{
    QDialog::reject();
}
