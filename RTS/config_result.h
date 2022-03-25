#ifndef COMMON_CONFIG_RESULT_H
#define COMMON_CONFIG_RESULT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "config_table.h"

class Q_Config_Result_Widget : public QWidget
{
public:
    Q_Config_Result_Widget(QWidget *parent) : QWidget(parent)
    {
        if (objectName().isEmpty()) {
            setObjectName(QString::fromUtf8("Config_Result_Widget"));
        }

        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
        setSizePolicy(sizePolicy);
        setMinimumSize(QSize(0, 0));
        setMaximumSize(QSize(280, 16777215));

        verticalLayout = new QVBoxLayout(this);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);

        splitter = new QSplitter(this);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        splitter->setHandleWidth(0);

        config = new Q_Config_Table_View(splitter);
        config->setObjectName(QString::fromUtf8("config"));
        splitter->addWidget(config);

        result = new Q_Config_Table_View(splitter);
        result->setObjectName(QString::fromUtf8("result"));
        splitter->addWidget(result);

        verticalLayout->addWidget(splitter);
        verticalLayout->setStretch(0, 10);

        QMetaObject::connectSlotsByName(this);
    }

    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    Q_Config_Table_View *config;
    Q_Config_Table_View *result;
};

#endif
