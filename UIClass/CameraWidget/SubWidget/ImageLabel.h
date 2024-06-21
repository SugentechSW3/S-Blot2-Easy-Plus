#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <opencv2/core.hpp>
#include <QLabel>
#include "Analysis/GlobalDataStruct.h"

class ImageLabel : public QLabel
{
private:
    Q_OBJECT

    struct privateStruct;

    const int BOXCAR_NUMBER_SIZE  = 10;
    const int NUMBER_TEXT_MARGIN = 2;
    const int BOXCAR_ORIGINAL_MARGIN = 4;

public:
    explicit ImageLabel(QWidget* parent= nullptr);
    virtual ~ImageLabel();

    void setBoxCarRect(const QVector<GlobalDataStruct::rectBandResult> &boxCarRectList);
    void setBandGapLine(int front, int back);
private:
    void init();
    virtual void paintEvent(QPaintEvent * event);
    virtual void mouseMoveEvent(QMouseEvent* event);

private slots:
    void onUpdateROIRect(GlobalDataStruct::HOUSING_ROI configData);
    void onUpdatePCLine(GlobalDataStruct::USE_BOXCAR_ANALYSIS configData);
    void onUpdateMultiBandLine(GlobalDataStruct::USE_MULTIPLE_BAND configData);
    void onUpdateRotation(double configData);
    void onUpdateGuideLine(GlobalDataStruct::GUIDE_LINE configData);
    void onUpdateUseMultiBand(bool configData);

    void drawROIBox();
    void drawBoxCar();
    void drawPCLine();
    void drawMultiPcLine();
    void drawRotationText();
    void drawGuideLine();
    void drawMouseMoveText();
    void drawBandGapLineFront();
    void drawBandGapLineBack();

private:
    QScopedPointer<privateStruct> d;
};

#endif // IMAGELABEL_H
