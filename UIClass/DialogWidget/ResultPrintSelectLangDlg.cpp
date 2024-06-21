#include "ResultPrintSelectLangDlg.h"
#include "ui_ResultPrintSelectLangDlg.h"

#include "Common/Io/CSettingIo.h"
#include "Analysis/ConfigInformation.h"

USING_SUGENTECH

struct ResultPrintSelectLangDlg::privateStruct
{
};

ResultPrintSelectLangDlg::ResultPrintSelectLangDlg(QWidget *parent) :
                                                      CDialogParentWidget(parent),
                                                      ui(new Ui_ResultPrintSelectLangDlg),
                                                      d(new privateStruct)
{
    ui->setupUi(this);

    this->initData();
    this->initSignalSlots();
    this->initStyleSheet();
}

ResultPrintSelectLangDlg::~ResultPrintSelectLangDlg()
{

}

void ResultPrintSelectLangDlg::initData()
{
    auto config = ConfigInformation::getInstance();

    for(int LanguageIndex = 0; LanguageIndex < GlobalDataStruct::LANGUAGE_LIST_COUNT; LanguageIndex++)
    {
        ui->comboLanguage->addItem(config->getLanguagetoString((GlobalDataStruct::LANGUAGE_LIST)LanguageIndex));
        ui->comboSecondLanguage->addItem(config->getLanguagetoString((GlobalDataStruct::LANGUAGE_LIST)LanguageIndex));
    }

    auto currentLanguage = (int)config->getDefaultLanguage();
    ui->comboLanguage->setCurrentIndex(currentLanguage);

    ui->comboSecondLanguage->setCurrentIndex(0);
}

void ResultPrintSelectLangDlg::initStyleSheet()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCANCEL->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");
}

void ResultPrintSelectLangDlg::initSignalSlots()
{
    connect(ui->btnOK, &QPushButton::clicked, this, [this]
            {
                auto config = ConfigInformation::getInstance();
                config->setExportChangeLanguage((GlobalDataStruct::LANGUAGE_LIST)ui->comboLanguage->currentIndex());
                config->setExportChangeSecondLanguage((GlobalDataStruct::LANGUAGE_LIST)ui->comboSecondLanguage->currentIndex());
                config->changeCurrentLanguage(config->getExportChangeLanguage());

                this->accept();
            });

    connect(ui->btnCANCEL, &QPushButton::clicked, this, &ResultPrintSelectLangDlg::reject);
}
