#include "ResultPrintWidgetClassTableItem.h"
#include "ui_ResultPrintWidgetClassTableItem.h"

ResultPrintWidgetClassTableItem::ResultPrintWidgetClassTableItem(QWidget *parent) :
                                                                                    QWidget(parent),
                                                                                    ui(new Ui_ResultPrintWidgetClassTableItem)
{
    ui->setupUi(this);
    ui->labelClassNumber->setStyleSheet("background-color: #F2F2F2;");
}

ResultPrintWidgetClassTableItem::~ResultPrintWidgetClassTableItem()
{
}

void ResultPrintWidgetClassTableItem::setClass(const QString& classData)
{

    int classNumber = classData.toInt();
    ui->labelClassNumber->setText(classData);

    if(classNumber == 0)
        return;

    switch(classNumber)
    {
    case 1:
        ui->labelClass1->setStyleSheet("background-color: #DCE6F1;");
        break;

    case 2:
        ui->labelClass1->setStyleSheet("background-color: #B8CCE4;");
        ui->labelClass2->setStyleSheet("background-color: #B8CCE4;");
        break;

    case 3:
        ui->labelClass1->setStyleSheet("background-color: #95B3D7;");
        ui->labelClass2->setStyleSheet("background-color: #95B3D7;");
        ui->labelClass3->setStyleSheet("background-color: #95B3D7;");
        break;

    case 4:
        ui->labelClass1->setStyleSheet("background-color: #4F81BD;");
        ui->labelClass2->setStyleSheet("background-color: #4F81BD;");
        ui->labelClass3->setStyleSheet("background-color: #4F81BD;");
        ui->labelClass4->setStyleSheet("background-color: #4F81BD;");
        break;

    case 5:
        ui->labelClass1->setStyleSheet("background-color: #366092;");
        ui->labelClass2->setStyleSheet("background-color: #366092;");
        ui->labelClass3->setStyleSheet("background-color: #366092;");
        ui->labelClass4->setStyleSheet("background-color: #366092;");
        ui->labelClass5->setStyleSheet("background-color: #366092;");
        break;

    case 6:
        ui->labelClass1->setStyleSheet("background-color: #244062;");
        ui->labelClass2->setStyleSheet("background-color: #244062;");
        ui->labelClass3->setStyleSheet("background-color: #244062;");
        ui->labelClass4->setStyleSheet("background-color: #244062;");
        ui->labelClass5->setStyleSheet("background-color: #244062;");
        ui->labelClass6->setStyleSheet("background-color: #244062;");
        break;
    }
}

void ResultPrintWidgetClassTableItem::setClassTextColor(const QString& hexColorCode)
{
    ui->labelClassNumber->setStyleSheet(QString("color: %1").arg(hexColorCode));
    auto currentText = ui->labelClassNumber->text();
    ui->labelClassNumber->setText(currentText.append("*"));
}

void ResultPrintWidgetClassTableItem::setClassReportV5(const QString& classData)
{
    ui->labelClass1->setMinimumWidth(14);
    ui->labelClass1->setMaximumWidth(14);
    ui->labelClass2->setMinimumWidth(14);
    ui->labelClass2->setMaximumWidth(14);
    ui->labelClass3->setMinimumWidth(14);
    ui->labelClass3->setMaximumWidth(14);
    ui->labelClass4->setMinimumWidth(14);
    ui->labelClass4->setMaximumWidth(14);
    ui->labelClass5->setMinimumWidth(14);
    ui->labelClass5->setMaximumWidth(14);
    ui->labelClass6->setMinimumWidth(14);
    ui->labelClass6->setMaximumWidth(14);

    ui->labelClass1->setMinimumHeight(22);
    ui->labelClass1->setMaximumHeight(22);
    ui->labelClass2->setMinimumHeight(22);
    ui->labelClass2->setMaximumHeight(22);
    ui->labelClass3->setMinimumHeight(22);
    ui->labelClass3->setMaximumHeight(22);
    ui->labelClass4->setMinimumHeight(22);
    ui->labelClass4->setMaximumHeight(22);
    ui->labelClass5->setMinimumHeight(22);
    ui->labelClass5->setMaximumHeight(22);
    ui->labelClass6->setMinimumHeight(22);
    ui->labelClass6->setMaximumHeight(22);

    int classNumber = classData.toInt();
    ui->labelClassNumber->setVisible(false);

    if(classNumber == 0)
        return;


    QString bgColor = "#FFFFFF";

    switch(classNumber)
    {
    case 1:
        bgColor = "#DDEBF7";
        ui->labelClass1->setStyleSheet(getLabelClassStyleSheet(bgColor));
        break;

    case 2:
        bgColor = "#BDD7EE";
        ui->labelClass1->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass2->setStyleSheet(getLabelClassStyleSheet(bgColor));
        break;

    case 3:
        bgColor = "#9BC2E6";
        ui->labelClass1->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass2->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass3->setStyleSheet(getLabelClassStyleSheet(bgColor));
        break;

    case 4:
        bgColor = "#5B9BD5";
        ui->labelClass1->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass2->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass3->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass4->setStyleSheet(getLabelClassStyleSheet(bgColor));
        break;

    case 5:
        bgColor = "#2F75B5";
        ui->labelClass1->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass2->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass3->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass4->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass5->setStyleSheet(getLabelClassStyleSheet(bgColor));
        break;

    case 6:
        bgColor = "#1F4E78";
        ui->labelClass1->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass2->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass3->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass4->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass5->setStyleSheet(getLabelClassStyleSheet(bgColor));
        ui->labelClass6->setStyleSheet(getLabelClassStyleSheet(bgColor));
        break;
    }
}

QString ResultPrintWidgetClassTableItem::getLabelClassStyleSheet(QString bgColor)
{
    return QString(" QLabel { "
            " border-bottom: 1px solid #D9D9D9; "
            " border-right: 1px solid #D9D9D9; "
            " border-top: 0px solid #D9D9D9; "
            " border-left: 0px solid #D9D9D9; "
            " margin: 0px; "
            " background-color: %1; "
            " }").arg(bgColor);
}
