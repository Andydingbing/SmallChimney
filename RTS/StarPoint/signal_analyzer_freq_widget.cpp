#include "signal_analyzer_freq_widget.h"
#include "signal_analyzer_widget.h"
#include <QStandardItemModel>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>

class Q_SA_Freq_Model : public Q_Config_Table_Model
{
public:
    Q_SA_Freq_Model(QObject *parent = nullptr) :
        Q_Config_Table_Model(parent)
    {
        _item.clear();
        _item << "Center Freq"
              << "Start Freq"
              << "Stop Freq"
              << "CF Step"
              << "Freq Offset";

        setRowCount(rowCount(QModelIndex()));
        setColumnCount(columnCount(QModelIndex()));
    }
};


Q_SA_Freq_Delegate::Q_SA_Freq_Delegate(QObject *parent) :
    Q_Config_Table_Delegate(parent)
{
    lineEditCenter = new QLineEdit;
    lineEditCenter->setFrame(false);

    lineEditStar = new QLineEdit;
    lineEditStar->setFrame(false);

    lineEditStop = new QLineEdit;
    lineEditStop->setFrame(false);

    lineEditCenterStep = new QLineEdit;
    lineEditCenterStep->setFrame(false);

    lineEditOffset = new QLineEdit;
    lineEditOffset->setFrame(false);
}


Q_SA_Freq_Widget_Helper::Q_SA_Freq_Widget_Helper(QWidget *parent)
{
    this->parent = parent;

    model = new Q_SA_Freq_Model;
    delegate = new Q_SA_Freq_Delegate;
}

void Q_SA_Freq_Widget_Helper::init()
{
    Q_SA_Widget *p = dynamic_cast<Q_SA_Widget *>(parent);

    p->tableView_Freq->setItemDelegate(delegate);
    p->tableView_Freq->setModel(model);
}
