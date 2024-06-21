#ifndef RELATIVECHECKBOXWIDGET_H
#define RELATIVECHECKBOXWIDGET_H

#include <QWidget>

class Ui_RelativeCheckBoxWidget;

class RelativeCheckBoxWidget : public QWidget
{
    Q_OBJECT
    struct PrivateStruct;
public:
    explicit RelativeCheckBoxWidget(QWidget *parent = nullptr);
    ~RelativeCheckBoxWidget();

    void createEditCheckBox(int count);
    void setGroubBoxName(const QString& gbName);
    QVector<bool> getCheckBoxData();
private:
    void clearCheckBox();

private:
    QScopedPointer<Ui_RelativeCheckBoxWidget> ui;
    QScopedPointer<PrivateStruct> d;
};

#endif // RELATIVECHECKBOXWIDGET_H
