#ifndef TEMPERATURECALCCLASS_H
#define TEMPERATURECALCCLASS_H

#include <QObject>

class TemperatureCalcClass : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit TemperatureCalcClass(QObject *parent = nullptr);
    ~TemperatureCalcClass();

    double clacTemperature(double res);
    double clacTemperature(double B,double res);
    double clacTempFromStripTemp(double stripTemp, double compensationValue);

    double adcRawDataCalcTemperature(double B, double celsiusTemperature);

private:
    QScopedPointer<privateStruct> d;
};

#endif // TEMPERATURECALCCLASS_H
