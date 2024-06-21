#ifndef RESULTPRINTDLG_H
#define RESULTPRINTDLG_H

#include <QDialog>
#include "Analysis/BasicSupremeIAnalysis.h"

class Ui_ResultPrintDlg;
class ResultPrintDlg : public QDialog
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit ResultPrintDlg(const GlobalDataStruct::AnalysisReportInfomation &data, QWidget *parent = nullptr);
    virtual ~ResultPrintDlg();

    void startPrint();

private:
    QScopedPointer<Ui_ResultPrintDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // RESULTPRINTDLG_H
