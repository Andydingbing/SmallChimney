#ifndef Q_ARB_WIDGET_H
#define Q_ARB_WIDGET_H

#include "global.h"

namespace Ui {
class Q_ARB_Widget;
}

class Q_ARB_Config_Widget_Helper;

class Q_ARB_Widget : public Q_RD_Widget
{
    Q_OBJECT
    friend class Q_ARB_Config_Widget_Helper;
    friend class NS_SP9500::Q_ARB_Config_Widget_Helper;
    friend class NS_SP9500X::Q_ARB_Config_Widget_Helper;

public:
    explicit Q_ARB_Widget(QWidget *parent = nullptr);
    ~Q_ARB_Widget();

    void keyPressEvent(QKeyEvent *event);

public slots:
    void init();

private slots:
    void on_pushButtonSelArbFile_clicked();

    void on_pushButtonLoad_clicked();

private:
    Q_ARB_Config_Widget_Helper *widget_Config[PROJECTS];

    Ui::Q_ARB_Widget *ui;
};

#endif // Q_ARB_WIDGET_H
