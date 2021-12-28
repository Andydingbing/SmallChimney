#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include "config_table.cpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    g_MainW = new MainWindow;
    g_MainW->setWindowTitle(QString("DDT"));
    g_MainW->show();
    g_MainW->setWindowState(Qt::WindowMaximized);

//    QApplication::setStyle(QStyleFactory::create("windows"));
//    QApplication::setPalette(QApplication::style()->standardPalette());

    return a.exec();
}
