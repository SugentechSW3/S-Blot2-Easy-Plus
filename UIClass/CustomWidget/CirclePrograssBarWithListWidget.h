#ifndef CIRCLEPROGRASSBARWITHLISTWIDGET_H
#define CIRCLEPROGRASSBARWITHLISTWIDGET_H

#include <QWidget>

class Ui_CirclePrograssBarWithListWidget;
class CircleListWidgetItem;

class CirclePrograssBarWithListWidget : public QWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit CirclePrograssBarWithListWidget(QWidget *parent = nullptr);
    virtual ~CirclePrograssBarWithListWidget();

    void addItem(const QString &labelText, bool isHideCircle = true);
    void removeItem(int idx);
    void clearItem();

    CircleListWidgetItem* getItem(int idx);
    int getCount();
private:
    QScopedPointer<Ui_CirclePrograssBarWithListWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // CIRCLEPROGRASSBARWITHLISTWIDGET_H
