#ifndef CIRCLELISTWIDGETITEM_H
#define CIRCLELISTWIDGETITEM_H

#include <QWidget>

class Ui_CircleListWidgetItem;
class CircleListWidgetItem : public QWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit CircleListWidgetItem(QWidget *parent = nullptr);
    virtual ~CircleListWidgetItem();

    void setLabelText(const QString& text);
    QString getLabelText() const;

    void setMaximumValue(int maximum);
    int getMaximumValue() const;

    void setCircleMaximumValue(int maximum);
    int getCircleMaximumValue() const;

    void setValue(int value);
    int getValue() const;

    void setPrograssFormat(const QString& text);
    QString getPrograssFormat() const;

    void setCircleValue(int value);
    int getCircleValue() const;

    void hideCircle();
    void showCircle();

    void startCircle();
    void endCircle();
    void pauseCircle();

    bool isPause() const;

private:
    QScopedPointer<Ui_CircleListWidgetItem> ui;
    QScopedPointer<privateStruct> d;
};

#endif // CIRCLELISTWIDGETITEM_H
