#ifndef Q_SP9500X_RRH_Widget_H
#define Q_SP9500X_RRH_Widget_H

#include "global.h"

namespace Ui {
class Q_SP9500X_RRH_Widget;
}

class Q_SP9500X_RRH_Widget : public Q_RD_Widget
{
    Q_OBJECT

public:
    explicit Q_SP9500X_RRH_Widget(QWidget *parent = nullptr);
    ~Q_SP9500X_RRH_Widget();

public slots:
    void init();

private slots:
    void on_lineEditTXFreq_textEdited(const QString &arg1);

    void on_pushButtonSetTXPwr_clicked();

    void on_lineEditTXPwr_textEdited(const QString &arg1);

    void on_lineEditRXFreq_textEdited(const QString &arg1);

    void on_pushButtonSetRXRef_clicked();

    void on_lineEditRXRef_textEdited(const QString &arg1);

    void on_pushButtonRegWrite_clicked();

    void on_pushButtonRegRead_clicked();

private:
    Ui::Q_SP9500X_RRH_Widget *ui;
};

#endif // Q_SP9500X_RRH_Widget_H
