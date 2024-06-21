#ifndef CUSTOMPROGRASSIMAGECIRCLEWIDGET_H
#define CUSTOMPROGRASSIMAGECIRCLEWIDGET_H

#include <QWidget>

class Ui_CustomPrograssImageCircleWidget;
class CustomPrograssImageCircleWidget : public QWidget
{
private:
    Q_OBJECT

public:
    explicit CustomPrograssImageCircleWidget(QWidget *parent = nullptr);
    virtual ~CustomPrograssImageCircleWidget();

    int getValue() const;

public slots:
    void setValue(int value);
    void setETATime(const QString& timeString);

private:
    QScopedPointer<Ui_CustomPrograssImageCircleWidget> ui;
};

#endif // CUSTOMPROGRASSIMAGECIRCLEWIDGET_H
