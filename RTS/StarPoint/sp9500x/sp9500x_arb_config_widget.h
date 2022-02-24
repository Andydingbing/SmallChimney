#ifndef Q_SP9500X_ARB_CONFIG_WIDGET_H
#define Q_SP9500X_ARB_CONFIG_WIDGET_H

#include "global.h"
#include "arb_config_widget.h"
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

namespace NS_SP9500X {

class Q_ARB_Config_Model;

class Q_ARB_Config_Delegate : public ::Q_Config_Table_Delegate
{
    Q_OBJECT

public:
    Q_ARB_Config_Delegate(QObject *parent = nullptr);

public slots:
    void setFreqOffset();
    void setRepetition();
    void setCycles();
    void setAdditionalSamples();
    void setReTrigger();
    void setAutoStart();
    void setTriggerSource();
    void setManualTrigger();
    void setTriggerSlope();
    void setTriggerDelay();

    void setMultiSegRepetition();
    void getMultiSegCurrentSeg();
    void setMultiSegNextSeg();
    void setMultiSegManualTrigger();

    void getCurrentSample();
    void getCurrentCycle();
    void getCurrentTime();

    void setStart();
    void setAbort();

public:
    FIRST_CONFIG_WIDGET(QComboBox,comboBoxSource)
    QLineEdit *lineEditFreqOffset;
    QComboBox *comboBoxRepetition;
    QLineEdit *lineEditCycles;
    QLineEdit *lineEditAdditionalSamples;
    QCheckBox *checkBoxReTrigger;
    QCheckBox *checkBoxAutoStart;
    QComboBox *comboBoxTriggerSource;
    QPushButton *pushButtonManualTrigger;
    QComboBox *comboBoxTriggerSlope;
    QLineEdit *lineEditTriggerDelay;

    QComboBox *comboBoxMultiSegRepetition;
    QLineEdit *lineEditMultiSegCurrentSeg;
    QLineEdit *lineEditMultiSegNextSeg;
    QComboBox *comboBoxMultiSegTriggerSource;
    QPushButton *pushButtonMultiSegManualTrigger;

    QLineEdit *lineEditCurrentSample;
    QLineEdit *lineEditCurrentCycle;
    QLineEdit *lineEditCurrentTime;

    QPushButton *pushButtonStart;
    QPushButton *pushButtonAbort;
};


class Q_ARB_Config_Widget_Helper : public ::Q_ARB_Config_Widget_Helper
{
public:
    Q_ARB_Config_Widget_Helper(QWidget *parent);

    void init();
    void show();
    void load();
    void updateARBStatus();
};

} // namespace NS_SP9500X

#endif // Q_SP9500X_ARB_CONFIG_WIDGET_H
