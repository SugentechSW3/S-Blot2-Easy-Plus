#ifndef CIRCLEPROGRASSDIALOG_H
#define CIRCLEPROGRASSDIALOG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_CirclePrograssDialog;
class CirclePrograssDialog : public CDialogParentWidget
{
    Q_OBJECT

public:
    explicit CirclePrograssDialog(QWidget *parent = nullptr);
    virtual ~CirclePrograssDialog();

    void start();
    void stop();

private:
   QScopedPointer<Ui_CirclePrograssDialog> ui;
};

#endif // CIRCLEPROGRASSDIALOG_H
