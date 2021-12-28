#ifndef Q_QPRESS_EVENT_COMBOBOX_HPP
#define Q_QPRESS_EVENT_COMBOBOX_HPP

#include <QComboBox>
#include <QMouseEvent>

class Q_Press_Event_ComboBox : public QComboBox
{
    Q_OBJECT

public:
    Q_Press_Event_ComboBox(QWidget *parent = 0) : QComboBox(parent) {}
    ~Q_Press_Event_ComboBox() {}

protected:
    virtual void mousePressEvent(QMouseEvent *event)
    {
        if(event->button() == Qt::LeftButton)
        { emit clicked(); }

        QComboBox::mousePressEvent(event);
    }


signals:
    void clicked();
};

#endif // QT_QPRESS_EVENT_COMBOBOX_HPP
