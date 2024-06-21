#ifndef TABLE_EXCEL_STYLE_WIDGET_H
#define TABLE_EXCEL_STYLE_WIDGET_H

#include <QTableWidget>
#include <QScopedPointer>

class TableExcelStyleWidget : public QTableWidget
{
private:
    Q_OBJECT

    struct privateStruct;
public:
    explicit TableExcelStyleWidget(QWidget* parent = nullptr);
    virtual ~TableExcelStyleWidget();

signals:
    void onKeyPressed(int row, int column, int qtKey);
    void onSortRequest(int columnNumber);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void initVariables();
    void initSignalSlot();
    void initShortCut();


private slots:
    void onEmittedCopyShortCut();

private:
    QScopedPointer<privateStruct> d;
};

#endif // TABLE_EXCEL_STYLE_WIDGET_H
