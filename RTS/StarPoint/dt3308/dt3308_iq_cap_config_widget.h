#ifndef Q_DT3308_IQ_CAP_CONFIG_WIDGET_H
#define Q_DT3308_IQ_CAP_CONFIG_WIDGET_H

#include "global.h"
#include "iq_cap_config_widget.h"
#include <QStyledItemDelegate>
#include <QComboBox>
#include <QLineEdit>

namespace NS_DT3308 {

class Q_IQ_Cap_Config_Model;

class Q_IQ_Cap_Config_Delegate : public ::Q_IQ_Cap_Config_Delegate
{
    Q_OBJECT

public:
    Q_IQ_Cap_Config_Delegate(QObject *parent = nullptr);

public slots:
    void setSamples(const QString &text);
    void setSource(int index);
    void setTriggerSource(int index);

public:
    QComboBox *comboBoxSource;
    QComboBox *comboBoxTriggerSource;
};

class Q_IQ_Cap_Config_Widget_Helper : public ::Q_IQ_Cap_Config_Widget_Helper
{
public:
    Q_IQ_Cap_Config_Widget_Helper(QWidget *parent);

    void init();
};

} // namespace NS_DT3308

#endif // Q_DT3308_IQ_CAP_CONFIG_WIDGET_H
