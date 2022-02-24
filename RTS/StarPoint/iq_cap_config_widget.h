#ifndef Q_IQ_CAP_CONFIG_WIDGET_H
#define Q_IQ_CAP_CONFIG_WIDGET_H

#include "global.h"

class Q_IQ_Cap_Config_Delegate : public Q_Config_Table_Delegate
{
public:
    Q_IQ_Cap_Config_Delegate(QObject *parent = nullptr) :
        Q_Config_Table_Delegate(parent) {}

public:
    FIRST_CONFIG_WIDGET(QLineEdit,lineEditSamples)
};


class Q_IQ_Cap_Config_Widget_Helper
{
public:
    Q_IQ_Cap_Config_Widget_Helper(QWidget *parent) :
    model(nullptr),delegate(nullptr) { this->parent = parent; }

    quint32 samples();

    virtual void init();

protected:
    QWidget *parent;
    Q_Config_Table_Model *model;
    Q_IQ_Cap_Config_Delegate *delegate;
};

#endif // Q_IQ_CAP_CONFIG_WIDGET_H
