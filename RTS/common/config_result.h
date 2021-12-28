#ifndef COMMON_CONFIG_RESULT_H
#define COMMON_CONFIG_RESULT_H

#include "ui_config_result.h"

class Q_Config_Result_Widget : public QWidget
{
public:
    Q_Config_Result_Widget(QWidget *parent) : 
    QWidget(parent),
    ui(new Ui::Config_Result_Widget) 
    { ui->setupUi(this); }

    Ui::Config_Result_Widget *ui;
};

#endif // COMMON_CONFIG_RESULT_H
