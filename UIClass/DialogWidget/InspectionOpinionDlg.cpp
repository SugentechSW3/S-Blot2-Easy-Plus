#include <QFile>

#include "InspectionOpinionDlg.h"
#include "ui_InspectionOpinionDlg.h"

struct InspectionOpinionDlg::privateStruct
{
    QVector<GlobalDataStruct::OPINION_DATA> mData;
};

InspectionOpinionDlg::InspectionOpinionDlg(QWidget *parent) :
                                                              CDialogParentWidget(parent),
                                                              ui(new Ui_InspectionOpinionDlg),
                                                              d(new privateStruct)
{
    ui->setupUi(this);

    this->initTableWidget();
    this->initSignalSlot();
    this->initStyleSheet();

    this->onLoadData();
}

InspectionOpinionDlg::~InspectionOpinionDlg()
{
}

void InspectionOpinionDlg::initTableWidget()
{
    for(auto row = 0 ; row < ui->tableWidget->rowCount(); row++)
    {
        for(auto column = 0 ; column < ui->tableWidget->columnCount(); column++)
        {
            auto item = new QTableWidgetItem;
            ui->tableWidget->setItem(row, column, item);
        }
    }
}

void InspectionOpinionDlg::initSignalSlot()
{
    connect(ui->btnOK, &QPushButton::clicked, this, &InspectionOpinionDlg::onClickOK);
    connect(ui->btnRemove, &QPushButton::clicked, this, &InspectionOpinionDlg::onClickRemove);
    connect(ui->btnCANCEL, &QPushButton::clicked, this, &InspectionOpinionDlg::onClickCancel);
}

void InspectionOpinionDlg::initStyleSheet()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCANCEL->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");

    ui->btnRemove->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");
}

QVector<GlobalDataStruct::OPINION_DATA> InspectionOpinionDlg::getOpinionData()
{
    QFile loadFile("opinionData.dat");
    QVector<GlobalDataStruct::OPINION_DATA> data;

    if(loadFile.open(QIODevice::ReadOnly) == false)
        return data;

    QDataStream stream(&loadFile);
    stream >> data;
    loadFile.close();

    return data;
}

QString InspectionOpinionDlg::findOpinionText(const QString& title)
{
    auto opinionData = InspectionOpinionDlg::getOpinionData();

    for(auto& itr :opinionData)
    {
        if(itr.title == title)
            return itr.opinion;
    }

    return QString();
}

void InspectionOpinionDlg::setOpinionData()
{
    QFile saveFile("opinionData.dat");

    if(saveFile.open(QIODevice::WriteOnly) == false)
        return ;

    QDataStream stream(&saveFile);
    stream << d->mData;
    saveFile.close();
}

void InspectionOpinionDlg::dataToTable()
{
    if(d->mData.isEmpty())
        return;

    for(auto row = 0; row < ui->tableWidget->rowCount(); row++)
    {
        auto titleItem = ui->tableWidget->item(row, OPINION_TABLE_COLUMN_TITLE);
        auto opinionItem = ui->tableWidget->item(row, OPINION_TABLE_COLUMN_OPINION);

        if(titleItem == nullptr)
            continue;

        if(opinionItem == nullptr)
            continue;

        if(d->mData.count() <= row)
            return;

        titleItem->setText(d->mData[row].title);
        opinionItem->setText(d->mData[row].opinion);
    }
}

void InspectionOpinionDlg::dataFromTable()
{
    QVector<GlobalDataStruct::OPINION_DATA> data;

    for(auto row = 0; row < ui->tableWidget->rowCount(); row++)
    {
        auto titleItem = ui->tableWidget->item(row, OPINION_TABLE_COLUMN_TITLE);
        auto opinionItem = ui->tableWidget->item(row, OPINION_TABLE_COLUMN_OPINION);

        if(titleItem == nullptr)
            continue;

        if(opinionItem == nullptr)
            continue;

        GlobalDataStruct::OPINION_DATA currentRowData;

        currentRowData.title = titleItem->text();
        currentRowData.opinion = opinionItem->text();

        data.push_back(currentRowData);
    }

    d->mData = data;
}

void InspectionOpinionDlg::removeSelectedRowData()
{
    auto selectedRow = ui->tableWidget->currentRow();

    if(selectedRow < 0)
        return;

    GlobalDataStruct::OPINION_DATA data;

    if(selectedRow < d->mData.count())
        d->mData[selectedRow] = data;

    auto titleItem = ui->tableWidget->item(selectedRow, OPINION_TABLE_COLUMN_TITLE);
    auto opinionItem = ui->tableWidget->item(selectedRow, OPINION_TABLE_COLUMN_OPINION);

    titleItem->setText("");
    opinionItem->setText("");
}

void InspectionOpinionDlg::onSaveData()
{
    this->dataFromTable();
    this->setOpinionData();
}

void InspectionOpinionDlg::onLoadData()
{
    d->mData = this->getOpinionData();
    this->dataToTable();
}

void InspectionOpinionDlg::onClickOK()
{
    this->onSaveData();
    this->accept();
}

void InspectionOpinionDlg::onClickRemove()
{
    this->removeSelectedRowData();
    this->onSaveData();
}

void InspectionOpinionDlg::onClickCancel()
{
    this->reject();
}
