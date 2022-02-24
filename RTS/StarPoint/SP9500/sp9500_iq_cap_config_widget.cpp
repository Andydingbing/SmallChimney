#include "sp9500_iq_cap_config_widget.h"

namespace NS_SP9500 {

class Q_IQ_Cap_Config_Model : public Q_Config_Table_Model
{
public:
    Q_IQ_Cap_Config_Model(QObject *parent = nullptr) :
        Q_Config_Table_Model(parent)
    {
        _item.clear();
        _item << "Samples"
              << "Trigger Source"
              << "Trigger Edge"
              << "Trigger Offset"
              << "Trigger Gap"
              << "Capture Offset"
              << "Timeout(us)"
              << "Threshold"
              << "Frame Trigger Mode"
              << "TrigIn Offset(ms)"
              << "TrigOut Offset(ms)"
              << "Frame Time(ms)"
              << "HF Trigger Mode"
              << "TrigIn Offset(ms)"
              << "TrigOut Offset(ms)"
              << "HF Period"
              << "Slot Time(ms)"
              << "Frame&Slot Trig Source"
              << "Trigger Frame"
              << "Trigger Slot"
              << "X"
              << "Y";

        setRowCount(rowCount(QModelIndex()));
        setColumnCount(columnCount(QModelIndex()));
    }
};

} // namespace NS_SP9500


NS_SP9500::Q_IQ_Cap_Config_Delegate::Q_IQ_Cap_Config_Delegate(QObject *parent) :
    ::Q_IQ_Cap_Config_Delegate(parent)
{
    lineEditSamples = new QLineEdit;
    lineEditSamples->setFrame(false);
    connect(lineEditSamples,SIGNAL(textEdited(QString)),this,SLOT(setSamples(QString)));

    comboBoxTriggerSource = new QComboBox;
    comboBoxTriggerSource->setFrame(false);
    comboBoxTriggerSource->addItem("Free Run");
    comboBoxTriggerSource->addItem("Int Trig");
    connect(comboBoxTriggerSource,SIGNAL(activated(int)),this,SLOT(setTriggerSource(int)));

    comboBoxTriggerEdge = new QComboBox;
    comboBoxTriggerEdge->setFrame(false);
    comboBoxTriggerEdge->addItem("Posedge");
    comboBoxTriggerEdge->addItem("Negedge");

    lineEditTriggerOffset = new QLineEdit;
    lineEditTriggerOffset->setFrame(false);
    connect(lineEditTriggerOffset,SIGNAL(textEdited(QString)),this,SLOT(setTriggerOffset(QString)));

    lineEditTriggerGap = new QLineEdit;
    lineEditTriggerGap->setFrame(false);

    lineEditCaptureOffset = new QLineEdit;
    lineEditCaptureOffset->setFrame(false);

    lineEditTimeout = new QLineEdit;
    lineEditTimeout->setFrame(false);
    connect(lineEditTimeout,SIGNAL(textEdited(QString)),this,SLOT(setTimeout(QString)));

    lineEditThreshold = new QLineEdit;
    lineEditThreshold->setFrame(false);
    connect(lineEditThreshold,SIGNAL(textEdited(QString)),this,SLOT(setThreshold(QString)));

    comboBoxFrameTriggerMode = new QComboBox;
    comboBoxFrameTriggerMode->setFrame(false);
    comboBoxFrameTriggerMode->addItem("Trigger In");
    comboBoxFrameTriggerMode->addItem("Trigger Out");

    lineEditFrameTriggerInOffset = new QLineEdit;
    lineEditFrameTriggerInOffset->setFrame(false);

    lineEditFrameTriggerOutOffset = new QLineEdit;
    lineEditFrameTriggerOutOffset->setFrame(false);

    lineEditFrameTime = new QLineEdit;
    lineEditFrameTime->setFrame(false);

    comboBoxHFTriggerMode = new QComboBox;
    comboBoxHFTriggerMode->setFrame(false);
    comboBoxHFTriggerMode->addItem("Trigger In");
    comboBoxHFTriggerMode->addItem("Trigger Out");

    lineEditHFTriggerInOffset = new QLineEdit;
    lineEditHFTriggerInOffset->setFrame(false);

    lineEditHFTriggerOutOffset = new QLineEdit;
    lineEditHFTriggerOutOffset->setFrame(false);

    lineEditHFPeriod = new QLineEdit;
    lineEditHFPeriod->setFrame(false);

    lineEditSlotTime = new QLineEdit;
    lineEditSlotTime->setFrame(false);

    comboBoxFrameTriggerSource = new QComboBox;
    comboBoxFrameTriggerSource->setFrame(false);
    comboBoxFrameTriggerSource->addItem("Next Frame");
    comboBoxFrameTriggerSource->addItem("Spec Frame");
    comboBoxFrameTriggerSource->addItem("Frame%X=Y");

    lineEditTriggerFrame = new QLineEdit;
    lineEditTriggerFrame->setFrame(false);

    lineEditTriggerSlot = new QLineEdit;
    lineEditTriggerSlot->setFrame(false);

    lineEditX = new QLineEdit;
    lineEditX->setFrame(false);

    lineEditY = new QLineEdit;
    lineEditY->setFrame(false);
}

void NS_SP9500::Q_IQ_Cap_Config_Delegate::setSamples(const QString &text)
{
    quint32 samples = text.toULong();;
    complexSequence->_new(samples);
    SP3301->set_iq_cap_samples(RFIdx,samples);
}

void NS_SP9500::Q_IQ_Cap_Config_Delegate::setTriggerSource(int index)
{
    SP1401->set_iq_cap_src(sp1401::iq_cap_src_t(index),true);
}

void NS_SP9500::Q_IQ_Cap_Config_Delegate::setTriggerOffset(const QString &text)
{
    SP3301->set_iq_cap_frame_trig_offset(RFIdx,text.toUInt());
}

void NS_SP9500::Q_IQ_Cap_Config_Delegate::setTimeout(const QString &text)
{
    SP1401->set_iq_cap_timeout(text.toULong());
}

void NS_SP9500::Q_IQ_Cap_Config_Delegate::setThreshold(const QString &text)
{
    SP3301->set_iq_cap_trig_level(RFIdx,text.toFloat());
}


NS_SP9500::Q_IQ_Cap_Config_Widget_Helper::Q_IQ_Cap_Config_Widget_Helper(QWidget *parent) :
    ::Q_IQ_Cap_Config_Widget_Helper(parent)
{

}

void NS_SP9500::Q_IQ_Cap_Config_Widget_Helper::init()
{
    SAFE_NEW(model,Q_IQ_Cap_Config_Model);
    SAFE_NEW(delegate,Q_IQ_Cap_Config_Delegate);

    ::Q_IQ_Cap_Config_Widget_Helper::init();

    quint32 samples = 0;
    sp2401_r1a::iq_cap_src_t src = sp2401_r1a::PWR_MEAS_FREE_RUN;
    bool posedge = false;

    SP3301->get_iq_cap_samples(RFIdx,samples);
    SP2401->get_iq_cap_src(src,posedge);

    Q_IQ_Cap_Config_Delegate *d = dynamic_cast<Q_IQ_Cap_Config_Delegate *>(delegate);
    d->lineEditSamples->setText(QString("%1").arg(samples));
    d->comboBoxTriggerSource->setCurrentIndex(src);
    d->comboBoxTriggerEdge->setCurrentIndex(posedge ? 0 : 1);
    d->lineEditTriggerOffset->setText("0");
    d->lineEditTriggerGap ->setText("0");
    d->lineEditCaptureOffset->setText("0");
    d->lineEditTimeout->setText("6553600");
    d->lineEditThreshold->setText("0");

    SAFE_NEW(complexSequence,complex_sequence);
    complexSequence->_new(samples);
}
