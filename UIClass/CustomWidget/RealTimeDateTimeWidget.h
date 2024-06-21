#ifndef REALTIMEDATETIMEWIDGET_H
#define REALTIMEDATETIMEWIDGET_H

#include <QDateEdit>

class RealTimeDateTimeWidget : public QDateEdit
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit RealTimeDateTimeWidget(QWidget *parent = nullptr);
    virtual ~RealTimeDateTimeWidget();

    void startDateTime();
    void stopDateTime();

private slots:
    void onSyncDateTime();

private:
    QScopedPointer<privateStruct> d;
};

#endif // REALTIMEDATETIMEWIDGET_H
