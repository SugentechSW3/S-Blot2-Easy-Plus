#include "ListToHiddenBandDlg.h"
#include "ui_ListToHiddenBandDlg.h"

#include "Analysis/ConfigInformation.h"
#include "Common/Io/INIFileControls/AllergyIniDataControls.h"

struct ListToHiddenBandDlg::privateStruct
{
};

ListToHiddenBandDlg::ListToHiddenBandDlg(QWidget *parent) :
                                                            CDialogParentWidget(parent),
                                                            ui(new Ui_ListToHiddenBandDlg),
                                                            d(new privateStruct)
{
    ui->setupUi(this);
    this->initData();
    this->initSignalSlots();

    auto config = ConfigInformation::getInstance();
    this->loadData(config->getCurrentContents());
}

ListToHiddenBandDlg::~ListToHiddenBandDlg()
{
}

void ListToHiddenBandDlg::initData()
{
    auto config = ConfigInformation::getInstance();
    auto contentsList = config->getContentsList();

    for(int i = 0; i < contentsList.count(); i++)
        ui->comboContents->addItem(contentsList[i]);
}

void ListToHiddenBandDlg::initSignalSlots()
{
    connect(ui->comboContents, &QComboBox::currentTextChanged, this, &ListToHiddenBandDlg::loadData);
    connect(ui->btnOK, &QPushButton::clicked, this, &ListToHiddenBandDlg::saveData);
    connect(ui->btnCancel, &QPushButton::clicked, this, &ListToHiddenBandDlg::reject);
    connect(ui->btnAdd, &QPushButton::clicked, this, &ListToHiddenBandDlg::onClickedAdd);
    connect(ui->btnRemove, &QPushButton::clicked, this, &ListToHiddenBandDlg::onClickedRemove);
}

void ListToHiddenBandDlg::loadData(const QString& contentsName)
{        
    auto idx = ConfigInformation::getContentsEnumFromName(contentsName);
    ui->listBandList->clear();
    ui->listHiddenList->clear();

    switch(idx)
    {
    case (int)ConfigInformation::STANDARD_CONTETNS_LIST::ALLERGY:
        this->allergyDataLoad();
        break;
    }
}

void ListToHiddenBandDlg::saveData()
{
    AllergyIniDataControls iniData;
    auto contentsName = ui->comboContents->currentText();
    QStringList codeList;

    for(auto i = 0; i < ui->listHiddenList->count(); i++)
    {
        auto item = ui->listHiddenList->item(i);
        auto hiddenText = item->text();
        auto code = hiddenText.split("/").first();

        codeList << code;
    }

    iniData.writeHiddenBandCodeData(contentsName, codeList);

    this->accept();
}

void ListToHiddenBandDlg::allergyDataLoad()
{
    auto config = ConfigInformation::getInstance();
    auto contentsName = config->getCurrentContents();

    auto codeList = config->getContentPanelCodes(contentsName);

    QMapIterator<QString,QString> itr(codeList);

    while(itr.hasNext())
    {
        itr.next();

        auto lineText = QString("%1/%2").arg(itr.key()).arg(itr.value());
        ui->listBandList->addItem(lineText);
    }


    AllergyIniDataControls iniData;
    auto hiddenList = iniData.readHiddenBandCodeData(contentsName);

    for(auto& itr : hiddenList)
    {
        auto name = codeList[itr];
        auto lineText = QString("%1/%2").arg(itr).arg(name);
        ui->listHiddenList->addItem(lineText);
    }
}

void ListToHiddenBandDlg::onClickedAdd()
{
  auto item = ui->listBandList->selectedItems();

  for(auto&itr : item)
  {
     auto text =  itr->text();
     auto isContains = false;

     for(auto i = 0 ; i < ui->listHiddenList->count(); i++)
     {
         auto currentHiddenItem = ui->listHiddenList->item(i);

         if(currentHiddenItem->text() == text)
         {
             isContains = true;
             break;
         }
     }

     if(isContains == false)
         ui->listHiddenList->addItem(text);
  }
}

void ListToHiddenBandDlg::onClickedRemove()
{
    auto items = ui->listHiddenList->selectedItems();
    qDeleteAll(items);
}
