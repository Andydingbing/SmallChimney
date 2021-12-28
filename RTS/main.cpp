#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>

void addMsgListCallback() {g_MainW->addMsgListCallback();}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoggerMsg.init(addMsgListCallback);

    g_MainW = new MainWindow;
    g_MainW->setWindowTitle(QString("Radio Test System"));
    g_MainW->show();
    g_MainW->setWindowState(Qt::WindowMaximized);

//    QApplication::setStyle(QStyleFactory::create("windows"));
//    QApplication::setPalette(QApplication::style()->standardPalette());

    return a.exec();
}
