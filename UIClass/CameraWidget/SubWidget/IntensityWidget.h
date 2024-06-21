#ifndef INTENSITYWIDGET_H
#define INTENSITYWIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CConfig.h>

class Ui_IntensityWidget;
class IntensityWidget : public QWidget, public CConfig
{
    Q_OBJECT

public:
    explicit IntensityWidget(QWidget *parent = nullptr);
    virtual ~IntensityWidget();

    int getParticle();
    void setParticle(int value);

    int getBand();
    void setBand(int value);

    int getBackGround();
    void setBackGround(int value);

    int getWhite();
    void setWhite(int value);

    virtual void readConfigData();
    virtual void writeConfigData();
private:
    QScopedPointer<Ui_IntensityWidget> ui;
};

#endif // INTENSITYWIDGET_H
