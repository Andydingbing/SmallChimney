#ifndef Q_SP9500_IQ_CAP_CONFIG_WIDGET_H
#define Q_SP9500_IQ_CAP_CONFIG_WIDGET_H

#include "global.h"
#include "iq_cap_config_widget.h"
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QLineEdit>

namespace NS_SP9500 {

class Q_IQ_Cap_Config_Model;

class Q_IQ_Cap_Config_Delegate : public ::Q_IQ_Cap_Config_Delegate
{
    Q_OBJECT

public:
    Q_IQ_Cap_Config_Delegate(QObject *parent = nullptr);

public slots:
    void setSamples(const QString &text);
    void setTriggerSource(int index);
    void setTriggerOffset(const QString &text);
    void setTimeout(const QString &text);
    void setThreshold(const QString &text);

public:
    QComboBox *comboBoxTriggerSource;
    QComboBox *comboBoxTriggerEdge;
    QLineEdit *lineEditTriggerOffset;
    QLineEdit *lineEditTriggerGap;
    QLineEdit *lineEditCaptureOffset;
    QLineEdit *lineEditTimeout;
    QLineEdit *lineEditThreshold;

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
};


class Q_IQ_Cap_Config_Widget_Helper : public ::Q_IQ_Cap_Config_Widget_Helper
{
public:
    Q_IQ_Cap_Config_Widget_Helper(QWidget *parent);

    void init();
};

} // namespace NS_SP9500

#endif // Q_SP9500_IQ_CAP_CONFIG_WIDGET_H
