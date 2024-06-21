#include <QPainter>
#include <QPropertyAnimation>
#include <QPixmapCache>
#include <QDebug>
#include "RotationLabel.h"

struct RotationLabel::privateStruct
{
    QPixmap mImagePixmap;
    QPropertyAnimation mRotationAnimation;
    double mRotationValue = 0.0;
};

RotationLabel::RotationLabel(QWidget *parent) :
                                                QLabel(parent),
                                                d(new privateStruct)
{
    d->mRotationAnimation.setTargetObject(this);
    d->mRotationAnimation.setPropertyName("rotationValue");
    d->mRotationAnimation.setLoopCount(-1);//infinite
    d->mRotationAnimation.setDuration(3000);
    d->mRotationAnimation.setStartValue(0.0);
    d->mRotationAnimation.setEndValue(360.0);
    d->mRotationAnimation.setEasingCurve(QEasingCurve::InOutQuad);


    connect(&d->mRotationAnimation, &QPropertyAnimation::valueChanged, this, [this]
            {
                this->update();
            });
}

RotationLabel::~RotationLabel()
{

}

void RotationLabel::startRotation()
{
    if(d->mRotationAnimation.state() == QAbstractAnimation::State::Paused)
        d->mRotationAnimation.resume();
    else
        d->mRotationAnimation.start();
}

void RotationLabel::stopRotation()
{
    d->mRotationAnimation.stop();
}

void RotationLabel::pauseRotation()
{
    d->mRotationAnimation.pause();
}

void RotationLabel::setImage(const QString& imagePath)
{
    d->mImagePixmap = QPixmap(imagePath);
}

void RotationLabel::paintEvent(QPaintEvent*)
{
    QPixmap pixmap;
    if (!QPixmapCache::find(key(), pixmap))
    {
        pixmap = this->generatePixmap();
        QPixmapCache::insert(key(), pixmap);
    }

    // Draw pixmap at center of item
    QPainter painter(this);
    painter.drawPixmap( 0.5 * ( width() - pixmap.width() ), 0.5 * ( height() - pixmap.height() ), pixmap );
}


double RotationLabel::rotationValue()
{
    return d->mRotationValue;
}

void RotationLabel::setRotationValue(double value)
{
    d->mRotationValue = value;
}

QString RotationLabel::key()
{
    return QString::number(d->mRotationValue, 'f', 4);
}

QPixmap RotationLabel::generatePixmap() const
{
    QMatrix rm;
    rm.rotate(d->mRotationValue);
    return d->mImagePixmap.transformed(rm);
}
