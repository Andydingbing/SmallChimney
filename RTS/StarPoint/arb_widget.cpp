#include "arb_widget.h"
#include "ui_arb.h"
#include "sp9500_arb_config_widget.h"
#include "sp9500x_arb_config_widget.h"
#include <QFileDialog>
#include <QKeyEvent>

using namespace rd::ns_sp1401;

Q_ARB_Widget::Q_ARB_Widget(QWidget *parent) :
    Q_RD_Widget(parent),
    ui(new Ui::Q_ARB_Widget)
{
    ui->setupUi(this);

    widget_Config[SP9500]  = new NS_SP9500::Q_ARB_Config_Widget_Helper(this);
    widget_Config[SP9500X] = new NS_SP9500X::Q_ARB_Config_Widget_Helper(this);
}

Q_ARB_Widget::~Q_ARB_Widget()
{
    delete ui;
}

void Q_ARB_Widget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F5) {
        widget_Config[project]->updateARBStatus();
    }
}

void Q_ARB_Widget::init()
{
    widget_Config[project]->init();
}

void Q_ARB_Widget::on_pushButtonSelArbFile_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Select ARB File"),"",tr("ARB Files (*.wv)"));
    if (!path.isEmpty()) {
        ui->plainTextEditPath->setPlainText(path);
        widget_Config[project]->show();
    }
}

void Q_ARB_Widget::on_pushButtonLoad_clicked()
{
    widget_Config[project]->load();
}
