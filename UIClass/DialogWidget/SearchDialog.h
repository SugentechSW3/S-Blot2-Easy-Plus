#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>
#include <QDate>
#include "Analysis/GlobalDataStruct.h"

class Ui_SearchDialog;
class SearchDialog : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit SearchDialog(QWidget *parent = nullptr);
    virtual ~SearchDialog();

    GlobalDataStruct::QUERY_CONDITION_DATA getData() const;

private:
    void initVariable();
    void initSignalSlots();

private slots:
   void onSearch();

private:
    QScopedPointer<Ui_SearchDialog> ui;
    QScopedPointer<privateStruct> d;
};

#endif // SEARCHDIALOG_H
