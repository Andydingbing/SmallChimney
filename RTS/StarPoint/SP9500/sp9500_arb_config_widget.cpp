#include "sp9500_arb_config_widget.h"
#include "arb_widget.h"
#include "ui_arb.h"

namespace NS_SP9500 {

class Q_ARB_Config_Model : public Q_Config_Table_Model
{
public:
    Q_ARB_Config_Model(QObject *parent = nullptr) :
        Q_Config_Table_Model(parent)
    {
        _item.clear();
        _item << "Trigger Source"
              << "Freq Offset(MHz)"
              << "ReTrigger"
              << "Cycles"
              << "Trigger Delay"
              << "AutoStart"
              << "Additional Samples"
              << "Repeat Mode"
              << "MultiSeg Trigger Source"
              << "MultiSeg Repeat Mode"
              << "Next Seg"
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
              << "Y"
              << "Current Seg"
              << "Current Sample"
              << "Current Cycle"
              << "Current Time"
              << "Current Frame"
              << "Current Slot"
              << ""
              << ""
              << ""
              << "";

        setRowCount(rowCount(QModelIndex()));
        setColumnCount(columnCount(QModelIndex()));
    }
};

} // namespace NS_SP9500


NS_SP9500::Q_ARB_Config_Delegate::Q_ARB_Config_Delegate(QObject *parent) :
    ::Q_Config_Table_Delegate(parent)
{
    comboBoxTriggerSource = new QComboBox;
    comboBoxTriggerSource->setFrame(false);
    comboBoxTriggerSource->addItem("Reserve");
    comboBoxTriggerSource->addItem("Manual");
    comboBoxTriggerSource->addItem("Reserve");
    comboBoxTriggerSource->addItem("Reserve");
    comboBoxTriggerSource->addItem("External A");
    comboBoxTriggerSource->addItem("External B");
    comboBoxTriggerSource->addItem("External C");
    comboBoxTriggerSource->addItem("External D");

    lineEditFreqOffset = new QLineEdit;
    lineEditFreqOffset->setFrame(false);

    checkBoxReTrigger = new QCheckBox;

    lineEditCycles = new QLineEdit;
    lineEditCycles->setFrame(false);

    lineEditTriggerDelay = new QLineEdit;
    lineEditTriggerDelay->setFrame(false);

    checkBoxAutoStart = new QCheckBox;

    lineEditAdditionalSamples = new QLineEdit;
    lineEditAdditionalSamples->setFrame(false);

    comboBoxRepeatMode = new QComboBox;
    comboBoxRepeatMode->setFrame(false);
    comboBoxRepeatMode->addItem("Single Shot");
    comboBoxRepeatMode->addItem("Continuous");

    comboBoxMultiSegTriggerSource = new QComboBox;
    comboBoxMultiSegTriggerSource->setFrame(false);
    comboBoxMultiSegTriggerSource->addItem("Manual");

    comboBoxMultiSegRepeatMode = new QComboBox;
    comboBoxMultiSegRepeatMode->setFrame(false);
    comboBoxMultiSegRepeatMode->addItem("Auto");
    comboBoxMultiSegRepeatMode->addItem("Continuous");
    comboBoxMultiSegRepeatMode->addItem("Continuous Seamless");

    lineEditNextSeg = new QLineEdit;
    lineEditNextSeg->setFrame(false);

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

    lineEditCurrentSeg = new QLineEdit;
    lineEditCurrentSeg->setFrame(false);
    lineEditCurrentSeg->setReadOnly(true);

    lineEditCurrentSample = new QLineEdit;
    lineEditCurrentSample->setFrame(false);
    lineEditCurrentSample->setReadOnly(true);

    lineEditCurrentCycle = new QLineEdit;
    lineEditCurrentCycle->setFrame(false);
    lineEditCurrentCycle->setReadOnly(true);

    lineEditCurrentTime = new QLineEdit;
    lineEditCurrentTime->setFrame(false);
    lineEditCurrentTime->setReadOnly(true);

    lineEditCurrentFrame = new QLineEdit;
    lineEditCurrentFrame->setFrame(false);
    lineEditCurrentFrame->setReadOnly(true);

    lineEditCurrentSlot = new QLineEdit;
    lineEditCurrentSlot->setFrame(false);
    lineEditCurrentSlot->setReadOnly(true);

    pushButtonFrameTriggerStart = new QPushButton("Frame Trigger Start");
    pushButtonManualTrigger = new QPushButton("Manual Trigger");
    pushButtonARBStart = new QPushButton("ARB Start");
    pushButtonARBStop = new QPushButton("ARB Stop");

    connect(comboBoxTriggerSource,SIGNAL(activated(int)),this,SLOT(setARBTrigger()));
    connect(checkBoxReTrigger,SIGNAL(clicked(bool)),this,SLOT(setARBTrigger()));
    connect(lineEditCycles,SIGNAL(textEdited(QString)),this,SLOT(setARBParam()));
    connect(lineEditTriggerDelay,SIGNAL(textEdited(QString)),this,SLOT(setARBTrigger()));
    connect(checkBoxAutoStart,SIGNAL(clicked(bool)),this,SLOT(setARBTrigger()));
    connect(lineEditAdditionalSamples,SIGNAL(textEdited(QString)),this,SLOT(setARBParam()));
    connect(comboBoxRepeatMode,SIGNAL(activated(int)),this,SLOT(setARBParam()));
    connect(comboBoxFrameTriggerMode,SIGNAL(activated(int)),this,SLOT(setFrameTriggerMode()));
    connect(lineEditFrameTriggerInOffset,SIGNAL(textEdited(QString)),this,SLOT(setFrameTriggerInOffset()));
    connect(lineEditFrameTriggerOutOffset,SIGNAL(textEdited(QString)),this,SLOT(setFrameTriggerOutOffset()));
    connect(lineEditFrameTime,SIGNAL(textEdited(QString)),this,SLOT(setFrameTime()));
    connect(comboBoxHFTriggerMode,SIGNAL(activated(int)),this,SLOT(setHFTriggerMode()));
    connect(lineEditHFTriggerInOffset,SIGNAL(textEdited(QString)),this,SLOT(setHFTriggerInOffset()));
    connect(lineEditHFTriggerOutOffset,SIGNAL(textEdited(QString)),this,SLOT(setHFTriggerOutOffset()));
    connect(lineEditHFPeriod,SIGNAL(textEdited(QString)),this,SLOT(setHFPeriod()));
    connect(lineEditSlotTime,SIGNAL(textEdited(QString)),this,SLOT(setSlotTime()));
    connect(comboBoxFrameTriggerSource,SIGNAL(activated(int)),this,SLOT(setFrameTriggerSource()));
    connect(lineEditTriggerFrame,SIGNAL(textEdited(QString)),this,SLOT(setTriggerFrame()));
    connect(lineEditTriggerSlot,SIGNAL(textEdited(QString)),this,SLOT(setTriggerSlot()));
    connect(lineEditX,SIGNAL(textEdited(QString)),this,SLOT(setFrameTriggerModXY()));
    connect(lineEditY,SIGNAL(textEdited(QString)),this,SLOT(setFrameTriggerModXY()));
    connect(pushButtonManualTrigger,SIGNAL(clickd(bool)),this,SLOT(setManualTrigger()));
    connect(pushButtonARBStart,SIGNAL(clicked(bool)),this,SLOT(setARBStart()));
    connect(pushButtonARBStop,SIGNAL(clicked(bool)),this,SLOT(setARBStop()));
}

void NS_SP9500::Q_ARB_Config_Delegate::setARBTrigger()
{
    bool reTrigger = checkBoxReTrigger->isChecked();
    bool autoStart = checkBoxAutoStart->isChecked();
    quint32 trigSrc = comboBoxTriggerSource->currentIndex();
    quint32 trigDelay = lineEditTriggerDelay->text().toUInt();
    SP1401->set_arb_trigger(reTrigger,autoStart,trigSrc,trigDelay);
}

void NS_SP9500::Q_ARB_Config_Delegate::setARBParam()
{
    quint32 additionalSamples = lineEditAdditionalSamples->text().toUInt();
    quint32 cycles = lineEditCycles->text().toUInt();
    quint32 repeatMode = comboBoxRepeatMode->currentIndex();

    SP1401->set_arb_param(additionalSamples,cycles,repeatMode);
}

void NS_SP9500::Q_ARB_Config_Delegate::setFrameTriggerMode()
{
    sp2401_r1a::frame_trig_mode_t frameTriggerMode =
            sp2401_r1a::frame_trig_mode_t(comboBoxFrameTriggerMode->currentIndex());

    SP2401->set_frame_trig_mode(frameTriggerMode);
}

void NS_SP9500::Q_ARB_Config_Delegate::setFrameTriggerInOffset()
{
    double frameTriggerInOffset = lineEditFrameTriggerInOffset->text().toDouble();
    bool en = frameTriggerInOffset != 0.0 ? true : false;

    SP2401->set_frame_trig_in_offset(en,frameTriggerInOffset);
}

void NS_SP9500::Q_ARB_Config_Delegate::setFrameTriggerOutOffset()
{
    double frameTriggerOutOffset = lineEditFrameTriggerOutOffset->text().toDouble();
    bool en = frameTriggerOutOffset != 0.0 ? true : false;

    SP2401->set_frame_trig_out_offset(en,frameTriggerOutOffset);
}

void NS_SP9500::Q_ARB_Config_Delegate::setFrameTime()
{
    SP2401->set_frame_time(lineEditFrameTime->text().toDouble());
}

void NS_SP9500::Q_ARB_Config_Delegate::setHFTriggerMode()
{
    sp2401_r1a::hyper_frame_trig_mode_t HFTriggerMode =
            sp2401_r1a::hyper_frame_trig_mode_t(comboBoxHFTriggerMode->currentIndex());

    SP2401->set_hyper_frame_trig_mode(HFTriggerMode);
}

void NS_SP9500::Q_ARB_Config_Delegate::setHFTriggerInOffset()
{
    double HFTriggerInOffset = lineEditHFTriggerInOffset->text().toDouble();
    bool en = HFTriggerInOffset != 0.0 ? true : false;

    SP2401->set_hyper_frame_trig_in_offset(en,HFTriggerInOffset);
}

void NS_SP9500::Q_ARB_Config_Delegate::setHFTriggerOutOffset()
{
    double HFTriggerOutOffset = lineEditHFTriggerOutOffset->text().toDouble();
    bool en = HFTriggerOutOffset != 0.0 ? true : false;

    SP2401->set_hyper_frame_trig_out_offset(en,HFTriggerOutOffset);
}

void NS_SP9500::Q_ARB_Config_Delegate::setHFPeriod()
{
    SP2401->set_hyper_frame_period(lineEditHFPeriod->text().toUInt());
}

void NS_SP9500::Q_ARB_Config_Delegate::setSlotTime()
{
    SP2401->set_slot_time(lineEditSlotTime->text().toDouble());
}

void NS_SP9500::Q_ARB_Config_Delegate::setFrameTriggerSource()
{
    sp2401_r1a::frame_trig_src_t source =
            sp2401_r1a::frame_trig_src_t(comboBoxFrameTriggerSource->currentIndex());

    SP2401->set_frame_trig_src(source);
}

void NS_SP9500::Q_ARB_Config_Delegate::setTriggerFrame()
{
    SP2401->set_trig_frame(lineEditTriggerFrame->text().toUInt());
}

void NS_SP9500::Q_ARB_Config_Delegate::setTriggerSlot()
{
    SP2401->set_trig_slot(lineEditTriggerSlot->text().toUInt());
}

void NS_SP9500::Q_ARB_Config_Delegate::setFrameTriggerModXY()
{
    quint16 X = lineEditX->text().toUInt();
    quint16 Y = lineEditY->text().toUInt();

    SP2401->set_frame_trig_mod_x_y(X,Y);
}

void NS_SP9500::Q_ARB_Config_Delegate::setManualTrigger()
{
    SP1401->set_arb_manual_trigger();
}

void NS_SP9500::Q_ARB_Config_Delegate::setARBStart()
{
    SP1401->arb_start();
}

void NS_SP9500::Q_ARB_Config_Delegate::setARBStop()
{
    SP1401->arb_stop();
}


NS_SP9500::Q_ARB_Config_Widget_Helper::Q_ARB_Config_Widget_Helper(QWidget *parent) :
    ::Q_ARB_Config_Widget_Helper(parent)
{

}

void NS_SP9500::Q_ARB_Config_Widget_Helper::init()
{
    SAFE_NEW(model,Q_ARB_Config_Model);
    SAFE_NEW(delegate,Q_ARB_Config_Delegate);

    ::Q_ARB_Config_Widget_Helper::init();

    Q_ARB_Config_Delegate *d = dynamic_cast<Q_ARB_Config_Delegate *>(delegate);

    d->comboBoxTriggerSource->setCurrentIndex(sp1401::TS_MANUAL);
    d->lineEditFreqOffset->setText("0");
    d->lineEditCycles->setText("1");
    d->lineEditTriggerDelay->setText("0");
    d->lineEditAdditionalSamples->setText("0");
    d->comboBoxRepeatMode->setCurrentIndex(sp1401::RM_CONTINUOUS);
    d->lineEditNextSeg->setText("0");

    d->comboBoxFrameTriggerMode->setCurrentIndex(sp2401_r1a::TRIGGER_IN);
    d->lineEditFrameTriggerInOffset->setText("0");
    d->lineEditFrameTriggerOutOffset->setText("0");
    d->lineEditFrameTime->setText("10");
    d->comboBoxHFTriggerMode->setCurrentIndex(sp2401_r1a::TRIGGER_IN);
    d->lineEditHFTriggerInOffset->setText("0");
    d->lineEditHFTriggerOutOffset->setText("0");
    d->lineEditHFPeriod->setText("1024");
    d->lineEditSlotTime->setText("0.5");
    d->comboBoxFrameTriggerSource->setCurrentIndex(sp2401_r1a::NEXT_FRAME);
    d->lineEditTriggerFrame->setText("0");
    d->lineEditTriggerSlot->setText("0");
    d->lineEditX->setText("1");
    d->lineEditY->setText("1");

    d->lineEditCurrentSeg->setText("0");
    d->lineEditCurrentSample->setText("0");
    d->lineEditCurrentCycle->setText("0");
    d->lineEditCurrentTime->setText("0");
    d->lineEditCurrentFrame->setText("0");
    d->lineEditCurrentSlot->setText("0");
}

void NS_SP9500::Q_ARB_Config_Widget_Helper::show()
{
    Q_ARB_Widget *p = dynamic_cast<Q_ARB_Widget *>(parent);
    std::string fileInfo;

    SP1401->arb()->load(p->ui->plainTextEditPath->toPlainText().toStdString().c_str());
    SP1401->arb()->get_file_info(fileInfo);
    p->ui->plainTextEditInfo->setPlainText(QString::fromStdString(fileInfo));
}

void NS_SP9500::Q_ARB_Config_Widget_Helper::load()
{
    Q_ARB_Widget *p = dynamic_cast<Q_ARB_Widget *>(parent);

    SP3301->arb_load(RFIdx,p->ui->plainTextEditPath->toPlainText().toStdString().c_str());
}

void NS_SP9500::Q_ARB_Config_Widget_Helper::updateARBStatus()
{
    Q_ARB_Config_Delegate *d = dynamic_cast<Q_ARB_Config_Delegate *>(delegate);

    d->lineEditCurrentSeg->setText(QString("%1").arg(SP1401->get_arb_current_seg()));
    d->lineEditCurrentSample->setText(QString("%1").arg(SP1401->get_arb_current_sample()));
    d->lineEditCurrentCycle->setText(QString("%1").arg(SP1401->get_arb_current_cycle()));
    d->lineEditCurrentTime->setText(QString("%1").arg(SP1401->get_arb_current_timer()));
    d->lineEditCurrentFrame->setText(QString("%1").arg(SP2401->get_cur_frame()));
    d->lineEditCurrentSlot->setText(QString("%1").arg(SP2401->get_cur_slot()));
}
