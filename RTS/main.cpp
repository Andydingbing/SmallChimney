#include "mainwindow.h"
#include "main_dialog.h"
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>

//#include "algo_chip_pll.inl.hpp"

void addMsgListCallback() { g_MainW->addMsgListCallback(); }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoggerMsg.init(addMsgListCallback);

    MainDialog *mainDialog = new MainDialog;
    mainDialog->setWindowTitle(QString("Mode Select"));
    mainDialog->exec();

    if (mainDialog->result() == QDialog::Rejected) {
        return 0;
    }

    g_MainW = new MainWindow;
    g_MainW->setWindowTitle(QString("Radio Test System"));
    g_MainW->show();
    g_MainW->setWindowState(Qt::WindowMaximized);

//    QApplication::setStyle(QStyleFactory::create("windows"));
//    QApplication::setPalette(QApplication::style()->standardPalette());

    return a.exec();
}
