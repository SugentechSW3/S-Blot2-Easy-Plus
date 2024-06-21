#ifndef PCLINEWIDGET_H
#define PCLINEWIDGET_H

#include <QWidget>
#include <QScopedPointer>

class Ui_PCLineWidget;
class PCLineWidget : public QWidget
{
private:
    Q_OBJECT

public:
    explicit PCLineWidget(QWidget *parent = 0);
    virtual ~PCLineWidget();

    int getXStart();
    void setXStart(int value);

    int getXRange();
    void setXRange(int value);

    int getPCThres();
    void setPCThres(int value);

    int getHousingThres();
    void setHousingThres(int value);

private:
    void setGroupBoxNameColor();

private:
    QScopedPointer<Ui_PCLineWidget> ui;
};

#endif // PCLINEWIDGET_H
