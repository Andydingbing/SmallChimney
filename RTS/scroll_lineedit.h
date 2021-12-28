#ifndef Q_SCROLL_LINEEDIT_H
#define Q_SCROLL_LINEEDIT_H

#include <QLineEdit>
#include <QWheelEvent>
#include "algo_math.hpp"
#include "freq_string.hpp"

inline int wheelStep(QWheelEvent *event)
{
    int step = 0;

#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    QPoint numSteps;
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;
    if (!numPixels.isNull()) {
        numSteps = numPixels;
    } else if (!numDegrees.isNull()) {
        numSteps = numDegrees / 15;
    }
    step = numSteps.ry();
#else
    step = event->delta() / 8 / 15;
#endif

    return step;
}


class QScrollLineEdit : public QLineEdit
{
public:
    explicit QScrollLineEdit(QWidget *parent = nullptr) :
        QLineEdit(parent)
    {
        stepDefault = 0;
        stepCtrl    = 0;
        stepShift   = 0;
    }

protected:
    void wheelEvent(QWheelEvent *event)
    {
        double step = stepDefault;

        if (event->modifiers() == Qt::ControlModifier) {
            step = stepCtrl;
        } else if (event->modifiers() == Qt::ShiftModifier) {
            step = stepShift;
        }

        double newValue = text().toDouble() + wheelStep(event) * step;
        limit_between(lower,upper,newValue);

        setText(QString("%1").arg(newValue));

        event->accept();
    }

    void keyPressEvent(QKeyEvent *event)
    {
        if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
            int orientation = (event->key() == Qt::Key_Up ? 1 : -1);
            double step = stepDefault;

            if (event->modifiers() == Qt::ControlModifier) {
                step = stepCtrl;
            } else if (event->modifiers() == Qt::ShiftModifier) {
                step = stepShift;
            }

            double newValue = text().toDouble() + orientation * step;
            limit_between(lower,upper,newValue);

            setText(QString("%1").arg(newValue));
        }

        QLineEdit::keyPressEvent(event);
    }

public:
    double stepDefault;
    double stepCtrl;
    double stepShift;
    double upper;
    double lower;
};


class QFreqScrollLineEdit : public QLineEdit
{
public:
    explicit QFreqScrollLineEdit(QWidget *parent = nullptr) :
        QLineEdit(parent)
    {
        upper = double(100e9);
        lower = -upper;
    }

protected:
    void wheelEvent(QWheelEvent *event)
    {
        double step = stepDefault;

        if (event->modifiers() == Qt::ControlModifier) {
            step = stepCtrl;
        } else if (event->modifiers() == Qt::ShiftModifier) {
            step = stepShift;
        }

        double newValue = freq_string_to_double(text().toStdString()) + wheelStep(event) * step;
        limit_before(upper,newValue);
        limit_after(lower,newValue);

        QString text = QString::fromStdString(freq_string_from<double>(newValue));

        setText(text);
        emit textEdited(text);

        event->accept();
    }

public:
    static double stepDefault;
    static double stepCtrl;
    static double stepShift;
    double upper;
    double lower;
};

#endif // Q_SCROLL_LINEEDIT_H
