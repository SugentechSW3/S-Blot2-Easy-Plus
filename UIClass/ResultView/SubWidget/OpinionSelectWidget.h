#ifndef OPINIONSELECTWIDGET_H
#define OPINIONSELECTWIDGET_H

#include <QWidget>
#include <Analysis/GlobalDataStruct.h>

class Ui_OpinionSelectWidget;
class OpinionSelectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OpinionSelectWidget(QWidget *parent = nullptr);
    virtual ~OpinionSelectWidget();

    void clearComment();

    QString getOpinionText();
    void setOpinionText(const QString& text);

signals:
    void onApply();

private:
    void initSignalSlot();
    void initOpinion();

    void setupData(const QVector<GlobalDataStruct::OPINION_DATA>& data);

    QString getCurrentComboboxData();
    QString getCurrentTitleText();
    void setCurrentTitleText(const QString&text);

    GlobalDataStruct::OPINION_DATA getCurrentSelectedData();

private slots:
    void onClickSetup();
    void onChangedComboboxSelect(const QString &text);

private:
    QScopedPointer<Ui_OpinionSelectWidget> ui;
};

#endif // OPINIONSELECTWIDGET_H
