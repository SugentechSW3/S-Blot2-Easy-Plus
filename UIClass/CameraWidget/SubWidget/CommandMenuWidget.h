#ifndef COMMANDMENUWIDGET_H
#define COMMANDMENUWIDGET_H

#include <QWidget>
#include <QScopedPointer>

class Ui_CommandMenuWidget;
class CommandMenuWidget : public QWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit CommandMenuWidget(QWidget *parent = nullptr);
    virtual ~CommandMenuWidget();

signals:
    void onSelectedLoadImage(QString path);
    void onSelectedSaveImage(QString path);

    void onSelectedAnalysis();
    void onSelectedFindPC();
    void onSelectedAngleReset();
    void onReadQRTest();
    void onGraphHide(bool isHide);

private:
    void init();

private slots:
    void onClickedLoadImage();
    void onClickedSaveImage();
    void onDelayButton(bool);

private:
   QScopedPointer<Ui_CommandMenuWidget> ui;
   QScopedPointer<privateStruct> d;
};

#endif // COMMANDMENUWIDGET_H
