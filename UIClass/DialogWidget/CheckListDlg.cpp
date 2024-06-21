#include <QSpacerItem>
#include <QTimer>

#include "CheckListDlg.h"
#include "ui_CheckListDlg.h"
#include "Common/CommonParentWidget/CCheckBoxParentWidget.h"

struct CheckListDlg::privateStruct
{
    QVector<CCheckBoxParentWidget*> mCheckBoxList;
    QVector<GlobalDataStruct::AnalysisReportInfomation> mSigleCheckBoxData;
};

CheckListDlg::CheckListDlg(QWidget *parent) :
                                              CDialogParentWidget(parent),
                                              ui(new Ui_CheckListDlg),
                                              d(new privateStruct)
{
    ui->setupUi(this);
    this->initStyleSheet();
    ui->btnContinue->setDisabled(true);

    connect(ui->btnContinue, &QPushButton::clicked, this, &CheckListDlg::accept);
}

CheckListDlg::~CheckListDlg()
{

}

void CheckListDlg::initStyleSheet()
{
    ui->btnContinue->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                        ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");
}

void CheckListDlg::setText(const QString& text)
{
    ui->labelText->setText(text);
}

void CheckListDlg::setCheckListSingleDataMode(const QVector<GlobalDataStruct::AnalysisReportInfomation>& checkList)
{
    this->clearCheckBox();
    d->mSigleCheckBoxData.clear();

    //QString checkBoxText = QString(tr("Press the continue button after checking the reagent injection."));
    QString checkBoxText = QString(tr("After checking the reagent injection and closing \nthe front door, press the continue button."));
    d->mSigleCheckBoxData = checkList;

    this->addCheckBox(checkBoxText);

    auto spacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->verticalLayout->addItem(spacer);

    this->showMinimized();
}

void CheckListDlg::setCheckList(const QVector<GlobalDataStruct::AnalysisReportInfomation>& checkList)
{
    this->clearCheckBox();

    for(auto& itr : checkList)
    {
        QString checkBoxText = QString(tr("ID : %1 / Name : %2 / Stripnumber : %3")
                                           .arg(itr.element.patientID).arg(itr.element.name).arg(itr.element.stripNumber));
        this->addCheckBox(checkBoxText);
    }

    auto spacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->verticalLayout->addItem(spacer);
}

QVector<int> CheckListDlg::getUncheckedStripNumberList()
{
    if(d->mSigleCheckBoxData.isEmpty())
        return this->getUncheckedStripForMulti();
    else
        return this->getUncheckedStripForSingle();
}

void CheckListDlg::setFontSize(const int& fontSize)
{
    auto currentFont = ui->labelText->font();
    currentFont.setPointSize(fontSize);
    ui->labelText->setFont(currentFont);
}

void CheckListDlg::setButtonHeight(const int &buttonHeight)
{
    ui->btnContinue->setFixedHeight(buttonHeight);
}

int CheckListDlg::getUncheckedStripNumberFromCheckBoxText(const QString& text)
{
    auto targetText = text.trimmed();
    auto firstSplitStringList = targetText.split("/");

    if(text.count() < 3)
        return 0;

    auto seconedSplitStringList = firstSplitStringList[2];
    auto finalStripString = seconedSplitStringList.split(":").last();

    return finalStripString.toInt();
}

QVector<int> CheckListDlg::getUncheckedStripForSingle()
{
    QVector<int> uncheckedList;

    if(d->mCheckBoxList.isEmpty())
        return uncheckedList;

    auto targetUI = d->mCheckBoxList.first();

    if(targetUI == nullptr)
        return uncheckedList;

    auto isChecked = targetUI->isChecked();

    if(isChecked)
        return uncheckedList;

    for(auto&itr : d->mSigleCheckBoxData)
        uncheckedList << itr.element.stripNumber;

    return uncheckedList;
}

QVector<int> CheckListDlg::getUncheckedStripForMulti()
{
    QVector<int> uncheckedList;

    for(auto&itr : d->mCheckBoxList)
    {
        if(itr == nullptr)
            continue;

        if(itr->isChecked())
            continue;

        auto currentText = itr->text();
        auto stripNumber = this->getUncheckedStripNumberFromCheckBoxText(currentText);
        uncheckedList.push_back(stripNumber);
    }

    return uncheckedList;
}

void CheckListDlg::addCheckBox(const QString& checkBoxName)
{
    auto checkBox =  new CCheckBoxParentWidget(ui->scrollAreaWidgetContents);
    checkBox->setText(checkBoxName);

    auto currentFont = checkBox->font();
    currentFont.setPointSize(19);
    checkBox->setFont(currentFont);

    ui->verticalLayout->addWidget(checkBox);
    d->mCheckBoxList.push_back(checkBox);

    connect(checkBox, &QCheckBox::clicked, this, &CheckListDlg::onUpdateWidget);
}

void CheckListDlg::clearCheckBox()
{
    for(auto itr : d->mCheckBoxList)
        itr->deleteLater();

    d->mCheckBoxList.clear();

    this->onUpdateWidget();
}

void CheckListDlg::onUpdateWidget()
{
    int sum = 0;

    if(d->mCheckBoxList.isEmpty())
        return;

    for(auto& itr : d->mCheckBoxList)
    {
        if(itr->isChecked())
            sum++;
    }

    double calcPercent = (100.0/(double)d->mCheckBoxList.count());

    if(d->mCheckBoxList.isEmpty())
        ui->progressBar->setMovingValue(0);
    else
        ui->progressBar->setMovingValue(calcPercent * (double)sum);


    if(sum == d->mCheckBoxList.count())
        ui->btnContinue->setEnabled(true);
    else
        ui->btnContinue->setDisabled(true);
}

