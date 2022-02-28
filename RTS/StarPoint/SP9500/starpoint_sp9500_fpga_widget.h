#ifndef Q_STARPOINT_SP9500_FPGA_WIDGET_H
#define Q_STARPOINT_SP9500_FPGA_WIDGET_H

#include "starpoint_sp9500_global.h"

class Q_StarPoint_SP9500_FPGA_Widget : public QWidget
{
    Q_OBJECT

public:
    Q_StarPoint_SP9500_FPGA_Widget(QWidget *parent = nullptr);
    ~Q_StarPoint_SP9500_FPGA_Widget();

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
    Ui::Q_StarPoint_SP9500_FPGA_Widget *ui;
};

#endif
