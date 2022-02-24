#ifndef Q_SP9500X_FPGA_WIDGET_H
#define Q_SP9500X_FPGA_WIDGET_H

#include "global.h"

namespace Ui {
class Q_SP9500X_FPGA_Widget;
}

namespace NS_SP9500X {

class Q_FPGA_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Q_FPGA_Widget(QWidget *parent = nullptr);
    ~Q_FPGA_Widget();

private slots:
    void on_pushButtonWriteRFU_V9_clicked();

    void on_pushButtonReadRFU_V9_clicked();

    void on_pushButtonWriteBBU_V9_clicked();

    void on_pushButtonReadBBU_V9_clicked();

    void on_pushButtonWriteS6_clicked();

    void on_pushButtonReadS6_clicked();

    void on_pushButtonSetPathRFU_V9_clicked();

    void on_pushButtonProgramRFU_V9_clicked();

    void on_pushButtonSetPathBBU_V9_clicked();

    void on_pushButtonProgramBBU_V9_clicked();

    void on_pushButtonWriteAD998X_clicked();

    void on_pushButtonReadAD998X_clicked();

private:
    Ui::Q_SP9500X_FPGA_Widget *ui;
};

} // namespace NS_SP9500X

#endif // Q_SP9500X_FPGA_WIDGET_H
