#ifndef GRAPHSTYLECHARTWIDGET_H
#define GRAPHSTYLECHARTWIDGET_H

#include <QWidget>
#include <QScopedPointer>
#include <QSplineSeries>
#include <opencv2/core/mat.hpp>

class Ui_GraphStyleChartWidget;
class GraphStyleChartWidget : public QWidget
{
private:
    Q_OBJECT

    struct privateStruct;
public:
    explicit GraphStyleChartWidget(QWidget *parent = nullptr);
    virtual ~GraphStyleChartWidget();

    void setXTick(int tick);
    void setYTick(int tick);

    void setXRange(const QVariant& min, const QVariant& max);
    void setYRange(const QVariant& min, const QVariant& max);

    void setTitle(const QString& title);

    void clear();

public slots:
    void drawLine(cv::Mat rawData, int yPoint, int beginWidgetPos = 0);
    void drawLine(const QVector<QPoint>& lines);
    void drawLine(const QVector<QPointF>& lines);

private:
    void initVariables();
    void initSignalSlots();

private:
    QScopedPointer<Ui_GraphStyleChartWidget> ui;
    QScopedPointer<privateStruct> d;
};

#endif // GRAPHSTYLECHARTWIDGET_H
