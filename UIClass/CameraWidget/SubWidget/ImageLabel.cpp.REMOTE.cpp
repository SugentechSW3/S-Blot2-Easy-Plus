#include <QPainter>

#include "ImageLabel.h"

#include "Analysis/ConfigInformation.h"

struct ImageLabel::privateStruct
{
    ConfigInformation* mConfigInformation = nullptr;
    QRect mRoiRect;
    QRect mGuideLine;
    QVector<GlobalDataStruct::rectBandResult> mBoxCarRectList;
    QVector<int> mMembBandCount;
    QPoint mCurrentMousePoint;

    bool mIsUseDualBand = false;
    int mXStart = 0;
    int mDualXStart = 0;
    int mBandGapLineFront = 0;
    int mBandGapLineBack = 0;
    double mRotationAngle = 0.0;
};

ImageLabel::ImageLabel(QWidget *parent) :
    QLabel(parent),
    d(new privateStruct)
{
    this->setMouseTracking(true);
    this->init();
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

ImageLabel::~ImageLabel()
{

}

void ImageLabel::init()
{
    d->mConfigInformation = ConfigInformation::getInstance();

    this->onUpdateUseDualBand(d->mConfigInformation->getIsUseDualBand());
    this->onUpdateDualBandLine(d->mConfigInformation->getDualBandSetting());
    this->onUpdateROIRect(d->mConfigInformation->getHousingROI());
    this->onUpdatePCLine(d->mConfigInformation->getBoxCarAnalysis());
    this->onUpdateGuideLine(d->mConfigInformation->getGuideLine());
    this->onUpdateRotation(d->mConfigInformation->getRotation());

    connect(d->mConfigInformation, &ConfigInformation::onChangedHousingROI, this, &ImageLabel::onUpdateROIRect);
    connect(d->mConfigInformation, &ConfigInformation::onChangedUseBoxCarAnalaysis, this, &ImageLabel::onUpdatePCLine);
    connect(d->mConfigInformation, &ConfigInformation::onChangedUseDualBand, this, &ImageLabel::onUpdateDualBandLine);
    connect(d->mConfigInformation, &ConfigInformation::onChangedRotation, this, &ImageLabel::onUpdateRotation);
    connect(d->mConfigInformation, &ConfigInformation::onChangedGuideLine,  this, &ImageLabel::onUpdateGuideLine);
    connect(d->mConfigInformation, &ConfigInformation::onChangedIsUseDualBand, this, &ImageLabel::onUpdateUseDualBand);
}

void ImageLabel::paintEvent(QPaintEvent * event)
{
    QLabel::paintEvent(event);

    this->drawROIBox();
    this->drawBoxCar();
    this->drawPCLine();
    this->drawDualPcLine();
    this->drawRotationText();
    this->drawGuideLine();
    this->drawMouseMoveText();
    this->drawBandGapLineFront();
    this->drawBandGapLineBack();
}

void ImageLabel::mouseMoveEvent(QMouseEvent* event)
{
    d->mCurrentMousePoint = event->pos();
    this->repaint();
}

void ImageLabel::drawROIBox()
{
    auto rect = d->mRoiRect;

    if(rect.isNull() == true || rect.isValid() == false)
        return;

    //분리된 설정파일 Read가필요함
    QPainter qPainter(this);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setPen(Qt::red);
    qPainter.drawRect(rect.x(), rect.y(), rect.width(), rect.height());
}

void ImageLabel::drawBoxCar()
{
    auto boxcarRectList = d->mBoxCarRectList;

    if(boxcarRectList.isEmpty() == true)
        return;

    QPainter qPainter(this);

    auto roiRect = d->mRoiRect;

    for(int i = 0 ; i< boxcarRectList.count(); i++)
    {
        const auto rect = boxcarRectList[i].rectDynamicBand;

        qPainter.setBrush(Qt::NoBrush);

        auto caclX = rect.x + roiRect.x() + 1;
        auto calcY = rect.y + roiRect.y() + 1;
        auto calcW = rect.width - 1;
        auto calcH = rect.height - 1;

        QRect drawRect;
        drawRect.setX(caclX);
        drawRect.setY(calcY);
        drawRect.setWidth(calcW);
        drawRect.setHeight(calcH);

        if(boxcarRectList[i].isFail == true)
            qPainter.setPen(QColor("purple"));
        else
            qPainter.setPen(Qt::green);

        qPainter.drawRect(drawRect);

        auto drawNumberRect = drawRect;
        drawNumberRect.setY(drawNumberRect.y() - (calcH * NUMBER_TEXT_MARGIN));
        drawNumberRect.setWidth(calcW);
        drawNumberRect.setHeight(calcH);

        QFont font = qPainter.font();
        font.setPointSize(BOXCAR_NUMBER_SIZE);
        qPainter.setFont(font);

        qPainter.drawText(QPoint(drawNumberRect.x(), drawNumberRect.y()), QString::number(i));

        QRect drawOrgRect;
        const auto orgRect = boxcarRectList[i].rectBand;
        drawOrgRect.setX(orgRect.x + roiRect.x());
        drawOrgRect.setY(orgRect.y + roiRect.y() - (calcH+ BOXCAR_ORIGINAL_MARGIN));
        drawOrgRect.setWidth(calcW);
        drawOrgRect.setHeight(calcH);

        qPainter.setPen(Qt::green);
        qPainter.drawRect(drawOrgRect);
    }
}

void ImageLabel::drawPCLine()
{
    auto xStartPoint = d->mXStart;

    QPoint beginLine(xStartPoint, 0);
    QPoint endLine(xStartPoint, 300);

    QPainter qPainter(this);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setPen(QColor("orange"));
    qPainter.drawLine(beginLine, endLine);
}

void ImageLabel::drawDualPcLine()
{
    auto dualXStartPoint = d->mDualXStart;

    if(d->mIsUseDualBand == false)
        return ;

    QPoint beginLine(dualXStartPoint, 0);
    QPoint endLine(dualXStartPoint, 300);

    QPainter qPainter(this);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setPen(QColor("orange"));
    qPainter.drawLine(beginLine, endLine);
}

void ImageLabel::drawRotationText()
{
    auto rotation = d->mRotationAngle;
    QPainter qPainter(this);
    QFont font = qPainter.font() ;

    font.setPointSize(15);
    qPainter.setPen(Qt::yellow);
    qPainter.setFont(font);
    qPainter.drawText(QPoint(100,20), QString("Angle : %1").arg(QString::number(rotation, 'f')));
}

void ImageLabel::drawGuideLine()
{
    auto rect = d->mGuideLine;

    if(rect.isNull() == true || rect.isValid() == false)
        return;

    //분리된 설정파일 Read가필요함
    QPainter qPainter(this);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setPen(Qt::blue);
    qPainter.drawRect(rect.x(), rect.y(), rect.width(), rect.height());
}

void ImageLabel::drawMouseMoveText()
{
    auto pos = d->mCurrentMousePoint;

    if(pos.isNull() == true)
        return ;

    QPainter qPainter(this);
    QFont font = qPainter.font() ;
    font.setPointSize(13);

    qPainter.setBrush(Qt::NoBrush);
    qPainter.setPen(Qt::magenta);
    qPainter.setFont(font);
    qPainter.drawText(pos, QString("X : %1 , Y : %2").arg(QString::number(pos.x())).arg(QString::number(pos.y())));
}

void ImageLabel::drawBandGapLineFront()
{
    auto frontPos = d->mBandGapLineFront;

    if(d->mIsUseDualBand == false)
        return ;

    QPoint beginLine(frontPos, 0);
    QPoint endLine(frontPos, 300);

    QPainter qPainter(this);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setPen(QColor("orange"));
    qPainter.drawLine(beginLine, endLine);
}

void ImageLabel::drawBandGapLineBack()
{
    auto backPos = d->mBandGapLineBack;

    if(d->mIsUseDualBand == false)
        return ;

    QPoint beginLine(backPos, 0);
    QPoint endLine(backPos, 300);

    QPainter qPainter(this);
    qPainter.setBrush(Qt::NoBrush);
    qPainter.setPen(QColor("orange"));
    qPainter.drawLine(beginLine, endLine);
}

void ImageLabel::onUpdateROIRect(GlobalDataStruct::HOUSING_ROI configData)
{
    auto x1 = configData.x1;
    auto x2 = configData.x2;
    auto y1 = configData.y1;
    auto y2 = configData.y2;

    d->mRoiRect = QRect(x1, y1, x2 - x1, y2 - y1);
    this->repaint();
}

void ImageLabel::onUpdatePCLine(GlobalDataStruct::USE_BOXCAR_ANALYSIS configData)
{
    d->mXStart = configData.xStart;
    this->repaint();
}

void ImageLabel::onUpdateDualBandLine(GlobalDataStruct::USE_DUAL_BAND configData)
{
    d->mDualXStart = configData.dualXStart;
    this->repaint();
}

void ImageLabel::onUpdateRotation(double configData)
{
    d->mRotationAngle = configData;
    this->repaint();
}

void ImageLabel::onUpdateGuideLine(GlobalDataStruct::GUIDE_LINE configData)
{
    auto x1 = configData.x1;
    auto x2 = configData.x2;
    auto y1 = configData.y1;
    auto y2 = configData.y2;

    d->mGuideLine = QRect(x1, y1, x2 - x1, y2 - y1);
    this->repaint();
}

void ImageLabel::setBoxCarRect(const QVector<GlobalDataStruct::rectBandResult>& boxCarRectList)
{
    d->mBoxCarRectList = boxCarRectList;
    this->repaint();
}

void ImageLabel::setBandGapLine(int front, int back)
{
    d->mBandGapLineFront = front;
    d->mBandGapLineBack = back;
    this->repaint();
}

void ImageLabel::onUpdateUseDualBand(bool configData)
{
    d->mIsUseDualBand = configData;
}
