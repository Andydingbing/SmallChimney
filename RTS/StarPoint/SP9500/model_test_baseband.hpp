#ifndef Q_SP9500X_MODEL_TEST_BASEBAND_H
#define Q_SP9500X_MODEL_TEST_BASEBAND_H

#include "q_base_model.h"
#include "enum.h"

namespace NS_SP9500X {

BETTER_ENUM(Test_Baseband_Item, quint32,
            DMA)

class Q_Test_Baseband_TableModel : public QCalBaseModel
{
public:
    typedef NS_SP9500X::Test_Baseband_Item Test_Baseband_Item;

    explicit Q_Test_Baseband_TableModel(QObject *parent = nullptr) :
        QCalBaseModel(parent) {}

    int columnCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return 3; }

    int rowCount(const QModelIndex &parent) const
    { Q_UNUSED(parent); return ARRAY_SIZE(item); }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (Qt::DisplayRole == role && Qt::Horizontal == orientation) {
            QString header[3] = { "", "Item", "" };
            return header[section];
        }
        return QVariant();
    }

    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        if (index.column() == 0) {
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
        }
        return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    QVariant data(const QModelIndex &index, int role) const
    {
        if (role == Qt::CheckStateRole) {
            if (index.column() == 0) {
               return item[index.row()] ? Qt::Checked : Qt::Unchecked;
            }
        }
        if (role == Qt::DisplayRole) {
            if (index.column() == 1) {
                return QString::fromStdString(Test_Baseband_Item::_from_index(size_t(index.row()))._to_string());
            }
        }
        return QVariant();
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role)
    {
        if (role == Qt::CheckStateRole && index.column() == 0) {
            item[index.row()] = value.toBool();
            emit dataChanged(index,index);
        }
        return true;
    }

public:
    bool item[Test_Baseband_Item::_size()];
};

} // namespace NS_SP9500X

#endif // Q_SP9500X_MODEL_TEST_BASEBAND_H
