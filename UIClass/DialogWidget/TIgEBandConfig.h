#ifndef TIGEBANDCONFIG_H
#define TIGEBANDCONFIG_H

#include <QWidget>

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_TIgEBandConfig;

class TIgEBandConfig : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;
    enum TIGE_BAND_CONFIG_COLUMN
    {
        TIGE_CLASS_MORE,
        TIGE_CLASS_UNDER,
        TIGE_MULTIPLICATION,
        TIGE_TABLE_COUNT
    };

public:
    explicit TIgEBandConfig(QWidget *parent = nullptr);
    virtual ~TIgEBandConfig();



private slots:
    void onClickedAdd();
    void onClickedUpdate();
    void onClickedDelete();
    void onClickedSave();
    void onCellClicked(int row,int colum);
private:
   void initVariable();
   void initSignalSlots();
   void initSetEditExp();
   void onLoadDisplay();
   void tableSetDisEnableEdit(int row);
   bool checkEdtEmpty();
private:
    QScopedPointer<Ui_TIgEBandConfig> ui;
    QScopedPointer<privateStruct> d;
};





#endif // TIGEBANDCONFIG_H
