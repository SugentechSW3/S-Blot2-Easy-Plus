#ifndef REAGENTSTRIPWIDGET_H
#define REAGENTSTRIPWIDGET_H

#include <QWidget>
#include <QLabel>
#include "Analysis/ConfigInformation.h"

class Ui_ReagentStripWidget;
class ReagentStripWidget : public QWidget
{
private:
    Q_OBJECT
    struct privateStruct;
    struct privateWidget
    {
        QWidget* widget = nullptr;
        QLabel*  label = nullptr;
    };

public:

    explicit ReagentStripWidget(QWidget *parent = nullptr);
    ~ReagentStripWidget();

    void updateStrip(const QVector<ConfigInformation::STRIP_COLOR> &data);

private:
    void clearStrip();
    void createStrip(ConfigInformation::STRIP_COLOR color, int idx);

private:
    QScopedPointer<Ui_ReagentStripWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // REAGENTSTRIPWIDGET_H
