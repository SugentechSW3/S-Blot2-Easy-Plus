#ifndef CUSTOMCOLORPROGRASSBAR_H
#define CUSTOMCOLORPROGRASSBAR_H

#include "MovingPrograssBar.h"

class CustomColorPrograssBar : public MovingPrograssBar
{
private:
    Q_OBJECT
public:
    explicit CustomColorPrograssBar(QWidget *parent = nullptr);
    ~CustomColorPrograssBar();

    void setColor(const QColor &color);
};

#endif // CUSTOMCOLORPROGRASSBAR_H
