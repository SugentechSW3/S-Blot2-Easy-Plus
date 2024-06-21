#ifndef SIGERANGESETTINGWIDGET_H
#define SIGERANGESETTINGWIDGET_H

#include <QWidget>
#include <QScopedPointer>

#include "Analysis/GlobalDataStruct.h"

class Ui_sIgERangeSettingWidget;
class sIgERangeSettingWidget : public QWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit sIgERangeSettingWidget(QWidget *parent = nullptr);
    ~sIgERangeSettingWidget();

    void savesIgEData();
    void loadsIgEData();

private:
    void readData1();
    void readData2();
    void readData3();
    void readData4();
    void readData5();
    void readData6();

    void writeData1();
    void writeData2();
    void writeData3();
    void writeData4();
    void writeData5();
    void writeData6();

    void readSelectedRadioClass();
    void writeSelectedRadioClass();

private:
    QScopedPointer<Ui_sIgERangeSettingWidget> ui;
};

#endif // SIGERANGESETTINGWIDGET_H
