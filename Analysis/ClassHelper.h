#ifndef CLASSHELPER_H
#define CLASSHELPER_H

#include <QObject>
#include "Analysis/GlobalDataStruct.h"


const double THRESHOLD_IgE_PC_LINE = 30.0;
const double THRESHOLD_TIGE = 30.0;
const double THRESHOLD_IgE_CLASS_6 = 100.0;
const double THRESHOLD_IgE_CLASS_5 = 50.0;
const double THRESHOLD_IgE_CLASS_4 = 17.50;
const double THRESHOLD_IgE_CLASS_3 = 3.50;
const double THRESHOLD_IgE_CLASS_2 = 0.70;
const double THRESHOLD_IgE_CLASS_1 = 0.350;

class ClassHelper : public QObject
{
private:
    Q_OBJECT

public:
    enum IgE_CLASS
    {
        IgE_CLASS0,
        IgE_CLASS1,
        IgE_CLASS2,
        IgE_CLASS3,
        IgE_CLASS4,
        IgE_CLASS5,
        IgE_CLASS6
    };

    explicit ClassHelper(QObject *parent = nullptr);
    virtual ~ClassHelper();

    static double calcAllergyValue(const double &rawIntensity, const GlobalDataStruct::FORMULA_PARAMETER& parameter);
    static double rounding(const double& x, const int digit);
    static QString checkClass(const double& value);
    static bool isValidPCLine(const double& value);
    static bool isValidTIGE(const double& value, const int &cutoff);
    static double getPCCutOff();
    static double getTigeCutOff();
    static double getVaildTigeCutOff(double const& DBCutOff);
    static double getInITigeCutOff();
};

#endif // CLASSHELPER_H
