#ifndef CCHECKBOXPARENTWIDGET_H
#define CCHECKBOXPARENTWIDGET_H

#include <QCheckBox>

class CCheckBoxParentWidget : public QCheckBox
{
    Q_OBJECT
public:
    explicit CCheckBoxParentWidget(QWidget *parent = nullptr);
    virtual ~CCheckBoxParentWidget();

private:
   virtual void initStyleSheet();


};

#endif // CCHECKBOXPARENTWIDGET_H
