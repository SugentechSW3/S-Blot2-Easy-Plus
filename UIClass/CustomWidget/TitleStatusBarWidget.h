#ifndef TITLESTATUSBARWIDGET_H
#define TITLESTATUSBARWIDGET_H

#include <QWidget>

class Ui_TitleStatusBarWidget;
class TitleStatusBarWidget : public QWidget
{
private:
    Q_OBJECT
public:
    explicit TitleStatusBarWidget(QWidget *parent = nullptr);
    ~TitleStatusBarWidget();

public slots:
    void onStatusMain();
    void onStatusPatientInfomation();
    void onStatusReagent();
    void onStatusRunningStatus();
    void onStatusResultView();

private:
    QScopedPointer<Ui_TitleStatusBarWidget> ui;
};

#endif // TITLESTATUSBARWIDGET_H
