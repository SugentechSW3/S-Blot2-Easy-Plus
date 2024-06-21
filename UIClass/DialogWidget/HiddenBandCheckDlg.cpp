#include "HiddenBandCheckDlg.h"
#include "ui_HiddenBandCheckDlg.h"
#include "Analysis/ConfigInformation.h"

#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <Common/Io/INIFileControls/AllergyIniDataControls.h>

struct HiddenBandCheckDlg::privateStruct
{
    QVector<QCheckBox*> mBandCheckBoxs;
    QScopedPointer<QWidget> mGroupWidget;
    ConfigInformation* mConfig = nullptr;
    QStringList mHiddenCode;
};

HiddenBandCheckDlg::HiddenBandCheckDlg(QWidget *parent) :
    CDialogParentWidget(parent),d(new privateStruct),
    ui(new Ui_HiddenBandCheckDlg)
{
    ui->setupUi(this);
    this->init();
}

HiddenBandCheckDlg::~HiddenBandCheckDlg()
{

}

void HiddenBandCheckDlg::initVariables()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCancel->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");

}

void HiddenBandCheckDlg::initSignalSlots()
{
    connect(ui->btnCancel,&CustomImageButtonWidget::clicked,this,[=]
    {
       this->reject();
    });
    connect(ui->btnOK, &CustomImageButtonWidget::clicked, this, &HiddenBandCheckDlg::onClickHiddenBandSettingSave);
}

void HiddenBandCheckDlg::initDrawUI()
{
    QMap<QString,QString> panelCodes = d->mConfig->getContentPanelCodes(d->mConfig->getCurrentContents());

    auto baseWidget = new QWidget(ui->groupBox);
    d->mGroupWidget.reset(baseWidget);

    ui->gridLayout->addWidget(baseWidget,0,0,1,1);

    auto verticalLayout = new QVBoxLayout(baseWidget);
    baseWidget->setLayout(verticalLayout);

    auto panelCodeKeys = panelCodes.keys();
    foreach (auto panelCode, panelCodeKeys)
    {
        QCheckBox* checkbox = new QCheckBox(baseWidget);
        checkbox->setText(panelCode);
        checkbox->setChecked(isCodeCheckHiddenCode(panelCode));
        checkbox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

        d->mBandCheckBoxs.push_back(checkbox);
    }

    QHBoxLayout* layout = nullptr;

    for(int i = 0; i < d->mBandCheckBoxs.count(); i++)
    {
        if(i % 10 == 0) //10개단위로 UI 배치
        {
            layout = new QHBoxLayout();
            verticalLayout->addLayout(layout);
        }

       layout->addWidget(d->mBandCheckBoxs.at(i));
    }

    this->repaint();
}

void HiddenBandCheckDlg::initLoadData()
{
    d->mConfig = ConfigInformation::getInstance();

    AllergyIniDataControls behindCodeIni;
    auto contentsName = d->mConfig->getCurrentContents();
    d->mHiddenCode = behindCodeIni.readHiddenBandCodeData(contentsName);
}

void HiddenBandCheckDlg::init()
{
    this->initVariables();
    this->initSignalSlots();
    this->initDrawUI();
}

bool HiddenBandCheckDlg::isCodeCheckHiddenCode(const QString &code)
{
    return d->mHiddenCode.contains(code);
}

void HiddenBandCheckDlg::onClickHiddenBandSettingSave()
{
    AllergyIniDataControls behindCodeIni;
    QStringList hiddenCodeList;

    foreach (auto checkbox, d->mBandCheckBoxs)
    {
        if(checkbox->isChecked() == true)
            hiddenCodeList.append(checkbox->text());
    }
    auto contentsName = d->mConfig->getCurrentContents();
    behindCodeIni.writeHiddenBandCodeData(contentsName,hiddenCodeList);
}
