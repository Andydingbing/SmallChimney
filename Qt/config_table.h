#ifndef Q_CONFIG_TABLE_H
#define Q_CONFIG_TABLE_H

#include "preprocessor/suffix.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

class Q_Config_Table_View : public QTableView
{
public:
    Q_Config_Table_View(QWidget *parent = nullptr);

    void setModel(QAbstractItemModel *model);
};


class Q_Config_Table_Model : public QStandardItemModel
{
public:
    Q_Config_Table_Model(QObject *parent = nullptr) : QStandardItemModel(parent) {}

    int rowCount(const QModelIndex & = QModelIndex()) const { return _item.size(); }

    int columnCount(const QModelIndex & = QModelIndex()) const { return 2; }

    QVariant data(const QModelIndex &index, int role) const;

protected:
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


class Q_Config_Table_Delegate : public QStyledItemDelegate
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

class Q_Config_Table_Delegate_Kase : public Q_Config_Table_Delegate
{
public:
    Q_Config_Table_Delegate_Kase(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

public:
    FIRST_CONFIG_WIDGET(QLabel,labelConfig)
};

class Q_Result_Table_Delegate_Kase : public Q_Config_Table_Delegate
{
public:
    Q_Result_Table_Delegate_Kase(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

public:
    FIRST_CONFIG_WIDGET(QLabel,labelResult)
};


#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_1()

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_3(_1,_2,_3) \
    _2 = new _1;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_6(_1,_2,_3,_4,_5,_6) \
    _2 = new _1; \
    _5 = new _4;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_9(_1,_2,_3,_4,_5,_6,_7,_8,_9) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_12(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_15(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10; \
    _14 = new _13;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_18(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15, \
                                                _16,_17,_18) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10; \
    _14 = new _13; \
    _17 = new _16;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_21(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15, \
                                                _16,_17,_18,_19,_20,_21) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10; \
    _14 = new _13; \
    _17 = new _16; \
    _20 = new _19;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_24(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15, \
                                                _16,_17,_18,_19,_20,_21,_22,_23,_24) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10; \
    _14 = new _13; \
    _17 = new _16; \
    _20 = new _19; \
    _23 = new _22;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_27(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15, \
                                                _16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10; \
    _14 = new _13; \
    _17 = new _16; \
    _20 = new _19; \
    _23 = new _22; \
    _26 = new _25;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_30(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15, \
                                                _16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28, \
                                                _29,_30) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10; \
    _14 = new _13; \
    _17 = new _16; \
    _20 = new _19; \
    _23 = new _22; \
    _26 = new _25; \
    _29 = new _28;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_33(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15, \
                                                _16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28, \
                                                _29,_30,_31,_32,_33) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10; \
    _14 = new _13; \
    _17 = new _16; \
    _20 = new _19; \
    _23 = new _22; \
    _26 = new _25; \
    _29 = new _28; \
    _32 = new _31;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_36(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15, \
                                                _16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28, \
                                                _29,_30,_31,_32,_33,_34,_35,_36) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10; \
    _14 = new _13; \
    _17 = new _16; \
    _20 = new _19; \
    _23 = new _22; \
    _26 = new _25; \
    _29 = new _28; \
    _32 = new _31; \
    _35 = new _34;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_39(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15, \
                                                _16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28, \
                                                _29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10; \
    _14 = new _13; \
    _17 = new _16; \
    _20 = new _19; \
    _23 = new _22; \
    _26 = new _25; \
    _29 = new _28; \
    _32 = new _31; \
    _35 = new _34; \
    _38 = new _37;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_42(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15, \
                                                _16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28, \
                                                _29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41, \
                                                _42) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10; \
    _14 = new _13; \
    _17 = new _16; \
    _20 = new _19; \
    _23 = new _22; \
    _26 = new _25; \
    _29 = new _28; \
    _32 = new _31; \
    _35 = new _34; \
    _38 = new _37; \
    _41 = new _40;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_45(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15, \
                                                _16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28, \
                                                _29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41, \
                                                _42,_43,_44,_45) \
    _2 = new _1; \
    _5 = new _4; \
    _8 = new _7; \
    _11 = new _10; \
    _14 = new _13; \
    _17 = new _16; \
    _20 = new _19; \
    _23 = new _22; \
    _26 = new _25; \
    _29 = new _28; \
    _32 = new _31; \
    _35 = new _34; \
    _38 = new _37; \
    _41 = new _40; \
    _44 = new _43;

#define CONFIG_TABLE_KASE_WIDGETS_IMPL_VAR_COUNT(count) CONFIG_TABLE_KASE_WIDGETS_IMPL_PARAM_##count
#define CONFIG_TABLE_KASE_WIDGETS_IMPL(...) MACRO_ID(PARAM_PICK_APPLY(CONFIG_TABLE_KASE_WIDGETS_IMPL_VAR_COUNT,PP_COUNT(__VA_ARGS__))(__VA_ARGS__))

#define CONFIG_TABLE_KASE(kase,...) \
class kase##_ConfigModel : public Q_Config_Table_Model { \
public: \
    kase##_ConfigModel(QObject *parent = nullptr) : Q_Config_Table_Model(parent) \
    { \
        _item.clear(); \
        _item = QStringList{ PARAM_PICK_1_OF_3_3(__VA_ARGS__) }; \
        _item.push_front("");\
        setRowCount(rowCount(QModelIndex())); \
        setColumnCount(columnCount(QModelIndex())); \
    } \
}; \
class kase##_ConfigDelegate : public Q_Config_Table_Delegate_Kase { \
public: \
    kase##_ConfigDelegate(QObject *parent = nullptr) \
    { CONFIG_TABLE_KASE_WIDGETS_IMPL(__VA_ARGS__) } \
    PARAM_PICK_2_OF_3_1(__VA_ARGS__,,,*,;,) \
};

#define RESULT_TABLE_KASE(kase,...) \
class kase##_ResultModel : public Q_Config_Table_Model { \
public: \
    kase##_ResultModel(QObject *parent = nullptr) : Q_Config_Table_Model(parent) \
    { \
        _item.clear(); \
        _item = QStringList{ PARAM_PICK_1_OF_3_3(__VA_ARGS__) }; \
        _item.push_front(""); \
        setRowCount(rowCount(QModelIndex())); \
        setColumnCount(columnCount(QModelIndex())); \
    } \
}; \
class kase##_ResultDelegate : public Q_Result_Table_Delegate_Kase { \
public: \
    kase##_ResultDelegate(QObject *parent = nullptr) \
    { CONFIG_TABLE_KASE_WIDGETS_IMPL(__VA_ARGS__) } \
    PARAM_PICK_2_OF_3_1(__VA_ARGS__,,,*,;,) \
};

#endif // Q_CONFIG_TABLE_H
