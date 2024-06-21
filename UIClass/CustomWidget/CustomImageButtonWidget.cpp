#include <QEvent>
#include <QGraphicsDropShadowEffect>
#include "CustomImageButtonWidget.h"

struct CustomImageButtonWidget::privateStruct
{
    QString mNormalImagePath;
    QString mMouseoverImagePath;
    QString mPushedImagePath;
    QString mDisableImagePath;
    BUTTON_ALIGN_FLAG mAlignFlag = BUTTON_ALIGN_FLAG_NONE;
    int mAlignMargin = 10;
};

CustomImageButtonWidget::CustomImageButtonWidget(QWidget *parent) :
                                                                    QPushButton(parent),
                                                                    d(new privateStruct)
{
}

CustomImageButtonWidget::~CustomImageButtonWidget()
{

}

void CustomImageButtonWidget::setAlighFlag(CustomImageButtonWidget::BUTTON_ALIGN_FLAG alignFlag)
{
    d->mAlignFlag = alignFlag;
    this->setNomal();
}

void CustomImageButtonWidget::setStyleSheetImage(const QString& normalImagePath, const QString& mouseoverImagePath,
                                                 const QString& pushedImagePath, const QString& disableImagePath)
{
    d->mNormalImagePath = normalImagePath;
    d->mMouseoverImagePath = mouseoverImagePath;
    d->mPushedImagePath = pushedImagePath;
    d->mDisableImagePath = disableImagePath;

    this->setNomal();
}

void CustomImageButtonWidget::setGraphicsDropShadowEffect(bool isActivate)
{
    if(isActivate)
    {
        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
        effect->setBlurRadius(1);
        effect->setOffset(3,3);
        this->setGraphicsEffect(effect);
    }
}

void CustomImageButtonWidget::seFont(const QFont &font)
{
    this->setFont(font);
}

void CustomImageButtonWidget::enterEvent(QEvent* e)
{
    QPushButton::enterEvent(e);

    if(this->isEnabled() == false)
        return;

    if(d->mMouseoverImagePath.isEmpty() == true)
        return;

    QString styleSheetStr = QString("border-image: url(%1);").arg(d->mMouseoverImagePath);
    auto styleSheetAlignStr = this->enumToAlignFlagText();

    if(styleSheetAlignStr.isEmpty() == false)
        styleSheetStr.append(styleSheetAlignStr);

    this->setStyleSheet(styleSheetStr);
}

void CustomImageButtonWidget::leaveEvent(QEvent* e)
{
     QPushButton::leaveEvent(e);

     if(this->isEnabled() == false)
         return;

    this->setNomal();
}

void CustomImageButtonWidget::mousePressEvent(QMouseEvent* e)
{
    QPushButton::mousePressEvent(e);

    if(this->isEnabled() == false)
        return;

    if(d->mPushedImagePath.isEmpty() == true)
        return;

    QString styleSheetStr = QString("border-image: url(%1);").arg(d->mPushedImagePath);
    auto styleSheetAlignStr = this->enumToAlignFlagText();

    if(styleSheetAlignStr.isEmpty() == false)
        styleSheetStr.append(styleSheetAlignStr);

    this->setStyleSheet(styleSheetStr);
}

void CustomImageButtonWidget::mouseReleaseEvent(QMouseEvent* e)
{
    QPushButton::mouseReleaseEvent(e);

    if(this->isEnabled() == false)
        return;

    if(d->mNormalImagePath.isEmpty() == true)
        return;

    QString styleSheetStr = QString("border-image: url(%1);").arg(d->mNormalImagePath);
    auto styleSheetAlignStr = this->enumToAlignFlagText();

    if(styleSheetAlignStr.isEmpty() == false)
        styleSheetStr.append(styleSheetAlignStr);

    this->setStyleSheet(styleSheetStr);
}

void CustomImageButtonWidget::changeEvent(QEvent* e)
{
    QPushButton::changeEvent(e);

    if(e->type() == QEvent::EnabledChange)
    {
        if(this->isEnabled() == false)
            this->setDisableStyle();
        else
            this->setNomal();
    }
}

void CustomImageButtonWidget::setNomal()
{
    if(d->mNormalImagePath.isEmpty() == true)
        return;

    QString styleSheetStr = QString("border-image: url(%1);").arg(d->mNormalImagePath);
    auto styleSheetAlignStr = this->enumToAlignFlagText();

    if(styleSheetAlignStr.isEmpty() == false)
        styleSheetStr.append(styleSheetAlignStr);

    this->setStyleSheet(styleSheetStr);
}

void CustomImageButtonWidget::setDisableStyle()
{
    if(d->mDisableImagePath.isEmpty() == true)
        return;

    QString styleSheetStr = QString("border-image: url(%1);").arg(d->mDisableImagePath);
    auto styleSheetAlignStr = this->enumToAlignFlagText();

    if(styleSheetAlignStr.isEmpty() == false)
        styleSheetStr.append(styleSheetAlignStr);

    this->setStyleSheet(styleSheetStr);
}

QString CustomImageButtonWidget::enumToAlignFlagText()
{
    QString alignText;

    switch(d->mAlignFlag)
    {
    case BUTTON_ALIGN_FLAG_LEFT:
        alignText = QString("Text-align:left; padding-left: %1px;").arg(d->mAlignMargin);
        break;

    case BUTTON_ALIGN_FLAG_CENTER:
        alignText = QString("Text-align:center; padding-left: %1px;").arg(d->mAlignMargin);
        break;

    case BUTTON_ALIGN_FLAG_RIGHT:
        alignText = QString("Text-align:right; padding-right: %1px;").arg(d->mAlignMargin);
        break;

    default:
        break;
    }

    return alignText;
}

int CustomImageButtonWidget::getAlignMargin() const
{
    return d->mAlignMargin;
}

void CustomImageButtonWidget::setAlignMargin(int margin)
{
    d->mAlignMargin = margin;
    this->enumToAlignFlagText();
}
