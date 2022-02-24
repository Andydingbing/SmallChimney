#include "sp9500x_iq_cap_config_widget.h"

using namespace ns_sp2406;

namespace NS_SP9500X {

class Q_IQ_Cap_Config_Model : public Q_Config_Table_Model
{
public:
    Q_IQ_Cap_Config_Model(QObject *parent = nullptr) :
        Q_Config_Table_Model(parent)
    {
        _item.clear();
        _item << "Samples"
              << "Source"
              << "Source DDC"
              << "DDC Ch"
              << "DL CPRI Ch"
              << "Source DUC"
              << "DUC Ch"
              << "Rate"
              << "Mode"
              << "Trigger Source"
              << "Trigger Edge"
              << "Trigger Offset"
              << "Trigger Gap"
              << "Timeout(us)"
              << "Threshold"
              << "Trigger Frame"
              << "X"
              << "Y";

        setRowCount(rowCount(QModelIndex()));
        setColumnCount(columnCount(QModelIndex()));
    }
};

} // namespace NS_SP9500X


NS_SP9500X::Q_IQ_Cap_Config_Delegate::Q_IQ_Cap_Config_Delegate(QObject *parent) :
    ::Q_IQ_Cap_Config_Delegate(parent)
{
    lineEditSamples = new QLineEdit;
    lineEditSamples->setFrame(false);
    connect(lineEditSamples,SIGNAL(textEdited(QString)),this,SLOT(setSamples(QString)));

    comboBoxSource = new QComboBox;
    comboBoxSource->setFrame(false);
    for (size_t i = 0;i < iq_cap_src_t::_size();++i) {
        comboBoxSource->addItem(QString::fromUtf8(iq_cap_src_t::_names()[i]));
    }
    connect(comboBoxSource,SIGNAL(activated(int)),this,SLOT(setCaptureSource(int)));

    comboBoxSourceDDC = new QComboBox;
    comboBoxSourceDDC->setFrame(false);
    comboBoxSourceDDC_Ch = new QComboBox;
    comboBoxSourceDDC_Ch->setFrame(false);

    comboBoxSourceDL_CPRI_Ch = new QComboBox;
    comboBoxSourceDL_CPRI_Ch->setFrame(false);
    comboBoxSourceDUC = new QComboBox;
    comboBoxSourceDUC->setFrame(false);
    comboBoxSourceDUC_Ch = new QComboBox;
    comboBoxSourceDUC_Ch->setFrame(false);

    for (size_t i = 0;i < iq_cap_src_ddc_t::_size();++i) {
        comboBoxSourceDDC->addItem(QString::fromUtf8(iq_cap_src_ddc_t::_names()[i]));
    }
    for (size_t i = 0;i < iq_cap_src_duc_t::_size();++i) {
        comboBoxSourceDUC->addItem(QString::fromUtf8(iq_cap_src_duc_t::_names()[i]));
    }
    for (size_t i = 0;i < 8;++i) {
        comboBoxSourceDDC_Ch->addItem(QString("%1").arg(i));
        comboBoxSourceDUC_Ch->addItem(QString("%1").arg(i));
        comboBoxSourceDL_CPRI_Ch->addItem(QString("%1").arg(i));
    }
    connect(comboBoxSourceDDC,SIGNAL(activated(int)),this,SLOT(setCaptureSourceDDC(int)));
    connect(comboBoxSourceDDC_Ch,SIGNAL(activated(int)),this,SLOT(setCaptureSourceDDC_Ch(int)));
    connect(comboBoxSourceDL_CPRI_Ch,SIGNAL(activated(int)),this,SLOT(setCaptureSourceDL_CPRI_Ch(int)));
    connect(comboBoxSourceDUC,SIGNAL(activated(int)),this,SLOT(setCaptureSourceDUC(int)));
    connect(comboBoxSourceDUC_Ch,SIGNAL(activated(int)),this,SLOT(setCaptureSourceDUC_Ch(int)));

    comboBoxSR = new QComboBox;
    comboBoxSR->setFrame(false);
    for (size_t i = 0;i < iq_cap_sr_t::_size();++i) {
        comboBoxSR->addItem(QString::fromUtf8(iq_cap_sr_t::_names()[i]));
    }
    connect(comboBoxSR,SIGNAL(activated(int)),this,SLOT(setCaptureSR(int)));

    comboBoxMode = new QComboBox;
    comboBoxMode->setFrame(false);
    for (size_t i = 0;i < iq_cap_mode_t::_size();++i) {
        comboBoxMode->addItem(QString::fromUtf8(iq_cap_mode_t::_names()[i]));
    }
    connect(comboBoxMode,SIGNAL(activated(int)),this,SLOT(setCaptureMode(int)));

    comboBoxTriggerSource = new QComboBox;
    comboBoxTriggerSource->setFrame(false);
    comboBoxTriggerSource->setMaxVisibleItems(iq_cap_trig_src_t::_size());
    for (size_t i = 0;i < iq_cap_trig_src_t::_size();++i) {
        comboBoxTriggerSource->addItem(QString::fromUtf8(iq_cap_trig_src_t::_names()[i]));
    }
    connect(comboBoxTriggerSource,SIGNAL(activated(int)),this,SLOT(setTriggerSource(int)));

    comboBoxTriggerEdge = new QComboBox;
    comboBoxTriggerEdge->setFrame(false);
    for (size_t i = 0;i < iq_cap_trig_edge_t::_size();++i) {
        comboBoxTriggerEdge->addItem(QString::fromUtf8(iq_cap_trig_edge_t::_names()[i]));
    }
    connect(comboBoxTriggerEdge,SIGNAL(activated(int)),this,SLOT(setTriggerEdge(int)));

    lineEditTriggerOffset = new QLineEdit;
    lineEditTriggerOffset->setFrame(false);
    connect(lineEditTriggerOffset,SIGNAL(textEdited(QString)),this,SLOT(setTriggerOffset(QString)));

    lineEditTriggerGap = new QLineEdit;
    lineEditTriggerGap->setFrame(false);
    connect(lineEditTriggerGap,SIGNAL(textEdited(QString)),this,SLOT(setTriggerGap(QString)));

    lineEditTimeout = new QLineEdit;
    lineEditTimeout->setFrame(false);
    connect(lineEditTimeout,SIGNAL(textEdited(QString)),this,SLOT(setTimeout(QString)));

    lineEditThreshold = new QLineEdit;
    lineEditThreshold->setFrame(false);
    connect(lineEditThreshold,SIGNAL(textEdited(QString)),this,SLOT(setThreshold(QString)));

    lineEditTriggerFrame = new QLineEdit;
    lineEditTriggerFrame->setFrame(false);
    connect(lineEditTriggerFrame,SIGNAL(textEdited(QString)),this,SLOT(setTriggerFrame(QString)));

    lineEditTriggerFrameX = new QLineEdit;
    lineEditTriggerFrameX->setFrame(false);
    connect(lineEditTriggerFrameX,SIGNAL(textEdited(QString)),this,SLOT(setTriggerFrameX(QString)));

    lineEditTriggerFrameY = new QLineEdit;
    lineEditTriggerFrameY->setFrame(false);
    connect(lineEditTriggerFrameY,SIGNAL(textEdited(QString)),this,SLOT(setTriggerFrameY(QString)));
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setSamples(const QString &text)
{
    quint32 samples = text.toULong();
    SP2406->set_iq_cap_samples(samples);
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setCaptureSource(int index)
{
    SP2406->set_iq_cap_src(iq_cap_src_t::_from_index(index));

    comboBoxSourceDDC->setEnabled(index == iq_cap_src_t::DDC);
    comboBoxSourceDDC_Ch->setEnabled(index == iq_cap_src_t::DDC);
    comboBoxSourceDL_CPRI_Ch->setEnabled(index == iq_cap_src_t::DL_CPRI);
    comboBoxSourceDUC->setEnabled(index == iq_cap_src_t::DUC_Ch0 || index == iq_cap_src_t::DUC_Ch1);
    comboBoxSourceDUC_Ch->setEnabled(index == iq_cap_src_t::DUC_Ch0 || index == iq_cap_src_t::DUC_Ch1);
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setCaptureSourceDDC(int index)
{
    SP2406->set_iq_cap_src_ddc(iq_cap_src_ddc_t::_from_index(index));
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setCaptureSourceDDC_Ch(int index)
{
    SP2406->set_iq_cap_src_ddc_ch(uint8_t(index));
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setCaptureSourceDL_CPRI_Ch(int index)
{
    SP2406->set_iq_cap_src_dl_cpri_ch(uint8_t(index));
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setCaptureSourceDUC(int index)
{
    SP2406->set_iq_cap_src_duc(iq_cap_src_duc_t::_from_index(index));
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setCaptureSourceDUC_Ch(int index)
{
    SP2406->set_iq_cap_src_duc_ch(uint8_t(index));
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setCaptureSR(int index)
{
    SP2406->set_iq_cap_sr(iq_cap_sr_t::_from_index(index));
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setCaptureMode(int index)
{
    SP2406->set_iq_cap_mode(iq_cap_mode_t::_from_index(index));
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setTriggerSource(int index)
{
    SP2406->set_iq_cap_trig_src(iq_cap_trig_src_t::_from_index(index));
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setTriggerEdge(int index)
{
    SP2406->set_iq_cap_trig_edge(iq_cap_trig_edge_t::_from_index(index));
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setTriggerOffset(const QString &text)
{
    SP2406->set_iq_cap_trig_offset(text.toUInt());
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setTriggerGap(const QString &text)
{
    SP2406->set_iq_cap_trig_gap(text.toUInt());
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setTimeout(const QString &text)
{
    SP2406->ddr()->set_fpga_w_timeout_time(text.toUInt());
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setThreshold(const QString &text)
{
    SP2406->set_iq_cap_trig_threshold(text.toUInt());
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setTriggerFrame(const QString &text)
{
    SP2406->set_iq_cap_trig_frame(text.toUInt());
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setTriggerFrameX(const QString &text)
{
    SP2406->set_iq_cap_trig_frame_x(text.toUShort());
}

void NS_SP9500X::Q_IQ_Cap_Config_Delegate::setTriggerFrameY(const QString &text)
{
    SP2406->set_iq_cap_trig_frame_y(text.toUShort());
}


NS_SP9500X::Q_IQ_Cap_Config_Widget_Helper::Q_IQ_Cap_Config_Widget_Helper(QWidget *parent) :
    ::Q_IQ_Cap_Config_Widget_Helper(parent)
{

}

void NS_SP9500X::Q_IQ_Cap_Config_Widget_Helper::init()
{
    SAFE_NEW(model,Q_IQ_Cap_Config_Model);
    SAFE_NEW(delegate,Q_IQ_Cap_Config_Delegate);

    ::Q_IQ_Cap_Config_Widget_Helper::init();

    quint32 samples = 0;
    iq_cap_src_t source = iq_cap_src_t::DDC;
    iq_cap_src_ddc_t sourceDDC = iq_cap_src_ddc_t::DDC;
    quint8 sourceDDC_Ch = 0;
    quint8 sourceDL_CPRI_Ch = 0;
    iq_cap_src_duc_t sourceDUC = iq_cap_src_duc_t::LPF;
    quint8 sourceDUC_Ch = 0;
    iq_cap_sr_t sr = iq_cap_sr_t::_8I_98304;
    iq_cap_mode_t mode  = iq_cap_mode_t::Single;
    iq_cap_trig_src_t triggerSource = iq_cap_trig_src_t::RestartMarker;
    iq_cap_trig_edge_t triggerEdge  = iq_cap_trig_edge_t::Posedge;
    qint32  triggerOffset = 0;
    quint32 triggerGap = 0;
    quint32 timeoutTime = 0;
    quint32 threshold = 0;
    quint16 frame = 0;
    quint16 frameX = 0;
    quint16 frameY = 0;

    INT_CHECKV(SP2406->get_iq_cap_samples(samples));
    INT_CHECKV(SP2406->get_iq_cap_src(source));
    INT_CHECKV(SP2406->get_iq_cap_src_ddc(sourceDDC));
    INT_CHECKV(SP2406->get_iq_cap_src_ddc_ch(sourceDDC_Ch));
    INT_CHECKV(SP2406->get_iq_cap_src_dl_cpri_ch(sourceDL_CPRI_Ch));
    INT_CHECKV(SP2406->get_iq_cap_src_duc(sourceDUC));
    INT_CHECKV(SP2406->get_iq_cap_src_duc_ch(sourceDUC_Ch));
    INT_CHECKV(SP2406->get_iq_cap_sr(sr));
    INT_CHECKV(SP2406->get_iq_cap_mode(mode));
    INT_CHECKV(SP2406->get_iq_cap_trig_src(triggerSource));
    INT_CHECKV(SP2406->get_iq_cap_trig_edge(triggerEdge));
    INT_CHECKV(SP2406->get_iq_cap_trig_offset(triggerOffset));
    INT_CHECKV(SP2406->get_iq_cap_trig_gap(triggerGap));
    INT_CHECKV(SP2406->ddr()->get_fpga_w_timeout_time(timeoutTime));
    INT_CHECKV(SP2406->get_iq_cap_trig_threshold(threshold));
    INT_CHECKV(SP2406->get_iq_cap_trig_frame(frame));
    INT_CHECKV(SP2406->get_iq_cap_trig_frame_x(frameX));
    INT_CHECKV(SP2406->get_iq_cap_trig_frame_y(frameY));

    Q_IQ_Cap_Config_Delegate *d = dynamic_cast<Q_IQ_Cap_Config_Delegate *>(delegate);

    d->lineEditSamples->setText(QString("%1").arg(samples));
    d->comboBoxSource->setCurrentIndex(int(source._to_index()));
    d->comboBoxSourceDDC->setCurrentIndex(int(sourceDDC._to_index()));
    d->comboBoxSourceDDC_Ch->setCurrentIndex(sourceDDC_Ch);
    d->comboBoxSourceDL_CPRI_Ch->setCurrentIndex(sourceDL_CPRI_Ch);
    d->comboBoxSourceDUC->setCurrentIndex(int(sourceDUC._to_index()));
    d->comboBoxSourceDUC_Ch->setCurrentIndex(sourceDUC_Ch);
    d->comboBoxSR->setCurrentIndex(int(sr._to_index()));
    d->comboBoxMode->setCurrentIndex(int(mode._to_index()));
    d->comboBoxTriggerSource->setCurrentIndex(int(triggerSource._to_index()));
    d->comboBoxTriggerEdge->setCurrentIndex(int(triggerEdge._to_index()));
    d->lineEditTriggerOffset->setText(QString("%1").arg(triggerOffset));
    d->lineEditTriggerGap->setText(QString("%1").arg(triggerGap));
    d->lineEditTimeout->setText(QString("%1").arg(timeoutTime));
    d->lineEditThreshold->setText(QString("%1").arg(threshold));
    d->lineEditTriggerFrame->setText(QString("%1").arg(frame));
    d->lineEditTriggerFrameX->setText(QString("%1").arg(frameX));
    d->lineEditTriggerFrameY->setText(QString("%1").arg(frameY));

    d->setCaptureSource(int(source._to_index()));
}
