#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDebug>

#include "QRStripCheckDlg.h"
#include "ui_QRStripCheckDlg.h"

#include "MessageShowDlg.h"
#include "DataBase/DataBaseCtrl.h"

#include "Common/Barcode/CQRCode.h"

#include "Analysis/ConfigInformation.h"

USING_SUGENTECH

struct QRStripCheckDlg::privateStruct
{
    QScopedPointer<DataBaseCtrl> mCodeFuncDataBase;
    QMap<int , QLineEdit*> mLineEdits;
    QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> mElement;
};

QRStripCheckDlg::QRStripCheckDlg(QWidget *parent) :
                                                    CDialogParentWidget(parent),
                                                    ui(new Ui_QRStripCheckDlg),
                                                    d(new privateStruct)
{
    ui->setupUi(this);
    this->initVariables();
    this->initStyleSheet();
    this->initSignalSlots();

    d->mCodeFuncDataBase.reset(new DataBaseCtrl(DataBaseCtrl::DATABASE_NAME_LIST::DBCodeFunc));
}

QRStripCheckDlg::~QRStripCheckDlg()
{

}

void QRStripCheckDlg::initVariables()
{
    auto config = ConfigInformation::getInstance();
    auto panelList = config->getPanelList();
    ui->comboPanelSelect->addItemForPanelColorText(panelList);
}

void QRStripCheckDlg::initStyleSheet()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCancel->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");
}

void QRStripCheckDlg::initSignalSlots()
{
    connect(ui->btnOK, &QPushButton::clicked, [this]
            {
                if(this->missingDataCheck() == true)
                    return;

                if(this->validDataCheck() == false)
                    return;

                if(this->validPanelCheck() == false)
                    return;

                this->accept();
            });

    connect(ui->btnCancel, &QPushButton::clicked, [this]
            {
                this->reject();
            });

    connect(ui->checkUseBatch, &QCheckBox::clicked, this, [this]
            (bool checked)
            {
                ui->editBatchLotNumber->setEnabled(checked);
                ui->comboPanelSelect->setEnabled(checked);

                if(checked)
                    this->onUpdateLotData();
            });


    connect(ui->editBatchLotNumber, &QLineEdit::textEdited, this, &QRStripCheckDlg::onUpdateLotData);
    connect(ui->comboPanelSelect, &QComboBox::currentTextChanged, this, &QRStripCheckDlg::onUpdateLotData);
}

void QRStripCheckDlg::setMissingQRList(const QVector<int>& missingStripQRPositionList)
{
    QVector<QVBoxLayout*> widgetSet;

    auto verticalLayout = new QVBoxLayout(ui->groupMissingList);
    ui->groupMissingList->setLayout(verticalLayout);

    for(auto itr : missingStripQRPositionList)
    {
        auto createdLabel = new QLabel(ui->groupMissingList);
        auto element = this->findTableElement(itr);
        createdLabel->setText(QString("%1-%2").arg(itr).arg(element.panelName));
        createdLabel->setObjectName(QString("label%1").arg(itr));

        auto createdEditBox = new QLineEdit(ui->groupMissingList);
        createdEditBox->setObjectName(QString("edit%1").arg(itr));
        createdEditBox->setMaximumSize(100, 20);

        auto createdVBox = new QVBoxLayout();
        createdVBox->setObjectName(QString("layoutV%1").arg(itr));

        createdVBox->addWidget(createdLabel);
        createdVBox->addWidget(createdEditBox);

        d->mLineEdits[itr] =  createdEditBox;
        widgetSet << createdVBox;
    }

    QHBoxLayout* layout = nullptr;

    for(int i = 0 ; i < widgetSet.count(); i++)
    {
        if(i % 5 == 0)
        {
            layout = new QHBoxLayout();
            verticalLayout->addLayout(layout);
        }

        if(layout != nullptr)
            layout->addLayout(widgetSet[i]);
    }

    int rem = widgetSet.count() % 5;
    if(layout != nullptr && rem != 0)
    {
        for(int i = 0 ; i < 5 - rem; i++)
        {
            auto emptyLabel = new QLabel("");
            emptyLabel->setMaximumSize(100, 40);
            auto emptyVBox = new QVBoxLayout();
            emptyVBox->addWidget(emptyLabel);
            layout->addLayout(emptyVBox);
        }
    }
}

void QRStripCheckDlg::setAnalysisData(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT>& element)
{
    d->mElement = element;
}

QMap<int , QString> QRStripCheckDlg::getInputedLotData()
{
    QMap<int, QString> lotDatas;
    QMapIterator<int,QLineEdit*> itr(d->mLineEdits);

    while(itr.hasNext())
    {
        itr.next();
        lotDatas[itr.key()] = itr.value()->text().trimmed().toUpper();
    }

    return lotDatas;
}

bool QRStripCheckDlg::missingDataCheck()
{
    for(auto& itr : d->mLineEdits)
    {
        if(itr->text().trimmed().isEmpty() == true)
        {
            MessageShowDlg* msgDlg = new MessageShowDlg(tr("There is a Test ID QR Code that you did not enter."), this);
            msgDlg->exec();

            return true;
        }
    }

    return false;
}

bool QRStripCheckDlg::validDataCheck()
{
    QString tableName("STRIP_REL_TABLE");
    QString columnNameLotNumber = d->mCodeFuncDataBase->convertEnumToQString((int)DataBaseCtrl::STRIP_REL_TABLE::LOT_NUMBER, tableName);

    QString query = QString("SELECT %1 FROM %2").arg(columnNameLotNumber).arg(tableName);
    auto queryData = d->mCodeFuncDataBase->select(query);

    if(queryData.isEmpty() == true)
    {
        MessageShowDlg* msgDlg = new MessageShowDlg(tr("Please check your QR database data"), this);
        msgDlg->exec();
        return false;
    }

    QVector<int> invalidStripNumberList;
    QMapIterator<int, QLineEdit*> itr(d->mLineEdits);

    while(itr.hasNext())
    {
        itr.next();
        bool isContains = false;

        auto inputedText = itr.value()->text();
        auto inputedTrimmedText = inputedText.trimmed();
        auto inputedUpperText = inputedTrimmedText.toUpper();

        for(auto& rowItr : queryData)
        {
            auto lotNumber = rowItr[columnNameLotNumber].toString();

            if(inputedUpperText == lotNumber)
            {
                isContains = true;
                break;
            }
        }

        if(isContains == false)
            invalidStripNumberList << itr.key();
    }

    if(invalidStripNumberList.isEmpty() == false)
    {
        QString stripText;

        for(auto& itr : invalidStripNumberList)
        {
            if(stripText.isEmpty() == false)
                stripText.push_back(", ");

            stripText.push_back(QString::number(itr));
        }

        MessageShowDlg* msgDlg = new MessageShowDlg(tr("Please check the Test ID QR Code of the next strip number. \n %1").arg(stripText), this);
        msgDlg->exec();

        return false;
    }


    return true;
}

bool QRStripCheckDlg::validPanelCheck()
{
    QMapIterator<int, QLineEdit*> itr(d->mLineEdits);

    while(itr.hasNext())
    {
        itr.next();

        for(auto& elementItr : d->mElement)
        {
            if(elementItr.stripNumber == itr.key())
            {
                if(elementItr.panelName != CQRCode::getPanelName(itr.value()->text()))
                {
                    MessageShowDlg* msgDlg = new MessageShowDlg(tr("The entered QR data does not match the panel data of the patient information"), this);
                    msgDlg->exec();
                    return false;
                }
                break;
            }
        }
    }

    return true;
}

GlobalDataStruct::TABLE_ITEM_ELEMENT QRStripCheckDlg::findTableElement(int stripNumber)
{
    GlobalDataStruct::TABLE_ITEM_ELEMENT element;

    for(auto& itr : d->mElement)
    {
        if(itr.stripNumber == stripNumber)
        {
            element = itr;
            break;
        }
    }

    return element;
}

void QRStripCheckDlg::onUpdateLotData()
{
    QMapIterator<int, QLineEdit*> itr(d->mLineEdits);

    while(itr.hasNext())
    {
        itr.next();

        auto stripNumber = itr.key();
        auto currentLotEdit = itr.value();

        auto currentElement = this->findTableElement(stripNumber);

        if(currentElement.stripNumber == 0)
            continue;

        auto comboText = ui->comboPanelSelect->currentText();
        auto isALL = (comboText == "ALL");
        auto isRightPanel = (comboText == currentElement.panelName);

        if(isALL || isRightPanel)
            currentLotEdit->setText(ui->editBatchLotNumber->text());
    }
}
