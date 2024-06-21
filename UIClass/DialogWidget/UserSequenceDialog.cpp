#include <QRadioButton>
#include <QLabel>

#include "UserSequenceDialog.h"
#include "ui_UserSequenceDialog.h"

#include "Analysis/ConfigInformation.h"

struct UserSequenceDialog::privateStruct
{
    QVector<QLabel*> mLabelList;
    QMap<int, QRadioButton*> mRadioButtonList;
    int mCurrentSelectedIdx = 1;
};


UserSequenceDialog::UserSequenceDialog(int currentSequence, QWidget *parent) :
    CDialogParentWidget(parent),
    ui(new Ui_UserSequenceDialog),
    d(new privateStruct)
{
    ui->setupUi(this);

    this->initVariable();
    this->initSignalSlots();
    this->setCurrentIdx(currentSequence);
    this->initRadioButton();
}

UserSequenceDialog::~UserSequenceDialog()
{
}

void UserSequenceDialog::initVariable()
{
    auto config = ConfigInformation::getInstance();
    auto currentContents = config->getCurrentContents();
    auto data = config->getSequenceList(currentContents);
    int count = 0;

    QHBoxLayout* hLayout = nullptr;
    QVBoxLayout* vLayout = ui->verticalLayout;

    for(auto& itr : data)
    {
        if((count%3) == 0)
        {
            hLayout = new QHBoxLayout();
            vLayout->addLayout(hLayout);
        }

        if(d->mLabelList.count() != 0)
        {
            QLabel* arrowLabel = new QLabel(this);
            arrowLabel->setText(" > ");
            arrowLabel->setAlignment(Qt::AlignCenter);
            arrowLabel->setMaximumWidth(10);

            if(hLayout != nullptr)
                hLayout->addWidget(arrowLabel);
        }

        QLabel* labelElement = new QLabel(this);
        labelElement->setText(ConfigInformation::convertStepEnumToStepName(itr.stepNumber));
        labelElement->setAlignment(Qt::AlignCenter);
        labelElement->setMinimumWidth(125);
        labelElement->setMaximumWidth(125);

        d->mLabelList.push_back(labelElement);
        count++;

        if(hLayout != nullptr)
            hLayout->addWidget(labelElement);
    }

    for(int i = 0; i< data.count(); i++)
    {
        d->mRadioButtonList[i] = new QRadioButton(d->mLabelList[i]->text(),this);
        ui->groupLayoutRadioButton->addWidget(d->mRadioButtonList[i]);
    }

    this->initStyleSheet();
}

void UserSequenceDialog::initSignalSlots()
{
    connect(ui->btnOK, &QPushButton::clicked, this, &UserSequenceDialog::accept);
    connect(ui->btnCancel, &QPushButton::clicked, this, &UserSequenceDialog::reject);

    for(int i =0; i< d->mRadioButtonList.count() ; i++)
        connect(d->mRadioButtonList[i], &QRadioButton::clicked, this, [this] (bool)
                {
                    for(int i = 0; i< d->mRadioButtonList.count(); i++)
                    {
                        if(d->mRadioButtonList[i]->isChecked() == true)
                        {
                            this->setCurrentIdx(i+1);
                            return;
                        }
                    }

                });
}

void UserSequenceDialog::initStyleSheet()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCancel->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");
}

void UserSequenceDialog::initRadioButton()
{
    if(d->mRadioButtonList.isEmpty())
        return;

    if(d->mCurrentSelectedIdx == 1)
    {
        d->mRadioButtonList.first()->setChecked(true);
        return;
    }

    auto radioIdx = d->mCurrentSelectedIdx - 1;

    if(d->mRadioButtonList.count() <= radioIdx)
        return;

    d->mRadioButtonList[radioIdx]->setChecked(true);
}

void UserSequenceDialog::onEnableStep(int begin)
{
    if(d->mLabelList.count() <= begin )
        return;

    for(int i = begin ; i > -1 ; i--)
        d->mLabelList[i]->setStyleSheet("color:red");

    for(int i = begin ; i< d->mLabelList.count(); i++)
        d->mLabelList[i]->setStyleSheet("color:green");

    d->mCurrentSelectedIdx = begin + 1;
}

void UserSequenceDialog::setCurrentIdx(int idx)
{
    if(idx == 0)
        return;

    this->onEnableStep(idx-1);
}

int UserSequenceDialog::getCurrentIdx()
{
    return d->mCurrentSelectedIdx;
}
