#ifndef Q_TABLE_MODEL_H
#define Q_TABLE_MODEL_H

#include <QAbstractTableModel>
#include "qwt_series_data.h"
#include "qwt_plot_curve.h"

#define MODEL_APPLY(macro,count) MACRO_ID(macro(count))

#define MODEL_HEADER(...) \
    int columnCount(const QModelIndex &parent) const \
    { \
        ignore_unused(parent); return PP_COUNT(__VA_ARGS__); \
    } \
    QVariant headerData(int section, Qt::Orientation orientation, int role) const \
    { \
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) { \
            QString header[PP_COUNT(__VA_ARGS__)] = { __VA_ARGS__ }; \
            return header[section]; \
        } \
        return QVariant(); \
    }

#define MODEL_PREFIX(class_name,...) \
    class class_name : public Q_Table_Model \
    { \
    public: \
        class_name(QObject *parent = nullptr) : Q_Table_Model(parent) {} \
        MODEL_HEADER(__VA_ARGS__)

#define MODEL_ROWCNT_DECL \
    int rowCount(const QModelIndex &) const; \
    QVariant data(const QModelIndex &index, int role) const;

#define MODEL_ROWCNT_PARAM_1(rows) \
    int rowCount(const QModelIndex &) const { return (rows); } \
    QVariant data(const QModelIndex &index, int role) const \
    { \
        size_t row = size_t(index.row()); \
        if (role == Qt::DisplayRole) {

#define MODEL_ROWCNT_PARAM_2(class_name,rows) \
    int class_name :: rowCount(const QModelIndex &) const { return (rows); } \
    QVariant class_name :: data(const QModelIndex &index, int role) const \
    { \
        size_t row = size_t(index.row()); \
        if (role == Qt::DisplayRole) {

#define MODEL_COLUMN_PARAM_2(col,text) \
            if (index.column() == col) { return text; }

#define MODEL_COLUMN_PARAM_3(col,text,end) \
            MODEL_COLUMN_PARAM_2(col,text) \
        } \
        return Q_Table_Model::data(index,role); \
    }

#define MODEL_SUFFIX };

#define MODEL_ROWCNT_VAR_COUNT(count) MODEL_ROWCNT_PARAM_##count
#define MODEL_ROWCNT(...) MACRO_ID(MODEL_APPLY(MODEL_ROWCNT_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#define MODEL_COLUMN_VAR_COUNT(count) MODEL_COLUMN_PARAM_##count
#define MODEL_COLUMN(...) MACRO_ID(MODEL_APPLY(MODEL_COLUMN_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))


#define MODEL_DISPLAY_HEADER(...) \
    int columnCount(const QModelIndex &parent) const \
    { \
        ignore_unused(parent); return PP_COUNT(__VA_ARGS__); \
    } \
    QVariant headerData(int section, Qt::Orientation orientation, int role) const \
    { \
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal) { \
            QString header[PP_COUNT(__VA_ARGS__)] = { __VA_ARGS__ }; \
            return header[section]; \
        } \
        return QVariant(); \
    }

#define MODEL_DISPLAY_PREFIX(...) \
    MODEL_DISPLAY_HEADER(__VA_ARGS__) \
    QVariant data(const QModelIndex &index, int role) const \
    { \
        size_t row = size_t(index.row()); \
        if (role == Qt::DisplayRole) {

#define KASE_MODEL(kase,...) \
    class kase##_Model : public Q_Table_Model \
    { \
    public: \
        kase##_Model(QObject *parent = nullptr) : Q_Table_Model(parent) {} \
        MODEL_HEADER(__VA_ARGS__) \
        \
        int rowCount(const QModelIndex &) const \
        { return (Radio.data_base(RFIdx)->db(kase)->data_reception_rows()); } \
        \
        QVariant data(const QModelIndex &index, int role) const \
        { \
            if (role == Qt::DisplayRole) { \
                return QString::fromStdString(Radio.data_base(RFIdx)->db(kase)->data_reception(index.row(),index.column())); \
            } \
            return Q_Table_Model::data(index,role); \
        } \
    };

class Q_Table_Model : public QAbstractTableModel
{
    Q_OBJECT

public:
    Q_Table_Model(QObject *parent = nullptr) : QAbstractTableModel(parent) {}
    virtual QVariant data(const QModelIndex &index, int role) const;

public slots:
    void reset();
    void uiInsert(const int first,const int last,const int kase);
    void uiUpdate(const int first,const int last,const int kase);
};


class Q_Checkable_Table_Model : public QAbstractTableModel
{
public:
    Q_Checkable_Table_Model(QObject *parent = nullptr) : QAbstractTableModel(parent) {}

    int rowCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return checked.size(); }

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant data(const QModelIndex &index, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

public:
    std::vector<bool> checked;
};


class Qwt_Series_Data : public QwtSeriesData<QPointF>
{
public:
    Qwt_Series_Data() : QwtSeriesData<QPointF>() {
        curve.setSamples(this);
    }

    QRectF boundingRect() const
    {

#if QWT_VERSION < 0x060200
        if (d_boundingRect.width() < 0) {
            d_boundingRect = qwtBoundingRect(*this);
        }
        return d_boundingRect;
#else
        if (cachedBoundingRect.width() < 0) {
            cachedBoundingRect = qwtBoundingRect(*this);
        }
        return cachedBoundingRect;
#endif
    }

public:
    QwtPlotCurve curve;
};

#define KASE_CURVE(kase) \
class kase##_Curve : public Qwt_Series_Data \
{ \
public: \
    size_t keyIdx; \
    kase##_Curve(size_t key = 0) : Qwt_Series_Data(), keyIdx(key) { } \
    \
    size_t size() const \
    { return Radio.data_base(RFIdx)->db(kase)->data_reception_points(keyIdx); } \
    \
    QPointF sample(size_t i) const { \
        return QPointF(Radio.data_base(RFIdx)->db(kase)->data_reception_x(keyIdx,i), \
                       Radio.data_base(RFIdx)->db(kase)->data_reception_y(keyIdx,i)); \
    } \
};

#endif
