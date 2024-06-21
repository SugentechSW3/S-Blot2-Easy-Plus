#ifndef RUNNINGSTATUSLISTITEM_H
#define RUNNINGSTATUSLISTITEM_H

#include <QWidget>

class Ui_RunningStatusListItem;
class RunningStatusListItem : public QWidget
{
private:
    Q_OBJECT

    struct privateStruct;

public:
    explicit RunningStatusListItem(QWidget *parent = nullptr);
    ~RunningStatusListItem();

    void setLabelText(const QString& text);
    QString getLabelText() const;

    void setMaximumValue(int maximum);
    int getMaximumValue() const;

    void setValue(int value);
    int getValue() const;

    void setPrograssFormat(const QString& text);
    QString getPrograssFormat() const;

    void setRotationValue(int value);
    int getRotationValue() const;

    void hideRotation();
    void showRotation();

    void startRotation();
    void stopRotation();
    void pauseRotation();
    void completionStopRotation();

    void setStatus(int percent);
    void setPrograssBarColor(const QColor &color);

    bool isActivated();
    bool isPaused();

private:
    QScopedPointer<Ui_RunningStatusListItem> ui;
    QScopedPointer<privateStruct> d;
};

#endif // RUNNINGSTATUSLISTITEM_H
