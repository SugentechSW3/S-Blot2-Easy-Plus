#include "ReagentUserWidget.h"
#include "ui_ReagentUserWidget.h"

#include "Common/CLogWriter.h"
#include "UIClass/DialogWidget/MessageShowDlg.h"

USING_SUGENTECH

struct ReagentUserWidget::privateStruct
{
    QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> mElement;
    int mStartIdx = 1;
    ConfigInformation* mConfig = nullptr;
};

ReagentUserWidget::ReagentUserWidget(QWidget *parent) :
                                                        CSerialNetworkWidget(parent),
                                                        ui(new Ui_ReagentUserWidget),
                                                        d(new privateStruct)
{
    ui->setupUi(this);
    this->init();
    ui->PlaceHolderWidget->setStyleSheet("border: 1px solid red;");
}

ReagentUserWidget::~ReagentUserWidget()
{

}

void ReagentUserWidget::initStyleSheet()
{
    this->initPlaceHolder();
}

void ReagentUserWidget::initVariables()
{
    d->mConfig = ConfigInformation::getInstance();
    this->initStyleSheet();
}

void ReagentUserWidget::initSignalSlots()
{
}

void ReagentUserWidget::initPlaceHolder()
{
    auto currentEnum = d->mConfig->getContentsEnumFromName(d->mConfig->getCurrentContents());

    switch(currentEnum)
    {
    case (int)ConfigInformation::SUGENTECH_CONTENTS_LIST::ALLERGY:
    {
        for(auto i = 0; i< ReagentPlaceHolderWidget::REAGENT_SELECT_IDX_ALLERGY_END; i++)
        {
            ui->PlaceHolderWidget->setPlaceAllergyHolderStyleSheet(i);
        }
    }break;

    }
}

void ReagentUserWidget::startWidget()
{
    this->initPlaceHolder();
    this->showEssentialNotice();    
}

void ReagentUserWidget::stopWidget()
{
}

void ReagentUserWidget::updateWidget(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT>& element, int startedSequenceIdx)
{
    d->mElement = element;
    d->mStartIdx = startedSequenceIdx;

    ui->StripWidget->updateStrip(this->refineStripWidgetData());
    this->updateAllergyMountIndexs();
}

QVector<ConfigInformation::STRIP_COLOR> ReagentUserWidget::refineStripWidgetData()
{
    auto element = d->mElement;
    auto panelList = d->mConfig->getPanelList();
    auto currentContents = d->mConfig->getContentsEnumFromName(d->mConfig->getCurrentContents());

    QVector<ConfigInformation::STRIP_COLOR> data;

    for(auto& itr : element)
    {
       auto color = this->findColor(currentContents, itr);

       if(color == ConfigInformation::STRIP_COLOR_NONE)
       {
           auto dlg = new MessageShowDlg(tr("Abnormal panel data detection"), this);
           dlg->exec();
           return QVector<ConfigInformation::STRIP_COLOR>();
       }

       data.push_back(color);
    }

    return data;
}

ConfigInformation::STRIP_COLOR ReagentUserWidget::findColor(int contentsIdx, const GlobalDataStruct::TABLE_ITEM_ELEMENT& data)
{
    switch(contentsIdx)
    {

    case (int)ConfigInformation::STANDARD_CONTETNS_LIST::ALLERGY:
    {
        auto panelIdx =  d->mConfig->getCurrentContentPanelIndex(data.panelName);
        return d->mConfig->getAllergyColor(panelIdx);
    }

    default:
        return ConfigInformation::STRIP_COLOR_NONE;
    }
}

void ReagentUserWidget::updateAllergyMountIndexs()
{
    auto startIdx = d->mStartIdx;
    this->updateAllergyPanel();

    if(startIdx <= ALLERGY_SEQUENCE_DISPENSE_SPECIMEN_SAMPLE)
        return;

    this->disableAllergySampleDI();

    if(startIdx <= ALLERGY_SEQUENCE_DISPENSE_ANTIBODY)
        return;

    this->disableAllergyAntiBody();

    if(startIdx <= ALLERGY_SEQUENCE_DISPENSE_ENZYME)
        return;

    this->disableAllergyEnzyme();
    if(startIdx <= ALLERGY_SEQUENCE_DW_WASHING_3)
        return;

    this->disableAllergyWashing();

    if(startIdx <= ALLERGY_SEQUENCE_DISPENSE_SUBSTRATE)
        return;

    this->disableAllergySubstrate();

    if(startIdx <= ALLERGY_SEQUENCE_DISPENSE_STOP_SOLUTION)
        return;

    this->disableAllergyWS_DW();
}

void ReagentUserWidget::updateAllergyPanel()
{
    auto panelType = this->getUseAllergyPanelType();

    switch(panelType)
    {

    case ALLERGY_PANEL_FLAG_ALL :
        break;

    default:
        CLogWriter::printLog("Undefined Panel Type");
        break;
    }
}

ReagentUserWidget::ALLERGY_PANEL_FLAG ReagentUserWidget::getUseAllergyPanelType()
{
    const auto element = d->mElement;

    return ALLERGY_PANEL_FLAG_ALL;
}

void ReagentUserWidget::disableAllergyInhalantHolder()
{
    ui->PlaceHolderWidget->setMountAllergyIndex(ReagentPlaceHolderWidget::REAGENT_SELECT_IDX_ALLERGY_ANTIBODY, false);
    ui->PlaceHolderWidget->setMountAllergyIndex(ReagentPlaceHolderWidget::REAGENT_SELECT_IDX_ALLERGY_ENZYME, false);
}

void ReagentUserWidget::disableAllergySampleDI()
{
    ui->PlaceHolderWidget->setMountAllergyIndex(ReagentPlaceHolderWidget::REAGENT_SELECT_IDX_ALLERGY_SAMPLE_DILUENT, false);
}

void ReagentUserWidget::disableAllergyAntiBody()
{
    ui->PlaceHolderWidget->setMountAllergyIndex(ReagentPlaceHolderWidget::REAGENT_SELECT_IDX_ALLERGY_ANTIBODY, false);
}

void ReagentUserWidget::disableAllergyEnzyme()
{
    ui->PlaceHolderWidget->setMountAllergyIndex(ReagentPlaceHolderWidget::REAGENT_SELECT_IDX_ALLERGY_ENZYME, false);
}

void ReagentUserWidget::disableAllergyWashing()
{
    ui->PlaceHolderWidget->setMountAllergyIndex(ReagentPlaceHolderWidget::REAGENT_SELECT_IDX_ALLERGY_WS, false);
}

void ReagentUserWidget::disableAllergySubstrate()
{
    ui->PlaceHolderWidget->setMountAllergyIndex(ReagentPlaceHolderWidget::REAGENT_SELECT_IDX_ALLERGY_SUBSTRATE, false);
}

void ReagentUserWidget::disableAllergyWS_DW()
{
    ui->PlaceHolderWidget->setMountAllergyIndex(ReagentPlaceHolderWidget::REAGENT_SELECT_IDX_ALLERGY_DW, false);
}

void ReagentUserWidget::showEssentialNotice()
{
    QString message;

    message.append(tr("Please check the items below.\n"));
    message.append(tr("1. Check the Strips and the Reagents at right position.\n"));
    message.append(tr("2. Empty the waste bottle. & Connect the 'Waste Tube'\n"));
    message.append(tr("3. Fill each bottle with washing solution and distilled water respectively.\n"));    
    message.append(tr("4. If using a new LOT reagent, mount the 'Test ID QR Code' on the right side of the tray.\n"));
    message.append(tr("5. Close the front door.\n"));

    auto dlg = new MessageShowDlg(message, this);    
    dlg->setDefaultTextAlignment();
    dlg->setFontSize(20);
    dlg->setButtonHeight(50);
    dlg->open();
}
