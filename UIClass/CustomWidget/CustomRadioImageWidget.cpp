#include "CustomRadioImageWidget.h"

CustomRadioImageWidget::CustomRadioImageWidget(QWidget *parent) :
                                                                  QRadioButton(parent)
{
    this->initStyleSheet();
}

CustomRadioImageWidget::~CustomRadioImageWidget()
{

}

void CustomRadioImageWidget::initStyleSheet()
{
    QString checked = QString("QRadioButton::indicator:checked{image: url(%1);} ").arg(":/Image/Image/Information/check_focus.png");
    QString unChecked = QString("QRadioButton::indicator:unchecked{image: url(%1);} ").arg(":/Image/Image/Information/check_normal.png");
    QString hovered = QString("QRadioButton::indicator:unchecked:hover{image: url(%1);} ").arg(":/Image/Image/Information/check_over.png");
    //QString disabled = QString("QRadioButton::indicator:unchecked:disabled{image: url(%1);} ");
    QString styleString;

    styleString.append(checked);
    styleString.append(unChecked);
    styleString.append(hovered);
    //styleString.append(disabled);

    this->setStyleSheet(styleString);
}
