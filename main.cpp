#include <QApplication>
#include <qtsingleapplication.h>
#include <QTranslator>


#include <UIClass/DialogWidget/HiddenBandCheckDlg.h>
#include <UIClass/DialogWidget/LanguageSelectDlg.h>

#include "Common/CUtil.h"
#include "Analysis/ConfigInformation.h"

#include "mainwindow.h"

QString languageStr(GlobalDataStruct::LANGUAGE_LIST languageEnum);

int main(int argc, char *argv[])
{
    QtSingleApplication a(argc, argv);
    QCoreApplication::setApplicationName("S-Blot 2 Easy PLUS");

    if(a.isRunning())
    {
        sugentech::CUtil::messageBox(QObject::tr("S-Blot 2 Easy PLUS Is Already Running"));
        a.quit();
    }
    auto config = ConfigInformation::getInstance();
    MainWindow w;
    w.show();

    return a.exec();
}
