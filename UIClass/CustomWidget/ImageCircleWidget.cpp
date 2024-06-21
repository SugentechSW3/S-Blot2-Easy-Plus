#include <QPainter>
#include <QPixmapCache>
#include <QPropertyAnimation>

#include "ImageCircleWidget.h"


struct ImageCircleWidget::privateStruct
{
    QPropertyAnimation mValueAnimation;

    double mOuterRadius = 0.0;
    double mInnerRadius = 0.0;
    int mValue = 0;
    int mMaximum = 100;
    QColor mColor;
};

ImageCircleWidget::ImageCircleWidget(QWidget* parent):
                                                        QWidget(parent),
                                                        d(new privateStruct)
{
    d->mColor = QColor(Qt::white);
    d->mInnerRadius = 0.6;
    d->mOuterRadius = 1.0;

    d->mValueAnimation.setTargetObject(this);
    d->mValueAnimation.setPropertyName("value");
    d->mValueAnimation.setDuration(1000);
    d->mValueAnimation.setEasingCurve(QEasingCurve(QEasingCurve::OutQuad));

    connect(this, &ImageCircleWidget::onValueChanged, this, [this]
            (int)
            {
                this->update();
            });
}

ImageCircleWidget::~ImageCircleWidget()
{

}

int ImageCircleWidget::getValue() const
{
    return d->mValue;
}

void ImageCircleWidget::setValue(int value)
{
    d->mValue = value;
    emit onValueChanged(d->mValue);
}

void ImageCircleWidget::setMovingValue(int value)
{
    if(value < 0)
        value = 0;

    if(value > 100)
        value = 100;

    d->mValueAnimation.setEndValue(value);
    d->mValueAnimation.start();
}

void ImageCircleWidget::paintEvent(QPaintEvent* /*event*/)
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

QString ImageCircleWidget::key() const
{
    return QString("%1,%2,%3,%4,%5,%6,%7")
        .arg(d->mValue)
        .arg(d->mMaximum)
        .arg(d->mInnerRadius)
        .arg(d->mOuterRadius)
        .arg(this->width())
        .arg(this->height())
        .arg(d->mColor.rgb());
}

QPixmap ImageCircleWidget::generatePixmap()
{
    QPixmap pixmap(this->squaredF(rect()).size().toSize());
    pixmap.fill(QColor(0,0,0,0));
    QPainter painter(&pixmap);

    painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF rect = pixmap.rect().adjusted(1,1,-1,-1);
    qreal margin = rect.width()*(1.0 - d->mOuterRadius)/2.0;
    rect.adjust(margin,margin,-margin,-margin);

    QBrush brush(QPixmap(":/Image/Image/RunningStatus/progress100.png"));
    painter.setBrush(brush);
    painter.setPen(d->mColor);

    int value = qMin(d->mValue, d->mMaximum);
    int startAngle = 90 * FIXED_SIZE;
    int spanAngle = -qreal(value) * 360 * FIXED_SIZE / d->mMaximum;

    painter.drawPie(rect, startAngle, spanAngle);

    return pixmap;
}

QRectF ImageCircleWidget::squaredF(QRectF rect)
{
    if(rect.width() > rect.height())
    {
        qreal diff = rect.width() - rect.height();
        return rect.adjusted(diff/2, 0, -diff/2, 0);
    }
    else
    {
        qreal diff = rect.height() - rect.width();
        return rect.adjusted(0, diff/2, 0, -diff/2);
    }
}
