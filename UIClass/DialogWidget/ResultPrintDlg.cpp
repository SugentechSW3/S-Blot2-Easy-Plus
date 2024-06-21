#include <QThread>
#include "ResultPrintDlg.h"
#include "ui_ResultPrintDlg.h"

#include "Common/Io/CPrintSupport.h"
#include "UIClass/ResultView/SubWidget/ResultPrintWidget.h"
#include "UIClass/ResultView/SubWidget/ResultPrintV5Widget.h"

USING_SUGENTECH

struct ResultPrintDlg::privateStruct
{
    GlobalDataStruct::AnalysisReportInfomation mData;
};

ResultPrintDlg::ResultPrintDlg(const GlobalDataStruct::AnalysisReportInfomation &data, QWidget *parent) :
                                                                                                          QDialog(parent),
                                                                                                          ui(new Ui_ResultPrintDlg),
                                                                                                          d(new privateStruct)
{
    ui->setupUi(this);
    d->mData = data;
}

ResultPrintDlg::~ResultPrintDlg()
{
}

void ResultPrintDlg::startPrint()
{
    auto widgetResult = new ResultPrintV5Widget(ui->stackedWidget);
    widgetResult->setData(d->mData);
    ui->stackedWidget->addWidget(widgetResult);

    QVector<QPicture> windows;
    windows += widgetResult->printWindow();

    CPrintSupport * mSupport = new CPrintSupport(nullptr);
    mSupport->setPrintData(windows, windows.count());
    mSupport->start();
}
