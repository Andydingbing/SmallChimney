#ifndef Q_TABLE_VIEW_HPP
#define Q_TABLE_VIEW_HPP

#include <QHeaderView>
#include <QTableView>

class Q_Table_View : public QTableView
{
public:
    Q_Table_View(QWidget *parent = nullptr) :
        QTableView(parent)
    {
        horizontalHeader()->setHighlightSections(false);
        verticalHeader()->setDefaultSectionSize(20);
        verticalHeader()->setVisible(false);
        setSelectionBehavior(QAbstractItemView::SelectRows);
        setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        setStyleSheet("QTableView{selection-background-color:rgb(0,162,232);selection-color:white}");
    }
};

#endif // Q_TABLE_VIEW_HPP
