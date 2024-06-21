#ifndef SPINSLIDERWIDGET_H
#define SPINSLIDERWIDGET_H

#include <QWidget>
#include <QScopedPointer>

class Ui_SpinSliderWidget;
class SpinSliderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpinSliderWidget(QWidget *parent = nullptr);
    virtual ~SpinSliderWidget();

    void setMinimumValue(int value);
    void setMaximumValue(int value);
    void setStep(int value);
    void setValue(int value);
    int getValue();

signals:
    void valueChanged(int value);

private:
    void initSignalSlots();


private:
    QScopedPointer<Ui_SpinSliderWidget> ui;
};

#endif // SPINSLIDERWIDGET_H
