#include "CCheckBoxParentWidget.h"

CCheckBoxParentWidget::CCheckBoxParentWidget(QWidget *parent) :
                                                                QCheckBox(parent)
{
    this->initStyleSheet();
}

CCheckBoxParentWidget::~CCheckBoxParentWidget()
{

}

void CCheckBoxParentWidget::initStyleSheet()
{
    QString checked = QString("QCheckBox::indicator:checked{image: url(%1);} ").arg(":/Image/Image/Information/check_focus.png");
    QString checkedDisabled = QString("QCheckBox::indicator:checked:disabled{image: url(%1);} ").arg(":/Image/Image/Information/check_disable2.png");
    QString unChecked = QString("QCheckBox::indicator:unchecked{image: url(%1);} ").arg(":/Image/Image/Information/check_normal.png");
    QString hovered = QString("QCheckBox::indicator:unchecked:hover{image: url(%1);} ").arg(":/Image/Image/Information/check_over.png");
    QString disabled = QString("QCheckBox::indicator:unchecked:disabled{image: url(%1);} ").arg(":/Image/Image/Information/check_disable.png");
    QString styleString;

    styleString.append(checked);
    styleString.append(checkedDisabled);
    styleString.append(unChecked);
    styleString.append(hovered);
    styleString.append(disabled);

    this->setStyleSheet(styleString);
    this->setMinimumSize(24, 24);
}
