#ifndef USERLOGINDATACONTROLS_H
#define USERLOGINDATACONTROLS_H

#include <QObject>

class UserLoginDataControls : public QObject
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    explicit UserLoginDataControls(QObject* parent = nullptr);
    virtual ~UserLoginDataControls();

    void writePassword(QString password);
    QString readPassword();

    void writeISFirstInstall(bool isFirst);
    bool isFirstInstall();
private:
    void initIo();

private:
    QScopedPointer<privateStruct> d;
};

#endif // USERLOGINDATACONTROLS_H
