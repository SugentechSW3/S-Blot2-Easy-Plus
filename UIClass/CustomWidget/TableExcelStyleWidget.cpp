#include <QApplication>
#include <QClipboard>
#include <QShortcut>
#include <QModelIndex>
#include <QModelIndexList>
#include <QDebug>
#include <QKeyEvent>
#include <QHeaderView>
#include "TableExcelStyleWidget.h"


struct TableExcelStyleWidget::privateStruct
{
    QScopedPointer<QShortcut> mCopyShortCut;
    QScopedPointer<QShortcut> mPasteShortCut;
};

TableExcelStyleWidget::TableExcelStyleWidget(QWidget *parent) :
    QTableWidget(parent),
    d(new privateStruct)
{
    this->initVariables();
    this->initSignalSlot();

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    this->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: #f5f5f5; }");
    this->setSizePolicy(sizePolicy);
}

TableExcelStyleWidget::~TableExcelStyleWidget()
{
}

void TableExcelStyleWidget::initVariables()
{
    this->initShortCut();
    this->setAlternatingRowColors(true);
}

void TableExcelStyleWidget::initShortCut()
{
    d->mCopyShortCut.reset(new QShortcut(this));
    d->mCopyShortCut->setKey(Qt::CTRL + Qt::Key_C);

    d->mPasteShortCut.reset(new QShortcut(this));
    d->mPasteShortCut->setKey(Qt::CTRL + Qt::Key_V);
}

void TableExcelStyleWidget::initSignalSlot()
{
    auto header = this->horizontalHeader();

    connect(header, &QHeaderView::sectionClicked, this, &TableExcelStyleWidget::onSortRequest);
    connect(d->mCopyShortCut.data(), &QShortcut::activated, this, &TableExcelStyleWidget::onEmittedCopyShortCut);
}

void TableExcelStyleWidget::onEmittedCopyShortCut()
{
    auto modelItems  = this->selectedIndexes();
    std::sort(modelItems.begin(), modelItems.end());

    int prevRow = 0;
    QString clipBoardString;

    for(auto currentItem : modelItems)
    {
        if(!clipBoardString.isEmpty())
        {
            if(currentItem.row() == prevRow)
                clipBoardString.append("\t");
            else
                clipBoardString.append("\n");
        }

        clipBoardString.append(currentItem.data().toString());
        prevRow = currentItem.row();
    }

    auto clipBoard = QApplication::clipboard();
    clipBoard->setText(clipBoardString);
}

//this function Used by setCellWidgetItems
void TableExcelStyleWidget::keyPressEvent(QKeyEvent *event)
{
    QTableWidget::keyPressEvent(event);
    emit onKeyPressed(this->currentRow(), this->currentColumn(), event->key());
}
