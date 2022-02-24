#include "sp9500x_arb_config_widget.h"
#include "arb_widget.h"
#include "ui_arb.h"

using namespace ns_arb;
using namespace ns_sp2406;

namespace NS_SP9500X {

class Q_ARB_Config_Model : public Q_Config_Table_Model
{
public:
    Q_ARB_Config_Model(QObject *parent = nullptr) :
        Q_Config_Table_Model(parent)
    {
        _item.clear();
        _item << "Source"
              << "Freq Offset(Hz)"
              << "Repetition"
              << "Cycles"
              << "Additional Samples"
              << "Retrigger"
              << "AutoStart"
              << "Trigger Source"
              << "" // Manual Trigger
              << "Trigger Slope"
              << "Trigger Delay"
              << "MultiSeg Repetition"
              << "MultiSeg Current Seg"
              << "MultiSeg Next Seg"
              << "MultiSeg Trigger Source"
              << "" // MultiSeg Manual Trigger
              << "Current Sample"
              << "Current Cycle"
              << "Current Time"
              << ""
              << "";

        setRowCount(rowCount(QModelIndex()));
        setColumnCount(columnCount(QModelIndex()));
    }
};

} // namespace NS_SP9500X


NS_SP9500X::Q_ARB_Config_Delegate::Q_ARB_Config_Delegate(QObject *parent) :
    ::Q_Config_Table_Delegate(parent)
{
    comboBoxSource = new QComboBox;
    comboBoxSource->setFrame(false);
    for (size_t i = 0;i < src_t::_size();++i) {
        comboBoxSource->addItem(QString::fromUtf8(src_t::_names()[i]));
    }

    lineEditFreqOffset = new QLineEdit;
    lineEditFreqOffset->setFrame(false);

    comboBoxRepetition = new QComboBox;
    comboBoxRepetition->setFrame(false);
    for (size_t i = 0;i < arb_repetition_t::_size();++i) {
        comboBoxRepetition->addItem(QString::fromUtf8(arb_repetition_t::_names()[i]));
    }

    lineEditCycles = new QLineEdit;
    lineEditCycles->setFrame(false);

    lineEditAdditionalSamples = new QLineEdit;
    lineEditAdditionalSamples->setFrame(false);

    checkBoxReTrigger = new QCheckBox;

    checkBoxAutoStart = new QCheckBox;

    comboBoxTriggerSource = new QComboBox;
    comboBoxTriggerSource->setFrame(false);
    for (size_t i = 0;i < arb_trig_src_t::_size();++i) {
        comboBoxTriggerSource->addItem(QString::fromUtf8(arb_trig_src_t::_names()[i]));
    }

    pushButtonManualTrigger = new QPushButton("Execute");

    comboBoxTriggerSlope = new QComboBox;
    comboBoxTriggerSlope->setFrame(false);
    for (size_t i = 0;i < arb_trig_edge_t::_size();++i) {
        comboBoxTriggerSlope->addItem(QString::fromUtf8(arb_trig_edge_t::_names()[i]));
    }

    lineEditTriggerDelay = new QLineEdit;
    lineEditTriggerDelay->setFrame(false);

    comboBoxMultiSegRepetition = new QComboBox;
    comboBoxMultiSegRepetition->setFrame(false);
    for (size_t i = 0;i < arb_multi_seg_repetition_t::_size();++i) {
        comboBoxMultiSegRepetition->addItem(QString::fromUtf8(arb_multi_seg_repetition_t::_names()[i]));
    }

    lineEditMultiSegCurrentSeg = new QLineEdit;
    lineEditMultiSegCurrentSeg->setFrame(false);
    lineEditMultiSegCurrentSeg->setEnabled(false);

    lineEditMultiSegNextSeg = new QLineEdit;
    lineEditMultiSegNextSeg->setFrame(false);

    comboBoxMultiSegTriggerSource = new QComboBox;
    comboBoxMultiSegTriggerSource->setFrame(false);
    comboBoxMultiSegTriggerSource->addItem("Manual");

    pushButtonMultiSegManualTrigger = new QPushButton("Execute");

    lineEditCurrentSample = new QLineEdit;
    lineEditCurrentSample->setFrame(false);
    lineEditCurrentSample->setEnabled(false);

    lineEditCurrentCycle = new QLineEdit;
    lineEditCurrentCycle->setFrame(false);
    lineEditCurrentCycle->setEnabled(false);

    lineEditCurrentTime = new QLineEdit;
    lineEditCurrentTime->setFrame(false);
    lineEditCurrentTime->setEnabled(false);

    pushButtonStart = new QPushButton("Start");
    pushButtonAbort = new QPushButton("Abort");

    connect(lineEditFreqOffset,SIGNAL(textEdited(QString)),this,SLOT(setFreqOffset()));
    connect(comboBoxRepetition,SIGNAL(activated(int)),this,SLOT(setRepetition()));
    connect(lineEditCycles,SIGNAL(textEdited(QString)),this,SLOT(setCycles()));
    connect(lineEditAdditionalSamples,SIGNAL(textEdited(QString)),this,SLOT(setAdditionalSamples()));
    connect(checkBoxReTrigger,SIGNAL(clicked(bool)),this,SLOT(setReTrigger()));
    connect(checkBoxAutoStart,SIGNAL(clicked(bool)),this,SLOT(setAutoStart()));
    connect(comboBoxTriggerSource,SIGNAL(activated(int)),this,SLOT(setTriggerSource()));
    connect(pushButtonManualTrigger,SIGNAL(clicked(bool)),this,SLOT(setManualTrigger()));
    connect(comboBoxTriggerSlope,SIGNAL(activated(int)),this,SLOT(setTriggerSlope()));
    connect(lineEditTriggerDelay,SIGNAL(textEdited(QString)),this,SLOT(setTriggerDelay()));

    connect(comboBoxMultiSegRepetition,SIGNAL(activated(int)),this,SLOT(setMultiSegRepetition()));
    connect(lineEditMultiSegNextSeg,SIGNAL(textEdited(QString)),this,SLOT(setMultiSegNextSeg()));
    connect(pushButtonMultiSegManualTrigger,SIGNAL(clicked(bool)),this,SLOT(setMultiSegManualTrigger()));

    connect(pushButtonStart,SIGNAL(clicked(bool)),this,SLOT(setStart()));
    connect(pushButtonAbort,SIGNAL(clicked(bool)),this,SLOT(setAbort()));
}

void NS_SP9500X::Q_ARB_Config_Delegate::setFreqOffset()
{

}

void NS_SP9500X::Q_ARB_Config_Delegate::setRepetition()
{
    arb_repetition_t rep = arb_repetition_t::_from_index(size_t(comboBoxRepetition->currentIndex()));
    SP2406->set_arb_repetition(rep);
}

void NS_SP9500X::Q_ARB_Config_Delegate::setCycles()
{
    SP2406->set_arb_cycles(lineEditCycles->text().toUShort());
}

void NS_SP9500X::Q_ARB_Config_Delegate::setAdditionalSamples()
{
    SP2406->set_arb_additional_samples(lineEditAdditionalSamples->text().toUInt());
}

void NS_SP9500X::Q_ARB_Config_Delegate::setReTrigger()
{
    SP2406->set_arb_retrig(checkBoxReTrigger->isChecked());
}

void NS_SP9500X::Q_ARB_Config_Delegate::setAutoStart()
{

}

void NS_SP9500X::Q_ARB_Config_Delegate::setTriggerSource()
{
    arb_trig_src_t trigSrc = arb_trig_src_t::_from_index(size_t(comboBoxTriggerSource->currentIndex()));
    SP2406->set_arb_trig_src(trigSrc);
}

void NS_SP9500X::Q_ARB_Config_Delegate::setManualTrigger()
{
    SP2406->set_arb_manual_trig();
}

void NS_SP9500X::Q_ARB_Config_Delegate::setTriggerSlope()
{
    arb_trig_edge_t trigEdge = arb_trig_edge_t::_from_index(size_t(comboBoxTriggerSlope->currentIndex()));
    SP2406->set_arb_trig_edge(trigEdge);
}

void NS_SP9500X::Q_ARB_Config_Delegate::setTriggerDelay()
{

}

void NS_SP9500X::Q_ARB_Config_Delegate::setMultiSegRepetition()
{
    arb_multi_seg_repetition_t rep = arb_multi_seg_repetition_t::_from_index(size_t(comboBoxMultiSegRepetition->currentIndex()));
    SP2406->set_arb_multi_seg_repetition(rep);
}

void NS_SP9500X::Q_ARB_Config_Delegate::getMultiSegCurrentSeg()
{

}

void NS_SP9500X::Q_ARB_Config_Delegate::setMultiSegNextSeg()
{
    SP2406->set_arb_multi_seg_next_seg(quint8(lineEditMultiSegNextSeg->text().toUShort()));
}

void NS_SP9500X::Q_ARB_Config_Delegate::setMultiSegManualTrigger()
{
    SP2406->set_arb_multi_seg_manual_trig();
}

void NS_SP9500X::Q_ARB_Config_Delegate::getCurrentSample()
{

}

void NS_SP9500X::Q_ARB_Config_Delegate::getCurrentCycle()
{

}

void NS_SP9500X::Q_ARB_Config_Delegate::getCurrentTime()
{

}

void NS_SP9500X::Q_ARB_Config_Delegate::setStart()
{
    SP2406->set_arb_start();
}

void NS_SP9500X::Q_ARB_Config_Delegate::setAbort()
{
    SP2406->set_arb_abort();
}


NS_SP9500X::Q_ARB_Config_Widget_Helper::Q_ARB_Config_Widget_Helper(QWidget *parent) :
    ::Q_ARB_Config_Widget_Helper(parent)
{

}

void NS_SP9500X::Q_ARB_Config_Widget_Helper::init()
{
    SAFE_NEW(model,Q_ARB_Config_Model);
    SAFE_NEW(delegate,Q_ARB_Config_Delegate);

    ::Q_ARB_Config_Widget_Helper::init();

    arb_repetition_t rep = arb_repetition_t::Single_Shot;
    quint16 cycles = 0;
    quint32 additionalSamples = 0;
    bool reTrigger = false;
    arb_trig_src_t trigSrc = arb_trig_src_t::AutoStart;
    arb_trig_edge_t trigEdge = arb_trig_edge_t::Posedge;

    arb_multi_seg_repetition_t multiSegRep = arb_multi_seg_repetition_t::Auto;
    quint8 nextSeg = 0;

    INT_CHECKV(SP2406->get_arb_repetition(rep));
    INT_CHECKV(SP2406->get_arb_cycles(cycles));
    INT_CHECKV(SP2406->get_arb_additional_samples(additionalSamples));
    INT_CHECKV(SP2406->get_arb_retrig(reTrigger));
    INT_CHECKV(SP2406->get_arb_trig_src(trigSrc));
    INT_CHECKV(SP2406->get_arb_trig_edge(trigEdge));

    INT_CHECKV(SP2406->get_arb_multi_seg_repetition(multiSegRep));
    INT_CHECKV(SP2406->get_arb_multi_seg_next_seg(nextSeg));

    Q_ARB_Config_Delegate *d = dynamic_cast<Q_ARB_Config_Delegate *>(delegate);

    d->lineEditFreqOffset->setText("0");
    d->comboBoxRepetition->setCurrentIndex(int(rep._to_index()));
    d->lineEditCycles->setText(QString("%1").arg(cycles));
    d->lineEditAdditionalSamples->setText(QString("%1").arg(additionalSamples));
    d->checkBoxReTrigger->setChecked(reTrigger);
    d->checkBoxAutoStart->setChecked(true);
    d->comboBoxTriggerSource->setCurrentIndex(int(trigSrc._to_index()));
    d->comboBoxTriggerSlope->setCurrentIndex(int(trigEdge._to_index()));
    d->lineEditTriggerDelay->setText("0");

    d->comboBoxMultiSegRepetition->setCurrentIndex(int(multiSegRep._to_index()));
    d->lineEditMultiSegCurrentSeg->setText("0");
    d->lineEditMultiSegNextSeg->setText(QString("%1").arg(nextSeg));
    d->comboBoxMultiSegTriggerSource->setCurrentIndex(0);

    d->lineEditCurrentSample->setText("0");
    d->lineEditCurrentCycle->setText("0");
    d->lineEditCurrentTime->setText("0");
}

void NS_SP9500X::Q_ARB_Config_Widget_Helper::show()
{
    Q_ARB_Widget *p = dynamic_cast<Q_ARB_Widget *>(parent);
    std::string fileInfo;

    SP2406->arb()->file()->load(p->ui->plainTextEditPath->toPlainText().toStdString().c_str());
    SP2406->arb()->file()->get_file_info(fileInfo);
    p->ui->plainTextEditInfo->setPlainText(QString::fromStdString(fileInfo));
}

void NS_SP9500X::Q_ARB_Config_Widget_Helper::load()
{
    Q_ARB_Config_Delegate *d = dynamic_cast<Q_ARB_Config_Delegate *>(delegate);
    Q_ARB_Widget *p = dynamic_cast<Q_ARB_Widget *>(parent);

    src_t source = src_t::_from_index(size_t(d->comboBoxSource->currentIndex()));
    SP3103->arb_load(RFIdx,source,p->ui->plainTextEditPath->toPlainText().toStdString());
}

void NS_SP9500X::Q_ARB_Config_Widget_Helper::updateARBStatus()
{
    Q_ARB_Config_Delegate *d = dynamic_cast<Q_ARB_Config_Delegate *>(delegate);
}
