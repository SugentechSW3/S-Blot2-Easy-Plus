#ifndef PROGRESSBARDLG_H
#define PROGRESSBARDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_ProgressBarDlg;
class ProgressBarDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit ProgressBarDlg(QWidget *parent = nullptr);
    virtual ~ProgressBarDlg();

    void start(double timeOutmSec);
    void stop();

signals:
    void onCompleted();

public slots:
    void setUseTimer(bool isUse);
    void setValue(double value);
    void setValue(double value, QString& text);
    void setText(QString text);

private slots:
    void onTimeOut();

private:
    void initSignalSlots();
private:
    QScopedPointer<Ui_ProgressBarDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // PROGRESSBARDLG_H
