#ifndef CUSTOMSTAYCLICKEVNETWIDGET_H
#define CUSTOMSTAYCLICKEVNETWIDGET_H

#include <QWidget>

class CustomStayClickEvnetWidget : public QWidget
{
    Q_OBJECT
    struct privateStruct;
public:
    explicit CustomStayClickEvnetWidget(QWidget* parent = nullptr, int count = 3);
    virtual ~CustomStayClickEvnetWidget();
protected:
    void mousePressEvent(QMouseEvent* event) override;

private:
    QScopedPointer<privateStruct> d;
};

#endif // CUSTOMSTAYCLICKEVNETWIDGET_H
