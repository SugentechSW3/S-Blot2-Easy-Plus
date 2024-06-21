#ifndef QUESTIONDLG_H
#define QUESTIONDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_QuestionDlg;
class QuestionDlg : public CDialogParentWidget
{
    Q_OBJECT

public:
    explicit QuestionDlg(QWidget *parent = nullptr);
    virtual ~QuestionDlg();

    void setQuestionMessage(const QString& message);

private:
    void initButton();

private:
    QScopedPointer<Ui_QuestionDlg> ui;
};

#endif // QUESTIONDLG_H
