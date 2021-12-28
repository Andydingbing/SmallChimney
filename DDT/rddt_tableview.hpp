#ifndef Q_RDDT_TABLEVIEW_HPP
#define Q_RDDT_TABLEVIEW_HPP

#include <QHeaderView>
#include <QTableView>

class Q_RDDT_TableView : public QTableView
{
public:
    Q_RDDT_TableView(QWidget *parent = nullptr) :
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

#endif // Q_RDDT_TABLEVIEW_HPP
