/////////////////////////////////////////////////////////////////////////////
// Date of creation: 04.07.2013
// Creator: Alexander Egorov aka mofr
// Modifier : sp.Park 2018.10.30
// Authors: mofr
/////////////////////////////////////////////////////////////////////////////

#include "ProgressCircle.h"
#include <QPainter>
#include <QPixmapCache>

struct ProgressCircle::privateStruct
{
    qreal mInnerRadius;
    qreal mOuterRadius;
    qreal mInfiniteAnimationValue;
    QColor mColor;

    QPropertyAnimation mValueAnimation;
    QPropertyAnimation mInfiniteAnimation;

    int mValue = 0;
    int mMaximum = 0;
    int mVisibleValue = 0;

    bool mIsInfinite = true;
};

ProgressCircle::ProgressCircle(QWidget *parent) :
    QWidget(parent),
    d(new privateStruct)
{
    d->mValueAnimation.setTargetObject(this);
    d->mValueAnimation.setPropertyName("visibleValue");

    d->mInfiniteAnimation.setTargetObject(this);
    d->mInfiniteAnimation.setPropertyName("infiniteAnimationValue");
    d->mInfiniteAnimation.setLoopCount(-1);//infinite
    d->mInfiniteAnimation.setDuration(1000);
    d->mInfiniteAnimation.setStartValue(0.0);
    d->mInfiniteAnimation.setEndValue(1.0);

    this->setValue(0);
}

ProgressCircle::~ProgressCircle()
{

}

int ProgressCircle::value() const
{
    return d->mValue;
}

int ProgressCircle::maximum() const
{
    return d->mMaximum;
}

qreal ProgressCircle::innerRadius() const
{
    return d->mInnerRadius;
}

qreal ProgressCircle::outerRadius() const
{
    return d->mOuterRadius;
}

QColor ProgressCircle::color() const
{
    return d->mColor;
}

void ProgressCircle::initAnimation()
{
    d->mColor = QColor(110, 190, 235);
    d->mInnerRadius = 0.6;
    d->mOuterRadius = 1.0;

    QPropertyAnimation * progressCircleAnimation = new QPropertyAnimation(this, "outerRadius", this);
    progressCircleAnimation->setDuration(1000);
    progressCircleAnimation->setEasingCurve(QEasingCurve::OutQuad);
    progressCircleAnimation->setStartValue(0.0);
    progressCircleAnimation->setEndValue(d->mOuterRadius);
    progressCircleAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    progressCircleAnimation = new QPropertyAnimation(this, "innerRadius", this);
    progressCircleAnimation->setDuration(500);
    progressCircleAnimation->setEasingCurve(QEasingCurve::OutQuad);
    progressCircleAnimation->setEndValue(d->mInnerRadius);
    progressCircleAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    progressCircleAnimation = new QPropertyAnimation(this, "color", this);
    progressCircleAnimation->setDuration(500);
    progressCircleAnimation->setEasingCurve(QEasingCurve::OutQuad);
    progressCircleAnimation->setEndValue(d->mColor);
    progressCircleAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ProgressCircle::animationStart()
{
    this->setValue(0);
    this->initAnimation();

    d->mIsInfinite = true;
    d->mInfiniteAnimation.start();
}

void ProgressCircle::animationFinished()
{
    d->mIsInfinite = false;

    if(d->mInfiniteAnimation.state() != QPropertyAnimation::Stopped)
        d->mInfiniteAnimation.stop();

    QPropertyAnimation * animation = new QPropertyAnimation(this, "outerRadius", this);
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setEndValue(0.5);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    animation = new QPropertyAnimation(this, "innerRadius", this);
    animation->setDuration(250);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setEndValue(0.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    QColor color(155,219,58);
    animation = new QPropertyAnimation(this, "color", this);
    animation->setDuration(250);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setEndValue(color);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ProgressCircle::animationPaused()
{
    QPropertyAnimation * animation = new QPropertyAnimation(this, "outerRadius", this);
    animation->setDuration(1000);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setEndValue(0.5);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    animation = new QPropertyAnimation(this, "innerRadius", this);
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setEndValue(0.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    QColor color(255,100,100);
    animation = new QPropertyAnimation(this, "color", this);
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setEndValue(color);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ProgressCircle::startInfiniteAnimation()
{
    this->animationStart();
}

void ProgressCircle::stopInfiniteAnimation()
{
    this->animationFinished();
}

void ProgressCircle::stopDirectAnimation()
{
    d->mIsInfinite = false;

    if(d->mInfiniteAnimation.state() != QPropertyAnimation::Stopped)
        d->mInfiniteAnimation.stop();

    QPropertyAnimation * animation = new QPropertyAnimation(this, "outerRadius", this);
    animation->setDuration(1);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setEndValue(0.5);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    animation = new QPropertyAnimation(this, "innerRadius", this);
    animation->setDuration(1);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setEndValue(0.0);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    QColor color(155,219,58);
    animation = new QPropertyAnimation(this, "color", this);
    animation->setDuration(1);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->setEndValue(color);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void ProgressCircle::pauseInfiniteAnimation()
{
    d->mInfiniteAnimation.pause();
}

void ProgressCircle::setValue(int value)
{       
    if(d->mIsInfinite == true)
        return;

    if(value < 0)
        value = 0;

    if((qFuzzyCompare(d->mInnerRadius, 0.6) == false) || (qFuzzyCompare(d->mOuterRadius, 1.0) == false))
    {
        this->initAnimation();

        if(this->maximum() == 0)
            this->setMaximum(100);
    }

    if(d->mValue != value)
    {
        d->mValueAnimation.stop();
        d->mValueAnimation.setEndValue(value);
        d->mValueAnimation.setDuration(250);
        d->mValueAnimation.start();

        d->mValue = value;
        emit valueChanged(value);
    }
}

void ProgressCircle::setMaximum(int maximum)
{
    if(maximum < 0)
        maximum = 0;

    if(d->mMaximum != maximum)
    {
        d->mMaximum = maximum;
        this->update();
        emit maximumChanged(maximum);
    }
}

void ProgressCircle::setInnerRadius(qreal innerRadius)
{
    if(innerRadius > 1.0)
        innerRadius = 1.0;

    if(innerRadius < 0.0)
        innerRadius = 0.0;

    if(d->mInnerRadius != innerRadius)
    {
        d->mInnerRadius = innerRadius;
        this->update();
    }
}

void ProgressCircle::setOuterRadius(qreal outerRadius)
{
    if(outerRadius > 1.0)
        outerRadius = 1.0;

    if(outerRadius < 0.0)
        outerRadius = 0.0;

    if(d->mOuterRadius != outerRadius)
    {
        d->mOuterRadius = outerRadius;
        this->update();
    }
}

void ProgressCircle::setColor(QColor color)
{
    if(color != d->mColor)
    {
        d->mColor = color;
        this->update();
    }
}

QRectF ProgressCircle::squared(QRectF rect)
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

void ProgressCircle::paintEvent(QPaintEvent *)
{
    QPixmap pixmap;

    if (!QPixmapCache::find(key(), pixmap))
    {
        pixmap = this->generatePixmap();
        QPixmapCache::insert(this->key(), pixmap);
    }

    QPainter painter(this);
    painter.drawPixmap( 0.5 * ( this->width() - pixmap.width() ), 0.5 * ( this->height() - pixmap.height() ), pixmap );
}

void ProgressCircle::setInfiniteAnimationValue(qreal value)
{
    d->mInfiniteAnimationValue = value;
    this->update();
}

void ProgressCircle::setVisibleValue(int value)
{
    if(d->mVisibleValue != value)
    {
        d->mVisibleValue = value;
        this->update();
    }
}

QString ProgressCircle::key() const
{
    return QString("%1,%2,%3,%4,%5,%6,%7,%8")
            .arg(d->mInfiniteAnimationValue)
            .arg(d->mVisibleValue)
            .arg(d->mMaximum)
            .arg(d->mInnerRadius)
            .arg(d->mOuterRadius)
            .arg(this->width())
            .arg(this->height())
            .arg(d->mColor.rgb());
}

QPixmap ProgressCircle::generatePixmap()
{
    QPixmap pixmap(squared(rect()).size().toSize());
    pixmap.fill(QColor(0,0,0,0));
    QPainter painter(&pixmap);

    painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF rect = pixmap.rect().adjusted(1,1,-1,-1);
    qreal margin = rect.width()*(1.0 - d->mOuterRadius)/2.0;
    rect.adjust(margin,margin,-margin,-margin);
    qreal innerRadius = d->mInnerRadius*rect.width()/2.0;

    painter.setBrush(QColor(225,225,225));
    painter.setPen(QColor(225,225,225));
    painter.drawPie(rect, 0, 360 * FIXED_SIZE);

    painter.setBrush(d->mColor);
    painter.setPen(d->mColor);

    if(d->mIsInfinite == true)
    {
        int startAngle = (-d->mInfiniteAnimationValue) * 360 * FIXED_SIZE;
        int spanAngle = 0.15 * 360 * FIXED_SIZE;
        painter.drawPie(rect, startAngle, spanAngle);
    }
    else
    {
        int value = qMin(d->mVisibleValue, d->mMaximum);
        int startAngle = 90 * FIXED_SIZE;
        int spanAngle = -qreal(value) * 360 * FIXED_SIZE / d->mMaximum;

        painter.drawPie(rect, startAngle, spanAngle);
    }

    painter.setBrush(QColor(255,255,255));
    painter.setPen(QColor(0,0,0, 60));
    painter.drawEllipse(rect.center(), innerRadius, innerRadius);

    painter.drawArc(rect, 0, 360*  FIXED_SIZE);

    return pixmap;
}

qreal ProgressCircle::infiniteAnimationValue() const
{
    return d->mInfiniteAnimationValue;
}

int ProgressCircle::visibleValue() const
{
    return d->mVisibleValue;
}
