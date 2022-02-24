#ifndef Q_SP9500_FPGA_WIDGET_H
#define Q_SP9500_FPGA_WIDGET_H

#include "global.h"

namespace Ui {
class Q_SP9500_FPGA_Widget;
}

namespace NS_SP9500 {

class Q_FPGA_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Q_FPGA_Widget(QWidget *parent = nullptr);
    ~Q_FPGA_Widget();

private slots:
    void on_pushButtonWriteK7_0_clicked();

    void on_pushButtonReadK7_0_clicked();

    void on_pushButtonWriteK7_1_clicked();

    void on_pushButtonReadK7_1_clicked();

    void on_pushButtonWriteS6_BAR0_clicked();

    void on_pushButtonReadS6_BAR0_clicked();

    void on_pushButtonWriteS6_BAR1_clicked();

    void on_pushButtonReadS6_BAR1_clicked();

    void on_pushButtonSetPathK7_0_clicked();

    void on_pushButtonProgramK7_0_clicked();

    void on_pushButtonSetPathK7_1_clicked();

    void on_pushButtonProgramK7_1_clicked();

private:
    Ui::Q_SP9500_FPGA_Widget *ui;
};

} // namespace NS_SP9500

#endif // Q_SP9500_FPGA_WIDGET_H
