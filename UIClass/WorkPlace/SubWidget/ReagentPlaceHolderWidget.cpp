#include "ReagentPlaceHolderWidget.h"
#include "ui_ReagentPlaceHolderWidget.h"
#include <QPainter>
#include <QRegion>
#include <QPalette>

struct ReagentPlaceHolderWidget::privateStruct
{    
    QMap<int,ReagentPlaceHolderWidget::privatReagenteHoldObject> mReagenteHoldMap;
};

ReagentPlaceHolderWidget::ReagentPlaceHolderWidget(QWidget *parent) :
    QWidget(parent),d(new privateStruct),
    ui(new Ui_ReagentPlaceHolderWidget)
{
    ui->setupUi(this);
    this->initValiables();
}

ReagentPlaceHolderWidget::~ReagentPlaceHolderWidget()
{

}

void ReagentPlaceHolderWidget::setPlaceAllergyHolderStyleSheet(int select)
{    
    QPoint movePoint;
    switch(select)
    {
    case REAGENT_SELECT_IDX_ALLERGY_WS:    
        movePoint = imageMovePoint(32,32);
        d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_WS].mRegentImagePoint = movePoint;
        break;
    case REAGENT_SELECT_IDX_ALLERGY_SAMPLE_DILUENT:
        movePoint = imageMovePoint(271,133);
        d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_SAMPLE_DILUENT].mRegentImagePoint = movePoint;
        break;
    case REAGENT_SELECT_IDX_ALLERGY_ANTIBODY:
        movePoint = imageMovePoint(353,19);
        d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_ANTIBODY].mRegentImagePoint = movePoint;
        break;
    case REAGENT_SELECT_IDX_ALLERGY_ENZYME:
        movePoint = imageMovePoint(436,133);
        d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_ENZYME].mRegentImagePoint = movePoint;
        break;
    case REAGENT_SELECT_IDX_ALLERGY_SUBSTRATE:
        movePoint = imageMovePoint(519,19);
        d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_SUBSTRATE].mRegentImagePoint = movePoint;
        break;
    case REAGENT_SELECT_IDX_ALLERGY_DW:
        movePoint = imageMovePoint(654,32);
        d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_DW].mRegentImagePoint = movePoint;
        break;
    default:
        break;
    }
    this->setMountAllergyIndex(select,true);
}

void ReagentPlaceHolderWidget::setMountAllergyIndex(int select, bool isMount)
{
    d->mReagenteHoldMap[select].mIsSelected = isMount;
}

void ReagentPlaceHolderWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/Image/Image/Reagent/reagent_tray3.png");
    painter.drawPixmap(0,0,pixmap);

    for(int index = REAGENT_SELECT_IDX_ALLERGY_WS; index < REAGENT_SELECT_IDX_ALLERGY_END; index++)
    {
        auto currentReagente = d->mReagenteHoldMap[index];
        if(currentReagente.mIsSelected == true)
        {
            auto moveX = currentReagente.mRegentImagePoint.x();
            auto moveY = currentReagente.mRegentImagePoint.y();
            painter.drawPixmap(moveX,moveY,currentReagente.mRegentCheckImage);
        }
    }
}

void ReagentPlaceHolderWidget::initValiables()
{
    QPixmap bigSizeReagentimage(":/Image/Image/Reagent/reagent_L.png");

    d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_WS].mRegentCheckImage = bigSizeReagentimage;
    d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_DW].mRegentCheckImage = bigSizeReagentimage;

    QPixmap smalSizeReagentimage(":/Image/Image/Reagent/reagent_S.png");

    d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_SAMPLE_DILUENT].mRegentCheckImage = smalSizeReagentimage;
    d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_ANTIBODY].mRegentCheckImage = smalSizeReagentimage;
    d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_ENZYME].mRegentCheckImage = smalSizeReagentimage;
    d->mReagenteHoldMap[REAGENT_SELECT_IDX_ALLERGY_SUBSTRATE].mRegentCheckImage = smalSizeReagentimage;    
}

QPoint ReagentPlaceHolderWidget::imageMovePoint(int x, int y)
{
    return QPoint(x,y);
}
