#include "qhelper_global.h"
#include "scroll_lineedit.h"

double QFreqScrollLineEdit::stepDefault = 1e6;
double QFreqScrollLineEdit::stepCtrl    = 5e6;
double QFreqScrollLineEdit::stepShift   = 10e6;

void addMenu(QMenuBar *bar, QList<QMenu *> menus)
{
    QList<QMenu *>::const_iterator iterMenus;

    for (iterMenus = menus.constBegin();iterMenus != menus.constEnd();++iterMenus) {
        bar->addMenu(*iterMenus);
    }
}
