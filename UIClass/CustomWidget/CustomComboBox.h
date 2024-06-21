#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QComboBox>

class CustomComboBox : public QComboBox
{
private:
    Q_OBJECT

public:
    explicit CustomComboBox(QWidget *parent = nullptr);
    virtual ~CustomComboBox();

    void addItemForPanelColorText(const QString& itemName);
    void addItemForPanelColorText(const QString& item, const QVariant& data);
    void addItemForPanelColorText(const QStringList& itemList);

private:
    void initStyleSheet();
    QColor findColor(const QString& itemName);

private slots:
    void onTextColorChanged();

};

#endif // CUSTOMCOMBOBOX_H
