#ifndef MOVECONTROLWIDGET_H
#define MOVECONTROLWIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <Common/CommonParentWidget/CSerialNetworkWidget.h>

class Ui_MoveControlWidget;
class MoveControlWidget : public CSerialNetworkWidget
{
    Q_OBJECT
    struct privateStruct;
public:
    explicit MoveControlWidget(QWidget *parent = nullptr);
    virtual ~MoveControlWidget();


    void setRange(int min, int max); //연동되야됨.


private:
    virtual void initVariables();
    virtual void initSignalSlots();
    virtual void startWidget();
    virtual void stopWidget();

    void sendMoveCommand(int position);
    void moveTripleQR();

private slots:
    void onClickedBack();
    void onClickedMove();
    void onClickedFront();
    void onClickedQR();


private:
    QScopedPointer<Ui_MoveControlWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // MOVECONTROLWIDGET_H
