#include "dt3308_iq_cap_config_widget.h"

namespace NS_DT3308 {

class Q_IQ_Cap_Config_Model : public Q_Config_Table_Model
{
public:
    Q_IQ_Cap_Config_Model(QObject *parent = nullptr) :
        Q_Config_Table_Model(parent)
    {
        _item.clear();
        _item << "Samples"
              << "Source"
              << "Trigger Source";

        setRowCount(rowCount(QModelIndex()));
        setColumnCount(columnCount(QModelIndex()));
    }
};

} // namespace NS_DT3308


NS_DT3308::Q_IQ_Cap_Config_Delegate::Q_IQ_Cap_Config_Delegate(QObject *parent) :
    ::Q_IQ_Cap_Config_Delegate(parent)
{
    lineEditSamples = new QLineEdit;
    lineEditSamples->setFrame(false);
    connect(lineEditSamples,SIGNAL(textEdited(QString)),this,SLOT(setSamples(QString)));

    comboBoxSource = new QComboBox;
    comboBoxSource->setFrame(false);
    comboBoxSource->addItem("JESD");
    comboBoxSource->addItem("FIR_FLATNESS");
    comboBoxSource->addItem("DDC");
    comboBoxSource->addItem("DEC_24576");
    comboBoxSource->addItem("DEC_12288");
    comboBoxSource->addItem("DEC_6144");
    comboBoxSource->addItem("DEC_3072");
    comboBoxSource->addItem("DEC_1536");
    connect(comboBoxSource,SIGNAL(activated(int)),this,SLOT(setSource(int)));

    comboBoxTriggerSource = new QComboBox;
    comboBoxTriggerSource->setFrame(false);
    comboBoxTriggerSource->addItem("Free Run");
    comboBoxTriggerSource->addItem("80ms");
    comboBoxTriggerSource->addItem("UL_IF_Power");
    comboBoxTriggerSource->addItem("GPRF_IF_Power");
    comboBoxTriggerSource->addItem("Marker1");
    comboBoxTriggerSource->addItem("Marker2");
    comboBoxTriggerSource->addItem("Marker3");
    comboBoxTriggerSource->addItem("Marker4");
    comboBoxTriggerSource->addItem("DL_Generator_header");
    connect(comboBoxTriggerSource,SIGNAL(activated(int)),this,SLOT(setTriggerSource(int)));
}

void NS_DT3308::Q_IQ_Cap_Config_Delegate::setSamples(const QString &text)
{
    quint32 samples = text.toULong();;
    complexSequence->_new(samples);
    DT3308_BB_F->ddr()->set_fpga_w_samples(samples);
}

void NS_DT3308::Q_IQ_Cap_Config_Delegate::setSource(int index)
{
    DT3308_BB_F->set_iq_cap_src(bb_dt3308_f::iq_cap_src_t(index));
}

void NS_DT3308::Q_IQ_Cap_Config_Delegate::setTriggerSource(int index)
{
    DT3308_BB_F->set_iq_cap_trig(bb_dt3308_f::iq_cap_trig_t(index));
}


NS_DT3308::Q_IQ_Cap_Config_Widget_Helper::Q_IQ_Cap_Config_Widget_Helper(QWidget *parent) :
    ::Q_IQ_Cap_Config_Widget_Helper(parent)
{

}

void NS_DT3308::Q_IQ_Cap_Config_Widget_Helper::init()
{
    SAFE_NEW(model,Q_IQ_Cap_Config_Model);
    SAFE_NEW(delegate,Q_IQ_Cap_Config_Delegate);

    ::Q_IQ_Cap_Config_Widget_Helper::init();

    quint32 samples = 0;

    DT3308_BB_F->ddr()->get_fpga_w_samples(samples);

    Q_IQ_Cap_Config_Delegate *d = dynamic_cast<Q_IQ_Cap_Config_Delegate *>(delegate);
    d->lineEditSamples->setText(QString("%1").arg(samples));
    d->comboBoxSource->setCurrentIndex(0);
    d->comboBoxTriggerSource->setCurrentIndex(0);

    SAFE_NEW(complexSequence,complex_sequence);
    complexSequence->_new(samples);
}
