#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScopedPointer>
#include <QEvent>

class Ui_MainWindow;

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    static MainWindow *getInstance();
    void setDisableTopLevelWidget(bool isDisable);
    void startAutoCleanProcess();

signals:
    void onDetectedAddDevice();
    void onDetectedRemoveDevice();
    void onFinishedAutoClean();

private:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void closeEvent(QCloseEvent* event) override;
    virtual bool nativeEvent(const QByteArray &/*eventType*/, void *msg, long */*result*/) override;

    void initShortCut();
    void initSignalSlots();
    void initDlg();
    void initCamera();
    void stopDlg();

    void programCloseProcess();

    void startDisconnectProcess();
    void stopDisconnectProcess();

    void writeLogForCompanyName();

    void sendAutoCleanProtocolAndShowMessage();
    void showAutoCleanMessage();
    void sendAutoCleanProtocol();
    void sendAutoCleanStatusProtocol();
    void sendResumeProtocol();

private slots:
    void onSerialConnected();
    void onSerialDisConnected();
    void onSendExitSignal();
    void onResetWait();
    void onResetEnd();
    void onResultResetProcess();
    void onProgramQuit();
    void onAutocleanProcess(const QByteArray& recvData);
    void onRecvData(QByteArray recvData);
    void onRecvStatusRead(const QByteArray& recvData);
    void onRecvErrorRead(const QByteArray& recvData);

private:
    QScopedPointer<Ui_MainWindow> ui;
    QScopedPointer<privateStruct> d;
    static MainWindow* instance;
};

#endif // MAINWINDOW_H
