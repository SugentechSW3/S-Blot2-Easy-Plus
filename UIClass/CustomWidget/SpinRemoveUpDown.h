#ifndef SPINREMOVEUPDOWN_H
#define SPINREMOVEUPDOWN_H

#include <QSpinBox>

class SpinRemoveUpDown : public QSpinBox
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit SpinRemoveUpDown(QWidget *parent = nullptr);
    virtual ~SpinRemoveUpDown();

    void setWheelEventDisable(bool isDisable);

signals:
    void onPressedArrowKeyBoard(int qtKey);
    void onFocusInEventSpinBox();

protected:
    void focusInEvent(QFocusEvent*evt);
    void keyPressEvent(QKeyEvent *event);

private :


private slots:
    void wheelEvent(QWheelEvent *event);
    void onDeleteActivated();

private:
    QScopedPointer<privateStruct> d;

};

#endif // SPINREMOVEUPDOWN_H
