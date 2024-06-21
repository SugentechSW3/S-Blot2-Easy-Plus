//////////////////////////////////////////////////////////////////////////////
// Date of creation: 04.07.2013
// Creator: Alexander Egorov aka mofr
// Modifier : sp.Park 2018.10.30
// Authors: mofr
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QWidget>
#include <QPropertyAnimation>

class ProgressCircle : public QWidget
{
private:
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
    Q_PROPERTY(qreal innerRadius READ innerRadius WRITE setInnerRadius)
    Q_PROPERTY(qreal outerRadius READ outerRadius WRITE setOuterRadius)
    Q_PROPERTY(QColor color READ color WRITE setColor)

    Q_PROPERTY(qreal infiniteAnimationValue READ infiniteAnimationValue WRITE setInfiniteAnimationValue)
    Q_PROPERTY(int visibleValue READ visibleValue WRITE setVisibleValue)

    struct privateStruct;
public:
    explicit ProgressCircle(QWidget *parent = nullptr);
    virtual ~ProgressCircle();

    int value() const;

    /**
     * @brief maximum
     * If maximum <= 0, widget shows infinite process;
     * @return
     */
    int maximum() const;

    /**
     * @brief innerRadius = [0.0 .. 1.0]
     * @return
     */
    qreal innerRadius() const;

    /**
     * @brief outerRadius = [0.0 .. 1.0]
     * @return
     */
    qreal outerRadius() const;

    QColor color() const;


    void animationStart();
    void animationFinished();
    void animationPaused();

    void startInfiniteAnimation();
    void stopInfiniteAnimation();
    void pauseInfiniteAnimation();
    void stopDirectAnimation();

public slots:
    void setValue(int value);
    void setMaximum(int maximum);

    void setInnerRadius(qreal innerRadius);
    void setOuterRadius(qreal outerRadius);

    void setColor(QColor color);
    QRectF squared(QRectF rect);

signals:
    void valueChanged(int);
    void maximumChanged(int);

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void setInfiniteAnimationValue(qreal value);
    void setVisibleValue(int value);

private:
    void initAnimation();
    QString key() const;
    QPixmap generatePixmap();
    qreal infiniteAnimationValue() const;
    int visibleValue() const;

private:
    const int FIXED_SIZE = 16;
    QScopedPointer<privateStruct> d;
};
