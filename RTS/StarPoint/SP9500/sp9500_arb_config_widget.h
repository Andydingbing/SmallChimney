#ifndef Q_SP9500_ARB_CONFIG_WIDGET_H
#define Q_SP9500_ARB_CONFIG_WIDGET_H

#include "global.h"
#include "arb_config_widget.h"
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

namespace NS_SP9500 {

class Q_ARB_Config_Model;

class Q_ARB_Config_Delegate : public ::Q_Config_Table_Delegate
{
    Q_OBJECT

public:
    Q_ARB_Config_Delegate(QObject *parent = nullptr);

    QWidget **first() const { return (QWidget **)(&comboBoxTriggerSource); }

public slots:
    void setARBTrigger();
    void setARBParam();
    void setFrameTriggerMode();
    void setFrameTriggerInOffset();
    void setFrameTriggerOutOffset();
    void setFrameTime();
    void setHFTriggerMode();
    void setHFTriggerInOffset();
    void setHFTriggerOutOffset();
    void setHFPeriod();
    void setSlotTime();
    void setFrameTriggerSource();
    void setTriggerFrame();
    void setTriggerSlot();
    void setFrameTriggerModXY();

    void setManualTrigger();
    void setARBStart();
    void setARBStop();

public:
    QComboBox *comboBoxTriggerSource;
    QLineEdit *lineEditFreqOffset;
    QCheckBox *checkBoxReTrigger;
    QLineEdit *lineEditCycles;
    QLineEdit *lineEditTriggerDelay;
    QCheckBox *checkBoxAutoStart;
    QLineEdit *lineEditAdditionalSamples;
    QComboBox *comboBoxRepeatMode;
    QComboBox *comboBoxMultiSegTriggerSource;
    QComboBox *comboBoxMultiSegRepeatMode;
    QLineEdit *lineEditNextSeg;

    QComboBox *comboBoxFrameTriggerMode;
    QLineEdit *lineEditFrameTriggerInOffset;
    QLineEdit *lineEditFrameTriggerOutOffset;
    QLineEdit *lineEditFrameTime;

    QComboBox *comboBoxHFTriggerMode;
    QLineEdit *lineEditHFTriggerInOffset;
    QLineEdit *lineEditHFTriggerOutOffset;
    QLineEdit *lineEditHFPeriod;
    QLineEdit *lineEditSlotTime;
    QComboBox *comboBoxFrameTriggerSource;
    QLineEdit *lineEditTriggerFrame;
    QLineEdit *lineEditTriggerSlot;
    QLineEdit *lineEditX;
    QLineEdit *lineEditY;

    QLineEdit *lineEditCurrentSeg;
    QLineEdit *lineEditCurrentSample;
    QLineEdit *lineEditCurrentCycle;
    QLineEdit *lineEditCurrentTime;

    QLineEdit *lineEditCurrentFrame;
    QLineEdit *lineEditCurrentSlot;

    QPushButton *pushButtonFrameTriggerStart;
    QPushButton *pushButtonManualTrigger;
    QPushButton *pushButtonARBStart;
    QPushButton *pushButtonARBStop;
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

} // namespace NS_SP9500

#endif // Q_SP9500_ARB_CONFIG_WIDGET_H
