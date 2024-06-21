#-------------------------------------------------
#
# Project created by QtCreator 2018-01-31T09:27:01
#
#-------------------------------------------------

QT       += core gui network sql

win32: QT += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts serialport multimedia printsupport xml

DEFINES += USE_QT

DEFINES += USE_SQL_LITE=0
DEFINES += USE_SQL_ACCESS=1

DEFINES += LIB_SIMPLE_ENCRYPTO=0
DEFINES += LIB_CRYPTO=0

DEFINES += USE_SORT_INTENSITY=0
DEFINES += USE_AREA_INTENSITY=1

DEFINES += ENABLE_SERIAL_MODE=1

#DEFINES += IS_DEBUG_PC

TARGET = S-Blot" "2" "Easy" "PLUS



win32 {
    RC_FILE = qt.rc
}


TEMPLATE = app

#include(../QXlsx/QXlsx.pri)
include(../qtsingleapplication/qtsingleapplication.pri)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS += lang_en.ts \
                lang_ko.ts \
                lang_fr.ts \
                lang_ar.ts \
                lang_ro.ts \
                lang_ru.ts \
                lang_de.ts \
                lang_bd.ts


SOURCES += \
    Analysis/Bases/DataClassifierBase.cpp \
    Analysis/Bases/TigEBandAnalyzerBase.cpp \
    Analysis/ConfigInformation.cpp \
    Analysis/ErrorResonIO.cpp \
    Analysis/TemperatureCalcClass.cpp \
    Common/Barcode/CQRCode.cpp \
    Common/CSHA256.cpp \
    Common/CommonParentWidget/CCheckBoxParentWidget.cpp \
    Common/CommonParentWidget/CDialogParentWidget.cpp \
    Common/Io/ExcelExportClass.cpp \
    Common/Io/INIFileControls/AllergyIniDataControls.cpp \
    Common/Io/INIFileControls/BatchUpAndDownLoadForTunningData.cpp \
    Common/Io/CImageToExcelData.cpp \
    Common/Io/CPrintSupport.cpp \
    Common/Io/INIFileControls/CameraGenericIniDataControls.cpp \
    Common/Io/INIFileControls/CameraIniDataControls.cpp \
    Common/Io/INIFileControls/FoodIntoleranceIniDataControls.cpp \
    Common/Io/INIFileControls/GenericIniDataControls.cpp \
    Common/Io/INIFileControls/TunningIniDataControls.cpp \
    Common/Io/INIFileControls/UserLoginDataControls.cpp \
    Common/Io/JsonFIleControls/ReportJsonDataControls.cpp \
    Common/Network/HL7InterFaceAdapter.cpp \
    Common/Network/HL7InterFaceReader.cpp \
    Common/Network/HL7InterFaceWriter.cpp \
    DataBase/BasicDatabaseCtrl.cpp \
    DataBase/CodeFuncDatabaseCtrl.cpp \
    Network/PacketWriter/TunnningDataProtocolSendHelper.cpp \
    Network/PacketWriter/WriteProtocolBarcodePage.cpp \
    Network/PacketWriter/WriteProtocolCameraLedSetting.cpp \
    Network/PacketWriter/WriteProtocolControlRead.cpp \
    Network/PacketWriter/WriteProtocolControlWrite.cpp \
    Network/PacketWriter/WriteProtocolDIWaterPage.cpp \
    Network/PacketWriter/WriteProtocolDispAspPage.cpp \
    Network/PacketWriter/WriteProtocolETCPage.cpp \
    Network/PacketWriter/WriteProtocolGeneralSetting.cpp \
    Network/PacketWriter/WriteProtocolPageRead.cpp \
    Network/PacketWriter/WriteProtocolProbePage.cpp \
    Network/PacketWriter/WriteProtocolSequenceGenrator.cpp \
    Network/PacketWriter/WriteProtocolStatusRead.cpp \
    Network/PacketWriter/WriteProtocolTraySetting.cpp \
    Report/LagacyReportData.cpp \
    Report/ReportData.cpp \
    UIClass/AdminButtonMenu.cpp \
    UIClass/CameraWidget/SubWidget/DualBandWidget.cpp \
    UIClass/CameraWidget/SubWidget/QRPositionSettingWidget.cpp \
    UIClass/CameraWidget/SubWidget/RelativeCheckBoxWidget.cpp \
    UIClass/CameraWidget/SubWidget/RotationWidget.cpp \
    UIClass/CustomWidget/CustomColorPrograssBar.cpp \
    UIClass/CustomWidget/CustomComboBox.cpp \
    UIClass/CustomWidget/CustomDelayButtonWidget.cpp \
    UIClass/CustomWidget/CustomImageButtonWidget.cpp \
    UIClass/CustomWidget/CustomImageLabelSet.cpp \
    UIClass/CustomWidget/CustomPrograssImageCircleWidget.cpp \
    UIClass/CustomWidget/CustomRadioImageWidget.cpp \
    UIClass/CustomWidget/CustomStayClickEvnetWidget.cpp \
    UIClass/CustomWidget/ImageCircleWidget.cpp \
    UIClass/CustomWidget/MovingPrograssBar.cpp \
    UIClass/CustomWidget/QRoundProgressBar.cpp \
    UIClass/CustomWidget/RotationLabel.cpp \
    UIClass/CustomWidget/RunningStatusListItem.cpp \
    UIClass/CustomWidget/RunningStatusListWidget.cpp \
    UIClass/CustomWidget/TitleStatusBarWidget.cpp \
    UIClass/CustomWidget/sIgERangeSettingWidget.cpp \
    UIClass/DialogWidget/AboutAndOpenSourceDlg.cpp \
    UIClass/DialogWidget/AdminSerialDialog.cpp \
    UIClass/DialogWidget/AspControlDlg.cpp \
    UIClass/DialogWidget/BatchUpAndDownLoadTunningDataDlg.cpp \
    UIClass/DialogWidget/CheckListDlg.cpp \
    UIClass/DialogWidget/CirclePrograssDialog.cpp \
    UIClass/DialogWidget/ContetnsSelectDlg.cpp \
    UIClass/DialogWidget/HiddenBandCheckDlg.cpp \
    UIClass/DialogWidget/InitDataDlg.cpp \
    UIClass/DialogWidget/InspectionOpinionDlg.cpp \
    UIClass/DialogWidget/InterfaceServerIPSettingDlg.cpp \
    UIClass/DialogWidget/LanguageSelectDlg.cpp \
    UIClass/DialogWidget/ListToHiddenBandDlg.cpp \
    UIClass/DialogWidget/LoginDialog.cpp \
    UIClass/DialogWidget/MessageShowDlg.cpp \
    UIClass/DialogWidget/PasswordChangeDig.cpp \
    UIClass/DialogWidget/ProgressBarDlg.cpp \
    UIClass/DialogWidget/QRStripCheckDlg.cpp \
    UIClass/DialogWidget/QuestionDlg.cpp \
    UIClass/DialogWidget/RemoveDlg.cpp \
    UIClass/DialogWidget/ResultExportDlg.cpp \
    UIClass/DialogWidget/ResultPrintDlg.cpp \
    UIClass/DialogWidget/ResultPrintSelectLangDlg.cpp \
    UIClass/DialogWidget/SearchDialog.cpp \
    UIClass/DialogWidget/StatusDialog.cpp \
    UIClass/DialogWidget/UserConfigDlg.cpp \
    UIClass/DialogWidget/UserSequenceDialog.cpp \
    UIClass/ETCView/ETCView.cpp \
    UIClass/ResultView/SubWidget/OpinionSelectWidget.cpp \
    UIClass/ResultView/SubWidget/ResultPrintWidget.cpp \
    UIClass/ResultView/SubWidget/ResultPrintWidgetClassTableItem.cpp \
    UIClass/ResultView/SubWidget/ResultPrintV5WidgetLabelTableItem.cpp \
    UIClass/ResultView/SubWidget/ResultPrintV5Widget.cpp \
    UIClass/SemiAutoCalibration/SemiAutoCalibrationWidget.cpp \
    UIClass/SequenceWidget/SubWidget/DDispenseWidget.cpp \
    UIClass/SequenceWidget/SubWidget/DPrimeWidget.cpp \
    UIClass/SequenceWidget/SubWidget/DRollBackWidget.cpp \
    UIClass/UserButtonMenu.cpp \
    UIClass/WorkPlace/PatientListWidgetStep2.cpp \
    UIClass/WorkPlace/ReagentUserWidget.cpp \
    UIClass/WorkPlace/RunningStatusWidget.cpp \
    UIClass/WorkPlace/SubWidget/ReagentPlaceHolderWidget.cpp \
    UIClass/WorkPlace/SubWidget/ReagentStripWidget.cpp \
        main.cpp \
        mainwindow.cpp \
    Common/DataBase/CDataBase.cpp \
    Common/Io/CFileio.cpp \
    Common/Network/CClientSocket.cpp \
    Common/Network/CTcpClient.cpp \
    Common/Network/CTcpServer.cpp \
    Common/CLogWriter.cpp \
    Common/CUtil.cpp \
    Common/Network/CSerialNetwork.cpp \
    Common/Io/CSettingIo.cpp \
    Common/Network/CPacketTimer.cpp \
    CameraControl/CameraCtrl.cpp \
    CameraControl/CameraInstance.cpp \
    Network/SerialCtrl.cpp \
    Analysis/BasicSupremeIAnalysis.cpp \
    UIClass/MainView.cpp \
    UIClass/CustomWidget/TableExcelStyleWidget.cpp \
    UIClass/CustomWidget/RadioGroupBoxWidget.cpp \
    UIClass/CustomWidget/GraphStyleChartWidget.cpp \
    UIClass/CameraWidget/CameraView.cpp \
    UIClass/CameraWidget/CameraWidget.cpp \
    UIClass/CameraWidget/CameraStripSettings.cpp \
    UIClass/CameraWidget/SubWidget/MoveControlWidget.cpp \
    UIClass/CameraWidget/SubWidget/HousingROIWidget.cpp \
    UIClass/CameraWidget/SubWidget/ParticleRemovalSettingWidget.cpp \
    UIClass/CameraWidget/SubWidget/CommandMenuWidget.cpp \
    UIClass/CameraWidget/SubWidget/IntensityWidget.cpp \
    Network/PacketReader/PacketReader.cpp \
    Common/CommonParentWidget/CSerialNetworkWidget.cpp \
    UIClass/CustomWidget/SpinSliderWidget.cpp \
    Common/CommonParentWidget/CConfig.cpp \
    UIClass/CameraWidget/CameraOptionSettings.cpp \
    UIClass/CameraWidget/CameraSettings.cpp \
    UIClass/CameraWidget/SubWidget/LEDSetting.cpp \
    UIClass/CameraWidget/SubWidget/CameraPositionSetting.cpp \
    UIClass/CameraWidget/SubWidget/CameraLightSetting.cpp \
    CameraControl/CameraConfigWatcher.cpp \
    UIClass/CameraWidget/SubWidget/ImageLabel.cpp \
    UIClass/CameraWidget/SubWidget/BoxCarWidget.cpp \
    UIClass/CameraWidget/SubWidget/PCLineWidget.cpp \
    UIClass/SequenceWidget/SequenceView.cpp \
    UIClass/SequenceWidget/SubWidget/PrimeWidget.cpp \
    UIClass/SequenceWidget/SubWidget/DispenseWidget.cpp \
    UIClass/SequenceWidget/SubWidget/AspirationWidget.cpp \
    UIClass/SequenceWidget/SubWidget/IncubationWidget.cpp \
    UIClass/SequenceWidget/SubWidget/DryWidget.cpp \
    UIClass/SequenceWidget/SubWidget/CommonSequenceWidget.cpp \
    UIClass/CameraWidget/SubWidget/GuideLineWidget.cpp \
    UIClass/CameraWidget/SubWidget/BandGapWidget.cpp \
    UIClass/CameraWidget/SubWidget/RelativePosWidget.cpp \
    UIClass/SequenceWidget/SubWidget/RollBackWidget.cpp \
    UIClass/WorkPlace/WorkPlaceView.cpp \
    UIClass/ResultView/ResultView.cpp \
    UIClass/CustomWidget/NavigatorArrowWidget.cpp \
    UIClass/WorkPlace/PatientListWidget.cpp \
    UIClass/CustomWidget/CirclePrograssBarWithListWidget.cpp \
    UIClass/SequenceWidget/SubWidget/DispAsp1Widget.cpp \
    UIClass/SequenceWidget/SubWidget/DispAsp2Widget.cpp \
    UIClass/CustomWidget/CheckBoxTableWidgetItem.cpp \
    UIClass/CustomWidget/PlaceHolderTableWidgetItem.cpp \
    UIClass/CustomWidget/ComboBoxTableWidgetItem.cpp \
    UIClass/CustomWidget/CustomTableWidgetItem.cpp \
    UIClass/CustomWidget/SpinTableWidgetItem.cpp \
    UIClass/CustomWidget/SpinRemoveUpDown.cpp \
    UIClass/CustomWidget/RealTimeDateTimeWidget.cpp \
    UIClass/SequenceWidget/SubWidget/Prime2Widget.cpp \
    UIClass/SequenceWidget/SubWidget/Prime3Widget.cpp \
    UIClass/SequenceWidget/SubWidget/Dispense2Widget.cpp \
    UIClass/SequenceWidget/SubWidget/DispAsp3Widget.cpp \
    UIClass/SequenceWidget/SubWidget/RollBack2Widget.cpp \
    UIClass/SequenceWidget/SubWidget/RollBack3Widget.cpp \
    UIClass/CustomWidget/ProgressCircle.cpp \
    UIClass/CustomWidget/CircleListWidgetItem.cpp \
    UIClass/SequenceWidget/SubWidget/IncubationWSWidget.cpp \
    Analysis/DeepNeuralNetwork.cpp \
    Common/Io/CZipIo.cpp \
    Analysis/ClassHelper.cpp \
    DataBase/DataBaseCtrl.cpp \
    UIClass/CameraWidget/SubWidget/FlipWidget.cpp \
    Common/Network/U2BioXmlLisProtocol.cpp

HEADERS += \
    Analysis/Bases/DataClassifierBase.h \
    Analysis/Bases/TigEBandAnalyzerBase.h \
    Analysis/ConfigInformation.h \
    Analysis/ErrorResonIO.h \
    Analysis/GlobalDataStruct.h \
    Analysis/Interfaces/IDataClassifier.h \
    Analysis/Interfaces/ITigEBandAnalyzer.h \
    Analysis/TemperatureCalcClass.h \
    Common/Barcode/CQRCode.h \
    Common/CSHA256.h \
    Common/CommonParentWidget/CCheckBoxParentWidget.h \
    Common/CommonParentWidget/CDialogParentWidget.h \
    Common/Io/ExcelExportClass.h \
    Common/Io/INIFileControls/AllergyIniDataControls.h \
    Common/Io/INIFileControls/BatchUpAndDownLoadForTunningData.h \
    Common/Io/CImageToExcelData.h \
    Common/Io/CPrintSupport.h \
    Common/Io/INIFileControls/CameraGenericIniDataControls.h \
    Common/Io/INIFileControls/CameraIniDataControls.h \
    Common/Io/INIFileControls/FoodIntoleranceIniDataControls.h \
    Common/Io/INIFileControls/GenericIniDataControls.h \
    Common/Io/INIFileControls/TunningIniDataControls.h \
    Common/Io/INIFileControls/UserLoginDataControls.h \
    Common/Io/JsonFIleControls/ReportJsonDataControls.h \
    Common/Network/HL7InterFaceAdapter.h \
    Common/Network/HL7InterFaceHeader.h \
    Common/Network/HL7InterFaceReader.h \
    Common/Network/HL7InterFaceWriter.h \
    DataBase/BasicDatabaseCtrl.h \
    DataBase/CodeFuncDatabaseCtrl.h \
    Network/PacketWriter/TunnningDataProtocolSendHelper.h \
    Network/PacketWriter/WriteProtocolBarcodePage.h \
    Network/PacketWriter/WriteProtocolCameraLedSetting.h \
    Network/PacketWriter/WriteProtocolControlRead.h \
    Network/PacketWriter/WriteProtocolControlWrite.h \
    Network/PacketWriter/WriteProtocolDIWaterPage.h \
    Network/PacketWriter/WriteProtocolDispAspPage.h \
    Network/PacketWriter/WriteProtocolETCPage.h \
    Network/PacketWriter/WriteProtocolGeneralSetting.h \
    Network/PacketWriter/WriteProtocolPageRead.h \
    Network/PacketWriter/WriteProtocolProbePage.h \
    Network/PacketWriter/WriteProtocolSequenceGenrator.h \
    Network/PacketWriter/WriteProtocolStatusRead.h \
    Network/PacketWriter/WriteProtocolTraySetting.h \
    Network/Protocols/CameraLED/ProtocolCameraLedSetting.h \
    Network/Protocols/Default/ProtocolControlRead.h \
    Network/Protocols/Default/ProtocolControlWrite.h \
    Network/Protocols/Default/ProtocolGeneralSettingMode.h \
    Network/Protocols/Default/ProtocolPageRead.h \
    Network/Protocols/Default/ProtocolStatusRead.h \
    Network/Protocols/Dispense/ProtocolDispenseSample.h \
    Network/Protocols/Dispense/ProtocolWashingSolution.h \
    Network/Protocols/ETC/ProtocolETC.h \
    Network/Protocols/Legacy/ProtocolCameraParameterMode.h \
    Network/Protocols/Legacy/ProtocolMotorSettingMode.h \
    Network/Protocols/Legacy/ProtocolPumpSettingMode.h \
    Network/Protocols/Legacy/ProtocolQCMode.h \
    Network/Protocols/Position/ProtocolBarcodeSettingMode.h \
    Network/Protocols/Position/ProtocolDispAspSettingMode.h \
    Network/Protocols/Position/ProtocolProbeSettingMode.h \
    Network/Protocols/Position/ProtocolTraySettingMode.h \
    Network/Protocols/Sequence/ProtocolSequenceGeneratorSetting.h \
    Network/Protocols/TuningData/ProtocolTuningDataSettingGlobal.h \
    Report/LagacyReportData.h \
    Report/ReportData.h \
    UIClass/AdminButtonMenu.h \
    UIClass/CameraWidget/SubWidget/DualBandWidget.h \
    UIClass/CameraWidget/SubWidget/QRPositionSettingWidget.h \
    UIClass/CameraWidget/SubWidget/RelativeCheckBoxWidget.h \
    UIClass/CameraWidget/SubWidget/RotationWidget.h \
    UIClass/CustomWidget/CustomColorPrograssBar.h \
    UIClass/CustomWidget/CustomComboBox.h \
    UIClass/CustomWidget/CustomDelayButtonWidget.h \
    UIClass/CustomWidget/CustomImageButtonWidget.h \
    UIClass/CustomWidget/CustomImageLabelSet.h \
    UIClass/CustomWidget/CustomPrograssImageCircleWidget.h \
    UIClass/CustomWidget/CustomRadioImageWidget.h \
    UIClass/CustomWidget/CustomStayClickEvnetWidget.h \
    UIClass/CustomWidget/ImageCircleWidget.h \
    UIClass/CustomWidget/MovingPrograssBar.h \
    UIClass/CustomWidget/QRoundProgressBar.h \
    UIClass/CustomWidget/RotationLabel.h \
    UIClass/CustomWidget/RunningStatusListItem.h \
    UIClass/CustomWidget/RunningStatusListWidget.h \
    UIClass/CustomWidget/TitleStatusBarWidget.h \
    UIClass/CustomWidget/sIgERangeSettingWidget.h \
    UIClass/DialogWidget/AboutAndOpenSourceDlg.h \
    UIClass/DialogWidget/AdminSerialDialog.h \
    UIClass/DialogWidget/AspControlDlg.h \
    UIClass/DialogWidget/BatchUpAndDownLoadTunningDataDlg.h \
    UIClass/DialogWidget/CheckListDlg.h \
    UIClass/DialogWidget/CirclePrograssDialog.h \
    UIClass/DialogWidget/ContetnsSelectDlg.h \
    UIClass/DialogWidget/HiddenBandCheckDlg.h \
    UIClass/DialogWidget/InitDataDlg.h \
    UIClass/DialogWidget/InspectionOpinionDlg.h \
    UIClass/DialogWidget/InterfaceServerIPSettingDlg.h \
    UIClass/DialogWidget/LanguageSelectDlg.h \
    UIClass/DialogWidget/ListToHiddenBandDlg.h \
    UIClass/DialogWidget/LoginDialog.h \
    UIClass/DialogWidget/MessageShowDlg.h \
    UIClass/DialogWidget/PasswordChangeDig.h \
    UIClass/DialogWidget/ProgressBarDlg.h \
    UIClass/DialogWidget/QRStripCheckDlg.h \
    UIClass/DialogWidget/QuestionDlg.h \
    UIClass/DialogWidget/RemoveDlg.h \
    UIClass/DialogWidget/ResultExportDlg.h \
    UIClass/DialogWidget/ResultPrintDlg.h \
    UIClass/DialogWidget/ResultPrintSelectLangDlg.h \
    UIClass/DialogWidget/SearchDialog.h \
    UIClass/DialogWidget/StatusDialog.h \
    UIClass/DialogWidget/UserConfigDlg.h \
    UIClass/DialogWidget/UserSequenceDialog.h \
    UIClass/ETCView/ETCView.h \
    UIClass/ResultView/SubWidget/OpinionSelectWidget.h \
    UIClass/ResultView/SubWidget/ResultPrintWidget.h \
    UIClass/ResultView/SubWidget/ResultPrintWidgetClassTableItem.h \
    UIClass/ResultView/SubWidget/ResultPrintV5WidgetLabelTableItem.h \
    UIClass/ResultView/SubWidget/ResultPrintV5Widget.h \
    UIClass/SemiAutoCalibration/SemiAutoCalibrationWidget.h \
    UIClass/SequenceWidget/SubWidget/DDispenseWidget.h \
    UIClass/SequenceWidget/SubWidget/DPrimeWidget.h \
    UIClass/SequenceWidget/SubWidget/DRollBackWidget.h \
    UIClass/UserButtonMenu.h \
    UIClass/WorkPlace/PatientListWidgetStep2.h \
    UIClass/WorkPlace/ReagentUserWidget.h \
    UIClass/WorkPlace/RunningStatusWidget.h \
    UIClass/WorkPlace/SubWidget/ReagentPlaceHolderWidget.h \
    UIClass/WorkPlace/SubWidget/ReagentStripWidget.h \
        mainwindow.h \
    Common/DataBase/CDataBase.h \
    Common/Io/CFileio.h \
    Common/Network/CClientSocket.h \
    Common/Network/CTcpClient.h \
    Common/Network/CTcpServer.h \
    Common/CLogWriter.h \
    Common/CUtil.h \
    Common/CUtilCommon.h \
    Common/Network/CSerialNetwork.h \
    Common/Io/CSettingIo.h \
    Common/Network/CPacketTimer.h \
    CameraControl/CameraCtrl.h \
    CameraControl/CameraInstance.h \
    Network/SerialCtrl.h \
    Network/Protocols/SerialProtocol.h \
    Analysis/BasicSupremeIAnalysis.h \
    UIClass/MainView.h \
    UIClass/CustomWidget/TableExcelStyleWidget.h \
    UIClass/CustomWidget/RadioGroupBoxWidget.h \
    UIClass/CustomWidget/GraphStyleChartWidget.h \
    UIClass/CameraWidget/CameraView.h \
    UIClass/CameraWidget/CameraWidget.h \
    UIClass/CameraWidget/CameraStripSettings.h \
    UIClass/CameraWidget/SubWidget/MoveControlWidget.h \
    UIClass/CameraWidget/SubWidget/HousingROIWidget.h \
    UIClass/CameraWidget/SubWidget/ParticleRemovalSettingWidget.h \
    UIClass/CameraWidget/SubWidget/CommandMenuWidget.h \
    UIClass/CameraWidget/SubWidget/IntensityWidget.h \
    Network/PacketReader/PacketReader.h \
    Common/CommonParentWidget/CSerialNetworkWidget.h \
    UIClass/CustomWidget/SpinSliderWidget.h \
    Common/CommonParentWidget/CConfig.h \
    UIClass/CameraWidget/CameraOptionSettings.h \
    UIClass/CameraWidget/CameraSettings.h \
    UIClass/CameraWidget/SubWidget/LEDSetting.h \
    UIClass/CameraWidget/SubWidget/CameraPositionSetting.h \
    UIClass/CameraWidget/SubWidget/CameraLightSetting.h \
    CameraControl/CameraConfigWatcher.h \
    UIClass/CameraWidget/SubWidget/ImageLabel.h \
    UIClass/CameraWidget/SubWidget/BoxCarWidget.h \
    UIClass/CameraWidget/SubWidget/PCLineWidget.h \
    UIClass/SequenceWidget/SequenceView.h \
    UIClass/SequenceWidget/SubWidget/PrimeWidget.h \
    UIClass/SequenceWidget/SubWidget/DispenseWidget.h \
    UIClass/SequenceWidget/SubWidget/AspirationWidget.h \
    UIClass/SequenceWidget/SubWidget/IncubationWidget.h \
    UIClass/SequenceWidget/SubWidget/DryWidget.h \
    UIClass/SequenceWidget/SubWidget/CommonSequenceWidget.h \
    UIClass/CameraWidget/SubWidget/GuideLineWidget.h \
    UIClass/CameraWidget/SubWidget/BandGapWidget.h \
    UIClass/CameraWidget/SubWidget/RelativePosWidget.h \
    UIClass/SequenceWidget/SubWidget/RollBackWidget.h \
    UIClass/WorkPlace/WorkPlaceView.h \
    UIClass/ResultView/ResultView.h \
    UIClass/CustomWidget/NavigatorArrowWidget.h \
    UIClass/WorkPlace/PatientListWidget.h \
    UIClass/CustomWidget/CirclePrograssBarWithListWidget.h \
    UIClass/SequenceWidget/SubWidget/DispAsp1Widget.h \
    UIClass/SequenceWidget/SubWidget/DispAsp2Widget.h \
    UIClass/CustomWidget/CheckBoxTableWidgetItem.h \
    UIClass/CustomWidget/PlaceHolderTableWidgetItem.h \
    UIClass/CustomWidget/ComboBoxTableWidgetItem.h \
    UIClass/CustomWidget/CustomTableWidgetItem.h \
    UIClass/CustomWidget/SpinTableWidgetItem.h \
    UIClass/CustomWidget/SpinRemoveUpDown.h \
    UIClass/CustomWidget/RealTimeDateTimeWidget.h \
    UIClass/SequenceWidget/SubWidget/Prime2Widget.h \
    UIClass/SequenceWidget/SubWidget/Prime3Widget.h \
    UIClass/SequenceWidget/SubWidget/Dispense2Widget.h \
    UIClass/SequenceWidget/SubWidget/DispAsp3Widget.h \
    UIClass/SequenceWidget/SubWidget/RollBack2Widget.h \
    UIClass/SequenceWidget/SubWidget/RollBack3Widget.h \
    UIClass/CustomWidget/ProgressCircle.h \
    UIClass/CustomWidget/CircleListWidgetItem.h \
    UIClass/SequenceWidget/SubWidget/IncubationWSWidget.h \
    Analysis/DeepNeuralNetwork.h \
    Common/Io/CZipIo.h \
    DataBase/DataBaseCtrl.h \
    Analysis/ClassHelper.h \
    UIClass/CameraWidget/SubWidget/FlipWidget.h \
    Common/Network/U2BioXmlLisProtocol.h



FORMS += \
    UIClass/AdminButtonMenu.ui \
    UIClass/CameraWidget/SubWidget/DualBandWidget.ui \
    UIClass/CameraWidget/SubWidget/QRPositionSettingWidget.ui \
    UIClass/CameraWidget/SubWidget/RelativeCheckBoxWidget.ui \
    UIClass/CameraWidget/SubWidget/RotationWidget.ui \
    UIClass/CustomWidget/CustomImageLabelSet.ui \
    UIClass/CustomWidget/CustomPrograssImageCircleWidget.ui \
    UIClass/CustomWidget/RunningStatusListItem.ui \
    UIClass/CustomWidget/RunningStatusListWidget.ui \
    UIClass/CustomWidget/TitleStatusBarWidget.ui \
    UIClass/CustomWidget/sIgERangeSettingWidget.ui \
    UIClass/DialogWidget/AboutAndOpenSourceDlg.ui \
    UIClass/DialogWidget/AdminSerialDialog.ui \
    UIClass/DialogWidget/AspControlDlg.ui \
    UIClass/DialogWidget/BatchUpAndDownLoadTunningDataDlg.ui \
    UIClass/DialogWidget/CheckListDlg.ui \
    UIClass/DialogWidget/CirclePrograssDialog.ui \
    UIClass/DialogWidget/ContetnsSelectDlg.ui \
    UIClass/DialogWidget/HiddenBandCheckDlg.ui \
    UIClass/DialogWidget/InitDataDlg.ui \
    UIClass/DialogWidget/InspectionOpinionDlg.ui \
    UIClass/DialogWidget/InterfaceServerIPSettingDlg.ui \
    UIClass/DialogWidget/LanguageSelectDlg.ui \
    UIClass/DialogWidget/ListToHiddenBandDlg.ui \
    UIClass/DialogWidget/LoginDialog.ui \
    UIClass/DialogWidget/MessageShowDlg.ui \
    UIClass/DialogWidget/PasswordChangeDig.ui \
    UIClass/DialogWidget/ProgressBarDlg.ui \
    UIClass/DialogWidget/QRStripCheckDlg.ui \
    UIClass/DialogWidget/QuestionDlg.ui \
    UIClass/DialogWidget/RemoveDlg.ui \
    UIClass/DialogWidget/ResultExportDlg.ui \
    UIClass/DialogWidget/ResultPrintDlg.ui \
    UIClass/DialogWidget/ResultPrintSelectLangDlg.ui \
    UIClass/DialogWidget/SearchDialog.ui \
    UIClass/DialogWidget/StatusDialog.ui \
    UIClass/DialogWidget/UserConfigDlg.ui \
    UIClass/DialogWidget/UserSequenceDialog.ui \
    UIClass/ETCView/ETCView.ui \
    UIClass/ResultView/SubWidget/OpinionSelectWidget.ui \
    UIClass/ResultView/SubWidget/ResultPrintWidget.ui \
    UIClass/ResultView/SubWidget/ResultPrintWidgetClassTableItem.ui \
    UIClass/ResultView/SubWidget/ResultPrintV5Widget.ui \
    UIClass/SemiAutoCalibration/SemiAutoCalibrationWidget.ui \
    UIClass/SequenceWidget/SubWidget/DDispenseWidget.ui \
    UIClass/SequenceWidget/SubWidget/DPrimeWidget.ui \
    UIClass/SequenceWidget/SubWidget/DRollBackWidget.ui \
    UIClass/UserButtonMenu.ui \
    UIClass/WorkPlace/PatientListWidgetStep2.ui \
    UIClass/WorkPlace/ReagentUserWidget.ui \
    UIClass/WorkPlace/RunningStatusWidget.ui \
    UIClass/WorkPlace/SubWidget/ReagentPlaceHolderWidget.ui \
    UIClass/WorkPlace/SubWidget/ReagentStripWidget.ui \
    mainwindow.ui \
    UIClass/MainView.ui \
    UIClass/CustomWidget/RadioGroupBoxWidget.ui \
    UIClass/CustomWidget/GraphStyleChartWidget.ui \
    UIClass/CameraWidget/CameraView.ui \
    UIClass/CameraWidget/CameraWidget.ui \
    UIClass/CameraWidget/CameraStripSettings.ui \
    UIClass/CameraWidget/SubWidget/MoveControlWidget.ui \
    UIClass/CameraWidget/SubWidget/HousingROIWidget.ui \
    UIClass/CameraWidget/SubWidget/ParticleRemovalSettingWidget.ui \
    UIClass/CameraWidget/SubWidget/CommandMenuWidget.ui \
    UIClass/CameraWidget/SubWidget/IntensityWidget.ui \
    UIClass/CustomWidget/SpinSliderWidget.ui \
    UIClass/CameraWidget/CameraOptionSettings.ui \
    UIClass/CameraWidget/CameraSettings.ui \
    UIClass/CameraWidget/SubWidget/LEDSetting.ui \
    UIClass/CameraWidget/SubWidget/CameraPositionSetting.ui \
    UIClass/CameraWidget/SubWidget/CameraLightSetting.ui \
    UIClass/CameraWidget/SubWidget/BoxCarWidget.ui \
    UIClass/CameraWidget/SubWidget/PCLineWidget.ui \
    UIClass/SequenceWidget/SequenceView.ui \
    UIClass/SequenceWidget/SubWidget/PrimeWidget.ui \
    UIClass/SequenceWidget/SubWidget/DispenseWidget.ui \
    UIClass/SequenceWidget/SubWidget/AspirationWidget.ui \
    UIClass/SequenceWidget/SubWidget/IncubationWidget.ui \
    UIClass/SequenceWidget/SubWidget/DryWidget.ui \
    UIClass/CameraWidget/SubWidget/GuideLineWidget.ui \
    UIClass/CameraWidget/SubWidget/BandGapWidget.ui \
    UIClass/CameraWidget/SubWidget/RelativePosWidget.ui \
    UIClass/SequenceWidget/SubWidget/RollBackWidget.ui \
    UIClass/WorkPlace/WorkPlaceView.ui \
    UIClass/ResultView/ResultView.ui \
    UIClass/CustomWidget/NavigatorArrowWidget.ui \
    UIClass/WorkPlace/PatientListWidget.ui \
    UIClass/CustomWidget/CirclePrograssBarWithListWidget.ui \
    UIClass/SequenceWidget/SubWidget/DispAsp1Widget.ui \
    UIClass/SequenceWidget/SubWidget/DispAsp2Widget.ui \
    UIClass/SequenceWidget/SubWidget/Prime2Widget.ui \
    UIClass/SequenceWidget/SubWidget/Prime3Widget.ui \
    UIClass/SequenceWidget/SubWidget/Dispense2Widget.ui \
    UIClass/SequenceWidget/SubWidget/DispAsp3Widget.ui \
    UIClass/SequenceWidget/SubWidget/RollBack2Widget.ui \
    UIClass/SequenceWidget/SubWidget/RollBack3Widget.ui \
    UIClass/CustomWidget/CircleListWidgetItem.ui \
    UIClass/SequenceWidget/SubWidget/IncubationWSWidget.ui \
    UIClass/CameraWidget/SubWidget/FlipWidget.ui

# WIN32 LIB
win32: LIBS += -lole32 -lstrmiids

LIBS += -fopenmp
QMAKE_LFLAGS += -fopenmp -static
QMAKE_CXXFLAGS += -fopenmp -static-libstdc++ -static-libgcc

INCLUDEPATH += $$PWD/OpenCVx86
DEPENDPATH += $$PWD/OpenCVx86

INCLUDEPATH += $$PWD/Zbar
DEPENDPATH += $$PWD/Zbar

INCLUDEPATH += $$PWD/../QXlsx/header
DEPENDPATH += $$PWD/../QXlsx

INCLUDEPATH += $$PWD/../Libxl/include_cpp
LIBS += $$PWD/../Libxl/lib/libxl.lib



CONFIG += c++1z

# 경고 아래버전에 끝에 d가 붙은 디버그 버전 사용하지 말것 사용하면 원인불명의 Crush에러 발생

win32: LIBS += -L$$PWD/OpenCVx86/ -llibopencv_core340.dll
win32: LIBS += -L$$PWD/OpenCVx86/ -llibopencv_imgcodecs340.dll
win32: LIBS += -L$$PWD/OpenCVx86/ -llibopencv_imgproc340.dll
win32: LIBS += -L$$PWD/OpenCVx86/ -llibopencv_videoio340.dll
win32: LIBS += -L$$PWD/OpenCVx86/ -llibopencv_video340.dll
win32: LIBS += -L$$PWD/OpenCVx86/ -llibopencv_dnn340.dll
win32: LIBS += -L$$PWD/OpenCVx86/ -llibopencv_flann340.dll
win32: LIBS += -L$$PWD/OpenCVx86/ -llibopencv_highgui340.dll
win32: LIBS += -L$$PWD/OpenCVx86/ -llibopencv_ml340.dll
win32: LIBS += -L$$PWD/Zbar/ -llibzbar-0
win32: LIBS += -L$$PWD/../QXlsx/ -lQXlsx -static




RESOURCES += \
    resource.qrc



