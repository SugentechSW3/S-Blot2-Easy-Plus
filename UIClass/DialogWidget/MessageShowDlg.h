#ifndef MESSAGESHOWDLG_H
#define MESSAGESHOWDLG_H

#include <Common/CommonParentWidget/CDialogParentWidget.h>

class Ui_MessageShowDlg;
class MessageShowDlg : public CDialogParentWidget
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit MessageShowDlg(const QString &message, QWidget *parent = nullptr);
    virtual ~MessageShowDlg();


    void enableNotShowingMessage(const QString& primaryKey);
    void closing();

    void setDefaultTextAlignment();
    void setFontSize(const int& fontSize);
    void setFont(const QFont &font);
    void setText(const QString& text);
    void setButtonText(const QString& text);    
    void setButtonHeight(const int& heightSize);
private:
    void init();
    void initStyleSheet();

    void saveShowingData();
    void setNotShowingCheckBoxVisible(bool isVisable);
private:
    QScopedPointer<Ui_MessageShowDlg> ui;
    QScopedPointer<privateStruct> d;
};

#endif // MESSAGESHOWDLG_H
