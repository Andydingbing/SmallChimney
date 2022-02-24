#ifndef Q_SP9500X_FAN_DLG_H
#define Q_SP9500X_FAN_DLG_H

#define DECL_SLOT_SLIDER_MOVED(idx) \
    void on_horizontalSlider##idx##_sliderMoved(int position)

#include "global.h"
#include <QDialog>

namespace Ui {
class Q_SP9500X_Fan_Dlg;
}

namespace NS_SP9500X {

class Q_Fan_Dlg: public QDialog
{
    Q_OBJECT

public:
    explicit Q_Fan_Dlg(QWidget *parent = nullptr);
    ~Q_Fan_Dlg();

private slots:

    DECL_SLOT_SLIDER_MOVED(0);
    DECL_SLOT_SLIDER_MOVED(1);
    DECL_SLOT_SLIDER_MOVED(2);
    DECL_SLOT_SLIDER_MOVED(3);
    DECL_SLOT_SLIDER_MOVED(4);
    DECL_SLOT_SLIDER_MOVED(5);
    DECL_SLOT_SLIDER_MOVED(6);
    DECL_SLOT_SLIDER_MOVED(7);
    DECL_SLOT_SLIDER_MOVED(8);
    DECL_SLOT_SLIDER_MOVED(9);
    DECL_SLOT_SLIDER_MOVED(10);
    DECL_SLOT_SLIDER_MOVED(11);
    DECL_SLOT_SLIDER_MOVED(12);
    DECL_SLOT_SLIDER_MOVED(13);
    DECL_SLOT_SLIDER_MOVED(14);
    DECL_SLOT_SLIDER_MOVED(15);

private:
    Ui::Q_SP9500X_Fan_Dlg *ui;
};

} // namespace NS_SP9500X

#endif // Q_SP9500X_FAN_DLG_H
