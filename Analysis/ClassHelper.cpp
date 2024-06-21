#include <math.h>
#include "ClassHelper.h"
#include "Common/Io/INIFileControls/GenericIniDataControls.h"

ClassHelper::ClassHelper(QObject *parent) : QObject(parent)
{

}

ClassHelper::~ClassHelper()
{

}

double ClassHelper::calcAllergyValue(const double &rawIntensity, const GlobalDataStruct::FORMULA_PARAMETER& parameter)
{
    long double rawData = rawIntensity;

    if(rawData <= 0.00000001)
        return ClassHelper::rounding(0.01, 2);

    const long double ax = parameter.a * pow(rawData, 6.0);
    const long double bx = parameter.b * pow(rawData, 5.0);
    const long double cx = parameter.c * pow(rawData, 4.0);
    const long double dx = parameter.d * pow(rawData, 3.0);
    const long double ex = parameter.e * pow(rawData, 2.0);
    const long double fx = parameter.f * pow(rawData, 1.0);

    long double ly = (ax + bx + cx + dx + ex + fx + parameter.g);

    if(ly <= 0.00000001)
        ly = 0.01;

    return ClassHelper::rounding(ly, 2);
}

double ClassHelper::rounding(const double& x, const int digit)
{
    return (floor((x)* pow(float(10), digit) /*+ 0.5f*/) / pow(float(10), digit));
}

QString ClassHelper::checkClass(const double& value)
{
    if (value > THRESHOLD_IgE_CLASS_6)
        return "6";

    else if (value >= THRESHOLD_IgE_CLASS_5)
        return "5";

    else if (value >= THRESHOLD_IgE_CLASS_4)
        return "4";

    else if (value >= THRESHOLD_IgE_CLASS_3)
        return "3";

    else if (value >= THRESHOLD_IgE_CLASS_2)
        return "2";

    else if (value >= THRESHOLD_IgE_CLASS_1)
        return "1";

    return "0";
}

bool ClassHelper::isValidPCLine(const double& value)
{
    return  value > ClassHelper::getPCCutOff();
}

bool ClassHelper::isValidTIGE(const double& value, const int &cutoff)
{
    return cutoff < value;
}

double ClassHelper::getPCCutOff()
{
    GenericIniDataControls io;
    auto cutOff = io.readPCCutOff();

    if(cutOff == 0)
        cutOff = THRESHOLD_IgE_PC_LINE;

    return cutOff;
}

double ClassHelper::getInITigeCutOff()
{
    GenericIniDataControls io;
    auto cutOff = io.readTIGECutOff();

    if(cutOff == 0)
        cutOff = THRESHOLD_TIGE;

    return cutOff;
}

double ClassHelper::getTigeCutOff()
{
    GenericIniDataControls io;
    auto cutOff = io.readTIGECutOff();

    if(cutOff == 0)
        cutOff = THRESHOLD_TIGE;

    return cutOff;
}

double ClassHelper::getVaildTigeCutOff(double const& DBCutOff)
{
    GenericIniDataControls io;

    bool isCutoffNonZero = (DBCutOff != 0);
    bool isNotDisableDBCutoff = (io.readDisableDatabaseTigeCutoff() == false);

    if(isCutoffNonZero && isNotDisableDBCutoff)
        return DBCutOff;
    else
        return ClassHelper::getInITigeCutOff();
}
