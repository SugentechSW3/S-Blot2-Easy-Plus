#ifndef PATIENTLISTWIDGETSTEP2_H
#define PATIENTLISTWIDGETSTEP2_H

#include <Common/CommonParentWidget/CSerialNetworkWidget.h>
#include "Analysis/GlobalDataStruct.h"

class Ui_PatientListWidgetStep2;
class PatientListWidgetStep2 : public QWidget
{
private:
    Q_OBJECT

    enum PATIENT_TABLE_COLUMN
    {
        PATIENT_TABLE_COLUMN_ID,
        PATIENT_TABLE_COLUMN_PATIENT_NAME,
        PATIENT_TABLE_COLUMN_PANEL
    };

public:
    explicit PatientListWidgetStep2(QWidget *parent = nullptr);
    virtual ~PatientListWidgetStep2();

    void setData(const QVector<GlobalDataStruct::TABLE_ITEM_ELEMENT>& data);

private:
    void initTableItem();
    void initTableWidgetItems();

private:
    QScopedPointer<Ui_PatientListWidgetStep2>ui;
};

#endif // PATIENTLISTWIDGETSTEP2_H
