#ifndef CUSTOMIMAGELABELSET_H
#define CUSTOMIMAGELABELSET_H

#include <QWidget>
class Ui_CustomImageLabelSet;
class CustomImageLabelSet : public QWidget
{
private:
    Q_OBJECT
    struct privateStruct;
public:
    explicit CustomImageLabelSet(QWidget *parent = nullptr);
    ~CustomImageLabelSet();

    void setStyleSheetImage(const QString& normalImagePath, const QString& mountImagePath);
    void setText(const QString& text);
    void setFont(const QFont &font);

    void setMountState(bool mount);
    bool isMounted();
signals:
    void onMountChanged(bool mount);


private:
    void setNormal();
    void setMounted();
private:
    QScopedPointer<Ui_CustomImageLabelSet> ui;
    QScopedPointer<privateStruct> d;
};

#endif // CUSTOMIMAGELABELSET_H
