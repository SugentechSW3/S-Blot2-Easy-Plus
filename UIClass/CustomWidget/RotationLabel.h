#ifndef ROTATIONLABEL_H
#define ROTATIONLABEL_H

#include <QLabel>

class RotationLabel : public QLabel
{
private:
    Q_OBJECT
    Q_PROPERTY(double rotationValue READ rotationValue WRITE setRotationValue)
    struct privateStruct;
public:
    explicit RotationLabel(QWidget *parent = nullptr);
    ~RotationLabel();

    void startRotation();
    void stopRotation();
    void pauseRotation();

    void setImage(const QString& imagePath);
private:
    void paintEvent(QPaintEvent*);

    double rotationValue();
    void setRotationValue(double value);


    QString key();
    QPixmap generatePixmap() const;

private:
    QScopedPointer<privateStruct> d;

};

#endif // ROTATIONLABEL_H
