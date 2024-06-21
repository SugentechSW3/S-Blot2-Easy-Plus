#ifndef MEMBBANDCOUNT_H
#define MEMBBANDCOUNT_H

#include <QWidget>

class Ui_RelativePosWidget;
class RelativePosWidget : public QWidget
{
private:
    Q_OBJECT

    struct privateStruct;
public:
    explicit RelativePosWidget(QWidget *parent = nullptr);
    virtual ~RelativePosWidget();

    void CreateEditBox(int count);
    void reCreateEditBox(int count);
    void clearEditBox();
    int getEditBoxData(int index);

    void setEditBoxData(int index, int value);
    int getWidgetCount();

    void setEnableSpin();
    void setDisableSpin();

    void setMaximumValue(int val);
    void setMinimumValue(int val);

signals:
    void onChangedBoxCarSpin(int idx, int value);

private:
    QScopedPointer<Ui_RelativePosWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // MEMBBANDCOUNT_H
