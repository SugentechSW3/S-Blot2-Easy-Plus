#include "sIgERangeSettingWidget.h"
#include "ui_sIgERangeSettingWidget.h"

#include "Common/Io/INIFileControls/GenericIniDataControls.h"

sIgERangeSettingWidget::sIgERangeSettingWidget(QWidget *parent) :
                                                                  QWidget(parent),
                                                                  ui(new Ui_sIgERangeSettingWidget)
{
    ui->setupUi(this);
}

sIgERangeSettingWidget::~sIgERangeSettingWidget()
{
}

void sIgERangeSettingWidget::savesIgEData()
{
    this->writeData1();
    this->writeData2();
    this->writeData3();
    this->writeData4();
    this->writeData5();
    this->writeData6();
    this->writeSelectedRadioClass();
}

void sIgERangeSettingWidget::loadsIgEData()
{
    this->readData1();
    this->readData2();
    this->readData3();
    this->readData4();
    this->readData5();
    this->readData6();
    this->readSelectedRadioClass();
}

void sIgERangeSettingWidget::readData1()
{
    GenericIniDataControls io;
    auto data = io.readsIgEData1();
    ui->spinsIgEConditionValue1->setValue(data);
}

void sIgERangeSettingWidget::readData2()
{
    GenericIniDataControls io;
    auto data = io.readsIgEData2();
    ui->spinsIgEConditionValue2->setValue(data);
}

void sIgERangeSettingWidget::readData3()
{
    GenericIniDataControls io;
    auto data = io.readsIgEData3();
    ui->spinsIgEConditionValue3->setValue(data);
}

void sIgERangeSettingWidget::readData4()
{
    GenericIniDataControls io;
    auto data = io.readsIgEData4();
    ui->spinsIgEConditionValue4->setValue(data);
}

void sIgERangeSettingWidget::readData5()
{
    GenericIniDataControls io;
    auto data = io.readsIgEData5();
    ui->spinsIgEConditionValue5->setValue(data);
}

void sIgERangeSettingWidget::readData6()
{
    GenericIniDataControls io;
    auto data = io.readsIgEData6();
    ui->spinsIgEConditionValue6->setValue(data);
}

void sIgERangeSettingWidget::writeData1()
{
    GenericIniDataControls io;
    io.writesIgEData1(ui->spinsIgEConditionValue1->value());
}

void sIgERangeSettingWidget::writeData2()
{
    GenericIniDataControls io;
    io.writesIgEData2(ui->spinsIgEConditionValue2->value());
}

void sIgERangeSettingWidget::writeData3()
{
    GenericIniDataControls io;
    io.writesIgEData3(ui->spinsIgEConditionValue3->value());
}

void sIgERangeSettingWidget::writeData4()
{
    GenericIniDataControls io;
    io.writesIgEData4(ui->spinsIgEConditionValue4->value());
}

void sIgERangeSettingWidget::writeData5()
{
    GenericIniDataControls io;
    io.writesIgEData5(ui->spinsIgEConditionValue5->value());
}

void sIgERangeSettingWidget::writeData6()
{
    GenericIniDataControls io;
    io.writesIgEData6(ui->spinsIgEConditionValue6->value());
}

void sIgERangeSettingWidget::readSelectedRadioClass()
{
    GenericIniDataControls io;
    auto selectedClass = io.readsIgESelectedRadioButtonClass();

    switch(selectedClass)
    {
    case 1:
        ui->radioClass1->setChecked(true);
        break;

    case 2:
        ui->radioClass2->setChecked(true);
        break;

    case 3:
        ui->radioClass3->setChecked(true);
        break;

    case 4:
        ui->radioClass4->setChecked(true);
        break;

    case 5:
        ui->radioClass5->setChecked(true);
        break;

    case 6:
        ui->radioClass6->setChecked(true);
        break;

    default:
        break;
    }
}

void sIgERangeSettingWidget::writeSelectedRadioClass()
{
    GenericIniDataControls io;

    auto isSelected1 = ui->radioClass1->isChecked();
    auto isSelected2 = ui->radioClass2->isChecked();
    auto isSelected3 = ui->radioClass3->isChecked();
    auto isSelected4 = ui->radioClass4->isChecked();
    auto isSelected5 = ui->radioClass5->isChecked();
    auto isSelected6 = ui->radioClass6->isChecked();

    if(isSelected1)
        io.writesIgESelectedRadioButtonClass(1);

    else if(isSelected2)
        io.writesIgESelectedRadioButtonClass(2);

    else if(isSelected3)
        io.writesIgESelectedRadioButtonClass(3);

    else if(isSelected4)
        io.writesIgESelectedRadioButtonClass(4);

    else if(isSelected5)
        io.writesIgESelectedRadioButtonClass(5);

    else if(isSelected6)
        io.writesIgESelectedRadioButtonClass(6);
}
