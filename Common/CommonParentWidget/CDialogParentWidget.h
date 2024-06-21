#ifndef CDIALOGPARENTWIDGET_H
#define CDIALOGPARENTWIDGET_H

#include <QDialog>

class CDialogParentWidget : public QDialog
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit CDialogParentWidget(QWidget *parent = nullptr);
    virtual ~CDialogParentWidget();

    virtual void setKeepBlockingBackground(bool isBlock);
protected:
   virtual void onDialogOpen();
   virtual void onDialogClose();

private:
    QScopedPointer<privateStruct> d;
};

#endif // CDIALOGPARENTWIDGET_H
