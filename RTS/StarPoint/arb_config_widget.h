#ifndef Q_ARB_CONFIG_WIDGET_H
#define Q_ARB_CONFIG_WIDGET_H

#include "global.h"

class Q_ARB_Config_Widget_Helper
{
public:
    Q_ARB_Config_Widget_Helper(QWidget *parent) :
    model(nullptr),delegate(nullptr) { this->parent = parent; }

    virtual void init();
    virtual void show() {}
    virtual void load() {}
    virtual void updateARBStatus() {}

protected:
    QWidget *parent;
    Q_Config_Table_Model *model;
    Q_Config_Table_Delegate *delegate;
};

#endif // Q_ARB_CONFIG_WIDGET_H
