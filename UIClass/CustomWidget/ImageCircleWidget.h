#ifndef IMAGECIRCLEWIDGET_H
#define IMAGECIRCLEWIDGET_H

#include <QWidget>

class ImageCircleWidget : public QWidget
{
private:
    Q_OBJECT
    Q_PROPERTY(int value READ getValue WRITE setValue NOTIFY onValueChanged)
    struct privateStruct;

public:
    explicit ImageCircleWidget(QWidget *parent = nullptr);
    ~ImageCircleWidget();

    void initStyleSheet();
    int getValue() const;

signals:
    void onValueChanged(int value);

public slots:
    void setMovingValue(int value);

private slots:
    void setValue(int value);

private:
    virtual void paintEvent(QPaintEvent* /*event*/) override;

    QString key() const;
    QPixmap generatePixmap();
    QRectF squaredF(QRectF rect);

private:
    QScopedPointer<privateStruct> d;
    const int FIXED_SIZE = 16;
};

#endif // IMAGECIRCLEWIDGET_H
