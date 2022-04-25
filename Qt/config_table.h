#ifndef Q_CONFIG_TABLE_H
#define Q_CONFIG_TABLE_H

#include "qhelper_global.h"
#include "preprocessor/suffix.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class QHELPER_EXPORT Q_Config_Table_View : public QTableView
{
public:
    Q_Config_Table_View(QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *model);
};


class QHELPER_EXPORT Q_Config_Table_Model : public QStandardItemModel
{
public:
    Q_Config_Table_Model(QObject *parent = nullptr) : QStandardItemModel(parent) {}

    int rowCount(const QModelIndex & = QModelIndex()) const { return _item.size(); }

    int columnCount(const QModelIndex & = QModelIndex()) const { return 2; }

    QVariant data(const QModelIndex &index, int role) const;

    void addItemTitle(const QString &title) { _item.push_back(title); }

public:
    QStringList _item;
};

typedef Q_Config_Table_Model Q_Result_Table_Model;

#define CONFIG_MODEL(name,...) \
class name : public Q_Config_Table_Model { \
public: \
    name(QObject *parent = nullptr) : Q_Config_Table_Model(parent) \
    { \
        _item.clear(); \
        _item = QStringList{__VA_ARGS__}; \
        setRowCount(rowCount(QModelIndex())); \
        setColumnCount(columnCount(QModelIndex())); \
    } \
};


class QHELPER_EXPORT Q_Config_Table_Delegate : public QStyledItemDelegate
{
public:
    Q_Config_Table_Delegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    virtual QWidget **first() const = 0;

#define FIRST_CONFIG_WIDGET(type,widget) \
    type *widget; \
    QWidget **first() const OVERRIDE { return reinterpret_cast<QWidget **>(const_cast<type **>(&widget)); }
};


class QHELPER_EXPORT Q_Config_Table_Delegate_Kase : public Q_Config_Table_Delegate
{
public:
    Q_Config_Table_Delegate_Kase(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

public:
    FIRST_CONFIG_WIDGET(QLabel,labelConfig)
    QList<QWidget *> delegatedWidgets;
};

class QHELPER_EXPORT Q_Result_Table_Delegate_Kase : public Q_Config_Table_Delegate
{
public:
    Q_Result_Table_Delegate_Kase(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

public:
    FIRST_CONFIG_WIDGET(QLabel,labelResult)
    QList<QWidget *> delegatedWidgets;
};


#define KASE_CONFIG(config_title,class_name) \
    configModel->addItemTitle(config_title); configDelegate->delegatedWidgets.push_back(new class_name);

#define KASE_RESULT(result_title,class_name) \
    resultModel->addItemTitle(result_title); resultDelegate->delegatedWidgets.push_back(new class_name);

#define Config(config_title,class_name) ((class_name *)(config(config_title)))
#define Result(result_title,class_name) ((class_name *)(result(result_title)))

#define RESULT_TABLE_KASE(...) \
class Q_Result_Model : public Q_Config_Table_Model { \
public: \
    Q_Result_Model(QObject *parent = nullptr) : Q_Config_Table_Model(parent) \
    { \
        _item.clear(); \
        _item << PARAM_PICK_1_OF_3_3(<<,,, __VA_ARGS__); \
        _item.push_front(""); \
        setRowCount(rowCount(QModelIndex())); \
        setColumnCount(columnCount(QModelIndex())); \
    } \
}; \
class Q_Result_Delegate : public Q_Result_Table_Delegate_Kase { \
public: \
    Q_Result_Delegate(QObject *parent = nullptr) \
    { PARAM_PICK_2_OF_3_2_1(,,= new,;,__VA_ARGS__) } \
    PARAM_PICK_2_OF_3_1(,,*,;,__VA_ARGS__) \
};

#endif
