#include "starpoint_arb_config_widget.h"
#include "starpoint_arb_widget.h"
#include "ui_starpoint_arb.h"

void Q_ARB_Config_Widget_Helper::init()
{
    Q_ARB_Widget *p = dynamic_cast<Q_ARB_Widget *>(parent);

    p->ui->tableView_Config->setItemDelegate(delegate);
    p->ui->tableView_Config->setModel(model);
}
