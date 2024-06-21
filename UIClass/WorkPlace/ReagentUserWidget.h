#ifndef INDICATORSETTTINGWIDGET_H
#define INDICATORSETTTINGWIDGET_H

#include <Common/CommonParentWidget/CSerialNetworkWidget.h>
#include "Analysis/ConfigInformation.h"

#include "UIClass/WorkPlace/SubWidget/ReagentStripWidget.h"

class Ui_ReagentUserWidget;
class ReagentUserWidget : public CSerialNetworkWidget
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit ReagentUserWidget(QWidget *parent = nullptr);
    virtual ~ReagentUserWidget();

    enum ALLERGY_PANEL_FLAG
    {
        ALLERGY_PANEL_FLAG_INHALANT,
        ALLERGY_PANEL_FLAG_FOOD,
        ALLERGY_PANEL_FLAG_ALL
    };

    enum ALLERGY_SEQUENCE
    {
        ALLERGY_SEQUENCE_NONE = 0x00,
        ALLERGY_SEQUENCE_DISPENSE_SPECIMEN_INCUBATE,
        ALLERGY_SEQUENCE_DISPENSE_SPECIMEN_SAMPLE,
        ALLERGY_SEQUENCE_WASHING_1,
        ALLERGY_SEQUENCE_DISPENSE_ANTIBODY,
        ALLERGY_SEQUENCE_WASHING_2,
        ALLERGY_SEQUENCE_DISPENSE_ENZYME,
        ALLERGY_SEQUENCE_DW_WASHING_3,
        ALLERGY_SEQUENCE_DISPENSE_SUBSTRATE,
        ALLERGY_SEQUENCE_DISPENSE_STOP_SOLUTION,
        ALLERGY_SEQUENCE_DRY,
        ALLERGY_SEQUENCE_ANALYST
    };

    void updateWidget(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT> &element, int startedSequenceIdx);

private:
    void initStyleSheet();
    void initPlaceHolder();

    void initVariables();
    void initSignalSlots();
    void startWidget();
    void stopWidget();

    void updateAllergyMountIndexs();
    void updateAllergyPanel();
    ReagentUserWidget::ALLERGY_PANEL_FLAG getUseAllergyPanelType();

    QVector<ConfigInformation::STRIP_COLOR> refineStripWidgetData();
    ConfigInformation::STRIP_COLOR findColor(int contentsIdx, const GlobalDataStruct::TABLE_ITEM_ELEMENT &data);


    void disableAllergyInhalantHolder();
    void disableAllergyFoodHolder();
    void disableAllergySampleDI();
    void disableAllergyAntiBody();
    void disableAllergyEnzyme();
    void disableAllergySubstrate();
    void disableAllergyWS_DW();
    void disableAllergyWashing();

    void showEssentialNotice();
private:
    QScopedPointer<Ui_ReagentUserWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // INDICATORSETTTINGWIDGET_H
