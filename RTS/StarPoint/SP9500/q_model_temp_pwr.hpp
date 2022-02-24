#ifndef Q_MODEL_TEMP_PWR_H
#define Q_MODEL_TEMP_PWR_H

#include "global.h"
#include "q_base_model.h"
#include "test_data.hpp"
#include "algo_fit.hpp"

class QwtTempPwrData : public QwtTestData
{
public:
    struct Point {
        rd::common_tmps_t temp;
        float avg_temp;
        float pwr;
        Point() : avg_temp(0.0), pwr(0.0) {}
    };

    QVector<QList<Point>> *allData() { return &_allData; }

    QVector<Point> *curveData() { return &_curveData; }

    bool add(const float temp, const Point &p)
    {
        int tempIdx = SERIE_INDEX(temp,rd::g_temp_star,rd::g_temp_step);
        QList<Point> *list = &(_allData[tempIdx]);
        QList<Point>::iterator iter;
        int times = list->size();

        if (!list->empty()) {
            for (iter = list->begin();iter != list->end();iter ++) {
                if (abs(iter->pwr - p.pwr) > 0.1f) {
                    return false;
                }
            }
        }
        list->push_back(p);

        _curveData[tempIdx].pwr = (_curveData.at(tempIdx).pwr * times + p.pwr) / (times + 1);
        return true;
    }

    size_t size() const { return size_t(_curveData.size()); }

    QPointF sample(size_t i) const
    { return QPointF(double(SERIE_VALUE(rd::g_temp_star,rd::g_temp_step,i)),double(_curveData[i].pwr)); }

    void reset()
    {
        _allData.clear();
        _curveData.clear();

        for (quint32 i = 0;i < SERIE_SIZE(g_temp_star,g_temp_stop,g_temp_step);i ++) {
            _allData.push_back(QList<Point>());
            _curveData.push_back(Point());
        }
    }

private:
    QVector<QList<Point>> _allData;
    QVector<Point> _curveData;
};


class QwtTempPropertyData : public QwtCalData
{
public:
    QwtTempPropertyData(quint8 state = 0) : QwtCalData() { _state = state;}

protected:
    quint8 _state;
};

typedef class QwtTXTempPropertyData : public QwtTempPropertyData
{
public:
    explicit QwtTXTempPropertyData(quint8 state = 0) : QwtTempPropertyData(state) {}

    size_t size() const { return size_t(ARRAY_SIZE(rd::tx_tp_table::data_f_t::pwr)); }

    QPointF sample(size_t i) const
    { return QPointF(SERIE_VALUE(rd::sp1401::temp_min,rd::sp1401::temp_granularity,i),_data.pwr[i]); }

    void set(const double temp,const double pwr) { _data.set(temp,pwr); }
    rd::tx_tp_table::data_f_t *data() { return &_data; }

private:
    rd::tx_tp_table::data_f_t _data;
} QwtRXTempPropertyData;


class QwtTempCompData : public QwtCalData
{
public:
    QwtTempCompData() : QwtCalData() {}
    size_t size() const { return _data.size(); }

    QPointF sample(size_t i) const { return _data.at(i); }

    void setCoef(const tx_tc_table::data_f_t &coef) { _coef = coef; }
    tx_tc_table::data_f_t *coef() { return &_coef; }

    QVector<QPointF> *data() { return &_data; }

    void makeCurve()
    {
        const double tempStar = sp1401::temperature_star();
        const double tempStep = sp1401::temperature_step();
        QPointF point;
        for (quint32 i = 0;i < sp1401::temperatures();i ++) {
            point.setX(SERIE_VALUE(tempStar,tempStep,i));
            point.setY(polynomial<double>(_coef.coef,_coef.order,point.x()));
            _data.push_back(point);
        }
    }

private:
    tx_tc_table::data_f_t _coef;
    QVector<QPointF> _data;
};


class QTxTempCompModel : public QCalBaseModel
{
public:
    explicit QTxTempCompModel(QObject *parent) :
        QCalBaseModel(parent),
        _table(new QVector<tx_tc_table::data_f_t>) {}

    ~QTxTempCompModel() { _table->clear(); delete _table; }

    int rowCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return _table->size(); }

    int columnCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return 4; }

    QVector<tx_tc_table::data_f_t> *table() const
    { return _table; }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole) {
            if (index.column() == 0) {
                return freqStringFrom(_table->at(index.row()).freq);
            } else if (index.column() == 1) {
                return QString("%1").arg(_table->at(index.row()).order);
            } else if (index.column() == 2) {
                return QString("%1").arg(_table->at(index.row()).coef[0]);
            } else if (index.column() == 3) {
                return tm2QString(_table->at(index.row()).time);
            }
        } else if (role == Qt::BackgroundColorRole) {
            return CLR_PROTECT_EYE;
        }
        return QVariant();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            QString header[10] = {
                tr("Freq(MHz)"),
                tr("Order"),
                tr("Polynomial"),
                tr("End Time")
            };
            return header[section];
        }
        return QVariant();
    }

private:
    QVector<tx_tc_table::data_f_t> *_table;
};


class QTxAttStateModel : public QAttStateModel
{
public:
    explicit QTxAttStateModel(QObject *parent = nullptr) :
        QAttStateModel(parent) {}

    int columnCount(const QModelIndex &parent) const { return 4; }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            QString header[4] = { tr(""), tr("Att0"), tr("Att1"), tr("Att2") };
            return header[section];
        }
        return QVariant();
    }

    QVariant data(const QModelIndex &index, int role) const {
        if (role == Qt::DisplayRole) {
            if (index.row() == 0) {
                return QVariant();
            }
            if (index.column() == 1) {
                return QString("%1").arg(att.at(index.row() - 1).att[0]);
            } else if (index.column() == 2) {
                return QString("%1").arg(att.at(index.row() - 1).att[1]);
            } else if (index.column() == 3) {
                return QString("%1").arg(att.at(index.row() - 1).att[2]);
            }
        }
        return QAttStateModel::data(index,role);
    }
};

class QRxAttStateModel : public QAttStateModel
{
public:
    explicit QRxAttStateModel(QObject *parent = nullptr) :
        QAttStateModel(parent) {}

    int columnCount(const QModelIndex &parent) const { return 5; }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            QString header[5] = { tr(""), tr("LNA/Att"), tr("Att0/19"), tr("Att1"), tr("Att2") };
            return header[section];
        }
        return QVariant();
    }

    QVariant data(const QModelIndex &index, int role) const {
        if (role == Qt::DisplayRole) {
            if (index.row() == 0) {
                return QVariant();
            }
            if (index.column() == 1) {
                return att.at(index.row() - 1).rx_sp1401_r1f.lna_att == ns_sp1401::r1f::RX_LNA ? tr("LNA") : tr("Att");
            } else if (index.column() == 2) {
                return att.at(index.row() - 1).rx_sp1401_r1f.att_019 == ns_sp1401::r1f::RX_ATT_0 ? tr("Att0") : tr("Att19");
            } else if (index.column() == 3) {
                return QString("%1").arg(att.at(index.row() - 1).att[0]);
            } else if (index.column() == 4) {
                return QString("%1").arg(att.at(index.row() - 1).att[1]);
            }
        }
        return QAttStateModel::data(index,role);
    }
};

#endif // Q_MODEL_TEMP_PWR_H
