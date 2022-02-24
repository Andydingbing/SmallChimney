#ifndef QREGLOGMODEL_H
#define QREGLOGMODEL_H

#include <QAbstractTableModel>

class QRegLogModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit QRegLogModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
public slots:
    void update(int iRow);
};

#endif // QREGLOGMODEL_H
