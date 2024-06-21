#ifndef MOVINGPROGRASSBAR_H
#define MOVINGPROGRASSBAR_H

#include <QProgressBar>

class MovingPrograssBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit MovingPrograssBar(QWidget *parent = nullptr);
    ~MovingPrograssBar();

    void setMovingValue(int value);
};

#endif // MOVINGPROGRASSBAR_H
