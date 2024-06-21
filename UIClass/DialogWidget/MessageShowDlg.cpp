#include "MessageShowDlg.h"
#include "ui_MessageShowDlg.h"

#include "Common/Io/CSettingIo.h"

USING_SUGENTECH


struct MessageShowDlg::privateStruct
{
    QString mPrimaryKey;
};

MessageShowDlg::MessageShowDlg(const QString& message, QWidget *parent) :
                                                                          CDialogParentWidget(parent),
                                                                          ui(new Ui_MessageShowDlg),
                                                                          d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
    this->setNotShowingCheckBoxVisible(false);
    this->setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->btnContinue, &QPushButton::clicked, this, [this]
            {
                this->closing();
            });

    ui->labelText->setText(message);
}

MessageShowDlg::~MessageShowDlg()
{
}

void MessageShowDlg::closing()
{
    this->saveShowingData();
    this->accept();
    this->close();
}

void MessageShowDlg::setDefaultTextAlignment()
{
    ui->labelText->setAlignment(Qt::AlignLeft);
}

void MessageShowDlg::setFontSize(const int& fontSize)
{
    auto currentFont = ui->labelText->font();
    currentFont.setPointSize(fontSize);
    ui->labelText->setFont(currentFont);
}
void MessageShowDlg::setFont(const QFont& font)
{
    ui->labelText->setFont(font);
}

void MessageShowDlg::setText(const QString& text)
{
    ui->labelText->setText(text);
}

void MessageShowDlg::setButtonText(const QString& text)
{
    ui->btnContinue->setText(text);
}

void MessageShowDlg::setButtonHeight(const int& heightSize)
{
    int height = heightSize;
    ui->btnContinue->setFixedHeight(height);
}

void MessageShowDlg::setNotShowingCheckBoxVisible(bool isVisable)
{
    if(isVisable == true)
        ui->checkReShow->show();
    else
        ui->checkReShow->hide();
}

void MessageShowDlg::init()
{
    this->initStyleSheet();
}

void MessageShowDlg::initStyleSheet()
{
    ui->btnContinue->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                        ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");
}

void MessageShowDlg::enableNotShowingMessage(const QString& primaryKey)
{
    CSettingIo io("Dialog");
    auto isHide = io.getConfigureData("MessageShow", primaryKey).toBool();

    if(isHide == true)
        this->closing();

    this->setNotShowingCheckBoxVisible(true);
    d->mPrimaryKey = primaryKey;
}

void MessageShowDlg::saveShowingData()
{
    if(d->mPrimaryKey.isEmpty() == true)
        return;

    auto isHide = ui->checkReShow->isChecked();

     CSettingIo io("Dialog");
     io.setConfigureData("MessageShow", d->mPrimaryKey, isHide);
}

