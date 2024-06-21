#include <QFileDialog>

#include "ResultExportDlg.h"
#include "ui_ResultExportDlg.h"

#include "Common/Io/CSettingIo.h"
#include "UIClass/DialogWidget/MessageShowDlg.h"
#include "Analysis/ConfigInformation.h"
#include "Analysis/GlobalDataStruct.h"

USING_SUGENTECH

struct ResultExportDlg::privateStruct
{
    QString mPath;    
};

ResultExportDlg::ResultExportDlg(QWidget *parent) :
                                                    CDialogParentWidget(parent),
                                                    ui(new Ui_ResultExportDlg),
                                                    d(new privateStruct)
{
    ui->setupUi(this);

    this->initData();
    this->initSignalSlots();
    this->initStyleSheet();
}

ResultExportDlg::~ResultExportDlg()
{

}

void ResultExportDlg::initData()
{        
    auto config = ConfigInformation::getInstance();

    this->loadData();

    for(int LanguageIndex = 0; LanguageIndex < GlobalDataStruct::LANGUAGE_LIST_COUNT; LanguageIndex++)
    {
        ui->cbReportLanguage->addItem(config->getLanguagetoString((GlobalDataStruct::LANGUAGE_LIST)LanguageIndex));
        ui->cbReportSecondLanguage->addItem(config->getLanguagetoString((GlobalDataStruct::LANGUAGE_LIST)LanguageIndex));
    }

    auto currentLanguage = (int)config->getDefaultLanguage();
    ui->cbReportLanguage->setCurrentIndex(currentLanguage);

    ui->cbReportSecondLanguage->setCurrentIndex(0);
}

void ResultExportDlg::initVariables()
{    

}

void ResultExportDlg::initStyleSheet()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCANCEL->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");
}

void ResultExportDlg::initSignalSlots()
{
    connect(ui->btnOK, &QPushButton::clicked, this, [this]
            {
                if(d->mPath.isEmpty())
                {
                    this->emptyPathErrorMsg();
                    return;
                }
                auto config = ConfigInformation::getInstance();
                config->setExportChangeLanguage((GlobalDataStruct::LANGUAGE_LIST)ui->cbReportLanguage->currentIndex());
                config->setExportChangeSecondLanguage((GlobalDataStruct::LANGUAGE_LIST)ui->cbReportSecondLanguage->currentIndex());
                config->changeCurrentLanguage(config->getExportChangeLanguage());

                this->saveData();
                this->accept();
            });


    connect(ui->btnPathChange, &QPushButton::clicked, this, [this]
            {
                auto path = QFileDialog::getExistingDirectory(this, QString(), d->mPath, QFileDialog::DontUseNativeDialog);

                if(path.isEmpty() == false)
                    d->mPath = path;

                this->setupPathForPathLabel();
            });

    connect(ui->btnCANCEL, &QPushButton::clicked, this, &ResultExportDlg::reject);
}

QString ResultExportDlg::getPath()
{
    return d->mPath;
}

void ResultExportDlg::loadData()
{
    CSettingIo io;
    d->mPath = io.getConfigureData("ResultExportPath", "Path").toString();

    this->setupPathForPathLabel();
}

void ResultExportDlg::saveData()
{
    CSettingIo io;
    io.setConfigureData("ResultExportPath", "Path", d->mPath);
}

void ResultExportDlg::setupPathForPathLabel()
{
    ui->labelFullPath->setText(d->mPath);
}

void ResultExportDlg::emptyPathErrorMsg()
{
    MessageShowDlg* dlg = new MessageShowDlg(tr("No Storage path selected"),this);
    dlg->exec();
}

