#ifndef CUSTOMIMAGEBUTTONWIDGET_H
#define CUSTOMIMAGEBUTTONWIDGET_H

#include <QPushButton>

class CustomImageButtonWidget : public QPushButton
{
private:
    Q_OBJECT
    struct privateStruct;

public:
    enum BUTTON_ALIGN_FLAG
    {
        BUTTON_ALIGN_FLAG_NONE,
        BUTTON_ALIGN_FLAG_LEFT,
        BUTTON_ALIGN_FLAG_CENTER,
        BUTTON_ALIGN_FLAG_RIGHT
    };

    explicit CustomImageButtonWidget(QWidget *parent = nullptr);
    ~CustomImageButtonWidget();

    void setAlighFlag(CustomImageButtonWidget::BUTTON_ALIGN_FLAG alignFlag);
    int getAlignMargin() const;
    void setAlignMargin(int margin);

    void setStyleSheetImage(const QString& normalImagePath, const QString& mouseoverImagePath,
                            const QString& pushedImagePath, const QString& disableImagePath);
    void setGraphicsDropShadowEffect(bool isActivate);
    void seFont(const QFont& font);
private:
    virtual void enterEvent(QEvent* e) override;
    virtual void leaveEvent(QEvent* e) override;
    virtual void mousePressEvent(QMouseEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;
    virtual void changeEvent(QEvent* e) override;

    void setNomal();
    void setDisableStyle();
    QString enumToAlignFlagText();

private:
    QScopedPointer<privateStruct> d;

};

#endif // CUSTOMIMAGEBUTTONWIDGET_H
