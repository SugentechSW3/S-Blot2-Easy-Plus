#include <QTimer>
#include "RunningStatusListWidget.h"
#include "ui_RunningStatusListWidget.h"

#include "RunningStatusListItem.h"

#include <Common/CLogWriter.h>
#include "Analysis/ConfigInformation.h"

USING_SUGENTECH

struct RunningStatusListWidget::privateStruct
{
    QVector<PROCESSING_ELEMENT> mItemList;
    QMap<int, QString> mColorMap;
    int mCurrentIdx = -1;
};

RunningStatusListWidget::RunningStatusListWidget(QWidget *parent) :
                                                                    QWidget(parent),
                                                                    ui(new Ui_RunningStatusListWidget),
                                                                    d(new privateStruct)
{
    ui->setupUi(this);
    this->initColorMap();
}

RunningStatusListWidget::~RunningStatusListWidget()
{
    this->clearItem();
}

void RunningStatusListWidget::initColorMap()
{
    d->mColorMap[ConfigInformation::STEP_LIST_WETTING] = "#BCD9F5";
    d->mColorMap[ConfigInformation::STEP_LIST_DISPENSE] = "#6ADB2F";
    d->mColorMap[ConfigInformation::STEP_LIST_WASHING1] = "#BCD9F5";
    d->mColorMap[ConfigInformation::STEP_LIST_ANTIBODY] = "#FFD200";
    d->mColorMap[ConfigInformation::STEP_LIST_WASHING2] = "#BDBEBD";
    d->mColorMap[ConfigInformation::STEP_LIST_ENZYME] = "#F14228";
    d->mColorMap[ConfigInformation::STEP_LIST_WASHINGDW] = "#82B4E4";
    d->mColorMap[ConfigInformation::STEP_LIST_SUBSTRATE] = "#8B5442";
    d->mColorMap[ConfigInformation::STEP_LIST_DRY] = "#C4765F";
    d->mColorMap[ConfigInformation::STEP_LIST_ANALYSIS] = "#7356F1";
}

void RunningStatusListWidget::addItem(int stepNumber, const QTime& totalTime, bool isHideImage)
{
    auto listWidgetItem = new RunningStatusListItem(this);
    listWidgetItem->setLabelText(ConfigInformation::convertStepEnumToStepName(stepNumber));

    if(isHideImage == true)
        listWidgetItem->hideRotation();

    ui->verticalLayout->addWidget(listWidgetItem);
    listWidgetItem->setPrograssBarColor(d->mColorMap[stepNumber]);

    PROCESSING_ELEMENT element;
    element.item = listWidgetItem;
    element.totalTime = totalTime;
    element.spendTime = QTime(0,0,0);
    element.stepNumber = stepNumber;

    d->mItemList.push_back(element);
}

void RunningStatusListWidget::removeItem(int idx)
{
    if(d->mItemList.count() <= idx)
        return;

    if(idx < 0)
        return;

    auto target = d->mItemList.takeAt(idx);

    if(target.item != nullptr)
        delete target.item;
}

void RunningStatusListWidget::clearItem()
{
    d->mCurrentIdx = -1;

    while(d->mItemList.count() != 0)
        this->removeItem(0);
}

RunningStatusListItem* RunningStatusListWidget::getItem(int idx)
{
    if(d->mItemList.count() <= idx)
        return nullptr;

    if(idx < 0)
        return nullptr;

    d->mCurrentIdx = idx;

    return d->mItemList[d->mCurrentIdx].item;
}

int RunningStatusListWidget::getCount()
{
    return d->mItemList.count();
}

int RunningStatusListWidget::getCurrentIndex()
{
    return d->mCurrentIdx;
}

bool RunningStatusListWidget::isCurrentActivated()
{
    if(d->mCurrentIdx == -1)
        return false;

    return d->mItemList[d->mCurrentIdx].item->isActivated();
}

bool RunningStatusListWidget::setCurrentIndex(int index)
{
    if(d->mItemList.count() <= index)
        return false;

    d->mCurrentIdx = index;
    return true;
}

void RunningStatusListWidget::startItem(int index)
{
    if(this->setCurrentIndex(index) == false)
        return ;

    if(this->isCurrentActivated() == false)
    {
        this->completionProcessing();
        this->startCurrentItem();
    }
}

void RunningStatusListWidget::startCurrentItem()
{
    if(d->mItemList.count() <= d->mCurrentIdx)
        return;

    d->mItemList[d->mCurrentIdx].item->startRotation();

    this->updateCurrentStatus();
}

void RunningStatusListWidget::startNextItem()
{
    if(d->mItemList.count() <= d->mCurrentIdx)
        return;

    d->mCurrentIdx++;

    if(this->isCurrentActivated() == false)
    {
        this->completionProcessing();        
        this->startCurrentItem();
    }
}

void RunningStatusListWidget::startLastItem()
{
    this->startItem(d->mItemList.count() - 1);
}

void RunningStatusListWidget::stopCurrentItem()
{
    if(d->mItemList.count() <= d->mCurrentIdx)
        return;

    if(d->mCurrentIdx < 0)
        return;

    d->mItemList[d->mCurrentIdx].item->stopRotation();
}

void RunningStatusListWidget::pauseCurrentItem()
{
    if(d->mItemList.count() <= d->mCurrentIdx)
        return;

    if(d->mCurrentIdx < 0)
        return;

    d->mItemList[d->mCurrentIdx].item->pauseRotation();
}

void RunningStatusListWidget::resumeCurrentItem()
{
    if(d->mItemList.count() <= d->mCurrentIdx)
        return;

    if(d->mCurrentIdx < 0)
        return;

    d->mItemList[d->mCurrentIdx].item->startRotation();
}

void RunningStatusListWidget::setCurrentItemStatusText(int percent)
{
    if(d->mItemList.count() <= d->mCurrentIdx)
        return;

    if(d->mCurrentIdx < 0)
        return;

    d->mItemList[d->mCurrentIdx].item->setStatus(percent);
}

QTime RunningStatusListWidget::getSpendTime(int idx) const
{
    if(d->mItemList.count() <= idx)
        return QTime(0,0,0);

    if(idx < 0)
        return QTime(0,0,0);

    return d->mItemList[idx].spendTime;
}

void RunningStatusListWidget::setSpendTime(const QTime &spendTime, int idx)
{
    if(d->mItemList.count() <= idx)
        return;

    if(idx < 0)
        return;

    d->mItemList[idx].spendTime = spendTime;
    this->updateStatus(idx);
}

QTime RunningStatusListWidget::getTotalTime(int idx) const
{
    if(d->mItemList.count() <= idx)
        return QTime(0,0,0);

    if(idx < 0)
        return QTime(0,0,0);

    return d->mItemList[idx].totalTime;
}

void RunningStatusListWidget::setTotalTime(const QTime& totalTime, int idx)
{
    if(d->mItemList.count() <= idx)
        return;

    if(idx < 0)
        return;

    d->mItemList[idx].totalTime = totalTime;
}

void RunningStatusListWidget::addCurrentTotalTime(int min)
{
    if(d->mItemList.count() <= d->mCurrentIdx)
        return ;

    if(d->mCurrentIdx < 0)
        return ;

    d->mItemList[d->mCurrentIdx].totalTime = d->mItemList[d->mCurrentIdx].totalTime.addSecs(min * 60);
}

void RunningStatusListWidget::updateStatus(int idx)
{
    if(d->mItemList.count() <= idx)
        return;

    if(idx < 0)
        return;

    auto listItem = d->mItemList[idx];
    double percent = 0.0;
    double secTotal = this->timeToSec(listItem.totalTime);
    double secSpend = this->timeToSec(listItem.spendTime);

    if( secTotal != 0 && secSpend != 0)
        percent = (secSpend / secTotal) * 100;

    listItem.item->setStatus(percent);
}

QString RunningStatusListWidget::getCurrentLabelName() const
{
    if(d->mItemList.count() <= d->mCurrentIdx)
        return QString();

    if(d->mCurrentIdx < 0)
        return QString();

    auto listItem = d->mItemList[d->mCurrentIdx];

    return listItem.item->getLabelText();
}

QString RunningStatusListWidget::getCurrentSequenceRunningTimeString() const
{
    if(d->mItemList.count() <= d->mCurrentIdx)
        return QString();

    if(d->mCurrentIdx < 0)
        return QString();

    auto listItem = d->mItemList[d->mCurrentIdx];

    auto spendTimeStr = this->timeToHHmmString(listItem.spendTime);
    auto totalTimeStr = this->timeToHHmmString(listItem.totalTime);

    return QString("%1/%2").arg(spendTimeStr).arg(totalTimeStr);
}

QTime RunningStatusListWidget::getCurrentSpendTime()
{
    return this->getSpendTime(d->mCurrentIdx);
}

void RunningStatusListWidget::setCurrentSpendTime(const QTime& time)
{
    this->setSpendTime(time, d->mCurrentIdx);
    this->updateStatus(d->mCurrentIdx);
}

QTime RunningStatusListWidget::getCurrentTotalTime() const
{
    return this->getTotalTime(d->mCurrentIdx);
}

void RunningStatusListWidget::setCurrentTotalTime(const QTime& time)
{
    this->setTotalTime(time, d->mCurrentIdx);
    this->updateStatus(d->mCurrentIdx);
}

double RunningStatusListWidget::getCurrentRunningPercent()
{
    double percent = 0.0;
    double secTotal = this->timeToSec(this->getCurrentTotalTime());
    double secSpend = this->timeToSec(this->getCurrentSpendTime());

    if( secTotal != 0 && secSpend != 0)
        percent = (secSpend / secTotal) * 100.0;

    return percent;
}

void RunningStatusListWidget::updateCurrentStatus()
{
    this->updateStatus(d->mCurrentIdx);
}

QTime RunningStatusListWidget::getTotalNeedTime()
{
    QTime sumTime(0,0,0);

    for(int i =0; i < d->mItemList.count(); i++)
        sumTime = sumTime.addSecs(QTime(0,0,0).secsTo(d->mItemList[i].totalTime));

    return sumTime;
}

QTime RunningStatusListWidget::getTotalUsedSpendTime()
{
    QTime sumTime(0,0,0);

    for(int i =0; i <= d->mCurrentIdx; i++)
        sumTime = sumTime.addSecs(QTime(0,0,0).secsTo(d->mItemList[i].spendTime));

    return sumTime;
}

double RunningStatusListWidget::getTotalRunningPercent()
{
    double percent = 0.0;
    double secTotal = this->timeToSec(this->getTotalNeedTime());
    double secSpend = this->timeToSec(this->getTotalUsedSpendTime());

    if( secTotal != 0 && secSpend != 0)
        percent = (secSpend / secTotal) * 100.0;

    return percent;
}

void RunningStatusListWidget::completionProcessing()
{
    for(int i = 0; i < d->mCurrentIdx; i++)
    {
        if(d->mItemList[i].isStoped == true)
            continue;

        d->mItemList[i].isStoped = true;
        d->mItemList[i].item->startRotation();

        QTimer::singleShot(400*i, this, [=]
                           {
                               this->setSpendTime(d->mItemList[i].totalTime, i);

                               if(i+1 == d->mCurrentIdx)
                                   d->mItemList[i].item->stopRotation();
                               else
                                   d->mItemList[i].item->completionStopRotation();
                           });
    }
}

int RunningStatusListWidget::calcAnalysisTime(int itemCount)
{
    int totalSec = 0;

    for(int i = 0; i< itemCount; i++)
        totalSec += this->ANALYSIS_TIME;

    return totalSec;
}

bool RunningStatusListWidget::isHaveAnalysisSequence()
{
    bool hasAnalysis = false;

    for(auto& itr : d->mItemList)
        if(itr.stepNumber == ConfigInformation::STEP_LIST_ANALYSIS)
            hasAnalysis = true;

    return hasAnalysis;
}

bool RunningStatusListWidget::isAnalysisSequence(int idx)
{
    if(idx < 0)
        return false;

    if(d->mItemList.count() <= idx)
        return false;

    if(d->mItemList[idx].stepNumber == ConfigInformation::STEP_LIST_ANALYSIS)
        return true;

    return false;
}

bool RunningStatusListWidget::isCurrentPause()
{
    if(d->mItemList.count() <= d->mCurrentIdx)
        return false;

    if(d->mCurrentIdx < 0)
        return false;

    return d->mItemList[d->mCurrentIdx].item->isPaused();
}

QString RunningStatusListWidget::timeToHHmmString(const QTime& time)
{
    return time.toString("hh:mm");
}

int RunningStatusListWidget::timeToSec(const QTime& time)
{
    return QTime(0,0,0).secsTo(time);
}

