#ifndef Q_SIGNAL_ANALYZER_FREQ_WIDGET_H
#define Q_SIGNAL_ANALYZER_FREQ_WIDGET_H

#include "global.h"

class Q_SA_Freq_Model;

class Q_SA_Freq_Delegate : public Q_Config_Table_Delegate
{
public:
    Q_SA_Freq_Delegate(QObject *parent = nullptr);

public:
    FIRST_CONFIG_WIDGET(QLineEdit,lineEditCenter)
    QLineEdit *lineEditStar;
    QLineEdit *lineEditStop;
    QLineEdit *lineEditCenterStep;
    QLineEdit *lineEditOffset;
};


class Q_SA_Freq_Widget_Helper
{
public:
    Q_SA_Freq_Widget_Helper(QWidget *parent);

    virtual void init();

protected:
    QWidget *parent;
    Q_SA_Freq_Model *model;
    Q_SA_Freq_Delegate *delegate;
};

#endif // Q_SIGNAL_ANALYZER_FREQ_WIDGET_H
