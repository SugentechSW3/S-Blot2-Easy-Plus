#ifndef RADIOGROUPBOXWIDGET_H
#define RADIOGROUPBOXWIDGET_H

#include <QWidget>
#include <QRadioButton>
#include <QScopedPointer>

class QRadioButton;
class Ui_RadioGroupBoxWidget;

class RadioGroupBoxWidget : public QWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit RadioGroupBoxWidget(QWidget* parent = nullptr);
    explicit RadioGroupBoxWidget(QString groupBoxName, QWidget *parent = nullptr);
    virtual ~RadioGroupBoxWidget();

    void setTitle(QString groupBoxTitle);
    void addRadioWidget(const QString &text, const QString& objectName);

    QRadioButton* findObject(const QString& objectName);
    QRadioButton* getCheckedRadioBox();

    static QString spinToRadioName(QString objectName);
    static QString doubleSpinToRadioName(QString objectName);
    static QString radioNameToSpinName(QString objectName);

private:
    QScopedPointer<Ui_RadioGroupBoxWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // RADIOGROUPBOXWIDGET_H
