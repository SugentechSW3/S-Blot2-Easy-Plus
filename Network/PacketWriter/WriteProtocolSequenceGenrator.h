#ifndef WRITEPROTOCOLSEQUENCEGENRATOR_H
#define WRITEPROTOCOLSEQUENCEGENRATOR_H

#include <QString>
#include "Network/Protocols/Sequence/ProtocolSequenceGeneratorSetting.h"


class WriteProtocolSequenceGenrator
{
public:
    WriteProtocolSequenceGenrator();

//#if !USE_SCAN
    static QString writeSQInputMode(uchar pMode);
    static QString writeSQTotalStep(uchar totalStep);
    static QString writeSQSave(uchar smb);
    static QString writeSQID(uchar smb, ushort sqID);
    static QString writeSQStepNumber(uchar stepNumber);
    static QString writeSQProcessNumber(ushort pfID, ushort processNumber);
    static QString writeSQProcessParameterByte(uchar d1, uchar d2, uchar d3, uchar d4);
    static QString writeSQProcessParameterWord(ushort d1, ushort d2);
    static QString writeSQProcessParameterDWord(uint d1);

    static QString writeSQProcessParameterDWordPrime1(uchar pumpChannel, ushort time);
    static QString writeSQProcessParameterDWordPrime2(uchar pumpChannel1, uchar pumpChannel2, ushort time);
    static QString writeSQProcessParameterDWordPrime3(uchar pumpChannel1, uchar pumpChannel2, uchar pumpChannel3, ushort time);

    static QString writeSQProcessParameterDWordDispense1(uchar pumpChannel, ushort volume);
    static QString writeSQProcessParameterDWordDispense2(uchar pumpChannel1, uchar pumpChannel2, ushort volume);

    static QString writeSQProcessParameterDWordDispAsp1(uchar pumpChannel, ushort volume);
    static QString writeSQProcessParameterDWordDispAsp2(uchar pumpChannel1, uchar pumpChannel2, ushort volume);
    static QString writeSQProcessParameterDWordDispAsp3(uchar pumpChannel1, uchar pumpChannel2, uchar pumpChannel3, ushort volume);

    static QString writeSQProcessParameterDWordIncubation(ushort time);
    static QString writeSQProcessParameterDWordIncubationWS(ushort time);
    static QString writeSQProcessParameterDWordAspiration(ushort time, uchar count);

    static QString writeSQProcessParameterDWordDry(ushort speed, uchar temp, uchar time);
    static QString writeSQProcessParameterDWordBuzzer(uchar buzzerCount);


    static QString writeSQProcessParameterDWordRollBack1(uchar pumpChannel, ushort time);

    static QString writeSQProcessParameterDwordDPrime(uchar channel, uchar option, ushort volumn);
    static QString writeSQProcessParameterDwordDRollBack(uchar channel, uchar option, ushort duration);
    static QString writeSQProcessParameterDwordDDispense(uchar channel, uchar option, ushort volumn);

    static QString writeQCPumpDispTest(uchar pump, uchar sampleNumber, ushort dispVol);
    static QString writeQCPumpAspTest(uchar sampleNumber, uchar time);
    static QString writeQCPrimeTest(uchar pumpChannel, uchar time);
    static QString writeQCRollBack(uchar pumpChannel, uchar time);
    static QString writeQCShakeTest(uchar speed, ushort time);
    static QString writeQCDryTest(ushort speed, uchar time);
    static QString writeQCPumpTest(uchar pumpCh);
    static QString writeQCBathDispCheck();
    static QString writeQCBathAspCheck();
    static QString writeQCFanTest();
    static QString writeQCStatusLed(bool state);
    static QString writeSQProcessNULLData();

//#endif
};

#endif // WRITEPROTOCOLSEQUENCEGENRATOR_H
