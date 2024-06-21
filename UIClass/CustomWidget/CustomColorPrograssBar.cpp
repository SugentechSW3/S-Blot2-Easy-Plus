#include <QStyleFactory>
#include "CustomColorPrograssBar.h"

#include "Common/CLogWriter.h"

CustomColorPrograssBar::CustomColorPrograssBar(QWidget *parent) :
                                                                  MovingPrograssBar(parent)
{
  //  this->setStyle(QStyleFactory::create("Fusion"));
}

CustomColorPrograssBar::~CustomColorPrograssBar()
{

}

void CustomColorPrograssBar::setColor(const QColor& color)
{
    this->setStyleSheet(QString("QProgressBar::chunk {background-color: %1;}").arg(color.name()));
}
