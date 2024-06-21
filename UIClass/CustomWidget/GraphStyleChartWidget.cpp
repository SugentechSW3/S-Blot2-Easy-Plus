#include <QChart>
#include <QChartView>
#include <QValueAxis>
#include <QChartGlobal>
#include <QSplineSeries>
#include <QDebug>
#include <QMutex>
#include <opencv2/imgproc.hpp>

#include "GraphStyleChartWidget.h"
#include "ui_GraphStyleChartWidget.h"

using namespace QtCharts;

struct GraphStyleChartWidget::privateStruct
{
    QScopedPointer<QChart> mChart;
    QChartView* mChartView = nullptr;
    QMutex mMutex;

};

GraphStyleChartWidget::GraphStyleChartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui_GraphStyleChartWidget),
    d(new privateStruct)
{
    ui->setupUi(this);

    this->initVariables();
    this->initSignalSlots();
}

GraphStyleChartWidget::~GraphStyleChartWidget()
{
}

void GraphStyleChartWidget::initVariables()
{
    d->mChart.reset(new QChart());
    d->mChart->legend()->hide();

    d->mChartView = new QChartView(d->mChart.data(), ui->scrollAreaWidgetContents); //take ownership by this
    d->mChartView->setRenderHint(QPainter::Antialiasing);

    ui->gridLayoutScroll->addWidget(d->mChartView);
    this->setYTick(10);

    d->mChartView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void GraphStyleChartWidget::initSignalSlots()
{
}

void GraphStyleChartWidget::clear()
{
    d->mMutex.lock();
    d->mChart->removeAllSeries();
    d->mMutex.unlock();
}

void GraphStyleChartWidget::drawLine(cv::Mat rawData, int yPoint, int beginWidgetPos)
{
     d->mMutex.lock();

    cv::Mat grayImg;
    const int additionalGeometryWidth = 105;
    const int revertGeometryWidth = 60;

    if(rawData.data != nullptr)
        cv::cvtColor(rawData, grayImg, cv::COLOR_BGR2GRAY);

    d->mChart->removeAllSeries();
    ui->scrollAreaWidgetContents->resize(grayImg.size().width + (additionalGeometryWidth*2) ,ui->scrollAreaWidgetContents->height());

    QSplineSeries* series = new QSplineSeries();

    //마우스가 위치한 Y축을 기준으로 라인값을 취함
#pragma omp parallel for ordered //병렬처리(순서처리)
    for(int x = 0; x< grayImg.size().width; x++)
    {
#pragma omp ordered
        {
            const uchar value = grayImg.at<uchar>(cv::Point(x, yPoint)); // y축 고정

            //차트에는 발색정도(데이터)가 Y축으로 표시됨.
            const QPoint point(x, value);
            *series << point;
        }
    }

    if(beginWidgetPos != 0)
        d->mChart->setGeometry(beginWidgetPos - revertGeometryWidth, 0, grayImg.size().width + additionalGeometryWidth, 310); //ROI와 Widget 사이즈 맞춰주기

    d->mChart->addSeries(series);
    d->mChart->createDefaultAxes();
    this->setYRange(0, 255);

    this->setYTick(10);
    this->setXTick(10);


    d->mMutex.unlock();
}

void GraphStyleChartWidget::drawLine(const QVector<QPoint>& lines)
{
    d->mMutex.lock();

    auto series  = new QSplineSeries();
#pragma omp parallel for ordered
    for(int i =0; i< lines.count(); i++)
#pragma omp ordered
        series->append(lines[i]);

    d->mChart->removeAllSeries();
    d->mChart->addSeries(series);
    d->mChart->createDefaultAxes(); //그래프를 먼저 그리고 그래프에 맞춰 축을 생성함
    this->setYTick(10);
    this->setXTick(10);

    d->mMutex.unlock();
}

void GraphStyleChartWidget::drawLine(const QVector<QPointF>& lines)
{
    d->mMutex.lock();

    auto series  = new QSplineSeries();

#pragma omp parallel for ordered
    for(int i =0; i< lines.count(); i++)
#pragma omp ordered
        series->append(lines[i]);

    d->mChart->removeAllSeries();
    d->mChart->addSeries(series);
    d->mChart->createDefaultAxes(); //그래프를 먼저 그리고 그래프에 맞춰 축을 생성함

    this->setYTick(10);
    this->setXTick(10);

    d->mMutex.unlock();
}

void GraphStyleChartWidget::setXTick(int tick)
{
    auto xPtr = qobject_cast<QValueAxis*> (d->mChart->axisX());

    if(xPtr == nullptr)
        return;

    xPtr->setTickCount(tick);
}

void GraphStyleChartWidget::setYTick(int tick)
{
    auto yPtr = qobject_cast<QValueAxis*> (d->mChart->axisY());

    if(yPtr == nullptr)
        return;

    yPtr->setTickCount(tick);
}

void GraphStyleChartWidget::setXRange(const QVariant& min, const QVariant& max)
{
    auto xPtr = d->mChart->axisX();

    if(xPtr == nullptr)
        return;

    xPtr->setRange(min, max);
}

void GraphStyleChartWidget::setYRange(const QVariant& min, const QVariant& max)
{
    auto yPtr = d->mChart->axisY();

    if(yPtr == nullptr)
        return;

    yPtr->setRange(min, max);
}

void GraphStyleChartWidget::setTitle(const QString& title)
{
    d->mChart->setTitle(title);
}
