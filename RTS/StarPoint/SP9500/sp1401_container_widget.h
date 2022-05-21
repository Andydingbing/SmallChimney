#ifndef Q_RF_CONTAINER_WIDGET_H
#define Q_RF_CONTAINER_WIDGET_H

#include "temp_ctrl_widget.h"
#include <QShowEvent>
#include <QVBoxLayout>

template<typename Widget_Type>
class Q_SP1401_Container_Widget : public QWidget
{
public:
    explicit Q_SP1401_Container_Widget(QWidget *parent = nullptr) : QWidget(parent)
    {
        widget = new Widget_Type(this);
        widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

        SAFE_NEW(dlgTempCtrl,QTempCtrlDlg);

        layout = new QVBoxLayout(this);
        layout->addWidget(widget);
        layout->addWidget(dlgTempCtrl);
    }

protected:
    void showEvent(QShowEvent *event)
    {
        if (event->type() == QEvent::Show) {
            layout->addWidget(dlgTempCtrl);
        }
        QWidget::showEvent(event);
    }

public:
    QVBoxLayout *layout;
    Widget_Type *widget;
};

#endif
