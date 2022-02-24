#include "sp9500x_fan_dlg.h"
#include "ui_sp9500x_fan.h"
#include "../rd/common/global.hpp"

#define IMPL_SLOT_SLIDER_MOVED(idx) \
void NS_SP9500X::Q_Fan_Dlg::on_horizontalSlider##idx##_sliderMoved(int position) \
{ \
    SP9500X_SetFan(idx,quint32(position)); \
    ui->labelSpeed##idx->setText(QString("%1").arg(position)); \
    setSliderHandleColor(ui->horizontalSlider##idx,position); \
}

void setSliderHandleColor(QSlider *widget,const int Speed)
{
    if (Speed < 80) {
        widget->setStyleSheet("QSlider::handle:horizontal{background:blue}");
    } else if (Speed < 160) {
        widget->setStyleSheet("QSlider::handle:horizontal{background:orange}");
    } else  {
        widget->setStyleSheet("QSlider::handle:horizontal{background:red}");
    }
}

using namespace NS_SP9500X;

Q_Fan_Dlg::Q_Fan_Dlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Q_SP9500X_Fan_Dlg)
{
    ui->setupUi(this);

    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    const int rangeMax = 255;

    ui->horizontalSlider0->setTracking(true);
    ui->horizontalSlider0->setRange(0,rangeMax);
    ui->horizontalSlider1->setTracking(true);
    ui->horizontalSlider1->setRange(0,rangeMax);
    ui->horizontalSlider2->setTracking(true);
    ui->horizontalSlider2->setRange(0,rangeMax);
    ui->horizontalSlider3->setTracking(true);
    ui->horizontalSlider3->setRange(0,rangeMax);
    ui->horizontalSlider4->setTracking(true);
    ui->horizontalSlider4->setRange(0,rangeMax);
    ui->horizontalSlider5->setTracking(true);
    ui->horizontalSlider5->setRange(0,rangeMax);
    ui->horizontalSlider6->setTracking(true);
    ui->horizontalSlider6->setRange(0,rangeMax);
    ui->horizontalSlider7->setTracking(true);
    ui->horizontalSlider7->setRange(0,rangeMax);
    ui->horizontalSlider8->setTracking(true);
    ui->horizontalSlider8->setRange(0,rangeMax);
    ui->horizontalSlider9->setTracking(true);
    ui->horizontalSlider9->setRange(0,rangeMax);
    ui->horizontalSlider10->setTracking(true);
    ui->horizontalSlider10->setRange(0,rangeMax);
    ui->horizontalSlider11->setTracking(true);
    ui->horizontalSlider11->setRange(0,rangeMax);
    ui->horizontalSlider12->setTracking(true);
    ui->horizontalSlider12->setRange(0,rangeMax);
    ui->horizontalSlider13->setTracking(true);
    ui->horizontalSlider13->setRange(0,rangeMax);
    ui->horizontalSlider14->setTracking(true);
    ui->horizontalSlider14->setRange(0,rangeMax);
    ui->horizontalSlider15->setTracking(true);
    ui->horizontalSlider15->setRange(0,rangeMax);

    setSliderHandleColor(ui->horizontalSlider0,0);
    setSliderHandleColor(ui->horizontalSlider1,0);
    setSliderHandleColor(ui->horizontalSlider2,0);
    setSliderHandleColor(ui->horizontalSlider3,0);
    setSliderHandleColor(ui->horizontalSlider4,0);
    setSliderHandleColor(ui->horizontalSlider5,0);
    setSliderHandleColor(ui->horizontalSlider6,0);
    setSliderHandleColor(ui->horizontalSlider7,0);
    setSliderHandleColor(ui->horizontalSlider8,0);
    setSliderHandleColor(ui->horizontalSlider9,0);
    setSliderHandleColor(ui->horizontalSlider10,0);
    setSliderHandleColor(ui->horizontalSlider11,0);
    setSliderHandleColor(ui->horizontalSlider12,0);
    setSliderHandleColor(ui->horizontalSlider13,0);
    setSliderHandleColor(ui->horizontalSlider14,0);
    setSliderHandleColor(ui->horizontalSlider15,0);
}

Q_Fan_Dlg::~Q_Fan_Dlg()
{
    delete ui;
}

void NS_SP9500X::Q_Fan_Dlg::on_horizontalSlider0_sliderMoved(int position)
{
    SP9500X_SetFan(0,quint32(position));
    ui->labelSpeed0->setText(QString("%1").arg(position));
    setSliderHandleColor(ui->horizontalSlider0,position);

    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
        on_horizontalSlider1_sliderMoved(position);
        on_horizontalSlider2_sliderMoved(position);
        on_horizontalSlider3_sliderMoved(position);
        on_horizontalSlider4_sliderMoved(position);
        on_horizontalSlider5_sliderMoved(position);
        on_horizontalSlider6_sliderMoved(position);
        on_horizontalSlider7_sliderMoved(position);
        on_horizontalSlider8_sliderMoved(position);
        on_horizontalSlider9_sliderMoved(position);
        on_horizontalSlider10_sliderMoved(position);
        on_horizontalSlider11_sliderMoved(position);
        on_horizontalSlider12_sliderMoved(position);
        on_horizontalSlider13_sliderMoved(position);
        on_horizontalSlider14_sliderMoved(position);
        on_horizontalSlider15_sliderMoved(position);

        ui->horizontalSlider1->setValue(position);
        ui->horizontalSlider2->setValue(position);
        ui->horizontalSlider3->setValue(position);
        ui->horizontalSlider4->setValue(position);
        ui->horizontalSlider5->setValue(position);
        ui->horizontalSlider6->setValue(position);
        ui->horizontalSlider7->setValue(position);
        ui->horizontalSlider8->setValue(position);
        ui->horizontalSlider9->setValue(position);
        ui->horizontalSlider10->setValue(position);
        ui->horizontalSlider11->setValue(position);
        ui->horizontalSlider12->setValue(position);
        ui->horizontalSlider13->setValue(position);
        ui->horizontalSlider14->setValue(position);
        ui->horizontalSlider15->setValue(position);
    }
}

IMPL_SLOT_SLIDER_MOVED(1);
IMPL_SLOT_SLIDER_MOVED(2);
IMPL_SLOT_SLIDER_MOVED(3);
IMPL_SLOT_SLIDER_MOVED(4);
IMPL_SLOT_SLIDER_MOVED(5);
IMPL_SLOT_SLIDER_MOVED(6);
IMPL_SLOT_SLIDER_MOVED(7);
IMPL_SLOT_SLIDER_MOVED(8);
IMPL_SLOT_SLIDER_MOVED(9);
IMPL_SLOT_SLIDER_MOVED(10);
IMPL_SLOT_SLIDER_MOVED(11);
IMPL_SLOT_SLIDER_MOVED(12);
IMPL_SLOT_SLIDER_MOVED(13);
IMPL_SLOT_SLIDER_MOVED(14);
IMPL_SLOT_SLIDER_MOVED(15);
