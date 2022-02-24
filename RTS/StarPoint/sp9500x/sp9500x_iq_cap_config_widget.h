#ifndef Q_SP9500X_IQ_CAP_CONFIG_WIDGET_H
#define Q_SP9500X_IQ_CAP_CONFIG_WIDGET_H

#include "global.h"
#include "iq_cap_config_widget.h"
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QLineEdit>

namespace NS_SP9500X {

class Q_IQ_Cap_Config_Model;

class Q_IQ_Cap_Config_Delegate : public ::Q_IQ_Cap_Config_Delegate
{
    Q_OBJECT

public:
    Q_IQ_Cap_Config_Delegate(QObject *parent = nullptr);

public slots:
    void setSamples(const QString &text);
    void setCaptureSource(int index);
    void setCaptureSourceDDC(int index);
    void setCaptureSourceDDC_Ch(int index);
    void setCaptureSourceDL_CPRI_Ch(int index);
    void setCaptureSourceDUC(int index);
    void setCaptureSourceDUC_Ch(int index);
    void setCaptureSR(int index);
    void setCaptureMode(int index);
    void setTriggerSource(int index);
    void setTriggerEdge(int index);
    void setTriggerOffset(const QString &text);
    void setTriggerGap(const QString &text);
    void setTimeout(const QString &text);
    void setThreshold(const QString &text);
    void setTriggerFrame(const QString &text);
    void setTriggerFrameX(const QString &text);
    void setTriggerFrameY(const QString &text);

public:
    QComboBox *comboBoxSource;
    QComboBox *comboBoxSourceDDC;
    QComboBox *comboBoxSourceDDC_Ch;
    QComboBox *comboBoxSourceDL_CPRI_Ch;
    QComboBox *comboBoxSourceDUC;
    QComboBox *comboBoxSourceDUC_Ch;
    QComboBox *comboBoxSR;
    QComboBox *comboBoxMode;
    QComboBox *comboBoxTriggerSource;
    QComboBox *comboBoxTriggerEdge;
    QLineEdit *lineEditTriggerOffset;
    QLineEdit *lineEditTriggerGap;
    QLineEdit *lineEditTimeout;
    QLineEdit *lineEditThreshold;

    QLineEdit *lineEditTriggerFrame;
    QLineEdit *lineEditTriggerFrameX;
    QLineEdit *lineEditTriggerFrameY;
};


class Q_IQ_Cap_Config_Widget_Helper : public ::Q_IQ_Cap_Config_Widget_Helper
{
public:
    Q_IQ_Cap_Config_Widget_Helper(QWidget *parent);

    void init();
};

} // namespace NS_SP9500X

#endif // Q_SP9500X_IQ_CAP_CONFIG_WIDGET_H
