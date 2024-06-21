#ifndef ADMINSERIALDIALOG_H
#define ADMINSERIALDIALOG_H

#include <QDialog>

class Ui_AdminSerialDialog;

class AdminSerialDialog : public QDialog
{
    Q_OBJECT
    struct privateStruct;
public:
    explicit AdminSerialDialog(QWidget *parent = nullptr);
    ~AdminSerialDialog();

private:
    void initVarialbes();
    void initSignalSlot();
    bool listWidgetScrollControl();
    bool isAddWidgetText(const QString& message);
private slots:
    void onClickedSeiralDataSend();
    void onRecvData(QByteArray recvData);

private:
    QScopedPointer<Ui_AdminSerialDialog> ui;
    QScopedPointer<privateStruct> d;
};

#endif // ADMINSERIALDIALOG_H
