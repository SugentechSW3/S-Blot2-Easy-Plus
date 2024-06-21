#include "TitleStatusBarWidget.h"
#include "ui_TitleStatusBarWidget.h"

TitleStatusBarWidget::TitleStatusBarWidget(QWidget *parent) :
                                                              QWidget(parent),
                                                              ui(new Ui_TitleStatusBarWidget)
{
    ui->setupUi(this);
    this->onStatusMain();
}

TitleStatusBarWidget::~TitleStatusBarWidget()
{
}

void TitleStatusBarWidget::onStatusMain()
{
    QString resourcePath;
    resourcePath.append(":/Image/Image/Basic/head_1.png");
    ui->widget->setStyleSheet(QString("QWidget#%1{border-image: url(%2);};").arg(ui->widget->objectName()).arg(resourcePath));
}

void TitleStatusBarWidget::onStatusPatientInfomation()
{
    QString resourcePath;
    resourcePath.append(":/Image/Image/Basic/head_2.png");
    ui->widget->setStyleSheet(QString("QWidget#%1{border-image: url(%2);};").arg(ui->widget->objectName()).arg(resourcePath));
}

void TitleStatusBarWidget::onStatusReagent()
{
    QString resourcePath;
    resourcePath.append(":/Image/Image/Basic/head_3.png");
    ui->widget->setStyleSheet(QString("QWidget#%1{border-image: url(%2);};").arg(ui->widget->objectName()).arg(resourcePath));
}

void TitleStatusBarWidget::onStatusRunningStatus()
{
    QString resourcePath;
    resourcePath.append(":/Image/Image/Basic/head_4.png");
    ui->widget->setStyleSheet(QString("QWidget#%1{border-image: url(%2);};").arg(ui->widget->objectName()).arg(resourcePath));
}

void TitleStatusBarWidget::onStatusResultView()
{
    QString resourcePath;
    resourcePath.append(":/Image/Image/Basic/head_5.png");
    ui->widget->setStyleSheet(QString("QWidget#%1{border-image: url(%2);};").arg(ui->widget->objectName()).arg(resourcePath));
}
