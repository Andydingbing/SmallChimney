#ifndef Q_STARPOINT_ARB_WIDGET_H
#define Q_STARPOINT_ARB_WIDGET_H

#include "global.h"

namespace Ui {
class Q_StarPoint_ARB_Widget;
}

class Q_ARB_Config_Widget_Helper;

class Q_ARB_Widget : public Q_Widget
{
    Q_OBJECT

public:
    explicit Q_ARB_Widget(QWidget *parent = nullptr);
    ~Q_ARB_Widget();

    void keyPressEvent(QKeyEvent *event);

public slots:
    void init();

private slots:
    void on_pushButtonSelArbFile_clicked();

    void on_pushButtonLoad_clicked();

public:
    Q_ARB_Config_Widget_Helper *widget_Config[PROJECTS];

    Ui::Q_StarPoint_ARB_Widget *ui;
};

#endif
