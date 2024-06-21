#include "CustomStayClickEvnetWidget.h"
#include <QMouseEvent>
#include <UIClass/DialogWidget/AdminSerialDialog.h>

struct CustomStayClickEvnetWidget::privateStruct
{
    int mClickCount = 0;
    int mMaxCount;
};

CustomStayClickEvnetWidget::CustomStayClickEvnetWidget(QWidget *parent, int count)
    :QWidget(parent), d(new privateStruct)
{
    d->mMaxCount = count;
}

CustomStayClickEvnetWidget::~CustomStayClickEvnetWidget()
{

}

void CustomStayClickEvnetWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        d->mClickCount++;
        if(d->mClickCount > d->mMaxCount)
        {
            AdminSerialDialog* dialog = new AdminSerialDialog();
            dialog->setModal(false);
            dialog->open();
            d->mClickCount = 0;
        }
    }
}

