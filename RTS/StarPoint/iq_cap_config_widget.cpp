#include "iq_cap_config_widget.h"
#include "signal_analyzer_widget.h"
#include <QTableView>
#include <QStyledItemDelegate>
#include <QHeaderView>
#include <QLineEdit>

void Q_IQ_Cap_Config_Widget_Helper::init()
{
    Q_SA_Widget *p = dynamic_cast<Q_SA_Widget *>(parent);

    p->tableView_Config->setItemDelegate(delegate);
    p->tableView_Config->setModel(model);
}

quint32 Q_IQ_Cap_Config_Widget_Helper::samples()
{
    if (delegate != nullptr) {
        if (delegate->lineEditSamples != nullptr) {
            return delegate->lineEditSamples->text().toUInt();
        }
    }
    return 0;
}
