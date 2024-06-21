#include "TIgEBandConfig.h"
#include "ui_TIgEBandConfig.h"
#include <QTableWidgetItem>
#include <QRegExpValidator>

#include "Common/CUtil.h"
#include "Common/Io/CSettingIo.h"
#include "Analysis/GlobalDataStruct.h"
#include "Common/Io/INIFileControls/TIeGBandDataTunningData.h"

USING_SUGENTECH

struct TIgEBandConfig::privateStruct
{
    int mRowIndex;
    QVector<GlobalDataStruct::TIGE_BAND_SETTING_DATA> mSettingData;
};

TIgEBandConfig::TIgEBandConfig(QWidget *parent) :
    CDialogParentWidget(parent),
    ui(new Ui_TIgEBandConfig),d(new privateStruct)
{
    ui->setupUi(this);
    this->initSetEditExp();    
    this->initVariable();
    this->initSignalSlots();        
}

TIgEBandConfig::~TIgEBandConfig()
{

}

void TIgEBandConfig::onClickedAdd()
{
    if(this->checkEdtEmpty() == false)
        return;

    int tableRow = ui->tbTIgEBand->rowCount();

    auto moreData = new QTableWidgetItem(ui->edtTotalClassMore->text());
    auto underData = new QTableWidgetItem(ui->edtTotalClassUnder->text());
    auto multiplicationTIgEData = new QTableWidgetItem(ui->edtIgEBandMul->text());

    ui->tbTIgEBand->setRowCount(tableRow+1);

    ui->tbTIgEBand->setItem(tableRow,TIGE_CLASS_MORE,moreData);
    ui->tbTIgEBand->setItem(tableRow,TIGE_CLASS_UNDER,underData);
    ui->tbTIgEBand->setItem(tableRow,TIGE_MULTIPLICATION,multiplicationTIgEData);

    this->tableSetDisEnableEdit(tableRow);

    int nextStartNumber = ui->edtTotalClassUnder->text().toInt()+1;

    ui->edtTotalClassMore->setText(QString::number(nextStartNumber));
    ui->edtIgEBandMul->setText("");
    ui->edtTotalClassUnder->setText("");

}

void TIgEBandConfig::onClickedUpdate()
{
    if(this->checkEdtEmpty() == false)
        return;

    auto moreData = new QTableWidgetItem(ui->edtTotalClassMore->text());
    auto underData = new QTableWidgetItem(ui->edtTotalClassUnder->text());
    auto multiplicationTIgEData = new QTableWidgetItem(ui->edtIgEBandMul->text());

    ui->tbTIgEBand->setItem(d->mRowIndex,TIGE_CLASS_MORE,moreData);
    ui->tbTIgEBand->setItem(d->mRowIndex,TIGE_CLASS_UNDER,underData);
    ui->tbTIgEBand->setItem(d->mRowIndex,TIGE_MULTIPLICATION,multiplicationTIgEData);

    this->tableSetDisEnableEdit(d->mRowIndex);
}

void TIgEBandConfig::onClickedDelete()
{
    ui->tbTIgEBand->removeRow(d->mRowIndex);
}

void TIgEBandConfig::onClickedSave()
{
    QVector<GlobalDataStruct::TIGE_BAND_SETTING_DATA> settingData;
    int rowsCount = ui->tbTIgEBand->rowCount();

    for(int i =0; i< rowsCount; i++)
    {
        auto moreData = ui->tbTIgEBand->item(i,TIGE_CLASS_MORE)->text().toInt();
        auto underData = ui->tbTIgEBand->item(i,TIGE_CLASS_UNDER)->text().toInt();
        auto multiplicationTIgEData = ui->tbTIgEBand->item(i,TIGE_MULTIPLICATION)->text().toDouble();
        GlobalDataStruct::TIGE_BAND_SETTING_DATA data;
        data.BAND_MAX = underData;
        data.BAND_MIN = moreData;
        data.TUNNING_DATA = multiplicationTIgEData;

        settingData.push_back(data);
    }

    TIeGBandDataTunningData io;
    io.writeAllSetting(settingData);
}

void TIgEBandConfig::onCellClicked(int row, int colum)
{
    auto moreData = ui->tbTIgEBand->item(row,TIGE_CLASS_MORE)->text();
    auto underData = ui->tbTIgEBand->item(row,TIGE_CLASS_UNDER)->text();
    auto multiplicationTIgEData = ui->tbTIgEBand->item(row,TIGE_MULTIPLICATION)->text();

    d->mRowIndex = row;

    ui->edtTotalClassMore->setText(moreData);
    ui->edtTotalClassUnder->setText(underData);
    ui->edtIgEBandMul->setText(multiplicationTIgEData);
}

void TIgEBandConfig::initVariable()
{
    ui->btnOK->setStyleSheetImage(":/Image/Image/Dialog/ok_normal.png", ":/Image/Image/Dialog/ok_over.png",
                                  ":/Image/Image/Dialog/ok_focus.png", ":/Image/Image/Dialog/ok_disable.png");

    ui->btnCancel->setStyleSheetImage(":/Image/Image/Dialog/cancel_normal.png", ":/Image/Image/Dialog/cancel_over.png",
                                      ":/Image/Image/Dialog/cancel_focus.png", ":/Image/Image/Dialog/cancel_disable.png");

    ui->tbTIgEBand->horizontalHeaderItem(TIGE_CLASS_MORE)->setText("Sum Class (≤)");
    ui->tbTIgEBand->horizontalHeaderItem(TIGE_CLASS_UNDER)->setText("Sum Class (>)");
    ui->tbTIgEBand->horizontalHeaderItem(TIGE_MULTIPLICATION)->setText("* Total IgE Value");

    int countHeader = ui->tbTIgEBand->model()->columnCount()+1;
    int tableWidth = this->width();

    for(int i = 0; i< countHeader;i++)
        ui->tbTIgEBand->setColumnWidth(i,(int)tableWidth/countHeader);

    TIeGBandDataTunningData io;
    d->mSettingData = io.readAllSettingData();
    this->onLoadDisplay();
}

void TIgEBandConfig::initSignalSlots()
{
    connect(ui->btnAdd,&QPushButton::clicked,this,&TIgEBandConfig::onClickedAdd);
    connect(ui->tbTIgEBand,&QTableWidget::cellClicked,this,&TIgEBandConfig::onCellClicked);
    connect(ui->btnRemove,&QPushButton::clicked,this,&TIgEBandConfig::onClickedDelete);
    connect(ui->btnUpdate,&QPushButton::clicked,this,&TIgEBandConfig::onClickedUpdate);
    connect(ui->btnOK,&CustomImageButtonWidget::clicked,this,&TIgEBandConfig::onClickedSave);
    connect(ui->btnCancel,&CustomImageButtonWidget::clicked,this,&TIgEBandConfig::reject);
}

void TIgEBandConfig::initSetEditExp()
{
    QRegExp sumClass("^[0-9]{1,2}$");
    QRegExpValidator *sumClassValidator = new QRegExpValidator(sumClass,this);

    QRegExp tIgEBand("^[0-1]+(.[0-9]{1,2})?$");
    QRegExpValidator *tIgEBandValidator = new QRegExpValidator(tIgEBand,this);

    ui->edtTotalClassMore->setValidator(sumClassValidator);
    ui->edtTotalClassUnder->setValidator(sumClassValidator);
    ui->edtIgEBandMul->setValidator(tIgEBandValidator);

    ui->edtTotalClassMore->setAcceptDrops(false);
    ui->edtTotalClassUnder->setAcceptDrops(false);
    ui->edtIgEBandMul->setAcceptDrops(false);
}

void TIgEBandConfig::onLoadDisplay()
{
    int rowsCount = d->mSettingData.count();

    if(rowsCount <= 0 )
        return;

    ui->tbTIgEBand->setRowCount(rowsCount);

    for(int i =0; i < rowsCount; i++)
    {
        auto itr = d->mSettingData[i];
        auto moreData = new QTableWidgetItem(QString::number(itr.BAND_MIN));
        auto underData = new QTableWidgetItem(QString::number(itr.BAND_MAX));
        auto multiplicationTIgEData = new QTableWidgetItem(QString::number(itr.TUNNING_DATA));

        ui->tbTIgEBand->setItem(i,TIGE_CLASS_MORE,moreData);
        ui->tbTIgEBand->setItem(i,TIGE_CLASS_UNDER,underData);
        ui->tbTIgEBand->setItem(i,TIGE_MULTIPLICATION,multiplicationTIgEData);

        this->tableSetDisEnableEdit(i);
    }
    int startNumber = d->mSettingData[rowsCount-1].BAND_MAX+1;
    QString startText = QString::number(startNumber);
    ui->edtTotalClassMore->setText(startText);
}

void TIgEBandConfig::tableSetDisEnableEdit(int row)
{
    ui->tbTIgEBand->item(row, TIGE_CLASS_MORE)->setFlags(Qt::ItemIsEditable);
    ui->tbTIgEBand->item(row, TIGE_CLASS_UNDER)->setFlags(Qt::ItemIsEditable);
    ui->tbTIgEBand->item(row, TIGE_MULTIPLICATION)->setFlags(Qt::ItemIsEditable);
}

bool TIgEBandConfig::checkEdtEmpty()
{
    if(ui->edtIgEBandMul->text() == "")
    {
        ui->edtIgEBandMul->setFocus();
        return false;
    }
    if(ui->edtTotalClassMore->text() == "")
    {
        ui->edtTotalClassMore->setFocus();
        return false;
    }
    return true;
}






