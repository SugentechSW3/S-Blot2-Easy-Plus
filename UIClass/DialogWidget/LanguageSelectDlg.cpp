#include "LanguageSelectDlg.h"
#include "ui_LanguageSelectDlg.h"

#include "Analysis/ConfigInformation.h"
#include "MessageShowDlg.h"

#include <QRadioButton>

struct LanguageSelectDlg::privateStruct
{
    GlobalDataStruct::LANGUAGE_LIST mSelectedLanguage = GlobalDataStruct::LANGUAGE_LIST_ENG;
    QVector<QRadioButton*> mLanguageRadioButtonList;
    bool mIsSave = true;
};

LanguageSelectDlg::LanguageSelectDlg(QWidget *parent, bool isSave) :
    CDialogParentWidget(parent),
    ui(new Ui_LanguageSelectDlg),
    d(new privateStruct)
{
    ui->setupUi(this);
    this->initButton();
    this->initDialog();
    this->initSignalSlot();
    d->mIsSave = isSave;
}

LanguageSelectDlg::~LanguageSelectDlg()
{
}


void LanguageSelectDlg::initButton()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCANCEL->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");
}

void LanguageSelectDlg::initDialog()
{
    auto config = ConfigInformation::getInstance();

    auto lang = config->getDefaultLanguage();

    int langaugeCount = (int)GlobalDataStruct::LANGUAGE_LIST_COUNT;

    for(int languageIdx = 0; languageIdx < langaugeCount; languageIdx++)
    {
        QString radioButtonText = config->getLanguagetoString((GlobalDataStruct::LANGUAGE_LIST)languageIdx);

        QRadioButton* lngRadioButton = new QRadioButton(radioButtonText);
        ui->verticalLayout->addWidget(lngRadioButton);
        d->mLanguageRadioButtonList.push_back(lngRadioButton);

        if(languageIdx == lang)
            lngRadioButton->setChecked(true);
    }

    this->repaint();
}

void LanguageSelectDlg::initSignalSlot()
{
    connect(ui->btnOK, &QPushButton::clicked, this, [this]
    {        
        auto config = ConfigInformation::getInstance();

        for(int index = 0; index < d->mLanguageRadioButtonList.count(); index++)
        {
            if(d->mLanguageRadioButtonList.at(index)->isChecked() == true)
            {
                d->mSelectedLanguage = (GlobalDataStruct::LANGUAGE_LIST)index;
                break;
            }
        }
        if(d->mIsSave)
        {
            config->setCurrentLanguage(d->mSelectedLanguage);
            this->infomationMessage();
        }
        else
            config->setExportChangeLanguage(d->mSelectedLanguage);
        this->accept();
    });

    connect(ui->btnCANCEL, &QPushButton::clicked, this, &QDialog::reject);

}

void LanguageSelectDlg::infomationMessage()
{
    MessageShowDlg* dlg = new MessageShowDlg(tr("Restart is required after the language is changed."),this);
    dlg->exec();
}
