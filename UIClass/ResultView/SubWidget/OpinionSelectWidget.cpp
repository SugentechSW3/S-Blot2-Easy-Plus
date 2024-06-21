#include "UIClass/DialogWidget/InspectionOpinionDlg.h"

#include "mainwindow.h"
#include "OpinionSelectWidget.h"
#include "ui_OpinionSelectWidget.h"

OpinionSelectWidget::OpinionSelectWidget(QWidget *parent) :
                                                            QWidget(parent),
                                                            ui(new Ui_OpinionSelectWidget)
{
    ui->setupUi(this);
    this->initSignalSlot();
    this->initOpinion();
}

OpinionSelectWidget::~OpinionSelectWidget()
{
}

void OpinionSelectWidget::initSignalSlot()
{
    connect(ui->comboTitle, QOverload<const QString &>::of(&QComboBox::activated), this, &OpinionSelectWidget::onChangedComboboxSelect);
    connect(ui->btnSetup, &QPushButton::clicked, this, &OpinionSelectWidget::onClickSetup);
    connect(ui->btnSave, &QPushButton::clicked, this, &OpinionSelectWidget::onApply);
    connect(ui->btnClear, &QPushButton::clicked, this, [this]
            {
                this->clearComment();
                this->onApply();
            });
}

void OpinionSelectWidget::initOpinion()
{
    auto data = InspectionOpinionDlg::getOpinionData();
    this->setupData(data);
}

void OpinionSelectWidget::clearComment()
{
    if(ui->comboTitle->count() != 0)
        ui->comboTitle->setCurrentIndex(0);

    ui->editOpinion->clear();
}

QString OpinionSelectWidget::getCurrentComboboxData()
{
    return ui->comboTitle->currentData().toString();
}

GlobalDataStruct::OPINION_DATA OpinionSelectWidget::getCurrentSelectedData()
{
    GlobalDataStruct::OPINION_DATA data;

    data.title = this->getCurrentTitleText();
    data.opinion = this->getCurrentComboboxData();

    return data;
}

void OpinionSelectWidget::setupData(const QVector<GlobalDataStruct::OPINION_DATA>& data)
{
    ui->comboTitle->clear();
    ui->comboTitle->addItem("","");

    for(auto& itr : data)
        ui->comboTitle->addItem(itr.title, itr.opinion);
}

QString OpinionSelectWidget::getOpinionText()
{
    return ui->editOpinion->toPlainText();
}

void OpinionSelectWidget::setOpinionText(const QString& text)
{
    ui->editOpinion->setText(text);
}

QString OpinionSelectWidget::getCurrentTitleText()
{
    return ui->comboTitle->currentText();
}

void OpinionSelectWidget::setCurrentTitleText(const QString& text)
{
    ui->comboTitle->setCurrentText(text);
}

void OpinionSelectWidget::onChangedComboboxSelect(const QString& text)
{
    auto opinion  = this->getCurrentComboboxData();
    this->setOpinionText(opinion);
}

void OpinionSelectWidget::onClickSetup()
{
    auto topLevelWidget = MainWindow::getInstance();
    InspectionOpinionDlg dlg(topLevelWidget);
    dlg.exec();

    auto data = dlg.getOpinionData();
    this->setupData(data);
}
