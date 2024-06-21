#include "NavigatorArrowWidget.h"
#include "ui_NavigatorArrowWidget.h"

NavigatorArrowWidget::NavigatorArrowWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_NavigatorArrowWidget)
{
    ui->setupUi(this);
    this->initSignalSlots();
}

NavigatorArrowWidget::~NavigatorArrowWidget()
{
}

void NavigatorArrowWidget::initSignalSlots()
{
    connect(ui->btnLeft, SIGNAL(clicked()), SIGNAL(onClickedPrev()));
    connect(ui->btnRight, SIGNAL(clicked()), SIGNAL(onClickedNext()));
}
