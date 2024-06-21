#ifndef NAVIGATORARROWWIDGET_H
#define NAVIGATORARROWWIDGET_H

#include <QWidget>


class Ui_NavigatorArrowWidget;
class NavigatorArrowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NavigatorArrowWidget(QWidget *parent = nullptr);
    virtual ~NavigatorArrowWidget();

signals:
    void onClickedPrev();
    void onClickedNext();

private :
    void initSignalSlots();

private:
   QScopedPointer<Ui_NavigatorArrowWidget> ui;
};

#endif // NAVIGATORARROWWIDGET_H
