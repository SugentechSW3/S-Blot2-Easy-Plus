#ifndef RUNNINGSTATUSLISTWIDGET_H
#define RUNNINGSTATUSLISTWIDGET_H

#include <QWidget>
#include <QTime>


class Ui_RunningStatusListWidget;
class RunningStatusListItem;
class RunningStatusListWidget : public QWidget
{
private:
    Q_OBJECT
    struct privateStruct;

    struct PROCESSING_ELEMENT
    {
        RunningStatusListItem* item = nullptr;
        QTime totalTime;
        QTime spendTime;
        int stepNumber = 0;
        bool isStoped = false;
    };

public :
    const int ANALYSIS_TIME = 15;

public:
    explicit RunningStatusListWidget(QWidget *parent = nullptr);
    ~RunningStatusListWidget();

    void addItem(int stepNumber, const QTime& totalTime, bool isHideImage = false);
    void clearItem();

    RunningStatusListItem* getItem(int idx);
    int getCount();

    int getCurrentIndex();

    bool isCurrentActivated();


    void startItem(int index);
    void startCurrentItem();
    void startNextItem();
    void startLastItem();
    void stopCurrentItem();
    void pauseCurrentItem();
    void resumeCurrentItem();

    void setCurrentItemStatusText(int percent);

    QTime getSpendTime(int idx) const;
    void setSpendTime(const QTime& spendTime, int idx);
    QTime getTotalTime(int idx) const;
    void setTotalTime(const QTime& totalTime, int idx);
    void addCurrentTotalTime(int min);
    void updateStatus(int idx);

    QString getCurrentLabelName() const;
    QString getCurrentSequenceRunningTimeString() const;
    QTime getCurrentSpendTime();
    void setCurrentSpendTime(const QTime& time);
    QTime getCurrentTotalTime() const;
    void setCurrentTotalTime(const QTime& time);
    double getCurrentRunningPercent();
    void updateCurrentStatus();

    QTime getTotalNeedTime();
    QTime getTotalUsedSpendTime();
    double getTotalRunningPercent();

    void completionProcessing();
    int calcAnalysisTime(int itemCount);

    bool isHaveAnalysisSequence();
    bool isAnalysisSequence(int idx);
    bool isCurrentPause();

    static QString timeToHHmmString(const QTime& time);
    static int timeToSec(const QTime& time);

private:
    bool setCurrentIndex(int index);

private:
    void initColorMap();
    void removeItem(int idx);

private:
    QScopedPointer<Ui_RunningStatusListWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // RUNNINGSTATUSLISTWIDGET_H
