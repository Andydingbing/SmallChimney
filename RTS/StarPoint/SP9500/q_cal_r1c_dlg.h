#ifndef Q_CAL_R1C_DLG_H
#define Q_CAL_R1C_DLG_H

#include "q_attach_thread_dlg.hpp"
#include "global.h"

class QTxTroubleShootModel;
class QRxTroubleShootModel;

namespace Ui {
class QCalR1CDlg;
}

class QCalR1CDlg : public QWidget, public QAttachThreadDlg
{
    Q_OBJECT

public:
    sp1401 *SP1401;
    sp2401_r1a *SP2401;
    QTxTroubleShootModel *modelTxTS;
    QRxTroubleShootModel *modelRxTS;

public:
    explicit QCalR1CDlg(QWidget *parent = nullptr);
    ~QCalR1CDlg();

private slots:
    void update(const QModelIndex &tl,
                const QModelIndex &br,
                cal_file::cal_item_t item = cal_file::TX_LOL,
                int sec = 0);

    void on_pushButtonStar_clicked();

    void on_pushButtonStop_clicked();

    void on_pushButtonPaus_clicked();

    void on_pushButtonCont_clicked();

    void on_pushButtonSetRFInfo_clicked();

    void on_pushButtonGetRFInfo_clicked();

private:
    Ui::QCalR1CDlg *ui;
};

#endif // Q_CAL_R1C_DLG_H
