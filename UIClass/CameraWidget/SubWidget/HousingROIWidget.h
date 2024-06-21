#ifndef HOUSINGROI_H
#define HOUSINGROI_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CConfig.h>

class Ui_HousingROIWidget;
class HousingROIWidget : public QWidget, public CConfig
{
    Q_OBJECT

public:
    explicit HousingROIWidget(QWidget *parent = nullptr);
    virtual ~HousingROIWidget();

    int getX1();
    void setX1(int x1);

    int getX2();
    void setX2(int x2);

    int getY1();
    void setY1(int y1);

    int getY2();
    void setY2(int y2);

    void setGroupBoxName(const QString &name);
    void setGroupBoxNameColor(Qt::GlobalColor color);

    void readConfigData();
    void writeConfigData();

private:
    QScopedPointer<Ui_HousingROIWidget> ui;
};

#endif // HOUSINGROI_H
