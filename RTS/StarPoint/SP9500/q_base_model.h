#ifndef Q_CAL_BASE_MODEL_H
#define Q_CAL_BASE_MODEL_H

#include <QAbstractTableModel>
#include "global.h"
#include "qwt_series_data.h"

#define DECLARE_CAL_TABLE_MODEL(class_name)                             \
public:                                                                 \
    explicit class_name(QObject *parent = 0);                           \
    int rowCount(const QModelIndex &parent) const override;             \
    int columnCount(const QModelIndex &parent) const override;          \
    QVariant data(const QModelIndex &index, int role) const override;   \
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;


typedef class QCalBaseModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QCalBaseModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {}

public slots:
    void reset()
    {
        beginResetModel();
        endResetModel();
    }

    void update(const QModelIndex &tl,
                const QModelIndex &br,
                cal_file::cal_item_t item,
                int sec)
    {
        Q_UNUSED(item);
        Q_UNUSED(sec);
        emit dataChanged(tl,br,QVector<int>(Qt::DisplayRole));
    }

    void update2(const QModelIndex &tl,
                const QModelIndex &br,
                test_item_t item,int sec)
    {
        Q_UNUSED(item);
        Q_UNUSED(sec);
        emit dataChanged(tl,br,QVector<int>(Qt::DisplayRole));
    }

    void uiInsert(const int first,const int last,const int cal_table)
    {
        beginInsertRows(QModelIndex(),first,last);
        for (int i = first;i <= last;++i) {
            insertRow(i);
        }
        endInsertRows();
        emit dataChanged(index(first,0),index(last,columnCount() - 1));
    }

    void uiUpdate(const int first,const int last,const int cal_table)
    {
        emit dataChanged(index(first,0),index(last,columnCount() - 1),QVector<int>(Qt::DisplayRole));
    }
} QCalBaseModel, QTestBaseModel;


// T/Rx attenuation states model
class QAttStateModel : public QAbstractTableModel
{
public:
    explicit QAttStateModel(QObject *parent = nullptr) :
        QAbstractTableModel(parent) {}

    int rowCount(const QModelIndex &parent) const { return checked.size(); }

    Qt::ItemFlags flags(const QModelIndex &index) const {
        if (index.column() == 0) {
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
        }
        return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    QVariant data(const QModelIndex &index, int role) const {
        if (role == Qt::CheckStateRole) {
            if (index.column() == 0) {
               return checked.at(index.row()) ? Qt::Checked : Qt::Unchecked;
            }
        }
        return QVariant();
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role) {
        if (role == Qt::CheckStateRole && index.column() == 0) {                                                                                                          \
            checked[index.row()] = value.toBool();
            emit dataChanged(index,index);
        }
        return true;
    }

public:
    std::vector<common_atts_t> att;
    std::vector<bool> checked;
};


#define DECL_QWT_CAL_DATA(class_name) \
public: \
    explicit class_name(); \
    virtual size_t size() const; \
    virtual QPointF sample(size_t i) const;

typedef class QwtCalData : public QwtSeriesData<QPointF>
{
public:
    QwtCalData() : QwtSeriesData<QPointF>() {}

    QRectF boundingRect() const
    {
        if (d_boundingRect.width() < 0) {
            d_boundingRect = qwtBoundingRect(*this);
        }
        return d_boundingRect;
    }
} QwtCalData, QwtTestData;


template <typename Data_Type>
class QwtFreqResponseData : public QwtCalData
{
public:
    QwtFreqResponseData() :
        QwtCalData(),
        table(new QVector<Data_Type>)
    { table->clear(); }

    size_t size() const
    { return table->size(); }

    QPointF sample(size_t i) const
    { return QPointF(table->at(i).freq / 1e6,table->at(i).pwr); }

    QVector<Data_Type> *calTable() const
    { return table; }

private:
    QVector<Data_Type> *table;
};

// CW Frequency Response Data
//template<typename data_f_t = data_f_fr<1>>
//class Qwt_FR_CW_Data : public QwtCalData
//{
//public:
//    Qwt_FR_CW_Data() { table = nullptr; }

//    size_t size() const
//    { return table == nullptr ? 0 : table->size(); }

//    QPointF sample(size_t i) const
//    { return QPointF(table->at(i).freq / 1e6,table->at(i).pts[0].y); }

//    std::vector<data_f_t> *table;
//};

// BW Frequency Response Data
//template<uint32_t n = 1>
//class Qwt_FR_BW_Data : public QwtCalData
//{
//public:
//    Qwt_FR_BW_Data() { table = nullptr; }

//    size_t size() const
//    { return table == nullptr ? 0 : n; }

//    QPointF sample(size_t i) const
//    { return QPointF(table->pts[i].x / 1e6,table->pts[i].y); }

//    data_f_fr<n> *table;
//};

#endif
