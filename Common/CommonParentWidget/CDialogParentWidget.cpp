#include "CDialogParentWidget.h"
#include "mainwindow.h"

#include <QDebug>

struct CDialogParentWidget::privateStruct
{
    bool mIsSkipClose = false;
};

CDialogParentWidget::CDialogParentWidget(QWidget *parent) :
                                                            QDialog(parent),
                                                            d(new privateStruct)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    this->onDialogOpen();

    auto parentPtr = qobject_cast<QDialog*>(parent);

    if(parentPtr != nullptr)
        this->setKeepBlockingBackground(true);
}

CDialogParentWidget::~CDialogParentWidget()
{
    if(d->mIsSkipClose == false)
        this->onDialogClose();
}

void CDialogParentWidget::onDialogOpen()
{
    auto instance = MainWindow::getInstance();

    if(instance == nullptr)
        return;

    instance->setDisableTopLevelWidget(true);
}

void CDialogParentWidget::onDialogClose()
{
    if(d->mIsSkipClose == true)
        return;

    auto instance = MainWindow::getInstance();

    if(instance == nullptr)
        return;

    instance->setDisableTopLevelWidget(false);
}

void CDialogParentWidget::setKeepBlockingBackground(bool isBlock)
{
    d->mIsSkipClose = isBlock;
}
