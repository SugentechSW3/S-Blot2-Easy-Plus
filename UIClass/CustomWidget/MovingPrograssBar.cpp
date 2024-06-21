#include <QPropertyAnimation>
#include <QStyleFactory>
#include "MovingPrograssBar.h"

MovingPrograssBar::MovingPrograssBar(QWidget *parent) : QProgressBar(parent)
{
    this->setStyle(QStyleFactory::create("Fusion"));
}

MovingPrograssBar::~MovingPrograssBar()
{

}

void MovingPrograssBar::setMovingValue(int value)
{
    QPropertyAnimation* animation = new QPropertyAnimation( (QProgressBar*)this, "value");
    animation->setEasingCurve( QEasingCurve( QEasingCurve::OutQuad ) );
    animation->setStartValue(this->value());
    animation->setEndValue(value);
    animation->setDuration(1000);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}
