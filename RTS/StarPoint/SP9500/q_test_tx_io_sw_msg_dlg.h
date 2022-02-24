#ifndef Q_TEST_TX_IO_SW_MSG_DLG_H
#define Q_TEST_TX_IO_SW_MSG_DLG_H

#include <QDialog>

namespace Ui {
class QTestTXIOSwMsgDlg;
}

class QTestTXIOSwMsgDlg : public QDialog
{
    Q_OBJECT

public:
    explicit QTestTXIOSwMsgDlg(const QColor TX, const QColor RX, QWidget *parent = 0);
    ~QTestTXIOSwMsgDlg();

private slots:
    void on_pushButtonYes_clicked();

    void on_pushButtonNo_clicked();

private:
    Ui::QTestTXIOSwMsgDlg *ui;
};

#endif // Q_TEST_TX_IO_SW_MSG_DLG_H
