#include "CustomImageLabelSet.h"
#include "ui_CustomImageLabelSet.h"

struct CustomImageLabelSet::privateStruct
{
    QString mNormalImagePath;
    QString mMonuntImagePath;
    bool mIsMount = false;
};

CustomImageLabelSet::CustomImageLabelSet(QWidget *parent) :
                                                            QWidget(parent),
                                                            ui(new Ui_CustomImageLabelSet),
                                                            d(new privateStruct)
{
    ui->setupUi(this);


    connect(this, &CustomImageLabelSet::onMountChanged, this, [this]
            (bool isMount)
            {
                if(isMount == true)
                    this->setMounted();
                else
                    this->setNormal();
            });
}

CustomImageLabelSet::~CustomImageLabelSet()
{
}

void CustomImageLabelSet::setMountState(bool mount)
{
    if(d->mIsMount != mount)
        emit onMountChanged(mount);

    d->mIsMount = mount;
}

bool CustomImageLabelSet::isMounted()
{
    return d->mIsMount;
}

void CustomImageLabelSet::setStyleSheetImage(const QString& normalImagePath, const QString& mountImagePath)
{
    d->mNormalImagePath = normalImagePath;
    d->mMonuntImagePath = mountImagePath;

    this->setMounted();
}

void CustomImageLabelSet::setText(const QString& text)
{
    ui->labelText->setText(text);    
}

void CustomImageLabelSet::setFont(const QFont &font)
{    
    ui->labelText->setFont(font);
    ui->labelText->setAlignment(Qt::AlignCenter);
}

void CustomImageLabelSet::setNormal()
{
    if(d->mNormalImagePath.isEmpty() == true)
        return;

    ui->widgetBackground->setStyleSheet(QString("QWidget#widgetBackground{border-image: url(%1);};").arg(d->mNormalImagePath));
    ui->labelText->setStyleSheet(QString("color: rgb(0, 0, 0);"));
}

void CustomImageLabelSet::setMounted()
{
    if(d->mMonuntImagePath.isEmpty() == true)
        return;

    ui->widgetBackground->setStyleSheet(QString("QWidget#widgetBackground{border-image: url(%1);};").arg(d->mMonuntImagePath));
    ui->labelText->setStyleSheet(QString("color: rgb(255, 255, 255);"));
}
