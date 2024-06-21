#ifndef BANDGAPWIDGET_H
#define BANDGAPWIDGET_H

#include <QWidget>



class Ui_BandGapWidget;
class BandGapWidget : public QWidget
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit BandGapWidget(QWidget *parent = nullptr);
    virtual ~BandGapWidget();

    void CreateEditBox(int count);
    void reCreateEditBox(int count);
    void clearEditBox();
    int getEditBoxData(int index);

    void setEditBoxData(int index, int value);
    int getWidgetCount();

    void setGroupBoxName(const QString& str);
    void setGroupBoxNameColor(Qt::GlobalColor color);

    void setDisableSpin();
    void setEnableSpin();

    void setMaximumValue(int val);
    void setMinimumValue(int val);

signals:
    void onChangedSpin(int idx, int val);

private slots:
    void onChangedSpinValue(int val);

private:
    QScopedPointer<Ui_BandGapWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // BANDGAPWIDGET_H
