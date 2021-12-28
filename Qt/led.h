#ifndef Q_LED_H
#define Q_LED_H

#include <QFrame>

class Q_Led : public QFrame
{
public:
    Q_Led(QWidget *parent = nullptr) :
        QFrame(parent) { setColor(Qt::gray); }

    void setColor(const QColor clr)
    {
        setStyleSheet(QString("background:rgb(%1,%2,%3)").
                    arg(clr.red()).
                    arg(clr.green()).
                    arg(clr.blue()));
    }
};

#endif // Q_LED_H
