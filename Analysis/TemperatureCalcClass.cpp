#include <math.h>
#include "TemperatureCalcClass.h"

struct TemperatureCalcClass::privateStruct
{
    const double RO = 10000.0;
    const double TO = 298.15;
    const double B = 3435.0;
    const double ABS_ZERO = 273.15;
    const double E = 2.7182;
};

TemperatureCalcClass::TemperatureCalcClass(QObject *parent) :
                                                              QObject(parent),
                                                              d(new privateStruct)
{

}

TemperatureCalcClass::~TemperatureCalcClass()
{

}

double TemperatureCalcClass::clacTemperature(double res)
{
    double t = 1.0/((1.0/d->TO) + ((1.0/d->B)* log(res/d->RO)));
    double c = t - d->ABS_ZERO;

    c+= 2;

    return c;
}

double TemperatureCalcClass::clacTemperature(double B,double res)
{
    double t = 1.0/((1.0/d->TO) + ((1.0/B)* log(res/d->RO)));
    double c = t - d->ABS_ZERO;

    c+= 2;

    return c;
}

double TemperatureCalcClass::clacTempFromStripTemp(double stripTemp, double compensationValue)
{
    auto temp = (2.8379 * stripTemp) - compensationValue;
    return temp;
}

double TemperatureCalcClass::adcRawDataCalcTemperature(double B, double celsiusTemperature)
{
    auto res = 0.0;
    auto t = 1/(celsiusTemperature + d->ABS_ZERO);
    auto T = 1/d->TO;
    auto e = (t-T)*B;

    res = d->RO* pow(d->E, e);

    return res;
}

