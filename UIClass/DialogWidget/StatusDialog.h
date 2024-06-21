#ifndef STATUSDIALOG_H
#define STATUSDIALOG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_StatusDialog;
class StatusDialog : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit StatusDialog(QWidget *parent = nullptr);
    virtual ~StatusDialog();

    void setMessage(QString msg);
    void setStatusMessage(QString msg);

    void startWaiting();
    void stopWaiting();
    bool isActivatedTimer();

    void startWaitingRefreshApplication();
private slots:
    void setWaitingMessage();
private:
    QScopedPointer<Ui_StatusDialog> ui;
    QScopedPointer<privateStruct> d;
};

#endif // STATUSDIALOG_H
